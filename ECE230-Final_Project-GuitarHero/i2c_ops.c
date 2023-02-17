#include "msp.h"
#include "oled.h"
/*
 * i2c_ops.c
 *
 *  Created on: 12-Mar-2019
 *      Author: RAJ JOHRI
 */

void i2c_init();
void i2c_send(unsigned char*, unsigned int);
static void i2c_write_txbuf(unsigned char);
static void enable_i2c();
static void disable_i2c();
static void generate_start();
static void generate_stop();

void i2c_init()
{
    /* make sure module is disabled */
    disable_i2c();

    /* change P1.6 and P1.7 as primary function */
    P1->SEL0 |= BIT6;
    P1->SEL1 &= ~(BIT6);

    P1->SEL0 |= BIT7;
    P1->SEL1 &= ~(BIT7);

    /* modes selected:
     * master, i2c, synchronous, SMCLK, transmitter */

    /* Configure eUSCI_B0 for I2C mode
     *  I2C master mode, synchronous, 7-bit address, SMCLK clock source,
     *  transmit mode, with automatic STOP condition generation
     */
    EUSCI_B0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Software reset enabled
    EUSCI_B0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset mode
            EUSCI_B_CTLW0_MODE_3 |          // I2C mode
            EUSCI_B_CTLW0_MST |             // Master mode
            EUSCI_B_CTLW0_SYNC |            // Sync mode
            EUSCI_B_CTLW0_TR |              // Transmitter mode
            EUSCI_B_CTLW0_SSEL__SMCLK;      // SMCLK

    /* I2C clock calculation
     * Refer to Section 26.3.6 of Technical Reference manual
     * BRCLK = 3MHz, I2C bit clock rate = 100kbps
    */
    // TODO configure eUSCI_B0 bit rate control for 100 kbps
    EUSCI_B0->BRW = 119;

    /* initialize slave address */
    EUSCI_B0->I2CSA = SLAVE_ADDR;


    /* enable module only when transmission is required */
}

void i2c_send(unsigned char *data, unsigned int len)
{

    /* write number of bytes to be sent to slave
     * enable auto stop generation in UCBxCTLW1
     */

    /* enable i2c module */
    enable_i2c();

    /* generate start condition and wait for the bus */
    generate_start();
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));

    /* write data byte by byte to buffer */
    while(len--)
    {
        i2c_write_txbuf(*data);
        data++;
    }

    /* force stop and check if stop is generated or not */
    generate_stop();
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_STPIFG));

    /* transmission completed, disable the module */
    disable_i2c();
}

/* write data on i2c bus */
static void i2c_write_txbuf(unsigned char byte)
{
    EUSCI_B0->TXBUF = byte;
    while((EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0) == 0);
}

/* enable i2c bus */
static void enable_i2c()
{
    EUSCI_B0->CTLW0 &= ~(0x01);

}

/* disable i2c bus */
static void disable_i2c()
{
    EUSCI_B0->CTLW0 |= 0x01;
}

/* generate start condition */
static void generate_start()
{
    EUSCI_B0->CTLW0 |= 0x0002;
}

/* force stop condition */
static void generate_stop()
{
    EUSCI_B0->CTLW0 |= 0x0004;
}
