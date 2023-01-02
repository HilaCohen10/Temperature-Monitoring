/*
 * led.h
 *
 *  Created on: Nov 17, 2022
 *      Author: student
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "main.h"
#include "stdint.h"

#define BLINK_DELAY 300
enum Status
{
	ON,
	OFF,
	BLINK
};


class Led {
private:
	Status _status;
	int _counter;
	int _period;
	uint16_t _Pin;
	GPIO_TypeDef  *_Port;
public:
	Led (GPIO_TypeDef  *GPIOx,uint32_t pin);

	void on();

	void off();

	void blink();

	void toggle()
	{
		if(this->_status == BLINK)
		{
			HAL_GPIO_TogglePin(this->_Port,this->_Pin);
		}
	}
	void SetOff()
	{
		_status = OFF;
	}
	void SetON()
	{
		_status = ON;
	}
	void SetBlink()
	{
		_status = BLINK;
	}
	Status GetState()
	{
		return _status;
	}


};

#endif /* INC_LED_H_ */
