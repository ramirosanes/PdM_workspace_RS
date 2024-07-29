/**
 ********************************************************************************
 * @file    API_uart.c
 * @author  rami
 * @date    Jul 25, 2024
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "API_uart.h"
/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/
static UART_HandleTypeDef huart3;
/************************************
 * GLOBAL VARIABLES
 ************************************/


/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = BAUDRATE;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}
/************************************
 * GLOBAL FUNCTIONS
 ************************************/
bool_t uartInit ()
{
	MX_USART3_UART_Init();
	uartSendString(UART_CONFIG_STRING);
	return true;
}

void uartSendString (uint8_t* pstring)
{
	HAL_StatusTypeDef uartStatus;
	uint16_t size = 0;

	if (pstring == NULL)
	{
		Error_Handler();
	}

	while (pstring[size] != '\0')
	{
		size++;
	}

	//TX and Status
	uartStatus = HAL_UART_Transmit(&huart3, pstring, size, HAL_MAX_DELAY);
	if (uartStatus != HAL_OK)
	{
		Error_Handler();
	}
}

void uartSendStringSize (uint8_t* pstring, uint16_t size)
{
	HAL_StatusTypeDef uartStatus;

	if (size == 0 | pstring == NULL)
	{
		Error_Handler();
	}

	uartStatus = HAL_UART_Transmit(&huart3, pstring, size, HAL_MAX_DELAY);
	if (uartStatus != HAL_OK)
	{
		Error_Handler();
	}
}

