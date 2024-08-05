/**
 ********************************************************************************
 * @file    API_utils.c
 * @author  rami
 * @date    Jul 26, 2024
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "API_utils.h"
#include "API_uart.h"
#include "API_lcd.h"
/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define DEBOUNCE 100

#define NO_DURATION 0
#define NOT_RUNNING false
#define NOT_PRESSED false

#define PRESSED 	true
#define RUNNING		true
/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/
static button_t userButton;
/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/
/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
 GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

 /* GPIO Ports Clock Enable */
 __HAL_RCC_GPIOC_CLK_ENABLE();
 __HAL_RCC_GPIOH_CLK_ENABLE();
 __HAL_RCC_GPIOA_CLK_ENABLE();
 __HAL_RCC_GPIOB_CLK_ENABLE();
 __HAL_RCC_GPIOD_CLK_ENABLE();
 __HAL_RCC_GPIOG_CLK_ENABLE();

 /*Configure GPIO pin Output Level */
 HAL_GPIO_WritePin(GPIOB, LED1_Pin|LED3_Pin|LED2_Pin, GPIO_PIN_RESET);

 /*Configure GPIO pin Output Level */
 HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_RESET);

 /*Configure GPIO pin : USER_BUTTON_Pin */
 GPIO_InitStruct.Pin = USER_BUTTON_Pin;
 GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 HAL_GPIO_Init(USER_BUTTON_GPIO_Port, &GPIO_InitStruct);

 /*Configure GPIO pins : RMII_MDC_Pin RMII_RXD0_Pin RMII_RXD1_Pin */
 GPIO_InitStruct.Pin = RMII_MDC_Pin|RMII_RXD0_Pin|RMII_RXD1_Pin;
 GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
 GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
 HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

 /*Configure GPIO pins : RMII_REF_CLK_Pin RMII_MDIO_Pin RMII_CRS_DV_Pin */
 GPIO_InitStruct.Pin = RMII_REF_CLK_Pin|RMII_MDIO_Pin|RMII_CRS_DV_Pin;
 GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
 GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
 HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

 /*Configure GPIO pins : LED1_Pin LED3_Pin LED2_Pin */
 GPIO_InitStruct.Pin = LED1_Pin|LED3_Pin|LED2_Pin;
 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
 HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

 /*Configure GPIO pin : RMII_TXD1_Pin */
 GPIO_InitStruct.Pin = RMII_TXD1_Pin;
 GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
 GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
 HAL_GPIO_Init(RMII_TXD1_GPIO_Port, &GPIO_InitStruct);

 /*Configure GPIO pin : PG6 */
 GPIO_InitStruct.Pin = GPIO_PIN_6;
 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
 HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

 /*Configure GPIO pin : USB_OverCurrent_Pin */
 GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
 GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

 /*Configure GPIO pins : USB_SOF_Pin USB_ID_Pin USB_DM_Pin USB_DP_Pin */
 GPIO_InitStruct.Pin = USB_SOF_Pin|USB_ID_Pin|USB_DM_Pin|USB_DP_Pin;
 GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
 GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
 HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

 /*Configure GPIO pin : USB_VBUS_Pin */
 GPIO_InitStruct.Pin = USB_VBUS_Pin;
 GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 HAL_GPIO_Init(USB_VBUS_GPIO_Port, &GPIO_InitStruct);

 /*Configure GPIO pins : RMII_TX_EN_Pin RMII_TXD0_Pin */
 GPIO_InitStruct.Pin = RMII_TX_EN_Pin|RMII_TXD0_Pin;
 GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
 GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
 HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

static void delayRead (delay_t* delay)
{
	delay->elapsedTime = HAL_GetTick() - delay->startTime;
	delay->running = (delay->elapsedTime <= delay->duration);
}
/************************************
 * BUTTON GLOBAL FUNCTIONS
 ************************************/
void buttonInit ()
{
	MX_GPIO_Init();
	userButton.State = BUTTON_UP;
	userButton.isPressed = NOT_PRESSED;
	delayInit(&userButton.debounceDelay);
	delayWrite(&userButton.debounceDelay, DEBOUNCE);
}

void buttonFSM()
{
	switch (userButton.State)
	{

	case BUTTON_UP:

		userButton.isPressed = NOT_PRESSED;
		if (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin))
		{
			userButton.State = BUTTON_FALLING;
			delayStart(&userButton.debounceDelay);
		}
		break;

	case BUTTON_DOWN:

		userButton.isPressed = PRESSED;
		if (!HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin))
		{
			userButton.State = BUTTON_RISING;
			delayStart(&userButton.debounceDelay);
			lcdPrintString("holi");
		}
		break;

	case BUTTON_FALLING:

		if (!delayIsRunning(&userButton.debounceDelay))
		{
			if (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin))
			{
				userButton.State = BUTTON_DOWN;
				break;
			} else
			{
				userButton.State = BUTTON_UP;
				break;
			}
		}
		break;

	case BUTTON_RISING:

		if (!delayIsRunning(&userButton.debounceDelay))
		{
			if (!HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin))
			{
				uartSendString((uint8_t*)"hola");
				userButton.State = BUTTON_UP;
				break;
			} else
			{
				userButton.State = BUTTON_DOWN;
				break;
			}
		}
		break;
	default:
		Error_Handler();
	}
}

bool_t readUserButton()
{
	return userButton.isPressed;
}
/************************************
 * DELAY GLOBAL FUNCTIONS
 ************************************/
void delayInit (delay_t* delay)
{
	delay->duration = NO_DURATION;
	delay->startTime = HAL_GetTick();
	delay->elapsedTime = NO_DURATION;
	delay->running = NOT_RUNNING;
}

void delayStart (delay_t* delay)
{
	if (!(delay->running))
	{
		delay->startTime = HAL_GetTick();
		delay->running = RUNNING;
	}
}

void delayWrite (delay_t* delay, tick_t duration)
{
	delay->duration = duration;
}

bool_t delayIsRunning (delay_t* delay)
{
	delayRead(delay);
	return delay->running;
}
