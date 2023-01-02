/*
 * Rtc.h
 *
 *  Created on: Oct 25, 2022
 *      Author: Hila
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_
#include "main.h"
#include "cmsis_os.h"
#include <stdio.h>
#define RTC_START_STOP      (1 << 7)
#define RTC_DATE_TIME_SIZE  7
#define DEVICE_ADDR         0xD0

class Rtc
{
public:
	I2C_HandleTypeDef * _hi2c;
	uint8_t _devAddr;

	struct DateTime
	{

		int _sec;
		int _min;
		int _hours;
		int _weekDay;
		int _day;
		int _month;
		int _year;
	}dt;

	const uint32_t DaysInYear    = 365;
	const uint32_t SecondsInMin  = 60;
	const uint32_t SecondsInHour = 3600;
	const uint32_t SecondsInDay  = 86400;

	const int DaysUntilMonth[13] = {
			0,
			31,
			59,
			90,
			120,
			151,
			181,
			212,
			243,
			273,
			304,
			334,
			365
	};

	Rtc(I2C_HandleTypeDef * hi2c, uint32_t devAddr);
	void rtcGetTime();
	HAL_StatusTypeDef rtcSetTime();

private:
	static int bcdToInt(uint8_t bcd) // private
	{
		return (bcd >> 4) * 10 + (bcd & 0x0F);
	}

	static uint8_t intToBcd(int value, int minVal, int maxVal)//private
	{
		if (value < minVal || value > maxVal) {
			return 0;
		}

		return ((value / 10) << 4) | (value % 10);
	}


};


#endif /* INC_RTC_H_ */
