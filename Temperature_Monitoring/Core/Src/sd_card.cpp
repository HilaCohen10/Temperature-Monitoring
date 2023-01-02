/*
 * sd_card.cpp
 *
 *  Created on: Dec 1, 2022
 *      Author: student
 */


#include "myfile.h"
#include "cmsis_os.h"
#include "parameters.h"
extern AlrarmState alrmState;
extern File logFile;
extern File eventFile;
#define ONE_SECOND 1000
#define ONE_MINUTE 60000U


extern "C" void LogTask(void *argument)
{
	/* USER CODE BEGIN LogTask */
	osDelay(ONE_SECOND); //a short delay is important to let the SD card settle
	FRESULT fres1;
	FATFS FatFs;
	fres1 = f_mount(&FatFs, "", 1); //1=mount now
	if (fres1 != FR_OK)
	{
		printf("f_mount error (%i)\r\n", fres1);
	}
	TickType_t ticks = 0;
	ticks = xTaskGetTickCount();
	/* Infinite loop */
	for(;;)
	{
		if (alrmState == ALARM_OFF)
		{
			logFile.Write();
		}
		else
		{
			eventFile.Write();
		}

		ticks += ONE_MINUTE;
		osDelayUntil(ticks);
	}
	/* USER CODE END LogTask */
}

