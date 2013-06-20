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
    IO_Write((ioAddress)0,(ioData)0x40);
    IO_Write((ioAddress)4096,(ioData)48879);

    IO_Read((ioAddress)0);
    IO_Read((ioAddress)0x1000);
	return (int)FLASH_SUCCESS;
}

void Flash_Create(){

}

void Flash_Destroy(){

}
