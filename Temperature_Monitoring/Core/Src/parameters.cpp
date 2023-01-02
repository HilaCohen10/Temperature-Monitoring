/*
 * parameters.cpp
 *
 *  Created on: Dec 19, 2022
 *      Author: Hila
 */

#include "flash.h"
#include "parameters.h"
Parameters* prmFromFlash;
Parameters prm1;
extern Flash flsh;

extern "C" void InitParameters()
{
	prmFromFlash = (Parameters*)flsh.GetFromFlash();
	if(prmFromFlash->magicNumber != 1234567890)
	{
		// Initialize default parameters
		prm1.warningTh = 26;
		prm1.criticalTh = 28;
		prm1.hysteresis =  1;
		prm1.magicNumber = 1234567890;

		// inserting to flash
		flsh.SetToFlash(&prm1);
	}
	else
	{
		// get data from flash
		prm1.warningTh = prmFromFlash->warningTh;
		prm1.criticalTh = prmFromFlash->criticalTh;
		prm1.hysteresis = prmFromFlash->hysteresis;
		prm1.magicNumber = prmFromFlash->magicNumber;

	}

}
