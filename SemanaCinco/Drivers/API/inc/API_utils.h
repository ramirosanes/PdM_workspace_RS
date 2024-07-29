/**
 ********************************************************************************
 * @file    API_utils.h
 * @author  rami
 * @date    Jul 26, 2024
 * @brief   
 ********************************************************************************
 */

#ifndef API_INC_API_UTILS_H_
#define API_INC_API_UTILS_H_

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
typedef uint32_t tick_t;

typedef struct
{
	tick_t startTime;
	tick_t elapsedTime;
	tick_t duration;
	bool_t running;
}
delay_t;

typedef struct
{
	enum
	{
		BUTTON_UP,
		BUTTON_FALLING,
		BUTTON_DOWN,
		BUTTON_RISING,
	}
	State;
	delay_t debounceDelay;
	bool_t isPressed;
}
button_t;
/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void buttonFSM_init ();
void buttonFSM_update();
bool_t readUserButton();

void delayInit (delay_t* delay);
void delayStart (delay_t* delay);
void delayWrite (delay_t* delay, tick_t duration);
bool_t delayIsRunning (delay_t* delay);

#ifdef __cplusplus
}
#endif

#endif 
