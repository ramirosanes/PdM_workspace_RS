/**
 ********************************************************************************
 * @file    APP_clock.h
 * @author  rami
 * @date    Aug 8, 2024
 * @brief   
 ********************************************************************************
 */

#ifndef APP_INC_APP_CLOCK_H_
#define APP_INC_APP_CLOCK_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "main.h"
#include "API_uart.h"
#include "API_rtc.h"
/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/
typedef struct
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t dayOfMonth;
	uint8_t month;
	uint16_t year;
	uint8_t timeZoneHour;
	uint16_t timeZoneMinute;
}
dateTime_t;
typedef enum
{
	CLOCK_INIT,
	UPDATE_SECONDS,
	UPDATE_MINUTES,
	UPDATE_HOURS,
	UPDATE_DATE,
}
clockState_t;
/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void clockFSM ();

uint8_t clockGetSeconds ();
uint8_t clockGetMinutes ();
uint8_t clockGetHours ();

uint8_t clockGetDate ();
uint8_t clockGetMonth ();
uint16_t clockGetYear ();

#ifdef __cplusplus
}
#endif

#endif 
