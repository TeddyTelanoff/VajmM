#include <stdio.h>
#include <stdlib.h>

#include "Program.h"

Value myProg[PROGRAM_SIZE] =
{
	IntPrint, 3,
	IntExit,

	'H', 'E', 'L', 'L', 'O', 0,
};

int main()
{
	int ec = ExecuteProgram(myProg);
	printf("Program exited with code %i (0x%x)\n", ec, ec);
	system("pause");
}