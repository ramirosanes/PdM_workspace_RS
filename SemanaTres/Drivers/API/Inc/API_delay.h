#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_

#include <stdint.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */

#define DURATION 100
#define DUTY 2
#define LEDCOUNT 3


typedef uint32_t tick_t;
typedef bool bool_t;

typedef struct {
	tick_t startTime;
	tick_t duration;
	bool_t running;
}
delay_t;

typedef struct {
	Led_TypeDef index;
	delay_t cycle;
	delay_t on;
}
led_t;

void delayInit (delay_t* delay, tick_t duration);
bool_t delayStart (delay_t* delay);
void delayWrite (delay_t*, tick_t duration);
bool_t delayIsRunning (delay_t* delay);
void increaseLed(led_t *leds);



#endif
