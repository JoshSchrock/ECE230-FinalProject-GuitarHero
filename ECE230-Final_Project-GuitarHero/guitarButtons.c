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

//globals
bool GB1active = true;
bool GB2active = true;
bool GB3active = true;
bool GB4active = true;

void GB_init(void) {
    GB_PORT->DIR = GB_PORT->DIR & ~GB_ALL_PINS;
    GB_PORT->SEL0 = GB_PORT->SEL0 & ~GB_ALL_PINS;
    GB_PORT->SEL1 = GB_PORT->SEL1 & ~GB_ALL_PINS;
    GB_PORT->OUT = GB_PORT->OUT | GB_ALL_PINS;
    GB_PORT->REN = GB_PORT->REN | GB_ALL_PINS;

    TIMER_A2->CCTL[1] = 0x0000;

    TIMER_A2->CCTL[2] = 0x0000;

    TIMER_A2->CCTL[3] = 0x0000;

    TIMER_A2->CCTL[4] = 0x0000;

    // Configure Timer_A1 in Continuous Mode with source ACLK prescale 1:1 and
    //  interrupt enabled
    //      Tick rate will be 32kHz with rollover at 0xFFFF
    TIMER_A2->CTL = 0x0126;

    NVIC->ISER[0] |= 1 << TA2_N_IRQn;
}

bool GB1_pressed(void) {
    if (!GB1active) {
        return false;
    } else {
        return (GB_PORT->IN & GB_1_PIN) == 0;
    }
}

bool GB2_pressed(void) {
    if (!GB2active) {
        return false;
    } else {
        return (GB_PORT->IN & GB_2_PIN) == 0;
    }
}

bool GB3_pressed(void) {
    if (!GB3active) {
        return false;
    } else {
        return (GB_PORT->IN & GB_3_PIN) == 0;
    }
}

bool GB4_pressed(void) {
    if (!GB4active) {
        return false;
    } else {
        return (GB_PORT->IN & GB_4_PIN) == 0;
    }
}

void GB1_debounce(void) {
    GB1active = false;
    TIMER_A2->CCR[1] = TIMER_A2->R + 200;
    // Configure CCR2 for Compare mode with interrupt enabled (no output mode - 0)
    TIMER_A2->CCTL[1] = 0x0010;
}

void GB2_debounce(void) {
    GB2active = false;
    TIMER_A2->CCR[2] = TIMER_A2->R + 200;
    // Configure CCR2 for Compare mode with interrupt enabled (no output mode - 0)
    TIMER_A2->CCTL[2] = 0x0010;
}

void GB3_debounce(void) {
    GB3active = false;
    TIMER_A2->CCR[3] = TIMER_A2->R + 200;
    // Configure CCR3 for Compare mode with interrupt enabled (no output mode - 0)
    TIMER_A2->CCTL[3] = 0x0010;
}

void GB4_debounce(void) {
    GB4active = false;
    TIMER_A2->CCR[4] = TIMER_A2->R + 200;
    // Configure CCR3 for Compare mode with interrupt enabled (no output mode - 0)
    TIMER_A2->CCTL[4] = 0x0010;
}


/* Timer_A2 and CCRx (except CCR0) interrupt service routine */
void TA2_N_IRQHandler(void)
{
    /* Check if interrupt triggered by timer overflow */
    if(TIMER_A2->CTL & TIMER_A_CTL_IFG)
    {
        // Clear timer overflow flag
        // clear flag in TA1CTL
        TIMER_A2->CTL = TIMER_A2->CTL & 0xFFFE;
    }

    /* Check if interrupt triggered by CCR2 */
    if(TIMER_A2->CCTL[1] & TIMER_A_CCTLN_CCIFG)
    {
        TIMER_A2->CCTL[1] = 0x0000;

        GB1active = true;
        if (GB1_pressed()) {
            GB1_debounce();
        }
    }

    /* Check if interrupt triggered by CCR2 */
    if(TIMER_A2->CCTL[2] & TIMER_A_CCTLN_CCIFG)
    {
        TIMER_A2->CCTL[2] = 0x0000;

        GB2active = true;
        if (GB2_pressed()) {
            GB2_debounce();
        }
    }

    /* Check if interrupt triggered by CCR3 */
    if(TIMER_A2->CCTL[3] & TIMER_A_CCTLN_CCIFG)
    {
        TIMER_A2->CCTL[3] = 0x0000;

        GB3active = true;
        if (GB3_pressed()) {
            GB3_debounce();
        }
    }

    /* Check if interrupt triggered by CCR4 */
    if(TIMER_A2->CCTL[4] & TIMER_A_CCTLN_CCIFG)
    {
        TIMER_A2->CCTL[4] = 0x0000;

        GB4active = true;
        if (GB4_pressed()) {
            GB4_debounce();
        }
    }
}

