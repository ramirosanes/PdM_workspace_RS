#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_

#include "main.h"

typedef uint32_t tick_t; //tick_t definition

typedef struct {
	tick_t startTime;
	tick_t elapsedTime;
	tick_t duration;
	bool_t running;
}
delay_t; //delay_t definition

void delayInit (delay_t*);
bool_t delayRead(delay_t* delay);
void delayStart (delay_t* delay);
void delayWrite (delay_t*, tick_t duration);
bool_t delayIsRunning (delay_t* delay);

#endif
