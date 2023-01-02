/*
 * manager.cpp
 *
 *  Created on: Nov 16, 2022
 *      Author: student
 */
#include "main.h"
#include "Rtc.h"
#include "led.h"
#include "dht.h"
#include "button.h"
#include "buzzer.h"
#include "cli_command.h"
#include "parameters.h"
#include "flash.h"



extern TIM_HandleTypeDef htim3;
Led rLed = Led(GPIOA,RedLed_Pin);
Button btn = Button(SW1_BTN_GPIO_Port,SW1_BTN_Pin);
Buzzer bzr = Buzzer(&htim3,TIM_CHANNEL_1);
extern Parameters* prmFromFlash;
extern Parameters prm1;
Flash flsh = Flash();
AlrarmState alrmState;

bool isDetected = false;
extern double temperature;
extern double humidity;



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == SW1_BTN_Pin)
	{
		printf("Temperature:%.1f  Humidity:%.1f \r\n", temperature ,humidity);
		bzr.SetOff();
		isDetected = true;
	}

}

extern "C"  void CheckTemperature()
{

	if((temperature >= prm1.criticalTh)&&(isDetected == false))
	{
		alrmState = ALARM_FULL;
	}
	if((temperature >= prm1.criticalTh)&&(isDetected == true))
	{
		alrmState = ALARM_DETECTED;
	}
	if((temperature >= prm1.warningTh)&&(temperature < prm1.criticalTh - prm1.hysteresis))
	{
		alrmState = ALARM_HALF;
	}
	if(temperature < prm1.warningTh - prm1.hysteresis)
	{
		alrmState = ALARM_OFF;
	}

}
extern "C" void Manager(void *argument)
{
  /* USER CODE BEGIN Manager */
	CliInit();
	InitParameters();
	printf("Temperature Monitoring\r\n");
	alrmState = ALARM_OFF;
  /* Infinite loop */
  for(;;)
  {
  CheckTemperature();
  switch(alrmState){
	  case ALARM_OFF:
		  rLed.SetOff();
		  bzr.SetOff();
		  isDetected = false;
		  break;
	  case ALARM_HALF:
		  rLed.SetON();
		  bzr.SetOff();
		  isDetected = false;
		  break;
	  case 	ALARM_DETECTED:
		  rLed.SetBlink();
		  bzr.SetOff();
		  break;
	  case 	ALARM_FULL:
		  rLed.SetBlink();
		  bzr.SetOn();
	  break;
	  default:
	  break;

	}
  osDelay(1);
  }

 }
  /* USER CODE END Manager */


