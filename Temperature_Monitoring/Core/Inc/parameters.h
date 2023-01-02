/*
 * parameters.h
 *
 *  Created on: 24 Nov 2022
 *      Author: student
 */

#ifndef INC_PARAMETERS_H_
#define INC_PARAMETERS_H_
#include <stdint.h>

struct Parameters
{

	uint32_t warningTh;
	uint32_t criticalTh;
	uint32_t hysteresis;
	uint32_t magicNumber;
};

enum AlrarmState
{
	ALARM_OFF,
	ALARM_HALF,
	ALARM_DETECTED,
	ALARM_FULL
};

extern "C" void InitParameters();
#endif /* INC_PARAMETERS_H_ */
