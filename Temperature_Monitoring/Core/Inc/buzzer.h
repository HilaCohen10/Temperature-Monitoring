/*
 * buzzer.h
 *
 *  Created on: Nov 19, 2022
 *      Author: Hila
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include <stdint.h>
#include "main.h"
 enum state
{
  BUZZER_OFF,
  BUZZER_ON,
  BUZZER_MELODY
};

class Buzzer
{
private:
	state _state;
	uint32_t _channel;
	TIM_HandleTypeDef* _pwmTimer;

public:
	Buzzer(TIM_HandleTypeDef* pwmTimer,uint32_t channel);
	void On();
	void Off();
	state GetState()
	{
		return _state;
	}
	void SetOn()
	{
		_state = BUZZER_ON;
	}
	void SetOff()
	{
		_state = BUZZER_OFF;
	}

};

#endif /* INC_BUZZER_H_ */
