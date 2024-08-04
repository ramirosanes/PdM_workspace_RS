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
#include "API_rtc.h"
#include "API_utils.h"
/************************************
 * MACROS AND DEFINES
 ************************************/
#define USERBUFFER_SIZE 10
/************************************
 * TYPEDEFS
 ************************************/
typedef struct
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t dayOfWeek;
	uint8_t dayOfMonth;
	uint8_t month;
	uint16_t year;
	uint8_t timeZoneHour;
	uint16_t timeZoneMinute;
}
dateTime_t;

typedef enum
{
	DISPLAY_DATA,
	DISPLAY_CONFIG,
}
displayState_t;

typedef struct
{
	displayState_t currentState;
	dateTime_t currentDateTime;
	uint8_t highFlanksCount;
	uint8_t lowFlanksCount;
	uint8_t userBuffer[USERBUFFER_SIZE];
}
displayData_t;


/************************************
 * EXPORTED VARIABLES
 ************************************/
extern uint8_t displayBuffer[ROWS][COLS];
extern displayData_t currentData;
/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void displayFSMInit();
void displayFSM();

#ifdef __cplusplus
}
#endif

#endif 
