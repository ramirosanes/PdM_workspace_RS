/**
 ********************************************************************************
 * @file    APP_clock.c
 * @author  rami
 * @date    Aug 8, 2024
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "APP_clock.h"
/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/
static clockState_t state = CLOCK_INIT;
static dateTime_t lastDateTime;
/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void clockFSM ()
{
	switch (state)
	{

	case CLOCK_INIT:

		static dateTime_t newDateTime;

		newDateTime.seconds = rtcGetSecond();
		newDateTime.minutes = rtcGetMinute();
		newDateTime.hours = rtcGetHour();
		newDateTime.dayOfMonth = rtcGetDate();
		newDateTime.month = rtcGetMonth();
		newDateTime.year = rtcGetYear();

		lastDateTime = newDateTime;
		state = UPDATE_SECONDS;

		break;

	case UPDATE_SECONDS:

		newDateTime.seconds = rtcGetSecond();
		if ((newDateTime.seconds != lastDateTime.seconds) | (newDateTime.seconds == 0))
		{
			lastDateTime.seconds = newDateTime.seconds;
			state = UPDATE_MINUTES;
		}

		break;

	case UPDATE_MINUTES:

		newDateTime.minutes = rtcGetMinute();
		state = UPDATE_SECONDS;
		if ((newDateTime.minutes != lastDateTime.minutes) | (newDateTime.minutes == 0))
		{
			lastDateTime.minutes = newDateTime.minutes;
			state = UPDATE_HOURS;
		}

		break;

	case UPDATE_HOURS:

		newDateTime.hours = rtcGetHour();
		state = UPDATE_SECONDS;
		if ((newDateTime.hours != lastDateTime.hours) | (newDateTime.hours == 0))
		{
			lastDateTime.hours = newDateTime.hours;
			state = UPDATE_DATE;
		}

		break;

	case UPDATE_DATE:

		newDateTime.dayOfMonth = rtcGetDate();
		newDateTime.month = rtcGetMonth();
		newDateTime.year = rtcGetYear();
		state = UPDATE_SECONDS;
		if (newDateTime.dayOfMonth != lastDateTime.month)
		{
			lastDateTime.dayOfMonth = newDateTime.dayOfMonth;
			lastDateTime.month = newDateTime.month;
			lastDateTime.year = newDateTime.year;
			state = UPDATE_SECONDS;
		}

		break;

	default:
		uartSendString("error clockfsm");
		break;

	}
}

void clockReset ()
{
	state = CLOCK_INIT;
}

uint8_t clockGetSeconds ()
{
	return lastDateTime.seconds;
}

uint8_t clockGetMinutes ()
{
	return lastDateTime.minutes;
}

uint8_t clockGetHours ()
{
	return lastDateTime.hours;
}

uint8_t clockGetDate ()
{
	return lastDateTime.dayOfMonth;
}

uint8_t clockGetMonth ()
{
	return lastDateTime.month;
}

uint16_t clockGetYear ()
{
	return lastDateTime.year;
}
