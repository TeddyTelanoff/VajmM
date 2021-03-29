#include <stdio.h>
#include <stdlib.h>

#include "Program.h"

Value myProg[PROGRAM_SIZE] =
{
	IntDbgRegs,
	IntDbgStackRange, 0, 4,

	IntMovVal, 6, RegAx,
	IntMovVal, 9, RegBx,
	IntMul, RegBx, RegAx,

	IntPush, RegAx,
	IntDbgStackRange, 0, 4,
	IntClearRegs,
	IntDbgRegs,

	IntPop, RegAx,
	IntDbgRegs,
	IntRet,
};

int main()
{
	ExecuteProgram(myProg);
	system("pause");
}