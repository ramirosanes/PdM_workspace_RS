/**
 ********************************************************************************
 * @file    APP_display.c
 * @author  rami
 * @date    Aug 3, 2024
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "APP_display.h"
#include "API_utils.h"

#include <stdio.h>

/************************************
 * EXTERN VARIABLES
 ************************************/
extern circularBuffer_t uartUserBuffer;
/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/
static displayState_t displayState = DISPLAY_INIT;
/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/
static void inputFunction (displayState_t state)
{
	static uint8_t userBuffer[COLS];
	static uint8_t option;
	static uint8_t day, month, seconds, minutes, hours;
	static uint16_t year;

	switch (state)
	{
	case DISPLAY_DATA:
		if (circularBufferAvailableBytes(&uartUserBuffer))
		{
			if (circularBufferReadAllBytes(&uartUserBuffer, userBuffer))
			{
				if (sscanf(userBuffer,"%c",&option))
				{
					switch (option)
					{
					case 0:
						break;
					case 'x':
						displayState = DISPLAY_CONFIG;
						option = 0;
						break;
					default:
						uartSendString((uint8_t*)"msj no reconocido");
						option = 0;
						break;

					}
				}
			}

		}
		break;

	case DISPLAY_CONFIG:
		if (circularBufferAvailableBytes(&uartUserBuffer))
		{
			if (circularBufferReadAllBytes(&uartUserBuffer, userBuffer))
			{
				if (sscanf(userBuffer, "%u", &option))
				{
					switch (option)
					{
					case 0:
						break;
					case 1:
						displayState = DISPLAY_SET_DATE;
						lcdClear();
						option = 0;
						break;
					case 2:
						displayState = DISPLAY_SET_TIME;
						lcdClear();
						option = 0;
						break;
					case 3:
						displayState = DISPLAY_RESET_COUNT;
						lcdClear();
						option = 0;
						break;
					case 4:
						displayState = DISPLAY_SET_CUSTOM_STRING_INPUT;
						lcdClear();
						option = 0;
						break;
					default:
						displayState = DISPLAY_DATA;
						lcdClear();
						option = 0;
						uartSendString((uint8_t*)"seleccion erronea, retorno a data");
						break;
					}
				}
			}
		}
		break;

	case DISPLAY_SET_DATE:
		if (circularBufferAvailableBytes(&uartUserBuffer)>=6)
		{
			if (circularBufferReadAllBytes(&uartUserBuffer, userBuffer))
			{
				if (sscanf(&userBuffer, "%2d%2d%2d", &day, &month, &year)<3)
				{
					day = 16;
					month = 11;
					year = 2005;
				}
				rtcSetDate(day);
				rtcSetMonth(month);
				rtcSetYear(year);
				clockReset();
				lcdClear();
				lcdHome();
				displayState = DISPLAY_DATA;
			}

		}
		break;

	case DISPLAY_SET_TIME:
		if (circularBufferAvailableBytes(&uartUserBuffer)>=6)
		{
			if (circularBufferReadAllBytes(&uartUserBuffer, userBuffer))
			{
				if (sscanf(&userBuffer, "%2d%2d%2d", &hours, &minutes, &seconds)<3)
				{
					hours = 16;
					minutes = 11;
					seconds = 05;
				}
				rtcSetHour(hours);
				rtcSetMinute(minutes);
				rtcSetSecond(seconds);
				clockReset();
				lcdClear();
				lcdHome();
				displayState = DISPLAY_DATA;
			}

		}
		break;

	case DISPLAY_RESET_COUNT:
		break;
	case DISPLAY_SET_CUSTOM_STRING_INPUT:
		break;
	default:
		break;

	}
}

static void displayFunction (displayState_t state)
{
	static uint8_t row = 0;
	switch (state)
	{
	case DISPLAY_INIT:
		lcdClear();
		lcdHome();
		lcdPrintfCenteredString((uint8_t*)"holis", ' ', 0, (uint8_t*)" ");
		HAL_Delay(1000);
		break;

	case DISPLAY_DATA:
		switch (row)
		{
		case 3:
			lcdSetCursor(row, 0);
			lcdPrintfTwoInts((uint8_t*)"%d %d", getHighFlankCount(), getLowFlankCount());
			row = (row+1)%ROWS;
			break;
		case 2:
			lcdSetCursor(row, 0);
			lcdPrintfThreeInts((uint8_t*)"%d:%d:%d", clockGetHours(), clockGetMinutes(), clockGetSeconds());
			row = (row+1)%ROWS;
			break;
		case 1:
			lcdSetCursor(row, 0);
			lcdPrintfThreeInts((uint8_t*)"%d/%d/%d", clockGetDate(), clockGetMonth(), clockGetYear());
			row = (row+1)%ROWS;
			break;
		case 0:
			lcdSetCursor(row, 0);
			lcdPrintfCenteredString((uint8_t*)"bienvenido!", ' ', 0, (uint8_t*)" ");
			row = (row+1)%ROWS;
			break;
		default:
			uartSendString((uint8_t*)"problems in display_data");
			break;
		}
		break;

	case DISPLAY_CONFIG:
		switch (row)
		{
		case 3:
			lcdSetCursor(row, 0);
			lcdPrintfCenteredString((uint8_t*)"4: print custom str", ' ', 0, (uint8_t*)" ");
			row = (row+1)%ROWS;
			break;
		case 2:
			lcdSetCursor(row, 0);
			lcdPrintfCenteredString((uint8_t*)"3: reset counter", ' ', 0, (uint8_t*)" ");
			row = (row+1)%ROWS;
			break;
		case 1:
			lcdSetCursor(row, 0);
			lcdPrintfCenteredString((uint8_t*)"2: set time", ' ', 0, (uint8_t*)" ");
			row = (row+1)%ROWS;
			break;
		case 0:
			lcdSetCursor(row, 0);
			lcdPrintfCenteredString((uint8_t*)"1: set date", ' ', 0, (uint8_t*)" ");
			row = (row+1)%ROWS;
			break;
		default:
			uartSendString((uint8_t*)"problems");
			break;
		}
		break;

	case DISPLAY_SET_DATE:
		switch (row)
		{
		case 3:
			lcdSetCursor(row, 0);
			lcdPrintfCenteredString((uint8_t*)"ddmmyy", ' ', 0, (uint8_t*)" ");
			row = (row+1)%ROWS;
			break;
		case 2:
			lcdSetCursor(row, 0);
			lcdPrintfCenteredString((uint8_t*)"send date format:", ' ', 0, (uint8_t*)" ");
			row = (row+1)%ROWS;
			break;
		case 1:
			lcdSetCursor(row, 0);
			lcdPrintfCenteredString((uint8_t*)"('x' to return)", ' ', 0, (uint8_t*)" ");
			row = (row+1)%ROWS;
			break;
		case 0:
			lcdSetCursor(row, 0);
			lcdPrintfCenteredString((uint8_t*)"CONFIG MENU", ' ', 0, (uint8_t*)" ");
			row = (row+1)%ROWS;
			break;
		default:
			uartSendString((uint8_t*)"problems");
			break;
		}
		break;

		case DISPLAY_SET_TIME:
		switch (row)
		{
		case 3:
			lcdSetCursor(row, 0);
			lcdPrintfCenteredString((uint8_t*)"hhmmss", ' ', 0, (uint8_t*)" ");
			row = (row+1)%ROWS;
			break;
		case 2:
			lcdSetCursor(row, 0);
			lcdPrintfCenteredString((uint8_t*)"send time format:", ' ', 0, (uint8_t*)" ");
			row = (row+1)%ROWS;
			break;
		case 1:
			lcdSetCursor(row, 0);
			lcdPrintfCenteredString((uint8_t*)"('x' to return)", ' ', 0, (uint8_t*)" ");
			row = (row+1)%ROWS;
			break;
		case 0:
			lcdSetCursor(row, 0);
			lcdPrintfCenteredString((uint8_t*)"CONFIG MENU", ' ', 0, (uint8_t*)" ");
			row = (row+1)%ROWS;
			break;
		default:
			uartSendString((uint8_t*)"problems");
			break;
		}
		break;

		case DISPLAY_RESET_COUNT:
		switch (row)
		{
		case 3:
			lcdSetCursor(row, 0);
			lcdPrintfCenteredString((uint8_t*)"'r' to reset count", ' ', 0, (uint8_t*)" ");
			row = (row+1)%ROWS;
			break;
		case 2:
			lcdSetCursor(row, 0);
			lcdPrintfCenteredString((uint8_t*)"send:", ' ', 0, (uint8_t*)" ");
			row = (row+1)%ROWS;
			break;
		case 1:
			lcdSetCursor(row, 0);
			lcdPrintfCenteredString((uint8_t*)"('x' to return)", ' ', 0, (uint8_t*)" ");
			row = (row+1)%ROWS;
			break;
		case 0:
			lcdSetCursor(row, 0);
			lcdPrintfCenteredString((uint8_t*)"CONFIG MENU", ' ', 0, (uint8_t*)" ");
			row = (row+1)%ROWS;
			break;
		default:
			uartSendString((uint8_t*)"problems");
			break;
		}
		break;

		case DISPLAY_SET_CUSTOM_STRING_INPUT:
		switch (row)
		{
		case 3:
			lcdSetCursor(row, 0);
			lcdPrintfCenteredString((uint8_t*)"max 20 chars", ' ', 0, (uint8_t*)" ");
			row = (row+1)%ROWS;
			break;
		case 2:
			lcdSetCursor(row, 0);
			lcdPrintfCenteredString((uint8_t*)"send string:", ' ', 0, (uint8_t*)" ");
			row = (row+1)%ROWS;
			break;
		case 1:
			lcdSetCursor(row, 0);
			lcdPrintfCenteredString((uint8_t*)"('x' to return)", ' ', 0, (uint8_t*)" ");
			row = (row+1)%ROWS;
			break;
		case 0:
			lcdSetCursor(row, 0);
			lcdPrintfCenteredString((uint8_t*)"CONFIG MENU", ' ', 0, (uint8_t*)" ");
			row = (row+1)%ROWS;
			break;
		default:
			uartSendString((uint8_t*)"problems");
			break;
		}
		break;
	}
}
/************************************
 * STATIC FUNCTIONS
 ************************************/

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void displayFSM()
{
	switch (displayState)
	{

	case DISPLAY_INIT:
		circularBufferInit(&uartUserBuffer);
		displayState = DISPLAY_DATA;
		break;

	case DISPLAY_DATA:

		displayFunction(displayState);
		inputFunction(displayState);
		break;

	case DISPLAY_CONFIG:
		displayFunction(displayState);
		inputFunction(displayState);
		break;

	case DISPLAY_SET_DATE:
		displayFunction(displayState);
		inputFunction(displayState);
		break;

	case DISPLAY_SET_TIME:
		displayFunction(displayState);
		inputFunction(displayState);
		break;

	case DISPLAY_RESET_COUNT:
		displayFunction(displayState);
		inputFunction(displayState);
		break;

	case DISPLAY_SET_CUSTOM_STRING_INPUT:
		displayFunction(displayState);
		inputFunction(displayState);
		break;

	default:
		uartSendString((uint8_t*)"error displayFSM");
		break;

	}
}
