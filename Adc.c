/**
 * Configure and read from ADC channels using the UCSC Nucleo I/O shield.
 *
 * @file    Adc.c
 * @author  nubby (jlee211@ucsc.edu)
 * @author  Adam Korycki
 *
 * @date    16 Sep 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "Adc.h"

// Boolean defines for TRUE, FALSE, SUCCESS and ERROR
#ifndef FALSE
#define FALSE ((int8_t) 0)
#define TRUE ((int8_t) 1)
#endif
#ifndef ERROR
#define ERROR ((int8_t) -1)
#endif
#ifndef SUCCESS
#define SUCCESS ((int8_t) 1)
#endif

static uint8_t init_status = FALSE;

/**
 * ADC_Init(void)
 *
 * @brief Initializes the ADC subsystem with an interrupt; selects ADC Pin4 by 
 *        default.
 * 
 * @return SUCCESS or ERROR
 */
char ADC_Init(void) {
	if (init_status == FALSE) { // if ADC has not been initialized
#ifdef STM32F4
    ADC_ChannelConfTypeDef sConfig = {0};

    /** Configure the global features of the ADC (Clock, Resolution, Data
     * Alignment and number of conversion).
    */
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
      return ERROR;
    }

    /** Configure for the selected ADC regular channel its corresponding rank in
     * the sequencer and its sample time.
    */
    sConfig.Channel = POT;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
      return ERROR;
    }
    
    // Start the ADC interrupt.
    HAL_ADC_Start_IT(&hadc1);
#endif  /*  STM32F4 */
    init_status = TRUE;
  }
	return SUCCESS;
}

/**
 * ADC_End(void)
 *
 * @brief Disables the A/D subsystem and release the pins used
 */
void ADC_End(void){
#ifdef STM32F4
  HAL_ADC_Stop_IT(&hadc1);
	HAL_ADC_DeInit(&hadc1);
#endif  /*  STM32F4 */
}

