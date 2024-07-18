#include "API_init.h"
#include "API_leds.h"
#include "API_delay.h"

static ledState_t ledFSM;
static bool_t isledOn;
delay_t onTime;
delay_t offTime;

void ledSetCycleDuration (tick_t duration) {
	delayWrite(&onTime, duration);
	delayWrite(&offTime, duration);
}

void ledSetOnDuration (tick_t duration) {
	delayWrite(&onTime, duration);
}

void ledSetOffDuration (tick_t duration) {
	delayWrite(&offTime, duration);
}

void ledFSM_init() {

	isledOn = true;
	ledFSM = LED_ON;

	onTime = delayInit();
	offTime = delayInit();


}

void ledFSM_update() {

	switch (ledFSM) {

	case LED_ON:
		delayStart(&onTime);
		isledOn = true;
		ledFSM = WAITING;

		break;

	case LED_OFF:
		delayStart(&offTime);
		isledOn = false;
		ledFSM = WAITING;

		break;

	case WAITING:
		if (delayRead(&onTime) && isledOn) {
			ledFSM = LED_OFF;
			break;
		}
		if (delayRead(&offTime) && !isledOn) {
			ledFSM = LED_ON;
			break;
		}

		break;

	default:
		while (1) {
			//Surprise!
		}
		break;

	}

}

bool_t readLed () {
	return isledOn;
}
