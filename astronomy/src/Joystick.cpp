/*
 * Joystick.cpp
 *
 *  Created on: Aug 28, 2019
 *      Author: caoyu
 */

#include "Joystick.h"
#include "stm32f4xx_hal.h"
#include "Debug.h"

// A0          = PB_1,
// A1          = PC_2,
#define A0_PIN	GPIO_PIN_1
#define A0_PORT	 GPIOB
#define A0_ADC_CHAN	ADC_CHANNEL_9
#define A1_PIN  GPIO_PIN_2
#define A1_PORT  GPIOC
#define A1_ADC_CHAN	ADC_CHANNEL_12

static ADC_HandleTypeDef hadc;
static DMA_HandleTypeDef dma_adc;
static uint16_t adc_data[2];

const int x_offset = 2040;
const int x_scale = -1100;
const int y_offset = 2080;
const int y_scale = 1250;

void JoyStick_Init() {
	// Init GPIO
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* Peripheral clock enable */
	__HAL_RCC_ADC1_CLK_ENABLE();

	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_DMA2_CLK_ENABLE();

	GPIO_InitStruct.Pin = A0_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(A0_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = A1_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(A1_PORT, &GPIO_InitStruct);

	hadc.Instance = ADC1;
	hadc.Init.Resolution = ADC_RESOLUTION_12B;
	hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.ScanConvMode = ENABLE;
	hadc.Init.EOCSelection = ADC_EOC_SEQ_CONV; // EOC only after all conversion finished
	hadc.Init.ContinuousConvMode = DISABLE;
	hadc.Init.DMAContinuousRequests = ENABLE;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	hadc.Init.NbrOfConversion = 2;
	hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;

	if (HAL_ADC_Init(&hadc) != HAL_OK) {
		error("Cannot initialize ADC");
	}

	ADC_ChannelConfTypeDef chan;

	// A0
	chan.Channel = A0_ADC_CHAN;
	chan.Rank = 1;
	chan.Offset = 0;
	chan.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc, &chan) != HAL_OK) {
		error("Cannot initialize ADC for A0");
	}

	// A1
	chan.Channel = A1_ADC_CHAN;
	chan.Rank = 2;
	chan.Offset = 0;
	chan.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc, &chan) != HAL_OK) {
		error("Cannot initialize ADC for A1");
	}

	// DMA init
	DMA_InitTypeDef &d = dma_adc.Init;

	d.Direction = DMA_PERIPH_TO_MEMORY;
	d.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	d.MemInc = DMA_MINC_ENABLE;
	d.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	d.PeriphInc = DMA_PINC_DISABLE;
	d.Mode = DMA_NORMAL;
	d.Priority = DMA_PRIORITY_MEDIUM;
	d.FIFOMode = DMA_FIFOMODE_DISABLE;
	d.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
	d.MemBurst = DMA_MBURST_SINGLE;
	d.PeriphBurst = DMA_PBURST_SINGLE;
	d.Channel = DMA_CHANNEL_0;

	dma_adc.Instance = DMA2_Stream0;
	__HAL_LINKDMA(&hadc, DMA_Handle, dma_adc);

	if (HAL_DMA_Init(&dma_adc) != HAL_OK) {
		error("Cannot initialize DMA for ADC");
	}

	// Enable DMA IRQ
	IRQn_Type irq_n = DMA2_Stream0_IRQn;
	NVIC_ClearPendingIRQ(irq_n);
	NVIC_DisableIRQ(irq_n);
	NVIC_SetPriority(irq_n, 10);
	NVIC_EnableIRQ(irq_n);
}

void JoyStick_Read(float &x, float &y) {
	while (HAL_ADC_GetState(&hadc) & HAL_ADC_STATE_BUSY)
		;

	HAL_ADC_Start_DMA(&hadc, (unsigned long int*) adc_data, 2);

	// Wait for conversion to complete
	while (HAL_ADC_GetState(&hadc) != HAL_ADC_STATE_EOC_REG)
		;

	HAL_ADC_Stop_DMA(&hadc);

	y = (float) (adc_data[0] - y_offset) / y_scale;
	x = (float) (adc_data[1] - x_offset) / x_scale;

	if (x > 1)
		x = 1;
	else if (x < -1)
		x = -1;
	if (y > 1)
		y = 1;
	else if (y < -1)
		y = -1;
}

void JoyStick_Calibrate() {

}

extern "C" void DMA2_Stream0_IRQHandler() {
	HAL_DMA_IRQHandler(&dma_adc);
}
