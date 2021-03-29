#include <stdio.h>

#include "Program.h"

char myProg[PROGRAM_SIZE] =
{
	IntDebugRegs,
	IntMovVal, 6, RegAx,
	IntMovVal, 9, RegBx,
	IntMul, RegBx, RegAx,

	IntMovVal, 0, RegIp,

	IntRet,
};

int main()
{
	ExecuteProgram(myProg);
	system("pause");
}