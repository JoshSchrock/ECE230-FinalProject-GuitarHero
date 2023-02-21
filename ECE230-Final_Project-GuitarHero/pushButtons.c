/*! \file */
/*!
 * pushButons.c
 *
 * Description: Pushbutton driver for P1.1 and P1.4 on MSP432P4111 Launchpad
 *
 *  Created on: 12/26/2022
 *      Author: Josh Schrock
 */

#include "msp.h"
#include "pushButtons.h"
#include <stdbool.h>

volatile uint32_t weakDelay;

void BUTTONS_init(void) {
    BUTTON_PORT->DIR = BUTTON_PORT->DIR & ~BUTTON_ALL_PINS;
    BUTTON_PORT->SEL0 = BUTTON_PORT->SEL0 & ~BUTTON_ALL_PINS;
    BUTTON_PORT->SEL1 = BUTTON_PORT->SEL1 & ~BUTTON_ALL_PINS;
    BUTTON_PORT->OUT = BUTTON_PORT->OUT | BUTTON_ALL_PINS;
    BUTTON_PORT->REN = BUTTON_PORT->REN | BUTTON_ALL_PINS;
}

bool BUTTON1_pressed(void) {
    return (BUTTON_PORT->IN & BUTTON_1_PIN) == 0;
}

bool BUTTON2_pressed(void) {
    return (BUTTON_PORT->IN & BUTTON_2_PIN) == 0;
}

void BUTTON1_debounce(void) {
    for(weakDelay = 0x2000; weakDelay > 0; weakDelay--) {
    }
    if (BUTTON1_pressed()) {
        BUTTON1_debounce();
    }
}

void BUTTON2_debounce(void) {
    for(weakDelay = 0x2000; weakDelay > 0; weakDelay--) {
    }
    if (BUTTON2_pressed()) {
        BUTTON2_debounce();
    }
}

