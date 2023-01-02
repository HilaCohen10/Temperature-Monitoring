/*
 * dht.cpp
 *
 *  Created on: Nov 20, 2022
 *      Author: student
 */


#include "dht.h"
#include "cmsis_os.h"
extern TIM_HandleTypeDef htim16;
extern Dht dht;
extern osSemaphoreId_t sem1Dht;
extern osSemaphoreId_t sem2Dht;
double temperature = -1;
double humidity = -1 ;

Dht dht = Dht(dht11_GPIO_Port,dht11_Pin,&htim16);
bool printDht = false;
Dht::Dht(GPIO_TypeDef * gpioPort, uint16_t gpioPin, TIM_HandleTypeDef * timer)
{
	_gpioPort = gpioPort;
	_gpioPin = gpioPin;
	_timer = timer;
	_counter = 0;
	_maxCounter = 0;
	_state = DHT_STATE_NO_DATA;
	_temperature = 0.0;
	_humidity = 0.0;
}
HAL_StatusTypeDef Dht::read()
{

	setGpioOutput();

	HAL_TIM_Base_Start(_timer);
	__HAL_TIM_SET_COUNTER(_timer, 0);

	// switch the sensor on by putting the line in '0'
	HAL_GPIO_WritePin(_gpioPort, _gpioPin, GPIO_PIN_RESET);
	while (__HAL_TIM_GET_COUNTER(_timer) < 19000);

	HAL_GPIO_WritePin(_gpioPort, _gpioPin, GPIO_PIN_SET);

	// start listening
	setGpioInput();

	// wait for response 20-40us
	if (!waitWhileEqual(1, 40)) {
		return HAL_ERROR;
	}

	// DHT should keep in low for 80us
	if (!waitWhileEqual(0, 80)) {
		return HAL_ERROR;
	}

	// DHT should keep in high for 80us
	if (!waitWhileEqual(1, 80)) {
		return HAL_ERROR;
	}

	// DHT start send data bits

	uint8_t data[5] = { 0 };
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 8; j++) {

			// DHT keeps in low for 50us before each bit
			if (!waitWhileEqual(0, 60)) {
				return HAL_ERROR;
			}

			// DHT sends bit keeping line in high maximum 70us
			if (!waitWhileEqual(1, 70)) {
				return HAL_ERROR;
			}

			data[i] <<= 1;

			// 26-28us for '0' and 70us for '1', so 50us is a good split time
			if (__HAL_TIM_GET_COUNTER(_timer) > 50) {
				data[i] |= 1;
			}
		}
	}

	uint8_t checksum = data[0] + data[1] + data[2] + data[3];
	if (checksum != data[4]) {
		return HAL_ERROR;
	}
	_humidity = (double)data[0] + ((double)data[1]) / 10;
	_temperature = (double)data[2] + ((double)data[3]) / 10;



	// wait while DHT return to high
	if (!waitWhileEqual(0, 100)) {
		return HAL_ERROR;
	}

	HAL_TIM_Base_Stop(_timer);

	_state = DHT_STATE_READY;// for 41 task

	return HAL_OK;

}
int Dht::hasData()
{
	int hasData = _state == DHT_STATE_READY;
	if (hasData) {
		// reset state to avoid multiple reads
		_state = DHT_STATE_NO_DATA;
	}
	return hasData;
}
double Dht::getHumidty()
{
	return _humidity;
}
double Dht::getTempperature()
{
	return _temperature;
}

extern "C" void DhtPrintFunc(void *argument)
{
  /* USER CODE BEGIN DhtPrintFunc */
  /* Infinite loop */
  for(;;)
  {

	  	  osSemaphoreAcquire(sem2Dht, osWaitForever);
	  	  if(printDht == true)
	  	  {
	  		 if (dht.hasData())
	  		 {
	  			 printf("Temperature:%.1f  Humidity:%.1f \r\n", temperature ,humidity);
	  		 }
	  		 else
	  		 {
	  			printf("Error printing dht\r\n");
	 		 }
	  	  }

  }
  /* USER CODE END DhtPrintFunc */
}
extern "C" void DhtDelay(void *argument)
{
  /* USER CODE BEGIN DhtDelay */

  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);

    osSemaphoreRelease(sem1Dht);
  }
  /* USER CODE END DhtDelay */
}
extern "C" void DhtFunc(void *argument)
{
  /* USER CODE BEGIN DhtFunc */

  /* Infinite loop */
  for(;;)
  {

	      osSemaphoreAcquire(sem1Dht, osWaitForever);
		  HAL_StatusTypeDef status = dht.read();
		  if(!status)
		  {
			  temperature = dht.getTempperature();
			  humidity = dht.getHumidty();
		  }
		  osSemaphoreRelease(sem2Dht);
    //osDelay(1);
  }
  /* USER CODE END DhtFunc */
}
