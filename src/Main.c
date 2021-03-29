#include <stdio.h>

typedef enum
{
	RegAx,
	RegBx,
	RegCx,
	RegDx,

	RegSp,
	RegBp,
	RegIp,

	RegCount,
} RegKind;

const char *const RegKindNames[RegCount] =
{
	"RegAx",
	"RegBx",
	"RegCx",
	"RegDx",

	"RegSp",
	"RegBp",
	"RegIp",
};

typedef enum
{
	IntRet,

	IntMov,
	IntMovVal,

	IntAdd,
	IntSub,
	IntMul,
	IntDiv,

	IntCount,
} IntKind;

char Reg[RegCount] = { 0 };

void MovInt(RegKind from, RegKind to)
{ Reg[to] = Reg[from]; }

void MovValInt(char val, RegKind to)
{ Reg[to] = val; }

void AddInt(RegKind from, RegKind to)
{ Reg[to] += Reg[from]; }

void PrintRegs()
{
	for (RegKind reg = 0; reg < RegCount; reg++)
		printf("%s: %hhi\n", RegKindNames[reg], Reg[reg]);
	putchar('\n');
}

void ExecuteProgram(const char *prog)
{
	IntKind $int;
	char args[4];

	while (*prog != IntRet)
	{
		switch (*prog++)
		{
		case IntMovVal:
			args[0] = *prog++;
			args[1] = *prog++;
			MovValInt(args[0], args[1]);
			break;

		case IntAdd:
			args[0] = *prog++;
			args[1] = *prog++;
			AddInt(args[0], args[1]);
			break;
		}
	}
}

char myProg[] =
{
	IntMovVal, 60, RegAx,
	IntMovVal, 9, RegBx,
	IntAdd, RegBx, RegAx,

	IntRet,
};

int main()
{
	PrintRegs();

	ExecuteProgram(myProg);

	PrintRegs();

	(void)getchar();
}