#include "main.h"
#include "API_delay.h"
#include "API_init.h"

#include <stdint.h>

#define DUTY_CYCLE 50 //Might work with other values (ToBe confirmed)

int main(void) {

	HW_Init();

	ledState_t state = ON;
	led_t leds;

	uint32_t TIEMPOS[] = { 500, 100, 100, 1000 };
	uint8_t TIEMPOS_index = 0;

	delayInit(&leds.cycle, TIEMPOS[TIEMPOS_index] * duty(DUTY_CYCLE));
	delayInit(&leds.on, TIEMPOS[TIEMPOS_index]);

	leds.index = LED2;

	while (1) {
		//Task 1: Check State, and Act according to it
		switch (state) {
		case ON:
			BSP_LED_On(leds.index);
			break;
		case OFF:
			BSP_LED_Off(leds.index);
			break;
		default:
			break;
		}
		//Task 2, Update OnState delay, and change State if required
		if (delayStart(&leds.on)) {
			delayWrite(&leds.on, TIEMPOS[TIEMPOS_index] * duty(DUTY_CYCLE));
			state = OFF;
		}
		//Task 3, update Cycle delay, and update delays if required
		if (delayStart(&leds.cycle)) {
			TIEMPOS_index = (TIEMPOS_index + 1) % len(TIEMPOS);
			delayWrite(&leds.cycle, TIEMPOS[TIEMPOS_index] * duty(DUTY_CYCLE));
			delayWrite(&leds.on, TIEMPOS[TIEMPOS_index]);
			state = ON;
		}

	}
}




