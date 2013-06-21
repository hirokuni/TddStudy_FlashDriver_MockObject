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
					.withParameter("addr",(int)addr)//�Ox00
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
	 * Setup : �ŏ��Ɋ��҂��`����B
	 *
	 * �}10-4�@�V�[�P���X�`���[�g�ɂ���V�i���I�̊���
	 * ���ǂ̂悤�ɃZ�b�g�A�b�v����Ă��邩������B
	 */
	//���҂P�F�v���O�����R�}���h�I�t�Z�b�g�A�h���X0x0��0x40���������ނ͂�
	MockIO_Expect_Write(CommandRegister,ProgramCommand);

	//���҂Q�F�w��I�t�Z�b�g�A�h���X�Ƀf�[�^�������͂�
	MockIO_Expect_Write(address,data);

	//���҂R�F�X�e�[�^�X���W�X�^��ǂ݁AReadyBit(1<<7)�̒l���ǂ߂�͂�
	MockIO_Expect_ReadThenReturn(StatusRegister,ReadyBit);

	//���҂S�F���҂Q�ŏ������A�h���X�̃f�[�^��ǂ񂾂�0x1000���ԋp�����͂�
	MockIO_Expect_ReadThenReturn(address,data);

	/*
	 * Execute : Flash�ւ̏������݂��s���B
	 *
	 * ���ׂẮu���ҁv���Z�b�g�ł�����A�e�X�g�̓v���_�N�g�R�[�h��Flash_Write()
	 * �֐����Ăяo���BFlash_Write()��IO_Read()��IO_Write()��
	 * MockIO�o�[�W�����Ƀ����N����Ă��邽�߁A�e�Ăяo�������҂��Ă���Ăяo���ɂ҂�����p�b�`���邩�A���b�N�ɂ����
	 * �`�F�b�N�����B�����Ăяo������������Ă�����A�p�����[�^���Ԉ���Ă����肷��ƁA�e�X�g�͑����Ɏ��s����B
	 */
	result = Flash_Write(address, data);

	/*
	 * �Ō�̌��؃t�F�[�Y�Ƃ��āAFlash_Write()��0��Ԃ����Ƃ��`�F�b�N����B����̓t���b�V��
	 * �������̓���̈�ɑ΂���v���O���~���O�������������Ƃ������Ă���B�e�X�g�P�[�X�̍Ō�ɌĂ΂��teardown�ł́A
	 * �g���Ă��Ȃ��u���ҁv���Ȃ����m�F����B��������΁A�e�X�g�͎��s����B
	 */
	LONGS_EQUAL(0,result);

}
