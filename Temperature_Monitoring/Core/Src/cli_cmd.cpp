/*
 * cli_cmd.cpp
 *
 *  Created on: Nov 17, 2022
 *      Author: Hila
 */
#include <stdlib.h>
#include "Rtc.h"
#include "main.h"
#include "buzzer.h"
#include "cli_command.h"
#include "comm_task.h"
#include "flash.h"
#include "parameters.h"
#include "led.h"
#include "myfile.h"
extern I2C_HandleTypeDef hi2c1;
Rtc rtc = Rtc(&hi2c1,0xD0);
extern Parameters* prmFromFlash;
extern Parameters prm1;
extern Flash flsh;
extern Buzzer bzr;
extern Led rLed;
extern File logFile;
extern File eventFile;
extern bool printDht;
//Led bLed = Led(GPIOA,BlueLed_Pin); //was added to test new connections



static int setDateTime(Rtc* rtc, char *param)
{
	// if string is empty
	if (*param == '\0') {
		return 1;
	}
	if(strlen(param) != 17){
		printf("date string length = %d\r\n",strlen(param));
		return 1;
	}
	int ret = 0;
	const char s[5] = ".-:";
	char *token;

	/* get the first token */
	token = strtok(param, s);

	/* walk through other tokens */
	do {
		rtc->dt._day = atoi(token);
		if (rtc->dt._day < 0 || rtc->dt._day > 31) {
			ret = 1;
			break;
		}
		token = strtok(NULL, s);
		rtc->dt._month = atoi(token);
		if (rtc->dt._month < 0 || rtc->dt._month > 12) {
			ret = 1;
			break;
		}
		token = strtok(NULL, s);
		rtc->dt._year = atoi(token);
		if (rtc->dt._year < 0 || rtc->dt._year > 99) {
			ret = 1;
			break;
		}
		token = strtok(NULL, s);
		rtc->dt._hours = atoi(token);
		if (rtc->dt._hours < 0 || rtc->dt._hours > 23) {
			ret = 1;
			break;
		}

		token = strtok(NULL, s);
		rtc->dt._min = atoi(token);
		if (rtc->dt._min < 0 || rtc->dt._min > 59) {
			ret = 1;
			break;
		}
		token = strtok(NULL, s);
		rtc->dt._sec = atoi(token);
		if (rtc->dt._sec < 0 || rtc->dt._sec > 59) {
			ret = 1;
			break;
		}

	} while (0);

	return ret;

}

class ShowTime: public CliCommand
{
private:
	Rtc *_rtc;
public:
	ShowTime(Rtc *r)
	{
		_rtc = r;
	}

	void doCommand (char* param)
	{
		_rtc->rtcGetTime();
		printf("%02d:%02d:%02d %02d.%02d.%02d\r\n",
		_rtc->dt._hours,_rtc->dt._min,_rtc->dt._sec,
	    _rtc->dt._day,_rtc->dt._month,_rtc->dt._year);
	}

};

class SetDateandTime: public CliCommand
{
private:

	Rtc *_rtc;
public:
	SetDateandTime(Rtc *r)
	{
		_rtc = r;
	}
	void doCommand (char* param)
	{
		HAL_StatusTypeDef status;
		char strHelp[] = "-h";
		if(strcmp(param,strHelp) == 0){
			printf("date format\r\n");
			printf("dd.mm.yy-HH:MM:SS\r\n");
			return;
		}
		if(setDateTime(_rtc,param) == 0){
			status =_rtc->rtcSetTime();
			if(status == HAL_OK){
				printf("date time set Successfully\r\n");
			}else{
				printf("Error set date time status code = %d\r\n",status);
			}
		}else{
			printf("Invalid date time, Try again \r\n");
		}
	}

};

class ReadFlash: public CliCommand
{
private :
	Flash *_flsh;

public:
	ReadFlash(Flash * f)
	{
		_flsh = f;
	}
	void doCommand (char* param)
	{
		prmFromFlash = (Parameters*)flsh.GetFromFlash();
		uint32_t TmpWarning = prmFromFlash->warningTh;
		uint32_t TmpCritical = prmFromFlash->criticalTh;
		uint32_t TmpHysteresis = prmFromFlash->hysteresis;
		printf("Warning Threshold: %ld, Critical Threshold: %ld, Hysteresis: %ld \r\n",TmpWarning,TmpCritical,TmpHysteresis);
	}

};
class SetCriticalThreshold: public CliCommand
{
private :
	Flash *_flsh;

public:
	SetCriticalThreshold(Flash * f)
	{
		_flsh = f;
	}
	void doCommand (char* param)
	{
		int IsOK;
		int tmp;
		tmp = (int)atoi(param);
		if(tmp <= 0)
		{
			printf("Threshold should be greater than 0\r\n");
			return;
		}
		if(tmp <= (int)prm1.warningTh+(int)prm1.hysteresis)
		{
			printf("Critical threshold should be greater than (warning threshold + hysteresis) %ld\r\n",prm1.warningTh+prm1.hysteresis);
			return;
		}
		prm1.criticalTh = (uint32_t)atoi(param);
		IsOK = flsh.SetToFlash(&prm1);
		if(IsOK == HAL_OK)
		{
			printf("Critical Threshold Was Set Successfully\r\n");
		}

	}

};

class SetWarningThreshold: public CliCommand
{
private :
	Flash *_flsh;

public:
	SetWarningThreshold(Flash * f)
	{
		_flsh = f;
	}
	void doCommand (char* param)
	{
		int IsOK;
		int tmp;
		tmp = (int)atoi(param);
		if(tmp <= 0)
		{
			printf("Threshold should be greater than 0\r\n");
			return;
		}
		if(tmp >= (int)prm1.criticalTh-(int)prm1.hysteresis)
		{
			printf("Warning threshold should be smaller than (critical threshold - hysteresis) %ld\r\n",prm1.criticalTh-prm1.hysteresis);
			return;
		}
		prm1.warningTh = (uint32_t)atoi(param);
		IsOK= flsh.SetToFlash(&prm1);
		if(IsOK == HAL_OK)
		{
			printf("Warning Threshold Was Set Successfully\r\n");
		}

	}

};
class SetBackwards: public CliCommand
{
private :
	Flash *_flsh;

public:
	SetBackwards(Flash * f)
	{
		_flsh = f;
	}
	void doCommand (char* param)
	{
		int IsOK;
		int tmp;
		tmp = (int)atoi(param);
		if(tmp <= 0)
		{
			printf("Hysteresis should be greater than 0\r\n");
			return;
		}
		prm1.hysteresis = (uint32_t)atoi(param);
		IsOK= flsh.SetToFlash(&prm1);
		if(IsOK == HAL_OK)
		{
			printf("Hysteresis Was Set Successfully\r\n");
		}

	}

};
class BuzzerOn: public CliCommand
{
	private :

	Buzzer* _bzr;
	public:
	BuzzerOn(Buzzer* b)
	{
		_bzr = b;
	}
	void doCommand (char* param)
	{
		_bzr->SetOn();
	}

};
class BuzzerOff: public CliCommand
{
	private :
	Buzzer* _bzr;

	public:
	BuzzerOff(Buzzer* b)
	{
		_bzr = b;
	}
	void doCommand (char* param)
	{

		_bzr->SetOff();
	}

};
class LedOn: public CliCommand
{
	private :
	Led* _led;

	public:
	LedOn(Led* l)
	{
		_led = l;
	}
	void doCommand (char* param)
	{

		_led->SetON();
	}

};
class LedBlink: public CliCommand
{
	private :
	Led* _led;

	public:
	LedBlink(Led* l)
	{
		_led = l;
	}
	void doCommand (char* param)
	{

		_led->SetBlink();
	}

};
class LedOff: public CliCommand
{
	private :
	Led* _led;

	public:
	LedOff(Led* l)
	{
		_led = l;
	}
	void doCommand (char* param)
	{

		_led->SetOff();
	}

};

class dhtprint: public CliCommand
{
	private :


	public:
	dhtprint()
	{

	}
	void doCommand (char* param)
	{
		printDht = true;
	}

};
class dhtnoprint: public CliCommand
{
	private :


	public:
	dhtnoprint()
	{

	}
	void doCommand (char* param)
	{
		printDht = false;
	}

};
class WriteToFile: public CliCommand
{
	private :
	File* _file;

	public:
	WriteToFile(File* f)
	{
		_file = f;
	}
	void doCommand (char* param)
	{
		_file->Write();
	}

};
class PrintFromFile: public CliCommand
{
	private :
	File* _file;

	public:
	PrintFromFile(File* f)
	{
		_file = f;
	}
	void doCommand (char* param)
	{
		_file->Print();
	}

};
class ClearFile: public CliCommand
{
	private :
	File* _file;

	public:
	ClearFile(File* f)
	{
		_file = f;
	}
	void doCommand (char* param)
	{
		_file->Clear();
	}

};
void CliInit()
{
	SetCmdArr();
	RegisterCommand("showtime", new ShowTime(&rtc));
	RegisterCommand("settime", new SetDateandTime(&rtc));

	RegisterCommand("setcritic", new SetCriticalThreshold(&flsh));
	RegisterCommand("setwarn", new SetWarningThreshold(&flsh));
	RegisterCommand("sethys", new SetBackwards(&flsh));
	RegisterCommand("readth", new ReadFlash(&flsh));

	RegisterCommand("buzzon", new BuzzerOn(&bzr));
	RegisterCommand("buzzoff", new BuzzerOff(&bzr));
	RegisterCommand("ledon", new LedOn(&rLed));
	RegisterCommand("ledoff", new LedOff(&rLed));
	RegisterCommand("ledblink", new LedBlink(&rLed));


	RegisterCommand("dhtloopon", new dhtprint()); //print temperature and humidity every second
	RegisterCommand("dhtloopoff", new dhtnoprint());

	RegisterCommand("writelog", new WriteToFile(&logFile));
	RegisterCommand("printlog", new PrintFromFile(&logFile));
	RegisterCommand("clearlog", new ClearFile(&logFile));
	RegisterCommand("writeevent", new WriteToFile(&eventFile));
	RegisterCommand("printevent", new PrintFromFile(&eventFile));
	RegisterCommand("clearevent", new ClearFile(&eventFile));




}
