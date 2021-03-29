#include <stdio.h>
#include <stdlib.h>

#include "Program.h"

Value myProg[PROGRAM_SIZE] =
{
	IntDbgRegs,
	IntDbgStackRange, 0, 4,

	IntCall, 8,
	
	IntDbgRegs,
	IntExit,

	IntMovVal, 6, RegAx,
	IntMovVal, 9, RegBx,
	IntMul, RegBx, RegAx,

	IntPush, RegAx,
	IntDbgStackRange, 0, 4,
	IntClearRegs,
	IntDbgRegs,

	IntPop, RegAx,
	IntRet,
};

int main()
{
	int ec = ExecuteProgram(myProg);
	printf("Program exited with code %i (0x%x)\n", ec, ec);
	system("pause");
}