#include "main.h"
#include "API_init.h"
#include "API_leds.h"
#include "API_debounce.h"

#define UNO 500
#define DOS 100

int main(void) {

	HW_Init();

	debounceFSM_init();
	ledFSM_init();

	while (1) {

		//Check
		ledFSM_update();
		debounceFSM_update();

		//Act
		(readLed()) ? BSP_LED_On(LED2) : BSP_LED_Off(LED2);
		(readKey()) ? ledSetCycleDuration(DOS) : ledSetCycleDuration(UNO);

	}
}

