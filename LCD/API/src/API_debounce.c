#include "main.h"
#include "API_debounce.h"
#include "API_delay.h"

#define DEBOUNCE 40

static debounceFSM_t debounceFSM;

void debounceFSM_init()
{
	debounceFSM.state = BUTTON_UP;
	debounceFSM.wasPressed = false;
	delayInit(&debounceFSM.debounceTimer);
	delayWrite(&debounceFSM.debounceTimer, DEBOUNCE);
}

void debounceFSM_update()
{
	switch (debounceFSM.state) {

	case BUTTON_UP:

		debounceFSM.wasPressed = false;
		if (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin)) {
			debounceFSM.state = BUTTON_FALLING;
			delayStart(&debounceFSM.debounceTimer);
		}

		break;

	case BUTTON_DOWN:

		debounceFSM.wasPressed = true;
		if (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin)) {
			debounceFSM.state = BUTTON_RISING;
			delayStart(&debounceFSM.debounceTimer);
		}
		break;

	case BUTTON_FALLING:

		if (delayRead(&debounceFSM.debounceTimer)) {

			if (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin)) {
				debounceFSM.state = BUTTON_DOWN;
				break;
			} else {
				debounceFSM.state = BUTTON_UP;
				break;
			}

		}

		break;

	case BUTTON_RISING:

		if (delayRead(&debounceFSM.debounceTimer)) {

			if (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin)) {
				debounceFSM.state = BUTTON_DOWN;
				break;
			} else {
				debounceFSM.state = BUTTON_UP;
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

bool_t readKey()
{
	return debounceFSM.wasPressed;
}
