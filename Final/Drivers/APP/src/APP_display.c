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

/************************************
 * GLOBAL VARIABLES
 ************************************/
uint8_t displayBuffer[ROWS][COLS];
uint8_t configBuffer[ROWS];
displayData_t currentData;
/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/
static void updateData()
{

}
/************************************
 * GLOBAL FUNCTIONS
 ************************************/

void displayInit()
{
	lcdInit();
	currentData.currentState = DISPLAY_DATA;
	currentData.currentDateTime.dayOfMonth = 16;
	currentData.currentDateTime.month = 11;
	currentData.currentDateTime.year = 1995;
	currentData.highFlanksCount = 0;
	currentData.lowFlanksCount = 0;
	DELAY_MS(1000);
}
void displayFSM()
{
	static uint8_t n = 9;
	static uint8_t* s = "Nov";
	updateData();
	switch (currentData.currentState)
	{
	case DISPLAY_DATA:
		static uint8_t i=0;
		//lcdSetCursor(i%ROWS, 0);
		//lcdPrintfCenteredString(displayBuffer[i], ' ', n, s);
		i++;
		break;
	case DISPLAY_CONFIG:
		break;
	default:
		uartSendString((uint8_t*)"error displayFSM");
		break;
	}
}
