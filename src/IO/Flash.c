/*
 * Flash.c
 *
 *  Created on: 2013/06/17
 *      Author: hirokuni
 */

#include "IO.h"
#include "m28w160ect.h"
#include "Flash.h"

static int writeError(int status){
	IO_Write(CommandRegister, Reset);
		if (status & VpErrorBit)
			return (int) FLASH_VPP_ERROR;
		else if (status & ProgramErrorBit)
			return (int) PROGRAM_ERROR;
		else if (status & BlockErrorBit)
			return (int) BLOCK_ERROR;
		else
			return (int)UNKNOWN_ERROR;
}

int Flash_Write(ioAddress addr, ioData data) {
	ioData status = 0;
	IO_Write(CommandRegister, ProgramCommand);
	IO_Write(addr, data);
	while ((status & ReadyBit) == 0)
		status = IO_Read(StatusRegister);

	if (status != ReadyBit)
		return writeError(status);

	IO_Read(addr);
	return (int) FLASH_SUCCESS;
}

void Flash_Create() {

}

void Flash_Destroy() {

}
