/**
 * @file    bounce_switch.c
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
#include "Leds_Lab07.h"
// **** Set macros and preprocessor directives ****
#define STROBE_SLOW         1000
#define STROBE_FAST         250
#define BOTH_SWITCHES_ON    0x03
#define LED_0               0x01
#define LED_1               0x02
#define LED_2               0x04
#define LED_3               0x08
#define LED_4               0x10
#define LED_5               0x20
#define LED_6               0x40
#define LED_7               0x80
#define GO                  1
#define WAIT                0
#define RIGHT               0
#define LEFT                1
// **** Declare any datatypes here ****
struct Timer {
  uint8_t event;
  int16_t timeRemaining;
};
// **** Define global, module-level, or external variables here ****

volatile struct Timer TimerSW = {WAIT, STROBE_SLOW};
char LED_state = 0x00;
uint8_t direction = RIGHT;
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
   /* printf(
        "Welcome to CRUZID's Lab07, Part 2 (bounce_switch)."
        "Compiled on %s %s.\n\r",
        __TIME__,
        __DATE__
    );
    printf("flip some switches!\n"); */
    while(1)    
    {
        if (TimerSW.event == GO)
        {   
            if(LED_state == 0x00) //no LEDs are on yet
            {
                LEDs_Set(LED_0);
                LED_state = LEDs_Get();
            }
            else if(direction == RIGHT)
            {
                if(LED_state != LED_7) //not on rightmost LED
                {
                    LEDs_Set(LEDs_Get() << 1);
                    LED_state = LEDs_Get();
                }
                else
                {
                    direction = LEFT;
                    LEDs_Set(LEDs_Get() >> 1);
                    LED_state = LEDs_Get();
                }
            }
            else if (direction == LEFT)
            {
                if(LED_state != LED_0) //not on leftmost LED
                {
                    LEDs_Set(LEDs_Get() >> 1);
                    LED_state = LEDs_Get();
                }
                else
                {
                    direction = RIGHT;
                    LEDs_Set(LEDs_Get() << 1);
                    LED_state = LEDs_Get();
                }
            }
            TimerSW.event = WAIT;
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
        TimerSW.timeRemaining--;
        if(TimerSW.timeRemaining <= 0 && (SWITCH_STATES()) == BOTH_SWITCHES_ON)
        {
                TimerSW.event = GO;
                TimerSW.timeRemaining = STROBE_FAST;
        }
        else if((TimerSW.timeRemaining) <= 0) //keep it slow if Switch_States )&& !(SWITCH_STATES())
        {
                TimerSW.event = GO;
                TimerSW.timeRemaining = STROBE_SLOW;
        } 
        
        __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);
    /***************************************************************************
     * Your code goes in between this comment and the preceding one with 
     * asterisks.
     **************************************************************************/
    }
}
