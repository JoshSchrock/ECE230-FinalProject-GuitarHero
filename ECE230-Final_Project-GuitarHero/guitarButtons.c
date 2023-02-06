/*! \file */
/*!
 * GuitarButtons.c
 *
 * Description: GuitarButton driver for P3.0 - P3.4 on MSP432P4111 Launchpad
 *
 *  Created on: 2/6/2022
 *      Author: Josh Schrock
 */

#include "msp.h"
#include "guitarButtons.h"
#include <stdbool.h>

volatile uint32_t weakDelay;

void GB_init(void) {
    GB_PORT->DIR = GB_PORT->DIR & ~GB_ALL_PINS;
    GB_PORT->SEL0 = GB_PORT->SEL0 & ~GB_ALL_PINS;
    GB_PORT->SEL1 = GB_PORT->SEL1 & ~GB_ALL_PINS;
    GB_PORT->OUT = GB_PORT->OUT | GB_ALL_PINS;
    GB_PORT->REN = GB_PORT->REN | GB_ALL_PINS;
}

bool GB1_pressed(void) {
    return (GB_PORT->IN & GB_1_PIN) == 0;
}

bool GB2_pressed(void) {
    return (GB_PORT->IN & GB_2_PIN) == 0;
}

bool GB3_pressed(void) {
    return (GB_PORT->IN & GB_3_PIN) == 0;
}

bool GB4_pressed(void) {
    return (GB_PORT->IN & GB_4_PIN) == 0;
}

void GB1_debounce(void) {
    for(weakDelay = 0x1400; weakDelay > 0; weakDelay--) {
    }
    if (GB1_pressed()) {
        GB1_debounce();
    }
}

void GB2_debounce(void) {
    for(weakDelay = 0x1400; weakDelay > 0; weakDelay--) {
    }
    if (GB2_pressed()) {
        GB2_debounce();
    }
}

void GB3_debounce(void) {
    for(weakDelay = 0x1400; weakDelay > 0; weakDelay--) {
    }
    if (GB3_pressed()) {
        GB3_debounce();
    }
}

void GB4_debounce(void) {
    for(weakDelay = 0x1400; weakDelay > 0; weakDelay--) {
    }
    if (GB4_pressed()) {
        GB4_debounce();
    }
}

