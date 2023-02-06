/*! \file */
/*!
 * pushButtons.h
 *
 * Description: Pushbutton driver for P1.1 and P1.4 on MSP432P4111 Launchpad
 *
 *  Created on: 12/26/2022
 *      Author: Josh Schrock
 */

#ifndef PUSHBUTTONS_H_
#define PUSHBUTTONS_H_

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

#include "msp.h"
#include <stdbool.h>

#define BUTTON_PORT                                         P1
#define BUTTON_1_PIN                                        (0x0002)
#define BUTTON_2_PIN                                        (0x0010)
#define BUTTON_ALL_PINS                                     (0x0012)

/*!
 * \brief This function configures Both pushbutton pins as input pins
 *
 * This function configures P1.1 and P1.4 as input pins
 *
 * \return None
 */
extern void BUTTONS_init(void);


/*!
 * \brief This function returns the state of PushButton1
 *
 * This function returns true if the button is pressed
 *
 * \return boolean
 */
extern bool BUTTON1_pressed(void);


/*!
 * \brief This function returns the state of PushButton2
 *
 * This function returns true if the button is pressed
 *
 * \return boolean
 */
extern bool BUTTON2_pressed(void);


/*!
 * \brief This function de-bounces pushButton1
 *
 * This function de-bounces pushButton1
 *
 * \return boolean
 */
extern void BUTTON1_debounce(void);


/*!
 * \brief This function de-bounces pushButton2
 *
 * This function de-bounces pushButton2
 *
 * \return boolean
 */
extern void BUTTON2_debounce(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* PUSHBUTTONS_H_ */
