/*
 * Comm.cpp
 *
 *  Created on: Jul 23, 2019
 *      Author: caoyu
 */

#include "Comm.h"
#include "Debug.h"
#include <cstdarg>

#ifndef SIMULATOR
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "stm32f4xx_hal.h"

#define HC_USART	USART6
#define HC_PORT		GPIOG
#define HC_TX		GPIO_PIN_14
#define HC_RX		GPIO_PIN_9
#define HC_AF		GPIO_AF8_USART6
#define HC_TX_DMA_STREAM DMA2_Stream7
#define HC_RX_DMA_STREAM DMA2_Stream1
#define HC_TX_DMA_REQUEST DMA_CHANNEL_5
#define HC_RX_DMA_REQUEST DMA_CHANNEL_5
#define HC_TX_DMA_IRQn	DMA2_Stream7_IRQn
#define HC_RX_DMA_IRQn	DMA2_Stream1_IRQn
#define HC_USART_IRQn	USART6_IRQn
#define HC_BAUD 1152000
#define BUF_SIZE 512

UART_HandleTypeDef huart;
int error_count = 0;

unsigned char rx_start[BUF_SIZE];
unsigned char tx_start[BUF_SIZE];
volatile unsigned char *rx_head = rx_start, *rx_tail = rx_start, *rx_end =
		rx_start + BUF_SIZE;
volatile unsigned char *tx_head = tx_start, *tx_tail = tx_start, *tx_end =
		tx_start + BUF_SIZE;

inline bool rx_empty() {
	return rx_head == rx_tail;
}
inline bool rx_full() {
	return ((rx_tail - rx_head + 1) % BUF_SIZE) == 0;
}
inline bool tx_empty() {
	return tx_head == tx_tail;
}
inline bool tx_full() {
	return ((tx_tail - tx_head + 1) % BUF_SIZE) == 0;
}

inline int available_rx() {
	return (rx_tail - rx_head + BUF_SIZE) % BUF_SIZE;
}

inline int capacity_tx() {
	return BUF_SIZE - (tx_tail - tx_head + BUF_SIZE) % BUF_SIZE - 1;
}

DMA_HandleTypeDef tx_dma;
DMA_HandleTypeDef rx_dma;

xSemaphoreHandle tx_sem;
xSemaphoreHandle rx_sem;

//volatile bool tx_complete;
//volatile bool rx_error;

//int c1 = 0;
//int c2 = 0;

//extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
//	tx_complete = true;
//	c1++;
//}
//
//extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
//	rx_error = true;
//}

extern "C" void USART6_IRQHandler() {
	uint32_t isr = huart.Instance->SR;
	uint32_t errorflags =
			(isr
					& (uint32_t) (USART_SR_PE | USART_SR_FE | USART_SR_ORE
							| USART_SR_NE));
	bool start_rx = false;
	bool start_tx = false;
	portBASE_TYPE px = pdFALSE;
	// Handle read
	if (errorflags) {
		__HAL_UART_CLEAR_FLAG(&huart, errorflags);
		// Stop DMA transfer
		huart.Instance->CR3 &= ~USART_CR3_DMAR;
		HAL_DMA_Abort(huart.hdmarx);
		rx_head = rx_tail = rx_start; // Discard all data
		// Restart
		start_rx = true;
	} else if (__HAL_UART_GET_IT_SOURCE(&huart, UART_IT_RXNE) != RESET) {
		// Not checking the isr flag because it's cleared by DMA
		__HAL_UART_CLEAR_FLAG(&huart, USART_SR_RXNE);

		int avail_before = available_rx();

		// update tail of rx queue
		rx_tail = rx_start + (BUF_SIZE - rx_dma.Instance->NDTR);
		if (rx_tail == rx_end)
			rx_tail = rx_start;
		if (avail_before > available_rx()) { // Overrun occurred. Discard old data
			rx_head = rx_tail + 1;
			if (rx_head == rx_end)
				rx_head = rx_start;
		}

		// Signal the waiting thread
		xSemaphoreGiveFromISR(rx_sem, &px);
	}
	// Handle write
	if ((isr & USART_SR_TC) != RESET
			&& __HAL_UART_GET_IT_SOURCE(&huart, UART_IT_TC) != RESET) {
		__HAL_UART_DISABLE_IT(&huart, UART_IT_TC);
		huart.State = HAL_UART_STATE_READY;

		// Signal the waiting thread
		xSemaphoreGiveFromISR(tx_sem, &px);

		// Start new tx
		start_tx = true;
	}

	if (start_rx) {
		HAL_UART_Receive_DMA(&huart, rx_start, BUF_SIZE);
		__HAL_UART_ENABLE_IT(&huart, UART_IT_RXNE);
	}

	if (start_tx) {
		size_t tx_len = 0;
		// Setup next transfer
		tx_head += huart.TxXferSize;
		if (tx_head >= tx_end)
			tx_head -= BUF_SIZE;
		if (tx_tail >= tx_head) {
			tx_len = tx_tail - tx_head;
		} else {
			tx_len = tx_end - tx_head;
		}
		if (tx_len > 0) {
			// Start next transmission
			HAL_UART_Transmit_DMA(&huart, (uint8_t*) tx_head, tx_len);
		}
	}

	portEND_SWITCHING_ISR(px);
//	uint8_t *ptr = huart.pRxBuffPtr;
//	bool was_tx_ongoing = (huart.State & HAL_UART_STATE_BUSY_TX) == HAL_UART_STATE_BUSY_TX;
//	tx_complete = false;
//	rx_error = false;
//
//	// Call STM handler
//	HAL_UART_IRQHandler(&huart);
//
//	// Error codes
//	if (rx_error) {
//		error_count++;
//
//		if (was_tx_ongoing && !tx_complete) {
//			// TX ongoing but not finished yet, we should continue even though there is an error in RX
//			huart.State = HAL_UART_STATE_BUSY_TX;
//		}
//
//		// Disable all interrupts
//		__HAL_UART_DISABLE_IT(&huart, UART_IT_RXNE);
//		__HAL_UART_DISABLE_IT(&huart, UART_IT_PE);
//		__HAL_UART_DISABLE_IT(&huart, UART_IT_ERR);
//		(void) huart.Instance->DR; // dummy read to clear RXNE
//		huart.ErrorCode = 0; // Clear error
//
//		size_t rx_len = 0;
//		if (rx_tail < rx_head) {
//			rx_len = rx_head - rx_tail - 1;
//		} else {
//			if (rx_head != rx_start)
//				rx_len = rx_end - rx_tail;
//			else
//				rx_len = rx_end - rx_tail - 1; // If rx_head=0, rx_tail=N-1, the queue is full and nothing can be read
//		}
//		if (rx_len > 0) {
//			// Start next reception
//			HAL_UART_Receive_IT(&huart, (uint8_t*) rx_tail, rx_len);
//		}
//	} else if (huart.pRxBuffPtr != ptr) { // Received something, then ptr would be increased
//		rx_tail++; // Push the queue
//		if (rx_tail == rx_end)
//			rx_tail = rx_start;
//		// Signal the waiting thread
//		xSemaphoreGiveFromISR(rx_sem, &px);
//		if ((huart.State & HAL_UART_STATE_BUSY_RX) != HAL_UART_STATE_BUSY_RX) {
//			size_t rx_len = 0;
//			if (rx_tail < rx_head) {
//				rx_len = rx_head - rx_tail - 1;
//			} else {
//				if (rx_head != rx_start)
//					rx_len = rx_end - rx_tail;
//				else
//					rx_len = rx_end - rx_tail - 1; // If rx_head=0, rx_tail=N-1, the queue is full and nothing can be read
//			}
//			if (rx_len > 0) {
//				// Start next reception
//				HAL_UART_Receive_IT(&huart, (uint8_t*) rx_tail, rx_len);
//			}
//		}
//	}
//	if (tx_complete) { // TX complete, prepare next transmission
//		size_t tx_len = 0;
//		// Setup next transfer
//		tx_head += huart.TxXferSize;
//		if (tx_head >= tx_end)
//			tx_head -= BUF_SIZE;
//		if (tx_tail >= tx_head) {
//			tx_len = tx_tail - tx_head;
//		} else {
//			tx_len = tx_end - tx_head;
//		}
//		if (tx_len > 0) {
//			// Start next transmission
//			HAL_UART_Transmit_DMA(&huart, (uint8_t*) tx_head, tx_len);
//		}
//		// Signal the tx_thread for completion
//		xSemaphoreGiveFromISR(tx_sem, &px);
//	}
//	portEND_SWITCHING_ISR(px);
}

// TX DMA
extern "C" void DMA2_Stream7_IRQHandler() {
	HAL_DMA_IRQHandler(&tx_dma);
//	c2++;
}

// RX DMA
//extern "C" void DMA2_Stream6_IRQHandler() {
//	HAL_DMA_IRQHandler(&rx_dma);
//}

#endif

void Comm::init() {
#ifdef SIMULATOR
	//TODO
#else
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* Peripheral clock enable */
	__HAL_RCC_USART6_CLK_ENABLE();

	__HAL_RCC_GPIOG_CLK_ENABLE();
	/**USART6 GPIO Configuration
	 PG14     ------> USART6_TX
	 PG9     ------> USART6_RX
	 */
	GPIO_InitStruct.Pin = HC_TX | HC_RX;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = HC_AF;
	HAL_GPIO_Init(HC_PORT, &GPIO_InitStruct);

	huart.Instance = HC_USART;
	huart.Init.BaudRate = HC_BAUD;
	huart.Init.WordLength = UART_WORDLENGTH_8B;
	huart.Init.StopBits = UART_STOPBITS_1;
	huart.Init.Parity = UART_PARITY_NONE;
	huart.Init.Mode = UART_MODE_TX_RX;
	huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&huart) != HAL_OK) {
		error("Cannot initialize USART");
	}

	// DMA initialization
	DMA_InitTypeDef &d = tx_dma.Init;

	d.Direction = DMA_MEMORY_TO_PERIPH;
	d.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	d.MemInc = DMA_MINC_ENABLE;
	d.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	d.PeriphInc = DMA_PINC_DISABLE;
	d.Mode = DMA_NORMAL;
	d.Priority = DMA_PRIORITY_MEDIUM;
	d.FIFOMode = DMA_FIFOMODE_DISABLE;
	d.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
	d.MemBurst = DMA_MBURST_SINGLE;
	d.PeriphBurst = DMA_PBURST_SINGLE;
	d.Channel = HC_TX_DMA_REQUEST;

	tx_dma.Instance = HC_TX_DMA_STREAM;
	huart.hdmatx = &tx_dma;
	tx_dma.Parent = &huart;

	DMA_InitTypeDef &e = rx_dma.Init;

	e.Direction = DMA_PERIPH_TO_MEMORY;
	e.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	e.MemInc = DMA_MINC_ENABLE;
	e.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	e.PeriphInc = DMA_PINC_DISABLE;
	e.Mode = DMA_CIRCULAR;
	e.Priority = DMA_PRIORITY_MEDIUM;
	e.FIFOMode = DMA_FIFOMODE_DISABLE;
	e.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
	e.MemBurst = DMA_MBURST_SINGLE;
	e.PeriphBurst = DMA_PBURST_SINGLE;
	e.Channel = HC_RX_DMA_REQUEST;

	rx_dma.Instance = HC_RX_DMA_STREAM;
	huart.hdmarx = &rx_dma;
	rx_dma.Parent = &huart;

	__HAL_RCC_DMA2_CLK_ENABLE();

	HAL_DMA_Init(&tx_dma);
	HAL_DMA_Init(&rx_dma);

	//  UART IRQ
	IRQn_Type irq_n = HC_USART_IRQn;
	NVIC_ClearPendingIRQ(irq_n);
	NVIC_DisableIRQ(irq_n);
	NVIC_SetPriority(irq_n, 10);
	NVIC_EnableIRQ(irq_n);

	// TX DMA IRQ
	irq_n = HC_TX_DMA_IRQn;
	NVIC_ClearPendingIRQ(irq_n);
	NVIC_DisableIRQ(irq_n);
	NVIC_SetPriority(irq_n, 10);
	NVIC_EnableIRQ(irq_n);

	// Create semaphores
	rx_sem = xSemaphoreCreateBinary();
	tx_sem = xSemaphoreCreateBinary();

	// Enable RX immediately
//	HAL_UART_Receive_IT(&huart, (uint8_t*) rx_start,
//	BUF_SIZE - 1);
//	HAL_UART_Receive_DMA(&huart, (uint8_t*) rx_start, BUF_SIZE);

    huart.Instance->CR3 |= USART_CR3_DMAR;
    HAL_DMA_Start(huart.hdmarx, (uint32_t)&huart.Instance->DR, (uint32_t)rx_start, BUF_SIZE);
	__HAL_UART_ENABLE_IT(&huart, UART_IT_RXNE); // Enable interrupt on reception of each char
#endif
}

int Comm::read(char *buf, size_t len) {
#ifdef SIMULATOR
#else
	taskENTER_CRITICAL();
	unsigned char *p = (unsigned char*) buf;
	while (len > 0) {
		size_t available = available_rx();
		// First read all available data up to length
		size_t copied = available < len ? available : len;
		// Pop the queue
		for (unsigned int i = 0; i < copied; i++) {
			*p++ = *rx_head++;
			if (rx_head == rx_end)
				rx_head = rx_start;
		}
		// Init next receive if not so
//		if ((huart.State & HAL_UART_STATE_BUSY_RX) != HAL_UART_STATE_BUSY_RX) {
//			// No ongoing reception
//			size_t rx_len;
//			if (rx_tail < rx_head) {
//				rx_len = rx_head - rx_tail - 1;
//			} else {
//				if (rx_head != rx_start)
//					rx_len = rx_end - rx_tail;
//				else
//					rx_len = rx_end - rx_tail - 1; // If rx_head=0, rx_tail=N-1, the queue is full and nothing can be read
//			}
//
//			HAL_UART_Receive_IT(&huart, (uint8_t*) rx_tail, rx_len); // Start receive
//		}
		// Break if done
		len -= copied;
		if (len == 0)
			break;
		// Wait for data to be available for copy
		while (rx_empty()) {
			// Clear the semiphore before exiting critical
			xSemaphoreTakeFromISR(rx_sem, NULL);
			// IRQ comes immediately after exiting critical section,
			taskEXIT_CRITICAL();
			// Wait for wakeup from ISR
			xSemaphoreTake(rx_sem, portMAX_DELAY);
			// Flag will be automatically cleared by now
			taskENTER_CRITICAL();
		}
	}

	taskEXIT_CRITICAL();
	size_t length = (p - (unsigned char*) buf);
	return length > 0 ? length : -1;

#endif
}

int Comm::write(char *buf, size_t len) {
#ifdef SIMULATOR
#else
	taskENTER_CRITICAL();
	unsigned char *p = (unsigned char*) buf;

	while (len > 0) {
		size_t capacity = capacity_tx();
		size_t copied = (len < capacity) ? len : capacity; // Maximum transfer
		// Push queue
		for (size_t i = 0; i < copied; i++) {
			*tx_tail++ = *p++;
			if (tx_tail == tx_end)
				tx_tail = tx_start;
		}

		if ((huart.State & HAL_UART_STATE_BUSY_TX) != HAL_UART_STATE_BUSY_TX) {
			// No ongoing transmission
			size_t tx_len;
			if (tx_tail >= tx_head) {
				tx_len = tx_tail - tx_head;
			} else {
				tx_len = tx_end - tx_head; // Should not wrap, the IRQ will take care of the rest of the transmission
			}

			//				HAL_UART_Transmit_IT(huart, (uint8_t*) tx_head, tx_len); // Start transmission
			HAL_UART_Transmit_DMA(&huart, (uint8_t*) tx_head, tx_len);
		}

		len -= copied;

		// Wait for transmission to finish
		// Due to the mutex, no other data will be added further.
		// So just wait for the current transmission to finish
		while (!tx_empty()) {
			// Clear the semiphore before exiting critical
			xSemaphoreTakeFromISR(tx_sem, NULL);
			// IRQ comes immediately after exiting critical section,
			taskEXIT_CRITICAL();
			// Wait for wakeup from ISR
			xSemaphoreTake(tx_sem, portMAX_DELAY);
			// Flag will be automatically cleared by now
			taskENTER_CRITICAL();
		}

	}
	taskEXIT_CRITICAL();
	return p - (unsigned char*) buf;
#endif
}

int Comm::printf(const char *fmt, ...) {
	char buf[1024];
	va_list args;
	va_start(args, fmt);
	int len = vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);
	write(buf, len);
	return len;
}
