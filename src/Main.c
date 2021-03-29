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
	IntMov,
	IntMovVal,

	IntAdd,
	IntSub,
	IntMul,
	IntDiv,

	IntCount,
} IntKind;

int Reg[RegCount] = { 0 };

void MovInt(RegKind from, RegKind to)
{ Reg[to] = Reg[from]; }

void MovValInt(int val, RegKind to)
{ Reg[to] = val; }

void AddInt(RegKind from, RegKind to)
{ Reg[to] += Reg[from]; }

void PrintRegs()
{
	for (RegKind reg = 0; reg < RegCount; reg++)
		printf("%s: %i\n", RegKindNames[reg], Reg[reg]);
	putchar('\n');
}

int main()
{
	PrintRegs();

	MovValInt(60, RegAx);
	MovValInt( 9, RegBx);
	AddInt(RegBx, RegAx);

	PrintRegs();

	(void)getchar();
}