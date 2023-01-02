/*
 * flash.h
 *
 *  Created on: 22 Nov 2022
 *      Author: student
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include "main.h"
#include <stdint.h>
#include <cstdio>
#include "parameters.h"
#define FLASH_MEM (0x08081000UL) // Bank 2 Page 258
#define FLASH_START_ADDRESS (0x08000000UL)
#define PAGE_SIZE (2048)
#define PAGES_IN_BANK (256)

class Flash
{
	private:
	FLASH_EraseInitTypeDef _flash;
	uint32_t *_dataFromFlash;


	public:
	Flash();
	int SetToFlash(Parameters* prmToFlash);
	uint32_t* GetFromFlash();
};


#endif /* INC_FLASH_H_ */
