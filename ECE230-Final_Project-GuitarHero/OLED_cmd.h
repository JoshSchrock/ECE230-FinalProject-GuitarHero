/*
 * OLED_cmd.h
 *
 *  Created on: Feb 17, 2023
 *      Author: schrocjq
 */

#ifndef OLED_CMD_H_
#define OLED_CMD_H_

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

extern void setupOLED();
extern void draw(int x, int y, unsigned char ascii);
extern void display_logos(int x, int y, unsigned char ascii_index);
extern void draw_line(int x ,int y,unsigned char ascii_str[16]);
extern void Display_Logo(int x ,int y, unsigned char logo_val);
/* Set Entire Display OFF */
extern void display_off();
/* Set Entire Display ON */
extern void display_on();
/* Clear display */
extern void display_clear();
extern void msdelay(int delay);
extern unsigned char* getTextArr();
extern unsigned char* getLogoArr();

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* OLED_CMD_H_ */
