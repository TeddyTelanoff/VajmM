#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include "Program.h"

Value myProg[PROGRAM_SIZE] =
{
	IntMovVal, 'H', RegAx,
	IntStore, RegAx, 0,

	IntMovVal, 'e', RegAx,
	IntStore, RegAx, 1,

	IntMovVal, 'l', RegAx,
	IntStore, RegAx, 2,

	IntMovVal, 'l', RegAx,
	IntStore, RegAx, 3,

	IntMovVal, 'o', RegAx,
	IntStore, RegAx, 4,

	IntMovVal, 0, RegAx,
	IntStore, RegAx, 5,

	IntKernel, KernelPrint, 0,

	IntMovVal, 69, RegAx,
	IntKernel, KernelExit,
};

int main()
{
	//FILE *fs = fopen("src/Prog.vajm", "rb");
	//assert(fs);
	//fread(myProg, sizeof(Value), PROGRAM_SIZE, fs);
	//fclose(fs);

	int ec = ExecuteProgram(myProg);
	printf("Program exited with code %i (0x%x)\n", ec, ec);
	system("pause");
}