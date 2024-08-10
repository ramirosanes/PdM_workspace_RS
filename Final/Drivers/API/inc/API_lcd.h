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
#include <stdio.h>
/************************************
 * MACROS AND DEFINES
 ************************************/
#define LCD_ADDRESS 0x27
#define COLS 20
#define ROWS 4
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
void lcdDisplayOn ();
void lcdDisplayOff ();
void lcdCursorOn ();
void lcdCursorOff ();
void lcdBlinkOn ();
void lcdBlinkOff ();

void lcdPrintInt(int16_t num);
void lcdPrintChar (uint8_t c);
void lcdPrintString (uint8_t* str);

void lcdPrintfCenteredString(uint8_t* str, uint8_t c, int16_t num, const uint8_t* string);
void lcdPrintfTwoInts(const uint8_t* format, int16_t num1, int16_t num2);
void lcdPrintfThreeInts(const uint8_t* format, int16_t num1, int16_t num2, int16_t num3);

#ifdef __cplusplus
}
#endif

#endif 
