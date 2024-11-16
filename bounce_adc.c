/**
 * @file    bounce_adc.c
 * @author
 * @date
 */

// **** Include libraries here ****
// Standard libraries.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Course libraries.
#include <BOARD.h>
#include <Adc.h>
#include <Oled.h>
#include <OledDriver.h>
// User libraries


// **** Set macros and preprocessor directives ****
#define ADC_WINDOW_SIZE 13
#define SMOOTHNESS 0.2f //smoothing factor for ISR avg
#define ADC_UPPER_AREA 4092 //upper area with wonky values to avg to top val
#define ADC_LOWER_AREA 4    //lower wonky value limit
#define ADC_TOP_VAL    4096
#define ADC_BOT_VAL    0
#define OLED_WINDOW_1000_SPACE 1000
#define OLED_WINDOW_100_SPACE  100
#define OLED_WINDOW_10_SPACE   10
// **** Declare any datatypes here ****

// **** Define global, module-level, or external variables here ****
volatile uint16_t adc_val = 0;
volatile uint16_t avg_val = 0;
volatile uint8_t hasNewADC = FALSE;
// **** Declare function prototypes ****


int main(void)
{
    BOARD_Init();
    ADC_Init();
    
    OledInit();
    printf(
        "Welcome to CRUZID's Lab07, Part 3 (bounce_adc)."
        "Compiled on %s %s.\n\r",
        __TIME__,
        __DATE__
    );
    char adc_val_string[20];
    char str_perc [5];
    while (1)
    {
        if (hasNewADC) //Wait until the ISR signals that we have something new to report
        {
            hasNewADC = FALSE; //Reset the flag so that we don't recycle the same measurement
            
            uint8_t perc = (adc_val*1.0f/ADC_TOP_VAL)*100;

            itoa(perc, str_perc, 10);
            itoa(adc_val, adc_val_string, 10); //converts int to string for OLEDdraw

            str_perc[3] = '%';
            str_perc[4] = '\0'; //null terminating

            if (adc_val < OLED_WINDOW_10_SPACE)
            {
                adc_val_string[1] = ' '; //unused num space becomes whitespace
            } 
            if (adc_val < OLED_WINDOW_100_SPACE)
            {
                adc_val_string[2] = ' '; 
            }
            if (adc_val < OLED_WINDOW_1000_SPACE)
            {
                adc_val_string[3] = ' '; 
            }
            
            adc_val_string[4] = ' ';
            adc_val_string[5] = ' ';
            adc_val_string[6] = ' ';
            adc_val_string[7] = '\0';

            if (perc < OLED_WINDOW_10_SPACE)
            {
                str_perc[1] = ' ';
            }
            if (perc < OLED_WINDOW_100_SPACE)
            {
                str_perc[2] = ' ';
            }
            
            strcat(adc_val_string, str_perc);

            OledDrawString(adc_val_string);
            OledUpdate();
        }
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

