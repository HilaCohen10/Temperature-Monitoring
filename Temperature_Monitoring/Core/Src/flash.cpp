/*
 * flash.cpp
 *
 *  Created on: 22 Nov 2022
 *      Author: student
 */

#include "flash.h"
#include "parameters.h"

Flash::Flash()
{
	_flash.TypeErase = FLASH_TYPEERASE_PAGES;
	_flash.Page = (FLASH_MEM - FLASH_START_ADDRESS)/PAGE_SIZE;
	_flash.Banks = _flash.Page < PAGES_IN_BANK ?1:2;
	_flash.NbPages = 1;
}
int Flash::SetToFlash(Parameters* prmToFlash)
{
	int ret = HAL_OK;
	HAL_StatusTypeDef isError;
	uint64_t DataToFlash;

	// Unlocking
	isError = HAL_FLASH_Unlock();
	if (isError == HAL_ERROR)
	{
		printf("flash error %d",__LINE__);
		return HAL_ERROR;
	}

	uint32_t PageError;
	if (HAL_FLASHEx_Erase(&_flash, &PageError) != HAL_OK)
	{
			printf("Erase failed %d \r\n", __LINE__);
			return HAL_ERROR;
	}

	// Writing
	unsigned int index = 0;
	while (index < sizeof(Parameters))
	{
		DataToFlash = *(uint64_t*)((uint8_t*)prmToFlash + index);
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_MEM + index, DataToFlash) != HAL_OK)
		{
			printf("Program failed %d\r\n",__LINE__);
			return HAL_ERROR;
		}
		index +=sizeof(uint64_t);//8
	}
	 // Locking
	HAL_FLASH_Lock();
	isError = HAL_FLASH_Unlock();
	if (isError != HAL_OK)
	{
		printf("flash error %d",__LINE__);
		return HAL_ERROR;
	}
	return ret;
}
uint32_t* Flash::GetFromFlash()
{
	_dataFromFlash = (uint32_t*)FLASH_MEM;
	return _dataFromFlash;
}

