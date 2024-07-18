#ifndef API_INC_API_LEDS_H_
#define API_INC_API_LEDS_H_

#include "API_init.h"
#include "API_delay.h"

typedef enum {
	LED_ON,
	LED_OFF,
	WAITING,
}ledState_t;

void ledFSM_init();
void ledFSM_update();

void ledSetCycleDuration (tick_t duration);
void ledSetOnDuration (tick_t duration);
void ledSetOffDuration (tick_t duration);

bool_t readLed ();

#endif
