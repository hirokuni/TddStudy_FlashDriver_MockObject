/*
 * Flash.h
 *
 *  Created on: 2013/06/18
 *      Author: hirokuni
 */

#ifndef FLASH_H_
#define FLASH_H_
#include "IO.h"

enum {
	FLASH_SUCCESS,
	FLASH_VPP_ERROR,
	PROGRAM_ERROR,
	BLOCK_ERROR,
	UNKNOWN_ERROR
};


void Flash_Create();
void Flash_Destroy();
int Flash_Write(ioAddress addr, ioData data);
int Flash_Read(ioAddress addr);

#endif /* FLASH_H_ */
