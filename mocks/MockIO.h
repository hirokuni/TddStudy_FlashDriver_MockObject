/*
 * MockIO.h
 *
 *  Created on: 2013/06/17
 *      Author: hirokuni
 */

#ifndef MOCKIO_H_
#define MOCKIO_H_

extern "C"{
#include "IO.h"
}

void MockIO_Create(int maxExpectations);
void MockIO_Destroy(void);
void MockIO_Expect_Write(ioAddress offset, ioData data);
void MockIO_Expect_ReadThenReturn(ioAddress offset, ioData returnData);
void MockIO_Verify_Complete(void);

#endif /* MOCKIO_H_ */
