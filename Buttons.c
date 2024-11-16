/**
 * @file    Buttons.c
 * @author  Nicolas Jorgensen njorgen1@ucsc.edu
 * @date    11/15/2024
 */

#include "Buttons.h"

void ButtonsInit(void)
{
    //would normally initialize GPIO pins for buttons
}

uint8_t ButtonsCheckEvents(void) {
    static uint8_t button_states = BUTTON_EVENT_NONE;    // Only lower 4 bits used
    static uint8_t button_prv_state = BUTTON_EVENT_NONE; // Only lower 4 bits used
    static uint8_t debounce_state = BUTTON_EVENT_NONE;   // prev 4bit button state val
    static uint8_t debounce_counter = 0;
    uint8_t button_changes = BUTTON_EVENT_NONE;          // All 8 bits used for up/down events
    
    // Get current button states (only lower 4 bits are valid)
    uint8_t current_buttons = BUTTON_STATES() & 0x0F;  // Mask to ensure only lower 4 bits
    
    // Check if any button state is different from our debouncing state
    if (current_buttons != debounce_state) {
        debounce_counter++;
        
        // If we've seen the same new state for long enough
        if (debounce_counter >= BUTTONS_DEBOUNCE_PERIOD) {
            // Update the stable state for all buttons at once
            button_states = current_buttons;
            debounce_counter = 0;
            debounce_state = current_buttons;
        }
    } else {
        // Same state, reset or decrease counter
        if (debounce_counter > 0) {
            debounce_counter--;
        }
    }
    
    // If no change in stable state, return no events
    if (button_states == button_prv_state) {
        return BUTTON_EVENT_NONE;
    }
    
    // Process all button changes at once
    // Check each button position and set appropriate up/down bits
    
    // Button 1 (bits 0-1: 01 for down, 10 for up)
    if ((button_states & BUTTON_STATE_1) != (button_prv_state & BUTTON_STATE_1)) {
        button_changes |= (button_states & BUTTON_STATE_1) ? 
            BUTTON_EVENT_1DOWN : BUTTON_EVENT_1UP;
    }
    
    // Button 2 (bits 2-3: 04 for down, 08 for up)
    if ((button_states & BUTTON_STATE_2) != (button_prv_state & BUTTON_STATE_2)) {
        button_changes |= (button_states & BUTTON_STATE_2) ? 
            BUTTON_EVENT_2DOWN : BUTTON_EVENT_2UP;
    }
    
    // Button 3 (bits 4-5: 10 for down, 20 for up)
    if ((button_states & BUTTON_STATE_3) != (button_prv_state & BUTTON_STATE_3)) {
        button_changes |= (button_states & BUTTON_STATE_3) ? 
            BUTTON_EVENT_3DOWN : BUTTON_EVENT_3UP;
    }
    
    // Button 4 (bits 6-7: 40 for down, 80 for up)
    if ((button_states & BUTTON_STATE_4) != (button_prv_state & BUTTON_STATE_4)) {
        button_changes |= (button_states & BUTTON_STATE_4) ? 
            BUTTON_EVENT_4DOWN : BUTTON_EVENT_4UP;
    }
    
    button_prv_state = button_states;

    return button_changes;
} 