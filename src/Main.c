#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include "Program.h"

Value myProg[PROGRAM_SIZE] =
{
	IntKernel, KernelPrint, 0,

	IntMovVal, 69, RegAx,
	IntPush, RegAx,
	IntDbgMemRange, 250, 255,

	IntMov, RegSp, RegAx,
	IntKernel, KernelExit,
};

Value myProgROM[PROGRAM_SIZE] = "Hello, world!";

int main()
{
	//FILE *fs = fopen("src/Prog.vajm", "rb");
	//assert(fs);
	//fread(myProg, sizeof(Value), PROGRAM_SIZE, fs);
	//fclose(fs);

	VajmProgram *prog = CreateProgram(myProg, myProgROM);
	int ec = ExecuteProgram(prog);
	printf("Program exited with code %i (0x%x)\n", ec, ec);
	DestroyProgram(prog);

	system("pause");
}