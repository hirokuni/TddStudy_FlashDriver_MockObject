/*
 * MockIO.c
 *
 *  Created on: 2013/06/17
 *      Author: hirokuni
 */

#include "CppUTestExt/MockSupport.h"
#include "MockIO.h"

typedef struct Expectation {
	int kind;ioAddress addr;ioData value;
} Expectation;

//static Expectation *expectations = 0;
//static int setExpectationCount;
//static int getExpectationCount;
//static int maxExpectationCount;
//static int failureAlreadyReported = 0;

void IO_Write(ioAddress addr, ioData data) {
	mock("IO")
			.actualCall("IO_Write").
			withParameter("addr", (int)addr).
			withParameter("value", (int)data);
}

ioData IO_Read(ioAddress addr) {
	/*
	 * 注意：returnValue().getIntValue()から返却される値は、
	 * Test caseのexpectationとしてSetされた値。
	 */
	return mock("IO")
			.actualCall("IO_Read").
			withParameter("addr", (int) addr).
			returnValue().getIntValue();
}

void MockIO_Create(int maxExpectations) {
//	expectations = calloc(maxExpectations, sizeof(Expectation));
//	setExpectationCount = 0;
//	getExpectationCount = 0;
//	maxExpectationCount = maxExpectations;
//	failureAlreadyReported = 0;
}

void MockIO_Destroy(void) {
//	if (expectations)
//		free(expectations);
//	expectations = 0;
}

void MockIO_Expect_Write(ioAddress addr, ioData value) {
//	failWhenNoRoomForExpectations(report_too_many_write_expectations);
//	recordExpectation(FLASH_WRITE, addr, value);
}

void MockIO_Expect_ReadThenReturn(ioAddress addr, ioData value) {
//	failWhenNoRoomForExpectations(report_too_many_read_expectations);
//	recordExpectation(FLASH_READ, addr, value);
}
