/*
 * buzzer.cpp
 *
 *  Created on: Nov 26, 2022
 *      Author: Hila
 */


#include "buzzer.h"
#include "cmsis_os.h"
#include <stdio.h>
extern Buzzer bzr;

Buzzer::Buzzer(TIM_HandleTypeDef* pwmTimer,uint32_t channel)
{
	_state = BUZZER_OFF;
	_pwmTimer = pwmTimer;
	_channel = channel;
}
void Buzzer::On()
{
			HAL_TIM_Base_Start(_pwmTimer);
			HAL_TIM_PWM_Start(_pwmTimer, _channel);
}
void Buzzer::Off()
{
	{

			HAL_TIM_Base_Stop(_pwmTimer);
			HAL_TIM_PWM_Stop(_pwmTimer,_channel);

	}
}
extern "C" void BuzzerFunc(void *argument)
{
  /* USER CODE BEGIN BuzzerFunc */

  /* Infinite loop */
  for(;;)
  {
	if(bzr.GetState() == BUZZER_ON)
	{
	  bzr.On();
	}
	else
	{
		bzr.Off();
	}
    osDelay(1);
  }
  /* USER CODE END BuzzerFunc */
}
