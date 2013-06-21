/*
 * FlashTest.cpp
 *
 *  Created on: 2013/06/12
 *      Author: hirokuni
 */

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "m28w160ect.h"
#include "Flash.h"
#include "FakeMicroTime.h"
}

TEST_GROUP(Flash) {
	ioAddress address;
	ioData data;
	int result;

	void setup() {
		FakeMicroTime_Init(0,500);
		address = 0x1000;
		data = 0xBEEF;
		result = -1;

		Flash_Create();
	}

	void teardown() {
		Flash_Destroy();

		mock("IO").checkExpectations();
		mock().clear();
	}

	void MockIO_Expect_Write(ioAddress addr, ioData data){
		mock("IO").expectOneCall("IO_Write")
					.withParameter("addr",(int)addr)//０x00
					.withParameter("value",(int)data);//0x40
	}

	void MockIO_Expect_ReadThenReturn(ioAddress addr, ioData data){
		mock("IO").expectOneCall("IO_Read")
						.withParameter("addr",(int)addr)
						.andReturnValue((int)data);
	}
};


TEST(Flash, WriteFails_TimeoutAtEndOfTime){
	FakeMicroTime_Init(0xffffffff,500);
	Flash_Create();
	MockIO_Expect_Write(CommandRegister, ProgramCommand);
	MockIO_Expect_Write(address, data);
	for(int i = 0; i < 10; i++)
		MockIO_Expect_ReadThenReturn(StatusRegister, ~ReadyBit);
	result = Flash_Write(address, data);
	LONGS_EQUAL(FLASH_TIMEOUT_ERROR,result);
}

TEST(Flash, WriteFails_Timeout){
	FakeMicroTime_Init(0,500);
	Flash_Create();
	MockIO_Expect_Write(CommandRegister, ProgramCommand);
	MockIO_Expect_Write(address, data);
	for(int i = 0; i < 10; i++)
		MockIO_Expect_ReadThenReturn(StatusRegister, ~ReadyBit);
	result = Flash_Write(address, data);
	LONGS_EQUAL(FLASH_TIMEOUT_ERROR,result);
}

TEST(Flash, WriteFails_Ignore_Other_Bits_Until_Ready) {
	MockIO_Expect_Write(CommandRegister,ProgramCommand);
	MockIO_Expect_Write(address,data);
	MockIO_Expect_ReadThenReturn(StatusRegister, ~ReadyBit);
	MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit);
	MockIO_Expect_ReadThenReturn(address,data);

	result = Flash_Write(address,data);

	LONGS_EQUAL(FLASH_SUCCESS,result);
}

TEST(Flash, WriteFails_BlockError) {
	MockIO_Expect_Write(CommandRegister,ProgramCommand);
	MockIO_Expect_Write(address,data);
	MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit|BlockErrorBit);
	MockIO_Expect_Write(CommandRegister,Reset);

	result = Flash_Write(address,data);

	LONGS_EQUAL(BLOCK_ERROR,result);
}

TEST(Flash, WriteFails_ProgramError) {
	MockIO_Expect_Write(CommandRegister,ProgramCommand);
	MockIO_Expect_Write(address,data);
	MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit|ProgramErrorBit);
	MockIO_Expect_Write(CommandRegister,Reset);

	result = Flash_Write(address,data);

	LONGS_EQUAL(PROGRAM_ERROR,result);
}

TEST(Flash, WriteFails_VppError) {
	MockIO_Expect_Write(CommandRegister,ProgramCommand);
	MockIO_Expect_Write(address,data);
	MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit|VpErrorBit);
	MockIO_Expect_Write(CommandRegister,Reset);

	result = Flash_Write(address,data);

	LONGS_EQUAL(FLASH_VPP_ERROR,result);
}
TEST(Flash, SucceedsNotImmediatelyReady) {
	MockIO_Expect_Write(CommandRegister,ProgramCommand);
	MockIO_Expect_Write(address,data);
	MockIO_Expect_ReadThenReturn(StatusRegister,0);
	MockIO_Expect_ReadThenReturn(StatusRegister,0);
	MockIO_Expect_ReadThenReturn(StatusRegister,0);
	MockIO_Expect_ReadThenReturn(StatusRegister,ReadyBit);
	MockIO_Expect_ReadThenReturn(address,data);

	result = Flash_Write(address, data);
	LONGS_EQUAL(FLASH_SUCCESS,result);
}
TEST(Flash, WriteSucceeds_ReadyImmediately) {
	/*
	 * Setup : 最初に期待を定義する。
	 *
	 * 図10-4　シーケンスチャートにあるシナリオの期待
	 * がどのようにセットアップされているか分かる。
	 */
	//期待１：プログラムコマンドオフセットアドレス0x0に0x40を書き込むはず
	MockIO_Expect_Write(CommandRegister,ProgramCommand);

	//期待２：指定オフセットアドレスにデータを書くはず
	MockIO_Expect_Write(address,data);

	//期待３：ステータスレジスタを読み、ReadyBit(1<<7)の値が読めるはず
	MockIO_Expect_ReadThenReturn(StatusRegister,ReadyBit);

	//期待４：期待２で書いたアドレスのデータを読んだら0x1000が返却されるはず
	MockIO_Expect_ReadThenReturn(address,data);

	/*
	 * Execute : Flashへの書き込みを行う。
	 *
	 * すべての「期待」がセットできたら、テストはプロダクトコードのFlash_Write()
	 * 関数を呼び出す。Flash_Write()はIO_Read()とIO_Write()の
	 * MockIOバージョンにリンクされているため、各呼び出しが期待している呼び出しにぴったりパッチするか、モックによって
	 * チェックされる。もし呼び出し順序が違っていたり、パラメータが間違っていたりすると、テストは即座に失敗する。
	 */
	result = Flash_Write(address, data);

	/*
	 * 最後の検証フェーズとして、Flash_Write()が0を返すことをチェックする。これはフラッシュ
	 * メモリの特定領域に対するプログラミングが成功したことを示している。テストケースの最後に呼ばれるteardownでは、
	 * 使われていない「期待」がないか確認する。もしあれば、テストは失敗する。
	 */
	LONGS_EQUAL(0,result);

}
