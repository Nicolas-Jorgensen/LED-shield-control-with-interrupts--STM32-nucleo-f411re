/**
 * @file    bounce_buttons.c
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
#define TIMER_TICK      10
#define SWITCH_1_UP SW5_STATE()
#define LED1_AND_2_ON   0x03
#define LED3_AND_4_ON   0x0C
#define LED5_AND_6_ON   0x30
#define LED7_AND_8_ON   0xC0
// **** Declare any datatypes here ****
struct Timer {
  uint8_t event;
  int16_t timeRemaining;
};
// **** Define global, module-level, or external variables here ****
volatile uint8_t buttonEvents; //Filled in by the Timer's ISR
volatile struct Timer TimerF = {FALSE,TIMER_TICK};

// **** Declare function prototypes ****


int main(void)
{
    BOARD_Init();
    Timers_Init();
   
    LEDs_Init();
    ButtonsInit(); //pointless rn
    printf(
        "Welcome to CRUZID's Lab07, Part 5 (bounce_buttons)."
        "Compiled on %s %s.\n\r",
        __TIME__,
        __DATE__
    );
    uint8_t LEDs_to_set = 0x00;

    while(1)
    {
        
        if(TimerF.event == TRUE)
        {
            //Button UP turns on LEDs
            if(SWITCH_1_UP)
            {
                if(buttonEvents & BUTTON_EVENT_1UP)
                {   //LEDs 1 & 2 aren't on
                    //toggle em with XOR
                        LEDs_to_set ^= LED1_AND_2_ON;
                }
                if(buttonEvents & BUTTON_EVENT_2UP)
                {
                    LEDs_to_set ^= LED3_AND_4_ON;
                }
                if(buttonEvents & BUTTON_EVENT_3UP)
                {
                    LEDs_to_set ^= LED5_AND_6_ON;
                }
                if(buttonEvents & BUTTON_EVENT_4UP)
                {
                    LEDs_to_set ^= LED7_AND_8_ON;
                }
                //send out all toggle instructions at once
                LEDs_Set(LEDs_to_set);
                
            }
            //Button down turns on LEDS
            else
            {
                if(buttonEvents & BUTTON_EVENT_1DOWN)
                {   //LEDs 1 & 2 aren't on
                    //toggle em with XOR
                        LEDs_to_set ^= LED1_AND_2_ON;
                }
                if(buttonEvents & BUTTON_EVENT_2DOWN)
                {
                    LEDs_to_set ^= LED3_AND_4_ON;
                }
                if(buttonEvents & BUTTON_EVENT_3DOWN)
                {
                    LEDs_to_set ^= LED5_AND_6_ON;
                }
                if(buttonEvents & BUTTON_EVENT_4DOWN)
                {
                    LEDs_to_set ^= LED7_AND_8_ON;
                }
                //send out all toggle instructions at once
                LEDs_Set(LEDs_to_set);

            }

            TimerF.event = FALSE;

        }
    }
   
    while (1);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim == &htim2) {
    /***************************************************************************
     * Your code goes in between this comment and the following one with 
     * asterisks.
     **************************************************************************/
     if (--TimerF.timeRemaining <= 0)
    //only runs if button_state was assigned something other than BUTTON_EVENT_NONE
    {
        uint8_t temp = ButtonsCheckEvents();
        if(temp != buttonEvents && temp != BUTTON_EVENT_NONE)
        {
            TimerF.event = TRUE;
            buttonEvents = temp;
        }
        TimerF.timeRemaining = TIMER_TICK;
        
    }
    /***************************************************************************
     * Your code goes in between this comment and the preceding one with 
     * asterisks.
     **************************************************************************/
    }
}
