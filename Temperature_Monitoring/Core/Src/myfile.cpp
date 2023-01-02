/*
 * myfile.cpp
 *
 *  Created on: Dec 1, 2022
 *      Author: student
 */

#include "myfile.h"
#include "Rtc.h"
#include "parameters.h"
extern Rtc rtc;
extern double temperature;
extern AlrarmState alrmState;
char strLog[LOG_MAX_SIZE];


File logFile = File ("Log.txt");
File eventFile = File ("Event.txt");
UINT bytesWrote;

File::File(const char* newFileName)
{
	strcpy(_fileName,newFileName);
}


void File:: GetNewLine()
{

	const char* severity[4] = {"Normal","Warning","Critical-Detected","Critical"};
	rtc.rtcGetTime();
	sprintf(strLog,"Time %d:%d:%d %d.%d.%d ,Severity: %s ,Temperature = %.2f\r\n",
			rtc.dt._hours,rtc.dt._min,rtc.dt._sec,rtc.dt._day,rtc.dt._month,rtc.dt._year,severity[alrmState],temperature);

}

void File::Write()
{
	BYTE readBuf[LOG_MAX_SIZE];
	_fres = f_open(&_fil,_fileName, FA_WRITE|FA_OPEN_ALWAYS|FA_OPEN_EXISTING);
	if(_fres != FR_OK)
	{
		printf("f_open error (%i)\r\n", _fres);
	}

	f_lseek(&_fil, f_size(&_fil));

	GetNewLine();
	strncpy((char*)readBuf, strLog, strlen(strLog));
	_fres = f_write(&_fil, readBuf, strlen(strLog), &bytesWrote);
	if(_fres != FR_OK)
	{
		printf("f_write error (%i)\r\n",_fres);
	}

	f_close(&_fil);
}

void File::Print()
{
	char line[LOG_MAX_SIZE];
	_fres = f_open(&_fil, _fileName, FA_READ);
	if(_fres != FR_OK)
	{
		printf("f_open error (%i)\r\n", _fres);
	}

	/* Read every line and display it */
	while (f_gets(line, sizeof(line), &_fil))
	{
		printf(line);
		printf("\r\n");
	}

	_fres = f_close(&_fil);
	if(_fres != FR_OK)
	{
		printf("f_close error (%i)\r\n",_fres);
	}
	else
	{
		printf("End of print\r\n");
	}


}
void File::Clear()
{
	_fres = f_open(&_fil, _fileName, FA_CREATE_ALWAYS);
	if(_fres != FR_OK)
	{
		printf("clear file error( %i ) \r\n", _fres);
	}
	_fres = f_close(&_fil);
	if(_fres != FR_OK)
	{
		printf("f_close error (%i)\r\n",_fres);
	}
	else
	{
		printf("File Cleared\r\n");
	}

}


