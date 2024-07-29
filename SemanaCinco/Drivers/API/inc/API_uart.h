/**
 ********************************************************************************
 * @file    API_uart.h
 * @author  rami
 * @date    Jul 25, 2024
 * @brief   
 ********************************************************************************
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "main.h"
/************************************
 * MACROS AND DEFINES
 ************************************/
#define BAUDRATE 115200
#define UART_CONFIG_STRING ((uint8_t*) \
		"UART Initialized:\r\n" \
		"Instance = USART3\r\n" \
		"BaudRate = 115200\r\n" \
		"WordLength = UART_WORDLENGTH_8B\r\n" \
		"StopBits = UART_STOPBITS_1\r\n" \
		"Parity = UART_PARITY_NONE\r\n" \
		"Mode = UART_MODE_TX_RX\r\n" \
		"HwFlowCtl = UART_HWCONTROL_NONE\r\n" \
		"OverSampling = UART_OVERSAMPLING_16\r\n")
/************************************
 * TYPEDEFS
 ************************************/

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
bool_t uartInit ();
void uartSendString (uint8_t* pstring);
void uartSendStringSize (uint8_t* pstring, uint16_t size);
#ifdef __cplusplus
}
#endif

#endif 
