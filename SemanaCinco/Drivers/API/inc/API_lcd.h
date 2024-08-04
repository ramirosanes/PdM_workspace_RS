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
//LCD INFO
#define LCD_ADDRESS 0x27
#define COLS 20
#define ROWS 4
/************************************
 * TYPEDEFS
 ************************************/
typedef struct
{
	I2C_HandleTypeDef* lcdHandle;
	uint8_t lcdAddress;
	uint8_t lcdCols;
	uint8_t lcdRows;
}
lcd_t;
/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void displayFSM();

void lcdInit ();
void lcdClear ();
void lcdHome ();
void lcdSetCursor (uint8_t row, uint8_t col);
void lcdPrintChar (uint8_t c);
void lcdPrintString (uint8_t* str);
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
