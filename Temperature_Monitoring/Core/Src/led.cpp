/*
 * led.cpp
 *
 *  Created on: Nov 20, 2022
 *      Author: student
 */


#include "led.h"
#include "cmsis_os.h"
extern Led rLed;
Led::Led (GPIO_TypeDef  *GPIOx,uint32_t pin)
{
	_Pin = pin;
	_Port = GPIOx;
	_status = OFF;
	_period = BLINK_DELAY;
	_counter = 0;
}
void Led::on()
{
	_status = ON;
	HAL_GPIO_WritePin(_Port, _Pin, GPIO_PIN_SET);
}
void Led::off()
{
	this->_status = OFF;
	HAL_GPIO_WritePin(_Port, _Pin, GPIO_PIN_RESET);
}
void Led::blink()
{
	this->_status = BLINK;
	this->_counter = 0;
	this->_period = BLINK_DELAY;
}



extern "C" void LedBlink(void *argument)
{
  /* USER CODE BEGIN LedBlink */

  /* Infinite loop */
  for(;;)
  {
	  if(rLed.GetState() == BLINK)
	  {
			rLed.toggle();
		    osDelay(BLINK_DELAY);
	  }
	  if(rLed.GetState() == ON)
	  {
			rLed.on();

	  }
	  if(rLed.GetState() == OFF)
	  {
			rLed.off();

	  }

	  osDelay(1);

  }
  /* USER CODE END LedBlink */
}
