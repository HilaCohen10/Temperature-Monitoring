/*
 * comm_task.cpp
 *
 *  Created on: Nov 16, 2022
 *      Author: student
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "cmsis_os.h"
#include "comm_task.h"


extern UART_HandleTypeDef huart2;

#define CMD_MAX 20
#define MAX_BUFFER_LENGTH 50

static uint8_t _cmdbuffer[MAX_BUFFER_LENGTH];
static int _cmdcount = 0;
static int _cmdprint = 0;

static Cmd _cmdArr[CMD_MAX];
static int _cmdArrCount = 0;


 extern "C" int _write(int fd, char* ptr, int len) {
    HAL_UART_Transmit(&huart2, (uint8_t *) ptr, len, HAL_MAX_DELAY);
    return len;
}
 void SetCmdArr()
{
	 _cmdArrCount = 0;
	 memset(_cmdArr,0,sizeof(_cmdArr));
}

 void RegisterCommand(const char* commandName,CliCommand* clicmd)
{

	_cmdArr[_cmdArrCount].commandName = commandName;
	_cmdArr[_cmdArrCount].cliCommand = clicmd;

	_cmdArrCount++;

}

 int commTask()
{
	uint8_t ch;

		HAL_StatusTypeDef Status = HAL_UART_Receive(&huart2, &ch, 1, 10);
		if (Status != HAL_OK)
		{
			if ((huart2.Instance->ISR & USART_ISR_ORE) != 0)
			{
				__HAL_UART_CLEAR_OREFLAG(&huart2);
			}

			// here we have a time to print the command
			while (_cmdprint < _cmdcount)
			{
				HAL_UART_Transmit(&huart2, &_cmdbuffer[_cmdprint++], 1, 0xFFFF);
			}

			return 0;
		}

		if (ch == '\r' || ch == '\n')
		{
			// here we have a time to print the command
			while (_cmdprint < _cmdcount)
			{
				HAL_UART_Transmit(&huart2, &_cmdbuffer[_cmdprint++], 1, 0xFFFF);
			}

			HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 0xFFFF);

			_cmdbuffer[_cmdcount] = 0;
			_cmdcount = 0;
			_cmdprint = 0;

			// command is ready
			return 1;
		}
		else if (ch == '\b')
		{
			char bs[] = "\b \b";
			_cmdcount--;
			_cmdprint--;
			HAL_UART_Transmit(&huart2, (uint8_t*)bs, strlen(bs), 0xFFFF);
		}
		else
		{
			if (_cmdcount >= MAX_BUFFER_LENGTH)
			{
				_cmdcount = 0;
				_cmdprint = 0;
			}

			_cmdbuffer[_cmdcount++] = ch;
		}

		return 0;
}


void handleCommand()
{
  char cmd[20];
  char secondCmd[30];


  int idx = 0;
  int params = sscanf((const char*)_cmdbuffer,"%s %s", cmd,secondCmd);

  if (params == 0)
  {
	  return;
  }


  for (idx = 0; idx < _cmdArrCount; idx++)
   {
 	  if (strcmp(cmd, _cmdArr[idx].commandName) == 0)
 	  {

 		  _cmdArr[idx].cliCommand->doCommand(secondCmd);
 		  break;
 	  }
   }

   if (idx == _cmdArrCount)
   {
 	  printf("Invalid command\r\n");
   }



}
extern "C" void UartRec(void *argument)
{
  /* USER CODE BEGIN UartRec */
  /* Infinite loop */
  for(;;)
  {
	  if (commTask())
	  {
		  handleCommand();
	  }
	  //osDelay(1);
  }
  /* USER CODE END UartRec */
}
