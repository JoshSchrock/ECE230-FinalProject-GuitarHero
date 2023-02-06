/*! \file */
/******************************************************************************
 * Final Project: Guitar Hero
 *
 * Description: A guitar hero game.
 * Authors: Josh Schrock and Lisa Sebastian
 * Last-modified: 2/6/2023
 *
 *
*******************************************************************************/
#include "msp.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "csHFXT.h"
#include "csLFXT.h"
#include "pushButtons.h"
#include "lcd.h"

//clock define
#define CLK_FREQUENCY       48000000    // MCLK using 48MHz HFXT

/* note defines */
#define NOTENONE 90.55
#define NOTEE3 164.81
#define NOTEF3 174.61
#define NOTEF3SHARP 185.00
#define NOTEG3 196.00
#define NOTEG3SHARP 207.65
#define NOTEA3 220.00
#define NOTEA3SHARP 233.08
#define NOTEB3 246.94
#define NOTEC4 261.63
#define NOTEC4SHARP 277.18
#define NOTED4 293.66
#define NOTED4SHARP 311.13
#define NOTEE4 329.63
#define NOTEF4 349.23
#define NOTEF4SHARP 369.99
#define NOTEG4 392.00
#define NOTEG4SHARP 415.30
#define NOTEA4 440.00
#define NOTEA4SHARP 466.16
#define NOTEB4 493.88
#define NOTEC5 523.25
#define NOTEC5SHARP 554.37
#define NOTED5 587.33

/* length defines */
#define NOTEREST 1000
//#define SIXTEENTHNOTE 3278 //1024
#define SIXTEENNOTE 3096
#define EIGHTNOTE 7192
#define SIXTIENOTE 11288
#define QUARTERNOTE 15384
#define TIENOTE 23576
#define HALFNOTE 31768
#define WHOLENOTE 64536

/* song defines */
#define NOTECNT 90
const uint16_t noteArray[NOTECNT] =   {NOTEG4,      NOTENONE,  NOTED4,    NOTEG4,      NOTENONE,  NOTED4,      NOTEG4,    NOTED4,    NOTEG4,    NOTEB4,    NOTED5,      NOTENONE,  NOTEC5,      NOTENONE,  NOTEA4,    NOTEC5,      NOTENONE,  NOTEA4,    NOTEC5,    NOTEA4,    NOTEF4SHARP, NOTEA4,    NOTED4,      NOTENONE,    NOTEG4,    NOTENONE,  NOTEG4,  NOTEB4,    NOTEA4,    NOTEG4,    NOTEG4,    NOTEF4SHARP, NOTEF4SHARP, NOTEG4,    NOTEC5,    NOTEF4SHARP, NOTEA4,    NOTEG4,    NOTEG4,  NOTEB4,    NOTEA4,    NOTEG4,    NOTEG4,    NOTEF4SHARP, NOTEF4SHARP, NOTEA4,    NOTEC5,    NOTEF4SHARP, NOTEG4,    NOTEG4,    NOTEG4,    NOTEF4SHARP, NOTEE4,    NOTEF4SHARP, NOTEG4,    NOTEG4,    NOTEB4,    NOTEA4,    NOTEG4,    NOTEA4,    NOTEB4,    NOTEB4,    NOTED5,    NOTEC5,    NOTEB4,    NOTEC5,    NOTED5,      NOTENONE,    NOTED4,   NOTEE4,   NOTED4,    NOTEC4,     NOTEC4,      NOTEC4,    NOTEB3,     NOTEB3,      NOTEB3,    NOTEA3,     NOTEA3,      NOTEG3,    NOTEF3SHARP, NOTEE3,    NOTEF3SHARP, NOTEG3,    NOTENONE,  NOTEA3,    NOTENONE,  NOTEB3,    NOTENONE,  NOTENONE};
const uint16_t lengthArray[NOTECNT] = {QUARTERNOTE, EIGHTNOTE, EIGHTNOTE, QUARTERNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, QUARTERNOTE, QUARTERNOTE, QUARTERNOTE, EIGHTNOTE, EIGHTNOTE, QUARTERNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE,   EIGHTNOTE, QUARTERNOTE, QUARTERNOTE, EIGHTNOTE, EIGHTNOTE, TIENOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE,   TIENOTE,     EIGHTNOTE, EIGHTNOTE, EIGHTNOTE,   EIGHTNOTE, EIGHTNOTE, TIENOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE,   TIENOTE,     EIGHTNOTE, EIGHTNOTE, EIGHTNOTE,   EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE,   EIGHTNOTE, EIGHTNOTE,   EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, QUARTERNOTE, QUARTERNOTE, HALFNOTE, HALFNOTE, EIGHTNOTE, SIXTIENOTE, QUARTERNOTE, EIGHTNOTE, SIXTIENOTE, QUARTERNOTE, EIGHTNOTE, SIXTIENOTE, QUARTERNOTE, EIGHTNOTE, EIGHTNOTE,   EIGHTNOTE, EIGHTNOTE,   EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, QUARTERNOTE};
typedef enum _GuitarButtons {
    BONE,        /* (0x0) */
    BTWO,        /* (0x1) */
    BTHREE,      /* (0x2) */
    BFOUR        /* (0x3) */
} GuitarButtons;
bool buttonExpexted[4] = {false, false, false, false}
/* globals */
int noteIndex = 0;


/**
 * main.c
 */
void main(void)
{
    volatile bool playing = false;

    /* Stop Watchdog timer */
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    /* Sets MCLK and SMCLK to 48MHz HFXT */
    configHFXT();
    configLFXT();
    BUTTONS_init();
    // configures pins and delay library
    configLCD(CLK_FREQUENCY);

    /* Configure GPIO for speaker */
    P2->DIR |= BIT4;            // set P2.4 as output
    P2->SEL0 |= BIT4;           // P2.4 set to TA0.1
    P2->SEL1 &= ~BIT4;

    /* Configure Timer_A0 and CCRs */
    // Set Period in CCR0 register
    TIMER_A0->CCR[0] = (6005000 / (noteArray[noteIndex] + 1)) - 1;
    // Set high pulse-width in CCR1 register (determines duty cycle)
    TIMER_A0->CCR[1] = ((6005000 / (noteArray[noteIndex]) + 1) / 2) - 1;
    // Configure CCR1 for Compare mode with Set/Reset output
    //          - sets when Timer_A0 == CCR1
    //          - resets when Timer_A0 == CCR0
    TIMER_A0->CCTL[1] = 0x60;
    // Configure Timer_A0 in Up Mode with source SMCLK prescale 8:1
    //      Tick rate will be (48MHz/8) with rollover at value in CCR0

    TIMER_A0->CTL = 0x02C4;

    /* Configure Timer_A1 and CCRs */
    // Set initial period in CCR2 register. This assumes timer starts at 0
    TIMER_A1->CCR[2] = lengthArray[noteIndex];
    // Configure CCR2 for Compare mode with interrupt enabled (no output mode - 0)
    TIMER_A1->CCTL[2] = 0x0010;

    // Set initial period in CCR3 register. This assumes timer starts at 0
    TIMER_A1->CCR[3] = lengthArray[noteIndex] + NOTEREST;
    // Configure CCR3 for Compare mode with interrupt enabled (no output mode - 0)
    TIMER_A1->CCTL[3] = 0x0010;

    // Configure Timer_A1 in Continuous Mode with source ACLK prescale 1:1 and
    //  interrupt enabled
    //      Tick rate will be 32kHz with rollover at 0xFFFF
    TIMER_A1->CTL = 0x0106;

    /* Configure global interrupts and NVIC */
    // Enable TA1 overflow and TA1CCR2 compare interrupt
    NVIC->ISER[0] |= 1 << TA1_N_IRQn;


    __enable_irq();                 // Enable global interrupt

    while(1)
    {
        if(BUTTON2_pressed()) {
            if(playing){
                TIMER_A0->CTL &= ~0x0010;
                TIMER_A1->CTL &= ~0x0020;
                BUTTON2_debounce();
                playing = false;
            }else{
                BUTTON2_debounce();
                TIMER_A0->CTL |= 0x0010;
                TIMER_A1->CTL |= 0x0020;
                playing = true;
            }
        }
    }
}

/* Timer_A1 and CCRx (except CCR0) interrupt service routine */
void TA1_N_IRQHandler(void)
{
    /* Check if interrupt triggered by timer overflow */
    if(TIMER_A1->CTL & TIMER_A_CTL_IFG)
    {
        // Clear timer overflow flag
        // clear flag in TA1CTL
        TIMER_A1->CTL = TIMER_A1->CTL & 0xFFFE;
    }

    /* Check if interrupt triggered by CCR2 */
    if(TIMER_A1->CCTL[2] & TIMER_A_CCTLN_CCIFG)
    {
        /* add note changing code (update compare registers) */
        TIMER_A0->CTL &= ~0x0010;
        TIMER_A0->CCTL[1] &= ~0x0004;

        // Clear CCR2 compare interrupt flag
        // clear flag in TA1CCTL2
        TIMER_A1->CCTL[2] = TIMER_A1->CCTL[2] & 0xFFFE;
    }

    /* Check if interrupt triggered by CCR2 */
    if(TIMER_A1->CCTL[3] & TIMER_A_CCTLN_CCIFG)
    {
        noteIndex = (noteIndex + 1) % NOTECNT;
        /* add note changing code (update compare registers) */
        TIMER_A0->CTL |= 0x0010;
        TIMER_A0->CCR[0] = (6005000 / (noteArray[noteIndex] + 1)) - 1;
        TIMER_A0->CCR[1] = ((6005000 / (noteArray[noteIndex]) + 1) / 2) - 1;

        // Clear CCR2 compare interrupt flag
        // clear flag in TA1CCTL2
        TIMER_A1->CCTL[3] = TIMER_A1->CCTL[3] & 0xFFFE;

        // Schedule next interrupt interval
        TIMER_A1->CCR[2] = (TIMER_A1->CCR[3] + lengthArray[noteIndex]) % 0xFFFF;
        TIMER_A1->CCR[3] = (TIMER_A1->CCR[3] + lengthArray[noteIndex] + NOTEREST) % 0xFFFF;

    }
}
