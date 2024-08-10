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
#include "APP_display.h"
/************************************
 * MACROS AND DEFINES
 ************************************/
#define CIRCULAR_BUFFER_SIZE COLS
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
    uint8_t buffer[CIRCULAR_BUFFER_SIZE];
    uint16_t head;
    uint16_t tail;
    uint16_t count;
}
circularBuffer_t;

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
	uint8_t highFlankCount;
	uint8_t lowFlankCount;
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
void buttonInit ();
void buttonFSM();
bool_t readUserButton();
uint8_t getHighFlankCount ();
uint8_t getLowFlankCount ();

void delayInit (delay_t* delay);
void delayStart (delay_t* delay);
void delayWrite (delay_t* delay, tick_t duration);
bool_t delayIsRunning (delay_t* delay);

void circularBufferInit (circularBuffer_t *cb);
bool_t circularBufferIsFull (circularBuffer_t *cb);
bool_t circularBufferIsEmpty (circularBuffer_t *cb);
bool_t circularBufferWriteByte (circularBuffer_t *cb, uint8_t data);
bool_t circularBufferReadByte (circularBuffer_t *cb, uint8_t *data);
uint8_t circularBufferAvailableBytes (circularBuffer_t *cb);

#ifdef __cplusplus
}
#endif

#endif 
