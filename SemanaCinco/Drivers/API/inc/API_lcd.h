/**
 ********************************************************************************
 * @file    API_lcd.h
 * @author  rami
 * @date    Jul 29, 2024
 * @brief   
 ********************************************************************************
 */

#ifndef API_INC_API_LCD_H_
#define API_INC_API_LCD_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "main.h"
/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void lcdInit ();
void lcdClear ();
void lcdHome ();
void lcdSetCursor (uint8_t row, uint8_t col);
void lcdSendChar (uint8_t c);
void lcdSendString (uint8_t* str);
void lcdDisplayOn ();
void lcdDisplayOff ();
void lcdCursorOn ();
void lcdCursorOff ();
void lcdBlinkOn ();
void lcdBlinkOff ();

#ifdef __cplusplus
}
#endif

#endif 
