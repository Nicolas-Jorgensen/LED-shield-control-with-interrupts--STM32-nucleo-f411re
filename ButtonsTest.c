/**
 * @file    ButtonsTest.c
 * @author
 * @date
 */

// **** Include libraries here ****
// Standard libraries.
#include <stdio.h>

// Course libraries.
#include <BOARD.h>
#include <Timers.h>

#include "Buttons.h"
#include "stm32f4xx_hal.h"

// User libraries

// **** Set macros and preprocessor directives ****
#define TIMER_TIME 10
// **** Declare any datatypes here ****
struct Timer {
  uint8_t event;
  int16_t timeRemaining;
};
// **** Define global, module-level, or external variables here ****
volatile uint8_t button_state = 0;
volatile struct Timer TimerE = {FALSE, TIMER_TIME};
// **** Declare function prototypes ****


int main(void)
{
    BOARD_Init();
    Timers_Init();
    /***************************************************************************
     * Your code goes in between this comment and the following one with
     * asterisks.
     **************************************************************************/
    printf(
        "Welcome to CRUZID's Lab07, Part 4 (ButtonsTest). Compiled on %s %s.\n",
        __TIME__,
        __DATE__
    );
    
    printf("Please press some buttons!\n");
    printf("It's juicin' time.\n");
    while(1)
    {
        if (TimerE.event == TRUE)
        {
            TimerE.event = FALSE;
            printf("EVENT: 4:%s 3:%s 2:%s 1:%s\n",
           // Button 4 (bits 6-7)
           (button_state & BUTTON_EVENT_4DOWN) ? "DOWN" :
           (button_state & BUTTON_EVENT_4UP)   ? " UP " : "----",
           
           // Button 3 (bits 4-5)
           (button_state & BUTTON_EVENT_3DOWN) ? "DOWN" :
           (button_state & BUTTON_EVENT_3UP)   ? " UP " : "----",
           
           // Button 2 (bits 2-3)
           (button_state & BUTTON_EVENT_2DOWN) ? "DOWN" :
           (button_state & BUTTON_EVENT_2UP)   ? " UP " : "----",
           
           // Button 1 (bits 0-1)
           (button_state & BUTTON_EVENT_1DOWN) ? "DOWN" :
           (button_state & BUTTON_EVENT_1UP)   ? " UP " : "----");
        }
    }
    /***************************************************************************
     * Your code goes in between this comment and the preceding one with
     * asterisks.
     **************************************************************************/
    while (1);
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
    /***************************************************************************
     * Your code goes in between this comment and the following one with 
     * asterisks.
     **************************************************************************/
    if (--TimerE.timeRemaining <= 0)
    //only runs if button_state was assigned something other than BUTTON_EVENT_NONE
    {
        uint8_t temp = ButtonsCheckEvents();
        if(temp != button_state && temp != BUTTON_EVENT_NONE)
        {
            TimerE.event = TRUE;
            button_state = temp;
        }
        TimerE.timeRemaining = TIMER_TIME;
        
    }
    /***************************************************************************
     * Your code goes in between this comment and the preceding one with 
     * asterisks.
     **************************************************************************/
    }
}
