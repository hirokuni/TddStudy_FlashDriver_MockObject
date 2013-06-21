/*
 * m28w160ect.h
 *
 *  Created on: 2013/06/18
 *      Author: hirokuni
 */

#ifndef M28W160ECT_H_
#define M28W160ECT_H_

/*
 * ヘッダはプロダクトコードとテストコードの両方で使われるため、定義に間違いがあってもテストで
 * みつけられないおそれがあることに注意しよう。これらはコードインスペクションによって見つかったり、
 * 最終的にハードウェアとのインテグレーションで見つかることになる。
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
