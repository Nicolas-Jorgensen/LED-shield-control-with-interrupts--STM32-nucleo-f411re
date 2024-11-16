/*
 * @file    BOARD.h
 * @author  Adam Korycki
 * @author  nubby (jlee211@ucsc.edu)
 *
 * @date    3 Sep 2023
 */

#ifndef BOARD_H
#define	BOARD_H

#include <stdint.h>
#ifdef STM32F4
#include "stm32f4xx_hal.h"
#endif  /*  STM32F4 */

// Boolean defines for TRUE, FALSE, SUCCESS and ERROR
#ifndef FALSE
#define FALSE ((int8_t) 0)
#define TRUE ((int8_t) 1)
#endif
#ifndef ERROR
#define ERROR ((int8_t) -1)
#endif
// SUCCESS is defined and used by some of the framework files, so this is
// safest.
#ifndef SUCCESS
#define SUCCESS ((int8_t) 1)
#endif

// More specific standard error definitions.
enum {
  SIZE_ERROR = -1,
  STANDARD_ERROR
};


/* minimum GPIO pin assocations for NUCLEO64*/
/*
#define B1_Pin GPIO_PIN_13 // onboard blue push button
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2 // USB-UART (USART2)
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
*/
#define USART2_TTL_TX_Pin GPIO_PIN_2
#define USART2_TTL_TX_GPIO_Port GPIOA
#define USART2_TTL_RX_Pin GPIO_PIN_3
#define USART2_TTL_RX_GPIO_Port GPIOA
#define NUCLEO_LED_Pin GPIO_PIN_5
#define NUCLEO_LED_GPIO_Port GPIOA
#define GP_IN3_Pin GPIO_PIN_6
#define GP_IN3_GPIO_Port GPIOA
#define GP_IN4_Pin GPIO_PIN_7
#define GP_IN4_GPIO_Port GPIOA
#define GP_IN0_Pin GPIO_PIN_4
#define GP_IN0_GPIO_Port GPIOC
#define GP_IN1_Pin GPIO_PIN_5
#define GP_IN1_GPIO_Port GPIOC
#define GP_LED4_Pin GPIO_PIN_0
#define GP_LED4_GPIO_Port GPIOB
#define GP_LED5_Pin GPIO_PIN_1
#define GP_LED5_GPIO_Port GPIOB
#define GP_LED6_Pin GPIO_PIN_2
#define GP_LED6_GPIO_Port GPIOB
#define GP_LED0_Pin GPIO_PIN_8
#define GP_LED0_GPIO_Port GPIOC
#define GP_LED1_Pin GPIO_PIN_9
#define GP_LED1_GPIO_Port GPIOC
#define GP_LED2_Pin GPIO_PIN_10
#define GP_LED2_GPIO_Port GPIOC
#define GP_LED3_Pin GPIO_PIN_11
#define GP_LED3_GPIO_Port GPIOC
#define GP_IN2_Pin GPIO_PIN_12
#define GP_IN2_GPIO_Port GPIOC
#define GP_LED7_Pin GPIO_PIN_3
#define GP_LED7_GPIO_Port GPIOB


/**
 * Helper macro definitions.
 */
#define SW5_STATE() HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)
#define SW6_STATE() HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)
#define SWITCH_STATES() (((SW5_STATE() << 1) + SW6_STATE()) & 0x03)

/**
 * @function BOARD_Init(void)
 * @param None
 * @return None
 * @brief Set the clocks up for the board, initializes the serial port, and turns on the A/D
 *        subsystem for battery monitoring
 * @author Adam Korycki, 2023.09.03  */
extern void BOARD_Init();


/**
 * @function BOARD_End(void)
 * @param None
 * @return None
 * @brief Shuts down all peripherals except for serial and A/D. Turns all pins into input
 * @author Adam Korycki, 2023.09.03  */
extern void BOARD_End();

/**
 * @function BUTTON_STATES(void)
 * @param None
 * @return uint8_t
 *
 * Provides a way to quickly get the status of all 4 push-buttons into 4 bits:
 *  + 0 = button is released.
 *  + 1 = button is pressed.
 *
 *  For example:
 *
 *  uint8_t buttonsState = BUTTON_STATES();
 *  if (buttonsState & BUTTON_STATE_3) {
 *    // Button 3 is pressed down.
 *  }
 */
uint8_t BUTTON_STATES(void);


#endif	/* BOARD_H */

