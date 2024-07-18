#include "main.h"
#include <stdint.h>
#include <stdbool.h>

static void SystemClock_Config(void);
static void Error_Handler(void);

int main(void) {

	//HardWare Initializations
	HW_Init();

	//Variables initialization
	delay_t onState;
	led_t leds;
	uint32_t duraciones[3] = {1000, 200, 100}; //Configurar
	uint8_t cantidad = 5; //Configurar

	//Timers Initialization, one keeps track of LEDs index, other keeps tracking OnTime
	delayInit(&leds.delay, 0);
	delayInit(&onState, 0);

	while (1) {
		//Do nothing to see LEDs pattern
		//Keep pressed BLUE BUTTON for custom pattern
		//"no bloqueante": dentro de pattern y specialPattern se pueden colocar otras rutinas donde fue indicado
		if (BSP_PB_GetState(BUTTON_USER)) {
			specialPattern(&leds, &onState, duraciones, DUTY, cantidad);
		} else {
			pattern(&leds, &onState, DURATION, DUTY);
		}
	}
}

void pattern(led_t *leds, delay_t* onState, uint32_t duration, uint8_t duty) {

	delayWrite(&leds->delay, duration);
	delayWrite(onState, duration/duty);

	do {
		//Main program
		BSP_LED_On(leds->index);
	} while (!delayStart(onState));
	do {
		//Main program pt2
		BSP_LED_Off(leds->index);
	} while (!delayStart(&leds->delay));

	increaseIndex(leds);
}

void specialPattern(led_t *leds, delay_t *onState, uint32_t duration[3], uint8_t duty,uint8_t cantidad) {

	for (int i = 0; i < LEDCOUNT; i++) {

		delayWrite(&leds->delay, duration[i]);
		delayWrite(onState, duration[i] / duty);
		uint8_t repeat = 0;

		while (repeat < cantidad) {
			do {
				//Main program'
				BSP_LED_On(leds->index);
			} while (!delayStart(onState));
			do {
				//Main program' pt2
				BSP_LED_Off(leds->index);
			} while (!delayStart(&leds->delay));
			repeat++;
		}

		increaseIndex(leds);
	}

}
//Initializes a pre-declared delay struct.
void delayInit (delay_t* delay, tick_t duration) {

	delay->duration = duration;
	delay->startTime  = HAL_GetTick();
	delay->running = false;

}

//Increase LED index
void increaseIndex(led_t *leds) {

	leds->index = (leds->index + 1) % LEDCOUNT;

}
//returns True when timers up. sets running flag on if not already set on, and returns False rest of the cases.
bool_t delayStart (delay_t* delay) {

	if (!(delay->running)) {

		delay->startTime = HAL_GetTick(); //Time at the moment
		delay->running = true; //running since startTime
		return false;

	} else {

		if ((HAL_GetTick() - delay->startTime) >= delay->duration) {
			delay->running = false;
			return true;
		} else {
			return false;
		}
	}
}

//Update timers duration
void delayWrite (delay_t* delay, tick_t duration) {
	delay->duration = duration;
}

void HW_Init(void) {

	HAL_Init();
	SystemClock_Config();
	BSP_LED_Init(LED1);
	BSP_LED_Init(LED2);
	BSP_LED_Init(LED3);
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

}

//Cosas que ya estaban (Ignorar)

static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 360;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}

	if(HAL_PWREx_EnableOverDrive() != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}
}
static void Error_Handler(void)
{
	BSP_LED_On(LED2);
	while (1)
	{
	}
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
	while (1)
	{
	}
}
#endif
