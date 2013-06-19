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
}

TEST_GROUP(Flash) {
	void setup() {
		Flash_Create();
	}

	void teardown() {
		Flash_Destroy();
		mock("IO").checkExpectations();
		mock().clear();
	}
};

TEST(Flash, WriteSucceeds_ReadyImmediately) {
	int result = -1;

	/*
	 * Setup : �ŏ��Ɋ��҂��`����B
	 *
	 * �}10-4�@�V�[�P���X�`���[�g�ɂ���V�i���I�̊���
	 * ���ǂ̂悤�ɃZ�b�g�A�b�v����Ă��邩������B
	 */
	//���҂P�F�v���O�����R�}���h�I�t�Z�b�g�A�h���X0x0��0x40���������ނ͂�
	mock("IO").expectOneCall("IO_Write")
			.withParameter("addr",(int)0)//�Ox00
			.withParameter("value",(int)0x40);//0x40

	//���҂Q�F�w��I�t�Z�b�g�A�h���X�Ƀf�[�^�������͂�
	mock("IO").expectOneCall("IO_Write")
			.withParameter("addr",0x1000)
			.withParameter("value",0xBEEF);

	//���҂R�F�X�e�[�^�X���W�X�^��ǂ݁AReadyBit(1<<7)�̒l���ǂ߂�͂�
	mock("IO").expectOneCall("IO_Read")
				.withParameter("addr",StatusRegister)
				.andReturnValue(ReadyBit);

	//���҂S�F���҂Q�ŏ������A�h���X�̃f�[�^��ǂ񂾂�0x1000���ԋp�����͂�
	mock("IO").expectOneCall("IO_Read")
			.withParameter("addr",(int)0xBEEF)
			.andReturnValue((int)0x1000);

	/*
	 * Execute : Flash�ւ̏������݂��s���B
	 *
	 * ���ׂẮu���ҁv���Z�b�g�ł�����A�e�X�g�̓v���_�N�g�R�[�h��Flash_Write()
	 * �֐����Ăяo���BFlash_Write()��IO_Read()��IO_Write()��
	 * MockIO�o�[�W�����Ƀ����N����Ă��邽�߁A�e�Ăяo�������҂��Ă���Ăяo���ɂ҂�����p�b�`���邩�A���b�N�ɂ����
	 * �`�F�b�N�����B�����Ăяo������������Ă�����A�p�����[�^���Ԉ���Ă����肷��ƁA�e�X�g�͑����Ɏ��s����B
	 */
	result = Flash_Write((ioAddress)0x1000, (ioData)0xBEEF);

	/*
	 * �Ō�̌��؃t�F�[�Y�Ƃ��āAFlash_Write()��0��Ԃ����Ƃ��`�F�b�N����B����̓t���b�V��
	 * �������̓���̈�ɑ΂���v���O���~���O�������������Ƃ������Ă���B�e�X�g�P�[�X�̍Ō�ɌĂ΂��teardown�ł́A
	 * �g���Ă��Ȃ��u���ҁv���Ȃ����m�F����B��������΁A�e�X�g�͎��s����B
	 */
	LONGS_EQUAL(0,result);

}
