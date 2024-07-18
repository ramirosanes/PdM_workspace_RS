#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

#include "API_delay.h"

typedef enum
{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RISING
}
debounceState_t;

typedef struct
{
	debounceState_t state;
	delay_t debounceTimer;
	bool_t wasPressed;
}
debounceFSM_t;

void debounceFSM_init();
void debounceFSM_update();
bool_t readKey();

#endif
