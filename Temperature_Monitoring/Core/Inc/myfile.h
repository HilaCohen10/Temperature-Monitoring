/*
 * myfile.h
 *
 *  Created on: Dec 1, 2022
 *      Author: Hila
 */

#ifndef INC_MYFILE_H_
#define INC_MYFILE_H_




#include <stdio.h>
#include <string.h>
#include "main.h"
#include "fatfs.h"


#define LOG_MAX_SIZE 65

class File
{
private:
	FIL _fil; 		//File handle
	FRESULT _fres; //Result after operations
	char _fileName[10] = {0};
public:
	File(const char* newFileName);
	void Write();
	void Print();
	void Clear();
	void GetNewLine();
};


//void LogTask(void *argument);



#endif /* INC_MYFILE_H_ */
