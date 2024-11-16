/**
 * @file    Leds.c
 * @author
 * @date
 *
 * This library provides an interface for controlling the LEDs on the UCSC
 * Nucleo I/O Shield development board.
 */

#include <stdio.h>
#include <stdlib.h>
#include <BOARD.h>
#include "Leds_Lab07.h"
#ifdef STM32F4
#include "stm32f4xx_hal.h"
#endif  /*  STM32F4 */

/**
 * LEDs_Init() Initializes the LED bar by doing three things:
 *      1) Enables usage of the GPIO clocks for needed ports.
 *      2) Use the appropriate SFRs to set each LED pin to "output" mode.
 *      3) Use the appropriate SFRs to set each LED pin's output value to 0 (low
 *         voltage). 
 * After calling LEDs_Init(), the other functions in this file can be used to 
 * manipulate the LED bar.
 */
void LEDs_Init(void) {
    // Enable GPIO clocks for ports C and B.
#ifdef STM32F4
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Init GOIO output pins for LEDs.
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif  /*  STM32F4 */
    LEDs_Set(0x0); // Reset LEDs.
}
//gloabal variable to communicate between LED_Set() and LED_Get()
static char ledsOnOff = 0x00; 

/**
 * LEDs_Set() controls the output on the LED bar.  Input values are 8-bit 
 * patterns, where each bit describes the desired state of the corresponding 
 * LED. So, for example, LEDs_Set(0x80) should  leave the first LED on, and the
 * last 7 LEDs off. LEDs_Set(0x1F) should turn off the first three LEDs and turn
 * on the remaining five LEDs. 
 *
 * @param newPattern:  An 8-bit value describing the desired output on the LED
 * bar. 
 *
 * LEDs_Set should not change any LED pins to inputs.
 */
void LEDs_Set(char newPattern) {
#ifdef STM32F4
   
    if(newPattern & 0x80)
    {
        HAL_GPIO_WritePin(GP_LED7_GPIO_Port, GP_LED7_Pin, 1);
    }
    else
    {
        HAL_GPIO_WritePin(GP_LED7_GPIO_Port, GP_LED7_Pin, 0);
    }

    if(newPattern & 0x40)
    {
        HAL_GPIO_WritePin(GP_LED6_GPIO_Port, GP_LED6_Pin, 1);
    }
    else
    {
        HAL_GPIO_WritePin(GP_LED6_GPIO_Port, GP_LED6_Pin, 0);
    }

    if(newPattern & 0x20)
    {
        HAL_GPIO_WritePin(GP_LED5_GPIO_Port, GP_LED5_Pin, 1);
    }
    else
    {
        HAL_GPIO_WritePin(GP_LED5_GPIO_Port, GP_LED5_Pin, 0);
    }

    if(newPattern & 0x10)
    {
        HAL_GPIO_WritePin(GP_LED4_GPIO_Port, GP_LED4_Pin, 1);
    }
    else
    {
        HAL_GPIO_WritePin(GP_LED4_GPIO_Port, GP_LED4_Pin, 0);
    }

    if(newPattern & 0x08)
    {
        HAL_GPIO_WritePin(GP_LED3_GPIO_Port, GP_LED3_Pin, 1);
    }
    else
    {
        HAL_GPIO_WritePin(GP_LED3_GPIO_Port, GP_LED3_Pin, 0);
    }

     if(newPattern & 0x04)
    {
        HAL_GPIO_WritePin(GP_LED2_GPIO_Port, GP_LED2_Pin, 1);
    }
    else
    {
        HAL_GPIO_WritePin(GP_LED2_GPIO_Port, GP_LED2_Pin, 0);
    }

    if(newPattern & 0x02)
    {
        HAL_GPIO_WritePin(GP_LED1_GPIO_Port, GP_LED1_Pin, 1);
    }
    else
    {
        HAL_GPIO_WritePin(GP_LED1_GPIO_Port, GP_LED1_Pin, 0);
    }

    if(newPattern & 0x01)
    {
        HAL_GPIO_WritePin(GP_LED0_GPIO_Port, GP_LED0_Pin, 1);
    }
    else
    {
        HAL_GPIO_WritePin(GP_LED0_GPIO_Port, GP_LED0_Pin, 0);
    }

    ledsOnOff = newPattern; //tracking which LEDs currently on once func ends

#endif  /*  STM32F4 */
}

/**
 * LEDs_Get() returns the current state of the LED bar.  Return values are 8-bit
 * patterns, where each bit describes the current state of the corresponding
 * LED. So, for example, if the LED bar's LED's are
 *
 * [ON OFF ON OFF   OFF ON OFF ON],
 *
 * LEDs_Get() should return 0xA5.
 * 
 * @return char:  An 8-bit value describing the current output on the LED bar.  
 * 
 * LEDs_Get() should not change the state of the LEDs, or any SFRs.
 */
char LEDs_Get(void) {
    char led_state = 0x00;
#ifdef STM32F4

    led_state = ledsOnOff;
  
#endif  /*  STM32F4 */
    return led_state;
}

