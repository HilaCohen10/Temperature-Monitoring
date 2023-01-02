/*
 * dht.h
 *
 *  Created on: Nov 17, 2022
 *      Author: student
 */

#ifndef INC_DHT_H_
#define INC_DHT_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"

#define MAX_BITS 40

enum DhtState
{
	DHT_STATE_NO_DATA,
	DHT_STATE_READY,
	DHT_STATE_ERROR,
	DHT_STATE_POWER_ON,
	DHT_STATE_POWER_ON_ACK,
	DHT_STATE_INIT_RESPONSE,
	DHT_STATE_RECEIVE_DATA
};

class Dht
{
private:
	GPIO_TypeDef * _gpioPort;
	uint16_t _gpioPin;
	TIM_HandleTypeDef * _timer;
	int _counter;
	int _maxCounter;
	int _state;
	uint8_t _data[5];
	int _bit;
	double _temperature;
	double _humidity;
public:
	Dht(GPIO_TypeDef * gpioPort, uint16_t gpioPin, TIM_HandleTypeDef * timer);


private:

	int waitWhileEqual(int value, int expectedTime)
	{
		// set a timeout 20% bigger than the expected time
		uint32_t timeout = expectedTime + (expectedTime) / 5;

		__HAL_TIM_SET_COUNTER(_timer, 0);
		while (HAL_GPIO_ReadPin(_gpioPort, _gpioPin) == value) {
			if (__HAL_TIM_GET_COUNTER(_timer) > timeout) {
				// too much time in the state
				return false;
			}
		}

		return true;
	}

	void setGpioOutput()
	{
		GPIO_InitTypeDef gpioStruct = {0};

		gpioStruct.Pin = _gpioPin;
		gpioStruct.Mode = GPIO_MODE_OUTPUT_PP;
		gpioStruct.Pull = GPIO_NOPULL;
		gpioStruct.Speed = GPIO_SPEED_FREQ_LOW;

		HAL_GPIO_Init(_gpioPort, &gpioStruct);
		HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
	}

	void setGpioInput()
	{
		GPIO_InitTypeDef gpioStruct = {0};

		gpioStruct.Pin =_gpioPin;
		gpioStruct.Mode = GPIO_MODE_INPUT;
		gpioStruct.Pull = GPIO_NOPULL;
		gpioStruct.Speed = GPIO_SPEED_FREQ_LOW;

		HAL_GPIO_Init(_gpioPort, &gpioStruct);
		HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
	}


public:
	HAL_StatusTypeDef read();
	int hasData();
	double getHumidty();
	double getTempperature();

};


#endif /* INC_DHT_H_ */
