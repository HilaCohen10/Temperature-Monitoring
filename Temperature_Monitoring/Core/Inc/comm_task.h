/*
 * comm_task.h
 *
 *  Created on: Nov 8, 2022
 *      Author: Hila
 */

#ifndef INC_COMM_TASK_H_
#define INC_COMM_TASK_H_

#include "cli_command.h"
#include "main.h"
#include <stdio.h>
#include <string.h>


struct Cmd
{
	const char* commandName;
	CliCommand* cliCommand;

};

int commTask();
void handleCommand();
void SetCmdArr();
void RegisterCommand(const char* commandName, CliCommand* clicmd);

#endif /* INC_COMM_TASK_H_ */
