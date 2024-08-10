/**
 ********************************************************************************
 * @file    API_rtc.c
 * @author  rami
 * @date    Aug 2, 2024
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "API_rtc.h"
#include "API_uart.h"
/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define CLOCKSPEED 100000
#define RTC_ADDRESS	(uint8_t)0b01101000
//Registers Address
#define RTC_SECOND 	0x00
#define RTC_MINUTE 	0x01
#define RTC_HOUR  	0x02
#define RTC_DOW    	0x03
#define RTC_DATE   	0x04
#define RTC_MONTH  	0x05
#define RTC_YEAR   	0x06
#define RTC_CONTROL 0x07
#define RTC_UTC_HR	0x08
#define RTC_UTC_MIN	0x09
#define RTC_CENT    0x10
#define RTC_RAM   	0x11
#define RTC_TIMEOUT	1000
#define HALT_BIT	(1<<7)
/************************************
 * PRIVATE TYPEDEFS
 ************************************/
typedef struct
{
	I2C_HandleTypeDef* hi2c;
	uint8_t address;
	bool_t halt;
}
rtc_t;
/************************************
 * STATIC VARIABLES
 ************************************/
static I2C_HandleTypeDef hi2c2;
static rtc_t rtcHandle =
		{
				.hi2c = &hi2c2,
				.address = RTC_ADDRESS,
				.halt = false,
		};
/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = CLOCKSPEED;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}
static void rtcSetRegByte (uint8_t regAddr, uint8_t val)
{
	uint8_t bytes[2]= {regAddr, val};
	if (HAL_I2C_Master_Transmit(rtcHandle.hi2c, (rtcHandle.address<<1), bytes, 2, 100)!= HAL_OK)
		{
		uartSendString((uint8_t*)"error tx RTC\n\r");
		}
}
static uint8_t rtcGetRegByte (uint8_t regAddr)
{
	uint8_t val;
	if (HAL_I2C_Master_Transmit(rtcHandle.hi2c, (rtcHandle.address<<1), &regAddr, 1, RTC_TIMEOUT)!= HAL_OK)
		{
		uartSendString((uint8_t*)"error tx RTC \n\r");
		}
	if (HAL_I2C_Master_Receive(rtcHandle.hi2c, (rtcHandle.address<<1), &val, 1, RTC_TIMEOUT)!= HAL_OK)
		{
		uartSendString((uint8_t*)"error rx RTC \n\r");
		}
	return val;
}
static uint8_t decodeBCD (uint8_t bcd)
{
	return (((bcd & 0xf0) >> 4) * 10) + (bcd & 0x0f);
}
static uint8_t encodeBCD (uint8_t dec)
{
	return (dec % 10 + ((dec / 10) << 4));
}
/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void rtcInit()
{
	MX_I2C2_Init();
	rtcHaltnt();
	rtcSetTimeZone(4,0); //Montevideo
}
void rtcHalt()
{
	rtcSetRegByte(RTC_SECOND, HALT_BIT);
	rtcHandle.halt = true;
}
void rtcHaltnt()
{
	rtcSetRegByte(RTC_SECOND, 0x00);
	rtcHandle.halt = false;
}
bool_t rtcIsHalted()
{
	return (bool_t)(rtcGetRegByte(RTC_SECOND)&0x80)>>7;
}
uint8_t rtcGetDate(void)
{
	return decodeBCD(rtcGetRegByte(RTC_DATE));
}
uint8_t rtcGetMonth(void)
{
	return decodeBCD(rtcGetRegByte(RTC_MONTH));
}
uint16_t rtcGetYear(void)
{
	return decodeBCD(rtcGetRegByte(RTC_YEAR));
}

uint8_t rtcGetHour(void)
{
	return decodeBCD(rtcGetRegByte(RTC_HOUR));
}
uint8_t rtcGetMinute(void)
{
	return decodeBCD(rtcGetRegByte(RTC_MINUTE));
}
uint8_t rtcGetSecond(void)
{
	return decodeBCD(rtcGetRegByte(RTC_SECOND));
}
int8_t rtcGetTimeZoneHour(void)
{
	return decodeBCD(rtcGetRegByte(RTC_UTC_HR));
}
uint8_t rtcGetTimeZoneMin(void)
{
	return decodeBCD(rtcGetRegByte(RTC_UTC_MIN));
}
void rtcSetDate(uint8_t date)
{
	if ((date>31) | (date<1))
	{
		Error_Handler();
	}
	rtcSetRegByte(RTC_DATE, encodeBCD(date));
}
void rtcSetMonth(uint8_t month)
{
	if ((month >12) | (month <1))
	{
		Error_Handler();
	}
	rtcSetRegByte(RTC_MONTH, encodeBCD(month));
}
void rtcSetYear(uint16_t year)
{
	if ((year>2099) | (year<2000))
	{
		Error_Handler();
	}
	rtcSetRegByte(RTC_YEAR, encodeBCD(year));
}

void rtcSetHour(uint8_t hour_24mode)
{
	if ((hour_24mode>23) | (hour_24mode<0))
	{
		Error_Handler();
	}
	rtcSetRegByte(RTC_HOUR, encodeBCD(hour_24mode));
}
void rtcSetMinute(uint8_t minute)
{
	if ((minute>59) | (minute<0))
	{
		Error_Handler();
	}
	rtcSetRegByte(RTC_MINUTE, encodeBCD(minute));
}
void rtcSetSecond(uint8_t second)
{
	if ((second>59) | (second<1))
	{
		Error_Handler();
	}
	rtcSetRegByte(RTC_SECOND, encodeBCD(second));
}
void rtcSetTimeZone(int8_t hr, uint8_t min)
{
	if ((hr>12) | (hr<-12))
	{
		Error_Handler();
	}
	if ((min>59) | (min<0))
	{
		Error_Handler();
	}
	rtcSetRegByte(RTC_UTC_HR, encodeBCD(hr));
	rtcSetRegByte(RTC_UTC_MIN, encodeBCD(min));
}
