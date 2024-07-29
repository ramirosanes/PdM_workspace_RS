#include "main.h"
#include "API_delay.h"
#include "API_leds.h"

static ledFSM_t ledFSM;

void ledFSM_init()
{

	ledFSM.isOn = true;
	ledFSM.state = LED_ON;

	delayInit(&ledFSM.onTime);
	delayInit(&ledFSM.offTime);

	ledSetCycleDuration(500);


}

void ledFSM_update()
{

	switch (ledFSM.state) {

	case LED_ON:
		delayStart(&ledFSM.onTime);
		ledFSM.isOn = true;
		ledFSM.state = WAITING;

		break;

	case LED_OFF:
		delayStart(&ledFSM.offTime);
		ledFSM.isOn = false;
		ledFSM.state = WAITING;

		break;

	case WAITING:
		if (delayRead(&ledFSM.onTime) && ledFSM.isOn) {
			ledFSM.state = LED_OFF;
			break;
		}
		if (delayRead(&ledFSM.offTime) && !ledFSM.isOn) {
			ledFSM.state = LED_ON;
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

void ledSetCycleDuration (tick_t duration) {
	delayWrite(&ledFSM.onTime, duration);
	delayWrite(&ledFSM.offTime, duration);
}

void ledSetOnDuration (tick_t duration) {
	delayWrite(&ledFSM.onTime, duration);
}

void ledSetOffDuration (tick_t duration) {
	delayWrite(&ledFSM.offTime, duration);
}

bool_t readLed () {
	return ledFSM.isOn;
}

void ledOn(userLeds_t ledNo)
{
	switch (ledNo)
	{
	case LED1:
		HAL_GPIO_WritePin(LED1_GPIO_Port, ledNo, LED_ON);
		break;

	case LED2:
		HAL_GPIO_WritePin(LED2_GPIO_Port, ledNo, LED_ON);
		break;

	case LED3:
		HAL_GPIO_WritePin(LED3_GPIO_Port, ledNo, LED_ON);
		break;

	default:
	break;
	}
}

void ledOff (uint16_t ledNo)
{
	switch (ledNo)
	{
	case LED1:
		HAL_GPIO_WritePin(LED1_GPIO_Port, ledNo, LED_OFF);
		break;

	case LED2:
		HAL_GPIO_WritePin(LED2_GPIO_Port, ledNo, LED_OFF);
		break;

	case LED3:
		HAL_GPIO_WritePin(LED3_GPIO_Port, ledNo, LED_OFF);
		break;

	default:
	break;
	}
}


