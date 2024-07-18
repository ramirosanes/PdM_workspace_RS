#ifndef API_INC_API_LEDS_H_
#define API_INC_API_LEDS_H_

#include "API_delay.h"

typedef enum
{
	LED1 = LED1_Pin,
	LED2 = LED2_Pin,
	LED3 = LED3_Pin,
}userLeds_t;

typedef enum
{
	LED_OFF,
	LED_ON,
	WAITING,
}
ledState_t;

typedef struct
{
	ledState_t state;
	bool_t isOn;
	delay_t onTime;
	delay_t offTime;
}
ledFSM_t;

void ledFSM_init();
void ledFSM_update();

void ledSetCycleDuration (tick_t duration);
void ledSetOnDuration (tick_t duration);
void ledSetOffDuration (tick_t duration);
bool_t readLed ();

void ledOn(userLeds_t ledNo);
void ledOff(userLeds_t ledNo);



#endif
