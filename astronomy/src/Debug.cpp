/*
 * Debug.cpp
 *
 *  Created on: Jul 23, 2019
 *      Author: caoyu
 */

#include "Debug.h"
#include <cstdarg>
#include <cstdio>

#ifndef SIMULATOR
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"

static const uint32_t timeout = 300;
static UART_HandleTypeDef huart;

#endif

static bool inited = false;


static void init() {
	inited = true;
#ifdef SIMULATOR
	// Nothing needs to be done
#else
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* Peripheral clock enable */
	__HAL_RCC_USART3_CLK_ENABLE();

	__HAL_RCC_GPIOB_CLK_ENABLE();
	/**USART6 GPIO Configuration
	 PB10     ------> USART3_TX
	 PB11     ------> USART3_RX
	 */
	GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	huart.Instance = USART3;
	huart.Init.BaudRate = 115200;
	huart.Init.WordLength = UART_WORDLENGTH_8B;
	huart.Init.StopBits = UART_STOPBITS_1;
	huart.Init.Parity = UART_PARITY_NONE;
	huart.Init.Mode = UART_MODE_TX_RX;
	huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&huart) != HAL_OK) {
		while (1)
			;
	}
#endif
}

static void write(const char *buf, size_t len) {
	if (!inited) {
		init();
	}
#ifdef SIMULATOR
	fwrite(buf, sizeof(char), len, stdout);
#else
	HAL_UART_Transmit(&huart, (unsigned char*) buf, len, timeout);
#endif
}

void debug_if(int condition, const char *format, ...) {
	char buf[256];
#if !defined(NDEBUG)
	if (condition) {
		va_list args;
		va_start(args, format);
		int len = vsnprintf(buf, sizeof(buf), format, args);
		va_end(args);
		write(buf, len);
	}
#endif
}

void error(const char *format, ...) {
#ifndef SIMULATOR
	taskDISABLE_INTERRUPTS();
#endif
	char buf[256];
	va_list args;
	va_start(args, format);
	int len = vsnprintf(buf, sizeof(buf), format, args);
	va_end(args);
	write(buf, len);

	for(;;);
}
