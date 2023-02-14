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
#include "guitarButtons.h"
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
#define SIXTEENNOTE 3096
#define EIGHTNOTE 7192
#define SIXTIENOTE 11288
#define QUARTERNOTE 15384
#define TIENOTE 23576
#define HALFNOTE 31768
#define WHOLENOTE 64536

// guitar button defines
#define SINGLENOTETIME 6000

//define game
#define INITHEALTH     9

typedef enum _GuitarButtons {
    BNONE,       /* (0x0) */
    BONE,        /* (0x1) */
    BTWO,        /* (0x2) */
    BTHREE,      /* (0x3) */
    BFOUR        /* (0x4) */
} GuitarButtons;

/* song defines */
#define NOTECNT 90
const uint16_t noteArray[NOTECNT] =   {NOTEG4,      NOTENONE,  NOTED4,    NOTEG4,      NOTENONE,  NOTED4,      NOTEG4,    NOTED4,    NOTEG4,    NOTEB4,    NOTED5,      NOTENONE,  NOTEC5,      NOTENONE,  NOTEA4,    NOTEC5,      NOTENONE,  NOTEA4,    NOTEC5,    NOTEA4,    NOTEF4SHARP, NOTEA4,    NOTED4,      NOTENONE,    NOTEG4,    NOTENONE,  NOTEG4,  NOTEB4,    NOTEA4,    NOTEG4,    NOTEG4,    NOTEF4SHARP, NOTEF4SHARP, NOTEG4,    NOTEC5,    NOTEF4SHARP, NOTEA4,    NOTEG4,    NOTEG4,  NOTEB4,    NOTEA4,    NOTEG4,    NOTEG4,    NOTEF4SHARP, NOTEF4SHARP, NOTEA4,    NOTEC5,    NOTEF4SHARP, NOTEG4,    NOTEG4,    NOTEG4,    NOTEF4SHARP, NOTEE4,    NOTEF4SHARP, NOTEG4,    NOTEG4,    NOTEB4,    NOTEA4,    NOTEG4,    NOTEA4,    NOTEB4,    NOTEB4,    NOTED5,    NOTEC5,    NOTEB4,    NOTEC5,    NOTED5,      NOTENONE,    NOTED4,   NOTEE4,   NOTED4,    NOTEC4,     NOTEC4,      NOTEC4,    NOTEB3,     NOTEB3,      NOTEB3,    NOTEA3,     NOTEA3,      NOTEG3,    NOTEF3SHARP, NOTEE3,    NOTEF3SHARP, NOTEG3,    NOTENONE,  NOTEA3,    NOTENONE,  NOTEB3,    NOTENONE,  NOTENONE};
const uint16_t lengthArray[NOTECNT] = {QUARTERNOTE, EIGHTNOTE, EIGHTNOTE, QUARTERNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, QUARTERNOTE, QUARTERNOTE, QUARTERNOTE, EIGHTNOTE, EIGHTNOTE, QUARTERNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE,   EIGHTNOTE, QUARTERNOTE, QUARTERNOTE, EIGHTNOTE, EIGHTNOTE, TIENOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE,   TIENOTE,     EIGHTNOTE, EIGHTNOTE, EIGHTNOTE,   EIGHTNOTE, EIGHTNOTE, TIENOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE,   TIENOTE,     EIGHTNOTE, EIGHTNOTE, EIGHTNOTE,   EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE,   EIGHTNOTE, EIGHTNOTE,   EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, QUARTERNOTE, QUARTERNOTE, HALFNOTE, HALFNOTE, EIGHTNOTE, SIXTIENOTE, QUARTERNOTE, EIGHTNOTE, SIXTIENOTE, QUARTERNOTE, EIGHTNOTE, SIXTIENOTE, QUARTERNOTE, EIGHTNOTE, EIGHTNOTE,   EIGHTNOTE, EIGHTNOTE,   EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, QUARTERNOTE};
const uint16_t buttonArray[NOTECNT] = {BTWO,        BONE,      BTWO,      BTHREE,      BNONE,     BTWO,      BTWO,      BTWO,      BTWO,      BTWO,      BTHREE,      BNONE,       BONE,         BNONE,     BNONE,     BNONE,       BNONE,     BNONE,     BNONE,     BNONE,     BNONE,       BNONE,     BNONE,       BNONE,       BNONE,     BNONE,     BNONE,   BNONE,     BNONE,     BNONE,     BNONE,     BNONE,       BNONE,       BNONE,     BNONE,     BNONE,        BNONE,    BNONE,     BNONE,   BNONE,     BNONE,     BNONE,     BNONE,     BNONE,        BNONE,       BNONE ,   BNONE,      BNONE,      BNONE,     BNONE};
bool buttonExpected[4] = {false, false, false, false};
uint16_t buttonTime[4] = {0, 0, 0, 0};

/* globals */
int noteIndex = 0;
int score = 0;
int health = INITHEALTH;
bool updateLCDflag = true;
char buffer[4];

void updateLCD(int score, int health);
void gameOverLCD(int score, int health);
void startLCD(int score, int health);

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
    GB_init();
    // configures pins and delay library
    configLCD(CLK_FREQUENCY);
    initLCD();

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

    // Set initial period in CCR4 register. This assumes timer starts at 0
    TIMER_A1->CCR[4] = SINGLENOTETIME;
    // Configure CCR3 for Compare mode with interrupt enabled (no output mode - 0)
    TIMER_A1->CCTL[4] = 0x0010;

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
                startLCD(score, health);
            }
        }
        // what to do if playing
        if (playing) {
            if (GB1_pressed()) {
                if(buttonExpected[0]) {
                    buttonExpected[0] = false;
                    score++;
                } else {
                    health--;
                }
                updateLCDflag = true;
                GB1_debounce();
            }
            if (GB2_pressed()) {
                if(buttonExpected[1]) {
                    buttonExpected[1] = false;
                    score++;
                } else {
                    health--;
                }
                updateLCDflag = true;
                GB2_debounce();
            }
            if (GB3_pressed()) {
                if (buttonExpected[2]) {
                    buttonExpected[2] = false;
                    score++;
                } else {
                    health--;
                }
                updateLCDflag = true;
                GB3_debounce();
            }
            if (GB4_pressed()) {
                if (buttonExpected[3]) {
                    buttonExpected[3] = false;
                    score++;
                } else {
                    health--;
                }
                updateLCDflag = true;
                GB4_debounce();
            }
            if (updateLCDflag) {
                updateLCD(score, health);
            }
            if (health <= 0) {
                gameOverLCD(score, health);
                TIMER_A0->CTL &= ~0x0010;
                TIMER_A1->CTL &= ~0x0020;
                playing = false;

                health = INITHEALTH;
                score = 0;

                noteIndex = 0;
                // Set Period in CCR0 register
                TIMER_A0->CCR[0] = (6005000 / (noteArray[noteIndex] + 1)) - 1;
                // Set high pulse-width in CCR1 register (determines duty cycle)
                TIMER_A0->CCR[1] = ((6005000 / (noteArray[noteIndex]) + 1) / 2) - 1;
                // Set initial period in CCR2 register. This assumes timer starts at 0
                TIMER_A1->CCR[2] = lengthArray[noteIndex];
                // Set initial period in CCR3 register. This assumes timer starts at 0
                TIMER_A1->CCR[3] = lengthArray[noteIndex] + NOTEREST;
                // Set initial period in CCR4 register. This assumes timer starts at 0
                TIMER_A1->CCR[4] = SINGLENOTETIME;
            }
        }
    }
}
void gameOverLCD(int score, int health) {
    clearDisplay();
    int i;

    printChar('G');
    printChar('A');
    printChar('M');
    printChar('E');
    printChar(' ');
    printChar('O');
    printChar('V');
    printChar('E');
    printChar('R');
    commandInstruction(SET_CURSOR_MASK | 0x40);
    snprintf(buffer, 4, "%d     ", score);
    printChar('S');
    printChar('c');
    printChar('o');
    printChar('r');
    printChar('e');
    printChar(':');
    printChar(' ');
    for (i = 0; i < 3; i++) {
        printChar(buffer[i]);
    }
    updateLCDflag = false;
}

void updateLCD(int score, int health) {
    commandInstruction(SET_CURSOR_MASK | 0x07);

    int i;
    snprintf(buffer, 4, "%d     ", score);
    for (i = 0; i < 3; i++) {
        printChar(buffer[i]);
    }

    commandInstruction(SET_CURSOR_MASK | 0x48);
    snprintf(buffer, 4, "%d     ", health);
    for (i = 0; i < 3; i++) {
        printChar(buffer[i]);
    }

    updateLCDflag = false;
}

void startLCD(int score, int health) {
    clearDisplay();
    int i;
    snprintf(buffer, 4, "%d     ", score);
    printChar('S');
    printChar('c');
    printChar('o');
    printChar('r');
    printChar('e');
    printChar(':');
    printChar(' ');
    for (i = 0; i < 3; i++) {
        printChar(buffer[i]);
    }

    commandInstruction(SET_CURSOR_MASK | 0x40);
    snprintf(buffer, 4, "%d     ", health);
    printChar('H');
    printChar('e');
    printChar('a');
    printChar('l');
    printChar('t');
    printChar('h');
    printChar(':');
    printChar(' ');
    for (i = 0; i < 3; i++) {
        printChar(buffer[i]);
    }

    updateLCDflag = false;
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

    /* Check if interrupt triggered by CCR3 */
    if(TIMER_A1->CCTL[3] & TIMER_A_CCTLN_CCIFG)
    {
        noteIndex = (noteIndex + 1) % NOTECNT;
        /* add note changing code (update compare registers) */
        TIMER_A0->CTL |= 0x0010;
        TIMER_A0->CCR[0] = (6005000 / (noteArray[noteIndex] + 1)) - 1;
        TIMER_A0->CCR[1] = ((6005000 / (noteArray[noteIndex]) + 1) / 2) - 1;

        // Update Buttons

        if(buttonArray[noteIndex] != BNONE) {
            buttonExpected[buttonArray[noteIndex] - 1] = true;
            buttonTime[buttonArray[noteIndex] - 1] = SINGLENOTETIME;
            TIMER_A1->CCR[4] = TIMER_A1->CCR[3] + SINGLENOTETIME;
            //Enable timer interrupt
            TIMER_A1->CCTL[4] = 0x0010;
        }


        // Clear CCR2 compare interrupt flag
        // clear flag in TA1CCTL2
        TIMER_A1->CCTL[3] = TIMER_A1->CCTL[3] & 0xFFFE;

        // Schedule next interrupt interval
        TIMER_A1->CCR[2] = (TIMER_A1->CCR[3] + lengthArray[noteIndex]) % 0xFFFF;
        TIMER_A1->CCR[3] = (TIMER_A1->CCR[3] + lengthArray[noteIndex] + NOTEREST) % 0xFFFF;

    }

    /* Check if interrupt triggered by CCR4 */
    if(TIMER_A1->CCTL[4] & TIMER_A_CCTLN_CCIFG)
    {
        // Update Buttons
        int i;
        for (i=0; i < 4; i++) {
            if (buttonExpected[i]) {
                health--;
                buttonExpected[i] = false;
                updateLCDflag = true;
            }
        }

        // Clear CCR4 compare interrupt flag and turn off interrupt
        TIMER_A1->CCTL[4] = 0x0000;

    }
}
