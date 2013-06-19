/*
 * IO.h
 *
 *  Created on: 2013/06/12
 *      Author: hirokuni
 */

#ifndef IO_H_
#define IO_H_

#include <stdint.h>
#define ioAddress uint32_t
#define ioData uint16_t

void IO_Write(ioAddress addr, ioData data);
ioData IO_Read(ioAddress addr);
#endif /* IO_H_ */
