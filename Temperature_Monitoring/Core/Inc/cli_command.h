/*
 * cli_command.h
 *
 *  Created on: 8 Nov 2022
 *      Author: student
 */

#ifndef INC_CLI_COMMAND_H_
#define INC_CLI_COMMAND_H_

class CliCommand
{
	public:
	virtual void doCommand (char* param) = 0;
	 virtual ~CliCommand(){};
};
void CliInit();

#endif /* INC_CLI_COMMAND_H_ */
