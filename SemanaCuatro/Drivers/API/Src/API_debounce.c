#include "API_debounce.h"
#include "API_init.h"
#include "API_delay.h"

#define DEBOUNCE 40

static debounceState_t debounceFSM;
static delay_t debounceTimer;
static bool_t wasPressed;

void debounceFSM_init() {

	debounceFSM = BUTTON_UP;
	wasPressed = false;
	delayInit(&debounceTimer);
	delayWrite(&debounceTimer, DEBOUNCE);

}

void debounceFSM_update(){

	switch (debounceFSM) {

	case BUTTON_UP:

		wasPressed = false;
		if (BSP_PB_GetState(BUTTON_USER)) {
			debounceFSM = BUTTON_FALLING;
			delayStart(&debounceTimer);
		}

		break;

	case BUTTON_DOWN:

		wasPressed = true;
		if (BSP_PB_GetState(BUTTON_USER)) {
			debounceFSM = BUTTON_RISING;
			delayStart(&debounceTimer);
		}
		break;

	case BUTTON_FALLING:

		if (delayRead(&debounceTimer)) {

			if (BSP_PB_GetState(BUTTON_USER)) {
				debounceFSM = BUTTON_DOWN;
				break;
			} else {
				debounceFSM = BUTTON_UP;
				break;
			}

		}

		break;

	case BUTTON_RISING:

		if (delayRead(&debounceTimer)) {

			if (BSP_PB_GetState(BUTTON_USER)) {
				debounceFSM = BUTTON_DOWN;
				break;
			} else {
				debounceFSM = BUTTON_UP;
				break;
			}

		}

		break;

	default:

		while(1) {
			//me encantaría ver cómo podriamos terminar acá
		}

		break;
	}
}

bool_t readKey(){
	return wasPressed;
}
