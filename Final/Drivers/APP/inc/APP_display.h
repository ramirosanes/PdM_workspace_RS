/**
 ********************************************************************************
 * @file    APP_display.h
 * @author  rami
 * @date    Aug 3, 2024
 * @brief   
 ********************************************************************************
 */

#ifndef APP_INC_APP_DISPLAY_H_
#define APP_INC_APP_DISPLAY_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "API_lcd.h"
#include "API_uart.h"
#include "API_utils.h"

#include "APP_clock.h"
/************************************
 * MACROS AND DEFINES
 ************************************/
#define USERBUFFER_SIZE COLS
/************************************
 * TYPEDEFS
 ************************************/
typedef enum
{
	DISPLAY_INIT,
	DISPLAY_DATA,
	DISPLAY_CONFIG,
	DISPLAY_SET_DATE,
	DISPLAY_SET_TIME,
	DISPLAY_RESET_COUNT,
	DISPLAY_SET_CUSTOM_STRING_INPUT,
}
displayState_t;

/************************************
 * EXPORTED VARIABLES
 ************************************/
extern uint8_t displayBuffer[ROWS][COLS];

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void displayFSM();

#ifdef __cplusplus
}
#endif

#endif 
