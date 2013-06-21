/*
 * m28w160ect.h
 *
 *  Created on: 2013/06/18
 *      Author: hirokuni
 */

#ifndef M28W160ECT_H_
#define M28W160ECT_H_

/*
 * �w�b�_�̓v���_�N�g�R�[�h�ƃe�X�g�R�[�h�̗����Ŏg���邽�߁A��`�ɊԈႢ�������Ă��e�X�g��
 * �݂����Ȃ������ꂪ���邱�Ƃɒ��ӂ��悤�B�����̓R�[�h�C���X�y�N�V�����ɂ���Č���������A
 * �ŏI�I�Ƀn�[�h�E�F�A�Ƃ̃C���e�O���[�V�����Ō����邱�ƂɂȂ�B
 */

enum
{
	CommandRegister = 0x0,
	StatusRegister = 0x0
};

enum
{
	ProgramCommand = 0x40,
	Reset = 0xff
};

enum
{
	ReadyBit = 1<<7,
	VpErrorBit = 1<<3,
	ProgramErrorBit = 1 << 4,
	BlockErrorBit = 1 << 0
};

#endif /* M28W160ECT_H_ */
