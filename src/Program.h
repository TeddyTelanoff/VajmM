#pragma once

#include <assert.h>
#include <memory.h>

#define PROGRAM_SIZE 255
#define STACK_SIZE 64

typedef char Value;

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
	RegXCount = RegDx + 1,
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
	IntExit,
	IntRet,
	IntClearRegs,

	IntMov,
	IntMovVal,

	IntLoad,
	IntStore,
	IntPush,
	IntPop,

	IntAdd,
	IntSub,
	IntMul,
	IntDiv,

	IntJmp,
	IntCall,

	IntDbgRegs,
	IntDbgStack,
	IntDbgStackRange,

	IntCount,
} IntKind;

Value Arg[4];
Value Reg[RegCount];
Value Stack[STACK_SIZE];

inline void RetInt();
inline void ClearRegsInt();

inline void MovInt();
inline void MovValInt();

inline void LoadInt();
inline void StoreInt();
inline void PushInt();
inline void PopInt();

inline void AddInt();
inline void SubInt();
inline void MulInt();
inline void DivInt();

inline void JmpInt();
inline void CallInt();

inline void DbgRegsInt();
inline void DbgStackInt();
inline void DbgStackRangeInt();

inline void RetInt()
{
	Reg[RegSp]--;
	Reg[RegIp] = Stack[Reg[RegSp]];
}

inline void ClearRegsInt()
{ memset(Reg, 0, RegXCount); }


inline void MovInt()
{ Reg[Arg[1]] = Reg[Arg[0]]; }

inline void MovValInt()
{ Reg[Arg[1]] = Arg[0]; }


inline void LoadInt()
{ Reg[Arg[1]] = Stack[Arg[0]]; }

inline void StoreInt()
{ Stack[Arg[1]] = Reg[Arg[0]]; }

inline void PushInt()
{
	Stack[Reg[RegSp]] = Reg[Arg[0]];
	Reg[RegSp]++;
}

inline void PopInt()
{
	Reg[RegSp]--;
	Reg[Arg[0]] = Stack[Reg[RegSp]];
}


inline void AddInt()
{ Reg[Arg[1]] += Reg[Arg[0]]; }

inline void SubInt()
{ Reg[Arg[1]] -= Reg[Arg[0]]; }

inline void MulInt()
{ Reg[Arg[1]] *= Reg[Arg[0]]; }

inline void DivInt()
{ Reg[Arg[1]] /= Reg[Arg[0]]; }


inline void JmpInt()
{ Reg[RegIp] = Arg[0]; }

inline void CallInt()
{
	Stack[Reg[RegSp]] = Reg[RegIp];
	Reg[RegSp]++;

	Reg[RegIp] = Arg[0];
}


inline void DbgRegsInt()
{
	for (RegKind reg = 0; reg < RegCount; reg++)
		printf("%s: %i\n", RegKindNames[reg], Reg[reg]);
	putchar('\n');
}

inline void DbgStackInt()
{
	for (Value i = 0; i < STACK_SIZE; i++)
		printf("[%i] %i\n", i, Stack[i]);
	putchar('\n');
}

inline void DbgStackRangeInt()
{
	assert(Arg[0] + Arg[1] < STACK_SIZE);
	for (Value i = 0; i < Arg[1]; i++)
		printf("[%i] %i\n", Arg[0] + i, Stack[Arg[0] + i]);
	putchar('\n');
}


#define PROG_STEP() prog[Reg[RegIp]++]
inline void ExecuteProgram(const char prog[PROGRAM_SIZE])
{
	memset(Reg, 0, RegCount);
	memset(Stack, 0, STACK_SIZE);

	IntKind $int;
	while (prog[Reg[RegIp]] != IntExit)
	{
		switch ($int = PROG_STEP())
		{
		case IntRet:
			RetInt();
			break;
		case IntClearRegs:
			ClearRegsInt();
			break;

		case IntMov:
			Arg[0] = PROG_STEP();
			Arg[1] = PROG_STEP();
			MovInt();
			break;
		case IntMovVal:
			Arg[0] = PROG_STEP();
			Arg[1] = PROG_STEP();
			MovValInt();
			break;

		case IntLoad:
			Arg[0] = PROG_STEP();
			Arg[1] = PROG_STEP();
			LoadInt();
			break;
		case IntStore:
			Arg[0] = PROG_STEP();
			Arg[1] = PROG_STEP();
			StoreInt();
			break;
		case IntPush:
			Arg[0] = PROG_STEP();
			PushInt();
			break;
		case IntPop:
			Arg[0] = PROG_STEP();
			PopInt();
			break;

		case IntAdd:
			Arg[0] = PROG_STEP();
			Arg[1] = PROG_STEP();
			AddInt();
			break;
		case IntSub:
			Arg[0] = PROG_STEP();
			Arg[1] = PROG_STEP();
			SubInt();
			break;
		case IntMul:
			Arg[0] = PROG_STEP();
			Arg[1] = PROG_STEP();
			MulInt();
			break;
		case IntDiv:
			Arg[0] = PROG_STEP();
			Arg[1] = PROG_STEP();
			DivInt();
			break;

		case IntJmp:
			Arg[0] = PROG_STEP();
			JmpInt();
			break;
		case IntCall:
			Arg[0] = PROG_STEP();
			CallInt();
			break;

		case IntDbgRegs:
			DbgRegsInt();
			break;
		case IntDbgStack:
			DbgStackInt();
			break;
		case IntDbgStackRange:
			Arg[0] = PROG_STEP();
			Arg[1] = PROG_STEP();
			DbgStackRangeInt();
			break;
		}
	}
}
#undef PROG_STEP