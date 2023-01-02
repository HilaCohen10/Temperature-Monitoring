/*
 * button.h
 *
 *  Created on: Nov 19, 2022
 *      Author: Hila
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_


#include "main.h"
#include <stdint.h>
enum ButtonState
{
	NOT_PRESSED,
	PRESSED,
};

class Button {
private:
	uint32_t _Pin;
	GPIO_TypeDef  *_Port;
	ButtonState _State;
public:
	Button (GPIO_TypeDef*  port,uint32_t pin)
	{
		_Port = port;
		_Pin = pin;
		_State = NOT_PRESSED;
	}

};

#endif /* INC_BUTTON_H_ */
