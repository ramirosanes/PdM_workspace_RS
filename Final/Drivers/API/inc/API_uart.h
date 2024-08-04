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
#define BAUDRATE 9600
#define UART_CONFIG_STRING ((uint8_t*) \
		"UART Initialized:11500 8B NP 1SP\r\n")
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
bool_t uartDeInit ();
void uartSendString (uint8_t* pstring);
void uartSendStringSize (uint8_t* pstring, uint16_t size);
#ifdef __cplusplus
}
#endif

#endif 
