#include "main.h"

#include "API_delay.h"
#include "API_leds.h"
#include "API_debounce.h"
#include "API_lcd.h"

#define UNO 500
#define DOS 100
#define MyI2C_LCD 	I2C_LCD_1

static void SystemClock_Config(void);

int main(void)
{
	//HAL Inits
	HAL_Init();
	SystemClock_Config();

	//Software Inits
	ledFSM_init();
	debounceFSM_init();
	I2C_LCD_Init(MyI2C_LCD);

	I2C_LCD_SetCursor(MyI2C_LCD, 0, 0);
	I2C_LCD_WriteString(MyI2C_LCD, "Me debes:");
	I2C_LCD_SetCursor(MyI2C_LCD, 0, 1);
	I2C_LCD_WriteString(MyI2C_LCD, "un combo de McDonalds");

	while (1)
	{

	//Poll
	ledFSM_update();
	debounceFSM_update();

	//Act
	(readLed()) ? ledOn(LED1) : ledOff(LED1);
	(readKey()) ? ledSetCycleDuration(DOS) : ledSetCycleDuration(UNO);

	}

}

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
void Error_Handler(void)
{

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
