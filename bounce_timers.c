/**
 * @file    bounce_timers.c
 * @author  Nicolas Jorgensen njorgen1@ucsc.edu
 * @date
 */

// **** Include libraries here ****
// Standard libraries.
#include <stdio.h>
#include <stdint.h>

// Course libraries.
#include <BOARD.h>
#include <Timers.h>

#include "Buttons.h"
#include "stm32f4xx_hal.h"

// User libraries
#include "Leds_Lab07.h"

// **** Set macros and preprocessor directives ****
#define TIMER_A_PERIOD 2000  // 2 seconds
#define TIMER_B_PERIOD 3000  // 3 seconds
#define TIMER_C_PERIOD 5000  // 5 seconds
#define LED_0_ON 0x01
#define LED_1_ON 0x02
#define LED_2_ON 0x04
// **** Declare any datatypes here ****
struct Timer {
  uint8_t event;
  int16_t timeRemaining;
};

// **** Define global, module-level, or external variables here ****
volatile struct Timer TimerA = {FALSE, TIMER_A_PERIOD};
volatile struct Timer TimerB = {FALSE, TIMER_B_PERIOD};
volatile struct Timer TimerC = {FALSE, TIMER_C_PERIOD};

// **** Declare function prototypes ****


int main(void)
{
    BOARD_Init();
    Timers_Init();
    /***************************************************************************
     * Your code goes in between this comment and the following one with
     * asterisks.
     **************************************************************************/
    LEDs_Init();
    printf(
        "Welcome to CRUZID's Lab07, Part 1 (bouce_timers)."
        "Compiled on %s %s.\n\r",
        __TIME__,
        __DATE__
    );
    
    

    /***************************************************************************
     * Your code goes in between this comment and the preceding one with
     * asterisks.
     **************************************************************************/
    while (1) {
        // Poll "Timer A".
        // React to "Timer A" events.
        // Clear "Timer A" event flag.
        if(TimerA.event == TRUE)
        {
            TimerA.event = FALSE;
            printf("A\n");
            LEDs_Set(LEDs_Get() ^ LED_0_ON); //flipping bit corresponding to LED0
            //should maintain other LEDs
            
        }
        if(TimerB.event == TRUE)
        {
            TimerB.event = FALSE;
            printf("B\n");
            LEDs_Set(LEDs_Get() ^ LED_1_ON); //flipping bit corresponding to LED1
        }
       
        if(TimerC.event == TRUE)
        {
            TimerC.event = FALSE;
            printf("C\n");
            LEDs_Set(LEDs_Get() ^ LED_2_ON); //flipping bit corresponding to LED2
        }
    }
}

/**
 * This is the interrupt for the Timer2 peripheral. It should check for button
 * events and store them in a module-level variable.
 * 
 * You should not modify this function for ButtonsTest.c or bounce_buttons.c!
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim == &htim2) {
 
        if(--TimerA.timeRemaining <= 0)
        {
            TimerA.event = TRUE;
            TimerA.timeRemaining = TIMER_A_PERIOD;
        }

        if(--TimerB.timeRemaining <= 0)
        {
            TimerB.event = TRUE;
            TimerB.timeRemaining = TIMER_B_PERIOD;
        }

        if(--TimerC.timeRemaining <= 0)
        {
            TimerC.event = TRUE;
            TimerC.timeRemaining = TIMER_C_PERIOD;
        }

        __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);
    }
}
