/*! \file */
/*!
 * guitarButtons.h
 *
 * Description: Pushbutton driver for P3.0 - P3.4 on MSP432P4111 Launchpad
 *
 *  Created on: 2/6/2022
 *      Author: Josh Schrock
 */

#ifndef GUITARBUTTONS_H_
#define GUITARBUTTONS_H_

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

#define GB_PORT                                         P3
#define GB_1_PIN                                        (0x0001)
#define GB_2_PIN                                        (0x0002)
#define GB_3_PIN                                        (0x0004)
#define GB_4_PIN                                        (0x0008)
#define GB_ALL_PINS                                     (0x000F)

/*!
 * \brief This function configures Both pushbutton pins as input pins
 *
 * This function configures P1.1 and P1.4 as input pins
 *
 * \return None
 */
extern void GB_init(void);


/*!
 * \brief This function returns the state of PushButton1
 *
 * This function returns true if the button is pressed
 *
 * \return boolean
 */
extern bool GB1_pressed(void);


/*!
 * \brief This function returns the state of PushButton2
 *
 * This function returns true if the button is pressed
 *
 * \return boolean
 */
extern bool GB2_pressed(void);


/*!
 * \brief This function returns the state of PushButton1
 *
 * This function returns true if the button is pressed
 *
 * \return boolean
 */
extern bool GB3_pressed(void);


/*!
 * \brief This function returns the state of PushButton2
 *
 * This function returns true if the button is pressed
 *
 * \return boolean
 */
extern bool GB4_pressed(void);


/*!
 * \brief This function de-bounces pushButton1
 *
 * This function de-bounces pushButton1
 *
 * \return boolean
 */
extern void GB1_debounce(void);


/*!
 * \brief This function de-bounces pushButton2
 *
 * This function de-bounces pushButton2
 *
 * \return boolean
 */
extern void GB2_debounce(void);

/*!
 * \brief This function de-bounces pushButton1
 *
 * This function de-bounces pushButton1
 *
 * \return boolean
 */
extern void GB3_debounce(void);


/*!
 * \brief This function de-bounces pushButton2
 *
 * This function de-bounces pushButton2
 *
 * \return boolean
 */
extern void GB4_debounce(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* GUITARBUTTONS_H_ */
