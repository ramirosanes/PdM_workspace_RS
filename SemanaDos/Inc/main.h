#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */

//Cosas especificadas por el practico

typedef uint32_t tick_t;
typedef bool bool_t;
typedef struct {
	tick_t startTime;
	tick_t duration;
	bool_t running;
}
delay_t;
void delayInit (delay_t* delay, tick_t duration);
bool_t delayStart (delay_t* delay);
void delayWrite (delay_t*, tick_t duration);

//Cosas mias
#define DURATION 100
#define DUTY 2
#define LEDCOUNT 3

typedef struct {
	uint8_t index;
	uint8_t repeat;
	delay_t delay;
}
led_t;

void HW_Init (void);
void increaseIndex(led_t *leds);
void pattern (led_t* leds, delay_t* onState, uint32_t duration, uint8_t duty);
void specialPattern(led_t *leds, delay_t *onState, uint32_t duration[LEDCOUNT], uint8_t duty, uint8_t cantidad);

//Cosas que ya estaban

#define USARTx                           USART3
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_8
#define USARTx_TX_GPIO_PORT              GPIOD
#define USARTx_TX_AF                     GPIO_AF7_USART3
#define USARTx_RX_PIN                    GPIO_PIN_9
#define USARTx_RX_GPIO_PORT              GPIOD
#define USARTx_RX_AF                     GPIO_AF7_USART3

#endif
