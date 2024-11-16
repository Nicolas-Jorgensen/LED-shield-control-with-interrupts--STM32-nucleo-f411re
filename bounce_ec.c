/**
 * @file    bounce_ec.c
 * @author
 * @date
 */

// **** Include libraries here ****
// Standard libraries.
#include <stdio.h>

// Course libraries.
#include <Adc.h>
#include <BOARD.h>
#include <Timers.h>

#include "Leds_Lab07.h"
#include "Buttons.h"
#include "stm32f4xx_hal.h"

// User libraries

// **** Set macros and preprocessor directives ****
#define TIMER_TICK      10
#define ADC_WINDOW_SIZE 13
#define SMOOTHNESS      0.2f //smoothing factor for ISR avg
#define ADC_UPPER_AREA  4092 //upper area with wonky values to avg to top val
#define ADC_LOWER_AREA  20    //lower wonky value limit
#define ADC_TOP_VAL     4096
#define ADC_BOT_VAL     0
#define SWITCH_1_UP     SW5_STATE()
#define ON              1
#define LED_0           0x01
#define LED_7           0x80
#define RIGHT           0
#define LEFT            1
#define LED_SPEED       40000
// **** Declare any datatypes here ****
typedef struct {
uint8_t event;
  int16_t timeRemaining;
}Timer;
// **** Define global, module-level, or external variables here ****
volatile Timer TimerL = {FALSE, TIMER_TICK};
volatile Timer TimerB = {FALSE, TIMER_TICK};
volatile uint16_t adc_val = 0;
volatile uint16_t avg_val = 0;
volatile uint8_t hasNewADC = FALSE;
volatile uint8_t buttonEvents = 0; 
uint8_t  ledToggle = ON;
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
    ADC_Init();
    ButtonsInit();
    printf(
        "Welcome to CRUZID's Lab07, Part 6 (extra credit!!)."
        "Compiled on %s %s.\n\r",
        __TIME__,
        __DATE__
    );
    
    while(1)
    {

        //Checking buttons first
        if(TimerB.event == TRUE)
        {
            //Button UP turns on LEDs
            if(SWITCH_1_UP)
            {
                if(buttonEvents & 
                  (BUTTON_EVENT_1UP +
                   BUTTON_EVENT_2UP +
                   BUTTON_EVENT_3UP +
                   BUTTON_EVENT_4UP))
                {
                    ledToggle ^= ON;
                }
            }
            //Button down turns on LEDS
            else
            {
                if(buttonEvents & 
                  (BUTTON_EVENT_1DOWN +
                   BUTTON_EVENT_2DOWN +
                   BUTTON_EVENT_3DOWN +
                   BUTTON_EVENT_4DOWN))
                {
                    ledToggle ^= ON;
                }

            }

            TimerB.event = FALSE;

        }
        //LED movement loop

        if(TimerL.event == TRUE && ledToggle == ON)
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

            TimerL.event = FALSE;
        }
    }
    /***************************************************************************
     * Your code goes in between this comment and the preceding one with
     * asterisks.
     **************************************************************************/
    while (1);
}


/**
 * This is the interrupt for the ADC1 peripheral. It will trigger whenever a new
 * ADC reading is available in the ADC buffer SFRs, ADC1BUF0 - ADC1BUF7.
 * 
 * It should not be called, and should communicate with main code only by using
 * module-level variables.
 */
void HAL_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    if (hadc == &hadc1) {
        //didn't know how to use this one
    }
}

void ADC_IRQHandler(void)
{
    uint16_t new_value = HAL_ADC_GetValue(&hadc1);
    
    // Apply moving average
    avg_val = (uint16_t)(SMOOTHNESS * new_value + (1.0 - SMOOTHNESS) * avg_val);
    
    if (avg_val > (adc_val + ADC_WINDOW_SIZE) || avg_val < (adc_val - ADC_WINDOW_SIZE))
    {
        adc_val = avg_val;
        hasNewADC = TRUE; //Set the flag to indicate a new value is available
    }
    //truncate top and bottom of values to max or min out
    else if (new_value > ADC_UPPER_AREA)
    {
        adc_val = ADC_TOP_VAL;
        hasNewADC = TRUE;
    }
    else if (new_value < ADC_LOWER_AREA)
    {
        adc_val = ADC_BOT_VAL;
        hasNewADC = TRUE;
    }

    HAL_ADC_Start_IT(&hadc1); //Start the next conversion

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
        //LED movement timer
        if(--TimerL.timeRemaining <= 0)
        {
                TimerL.event = TRUE;
                TimerL.timeRemaining = LED_SPEED / (adc_val + 2); //strobe time is set by adc val
        }                                                         //+10 to avoid division by zero

        //Button event timer
        if (--TimerB.timeRemaining <= 0)
        //only runs if button_state was assigned something other than BUTTON_EVENT_NONE
        {
            uint8_t temp = ButtonsCheckEvents();
            if(temp != buttonEvents && temp != BUTTON_EVENT_NONE)
            {
                TimerB.event = TRUE;
                buttonEvents = temp;
            }
            TimerB.timeRemaining = TIMER_TICK;
            
        }
        __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);



    /***************************************************************************
     * Your code goes in between this comment and the preceding one with 
     * asterisks.
     **************************************************************************/
    }
}
