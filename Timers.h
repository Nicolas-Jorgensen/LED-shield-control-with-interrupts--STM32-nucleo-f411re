/**
 * @file    Timers.h
 * @author  Adam Korycki
 * @author  nubby (jlee211@ucsc.edu)
 *
 * @date    29 Sept 2023
 */

#ifndef TIMERS_H
#define	TIMERS_H

#include <stdint.h>
#ifdef STM32F4
#include "stm32f4xx_hal.h"
#include <stm32f4xx_hal_rcc.h>
#include <stm32f4xx_hal_tim.h>

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
#endif  /*  STM32F4 */

/**
 * @function Timer_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief Initializes the timer (TIM2) peripheral
 * @author Adam Korycki, 2023.09.29 */
extern char Timers_Init(void);

/**
 * @function Timers_GetMilliSeconds(void)
 * @param None
 * @return current millisecond count
 * @brief ^
 * @author Adam Korycki, 2023.09.29 */
extern uint32_t Timers_GetMilliSeconds(void);

/**
 * @function Timers_GetMicroSeconds(void)
 * @param None
 * @return current microsecond count
 * @brief ^
 * @author Adam Korycki, 2023.09.29 */
extern uint32_t Timers_GetMicroSeconds(void);

/**
 * @function Timers_GetSystemClockFreq(void)
 * @param None
 * @return frequnecy of system clock in hz
 * @brief ^
 * @author Adam Korycki, 2023.09.29 */
extern uint32_t Timers_GetSystemClockFreq(void);

#endif
