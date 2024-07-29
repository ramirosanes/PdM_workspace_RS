#include "API_delay.h"

void delayInit(delay_t *delay)
{

	delay->duration = 0;
	delay->startTime = HAL_GetTick();
	delay->elapsedTime = (HAL_GetTick() - delay->startTime);
	delay->running = false;

}

void delayWrite(delay_t *delay, tick_t duration)
{
	delay->duration = duration;
}

void delayStart(delay_t *delay)
{

	if (!(delay->running)) {
		delay->startTime = HAL_GetTick();
		delay->running = true;
	}

}

bool_t delayRead(delay_t *delay)
{

	delay->elapsedTime = HAL_GetTick() - delay->startTime;
	bool_t timeOut = delay->elapsedTime >= delay->duration;

	if (delayIsRunning(delay) && timeOut) {
		delay->running = false;
	}

	return timeOut;
}

bool_t delayIsRunning(delay_t *delay)
{
	return delay->running;
}

