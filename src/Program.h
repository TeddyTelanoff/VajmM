#pragma once

#include <memory.h>

#define PROGRAM_SIZE 255
#define STACK_SIZE 64

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
	IntClearRegs,

	IntMov,
	IntMovVal,

	IntAdd,
	IntSub,
	IntMul,
	IntDiv,


	IntDebugRegs,

	IntCount,
} IntKind;

char Reg[RegCount];
char Stack[STACK_SIZE];

inline void ClearRegsInt()
{ memset(Reg, 0, RegCount); }


inline void MovInt(RegKind from, RegKind to)
{ Reg[to] = Reg[from]; }

inline void MovValInt(char val, RegKind to)
{ Reg[to] = val; }


inline void AddInt(RegKind from, RegKind to)
{ Reg[to] += Reg[from]; }

inline void SubInt(RegKind from, RegKind to)
{ Reg[to] -= Reg[from]; }

inline void MulInt(RegKind from, RegKind to)
{ Reg[to] *= Reg[from]; }

inline void DivInt(RegKind from, RegKind to)
{ Reg[to] /= Reg[from]; }

inline void DebugRegsInt()
{
	for (RegKind reg = 0; reg < RegCount; reg++)
		printf("%s: %hhi\n", RegKindNames[reg], Reg[reg]);
	putchar('\n');
}


#define PROG_STEP() prog[Reg[RegIp]++]
inline void ExecuteProgram(const char prog[PROGRAM_SIZE])
{
	memset(Reg, 0, RegCount);
	memset(Stack, 0, STACK_SIZE);

	IntKind $int;
	char args[4];

	while (prog[Reg[RegIp]] != IntRet)
	{
		switch ($int = PROG_STEP())
		{
		case IntMov:
			args[0] = PROG_STEP();
			args[1] = PROG_STEP();
			MovInt(args[0], args[1]);
			break;
		case IntMovVal:
			args[0] = PROG_STEP();
			args[1] = PROG_STEP();
			MovValInt(args[0], args[1]);
			break;

		case IntAdd:
			args[0] = PROG_STEP();
			args[1] = PROG_STEP();
			AddInt(args[0], args[1]);
			break;
		case IntSub:
			args[0] = PROG_STEP();
			args[1] = PROG_STEP();
			SubInt(args[0], args[1]);
			break;
		case IntMul:
			args[0] = PROG_STEP();
			args[1] = PROG_STEP();
			MulInt(args[0], args[1]);
			break;
		case IntDiv:
			args[0] = PROG_STEP();
			args[1] = PROG_STEP();
			DivInt(args[0], args[1]);
			break;

		case IntDebugRegs:
			DebugRegsInt();
			break;
		}
	}
}
#undef PROG_STEP