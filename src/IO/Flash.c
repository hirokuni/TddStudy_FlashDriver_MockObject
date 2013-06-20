/*
 * Flash.c
 *
 *  Created on: 2013/06/17
 *      Author: hirokuni
 */

#include "IO.h"
#include "m28w160ect.h"
#include "Flash.h"


int Flash_Write(ioAddress addr, ioData data){
	ioData status = 0;
    IO_Write(CommandRegister,ProgramCommand);
    IO_Write(addr,data);
    while((status & ReadyBit) == 0)
    	status = IO_Read(StatusRegister);
    IO_Read(addr);
	return (int)FLASH_SUCCESS;
}

void Flash_Create(){

}

void Flash_Destroy(){

}
