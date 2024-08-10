/**
 ********************************************************************************
 * @file    API_rtc.h
 * @author  rami
 * @date    Aug 2, 2024
 * @brief   
 ********************************************************************************
 */

#ifndef API_INC_API_RTC_H_
#define API_INC_API_RTC_H_

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

/************************************
 * TYPEDEFS
 ************************************/

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void rtcInit();
void rtcHalt();
void rtcHaltnt();
bool_t rtcIsHalted();

uint8_t rtcGetDate(void);
uint8_t rtcGetMonth(void);
uint16_t rtcGetYear(void);

uint8_t rtcGetHour(void);
uint8_t rtcGetMinute(void);
uint8_t rtcGetSecond(void);
int8_t rtcGetTimeZoneHour(void);
uint8_t rtcGetTimeZoneMin(void);

void rtcSetDate(uint8_t date);
void rtcSetMonth(uint8_t month);
void rtcSetYear(uint16_t year);

void rtcSetHour(uint8_t hour_24mode);
void rtcSetMinute(uint8_t minute);
void rtcSetSecond(uint8_t second);
void rtcSetTimeZone(int8_t hr, uint8_t min);

#ifdef __cplusplus
}
#endif

#endif 
