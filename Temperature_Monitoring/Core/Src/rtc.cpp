/*
 * rtc.cpp
 *
 *  Created on: Dec 18, 2022
 *      Author: Hila
 */


#include "Rtc.h"

Rtc::Rtc(I2C_HandleTypeDef * hi2c, uint32_t devAddr)
{
	_hi2c = hi2c;
	_devAddr = devAddr;
}
void Rtc::rtcGetTime()
{
	uint8_t buffer[RTC_DATE_TIME_SIZE];
	HAL_StatusTypeDef isOk;
	isOk = HAL_I2C_Mem_Read(_hi2c, _devAddr, 0, 1, buffer, RTC_DATE_TIME_SIZE, 0xFF);
	if(isOk != HAL_OK)
	{
		printf("error in getting time in line: %d and file: %s\n\n\r",__LINE__,__FILE__);
		return;
	}
	// remove stop bit if set
	buffer[0] &= ~RTC_START_STOP;
	dt._sec = bcdToInt(buffer[0]);
	dt._min = bcdToInt(buffer[1]);
	dt._hours = bcdToInt(buffer[2]);
	dt._weekDay = buffer[3] & 0x07;
	dt._day = bcdToInt(buffer[4]);
	dt._month = bcdToInt(buffer[5]);
	dt._year = bcdToInt(buffer[6]);
}
HAL_StatusTypeDef Rtc::rtcSetTime()
{
	HAL_StatusTypeDef status;
	uint8_t buffer[RTC_DATE_TIME_SIZE];

	buffer[0] = intToBcd(dt._sec, 0, 59);
	buffer[1] = intToBcd(dt._min, 0, 59);
	buffer[2] = intToBcd(dt._hours, 0, 59);
	buffer[3] = dt._weekDay < 1 || dt._weekDay > 7 ? 0 : dt._weekDay;
	buffer[4] = intToBcd(dt._day, 1, 31);
	buffer[5] = intToBcd(dt._month, 1, 12);
	buffer[6] = intToBcd(dt._year, 1, 99);

	status = HAL_I2C_Mem_Write(_hi2c, _devAddr, 0, 1, buffer, RTC_DATE_TIME_SIZE, 0xFF);
	return status;
}
