#pragma once

#include <assert.h>
#include <memory.h>

#define PROGRAM_SIZE 0xFF
#define MEM_SIZE 0xFF

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

	IntPrint,

	IntDbgRegs,
	IntDbgStack,
	IntDbgStackRange,

	IntCount,
} IntKind;

typedef struct
{
	Value Arg[4];
	Value Reg[RegCount];
	Value Memory[MEM_SIZE];
	const Value *Prog;
} VajmProgram;

inline void RetInt(VajmProgram *);
inline void ClearRegsInt(VajmProgram *);

inline void MovInt(VajmProgram *);
inline void MovValInt(VajmProgram *);

inline void LoadInt(VajmProgram *);
inline void StoreInt(VajmProgram *);
inline void PushInt(VajmProgram *);
inline void PopInt(VajmProgram *);

inline void AddInt(VajmProgram *);
inline void SubInt(VajmProgram *);
inline void MulInt(VajmProgram *);
inline void DivInt(VajmProgram *);

inline void JmpInt(VajmProgram *);
inline void CallInt(VajmProgram *);

inline void PrintInt(VajmProgram *);

inline void DbgRegsInt(VajmProgram *);
inline void DbgStackInt(VajmProgram *);
inline void DbgStackRangeInt(VajmProgram *);


#define PROG_STEP() prog[this->Reg[RegIp]++]
#define INSTRUCTION_0(name) case Int##name: name##Int(this); break
#define INSTRUCTION_1(name) case Int##name: this->Arg[0] = PROG_STEP(); name##Int(this); break
#define INSTRUCTION_2(name) case Int##name: this->Arg[0] = PROG_STEP(); this->Arg[1] = PROG_STEP(); name##Int(this); break
inline int ExecuteProgram(const Value prog[PROGRAM_SIZE])
{
	VajmProgram *this = &(VajmProgram) { .Prog = prog };

	IntKind $int;
	while (prog[this->Reg[RegIp]] != IntExit)
	{
		switch ($int = PROG_STEP())
		{
			INSTRUCTION_0(Ret);
			INSTRUCTION_0(ClearRegs);

			INSTRUCTION_2(Mov);
			INSTRUCTION_2(MovVal);

			INSTRUCTION_2(Load);
			INSTRUCTION_2(Store);

			INSTRUCTION_1(Push);
			INSTRUCTION_1(Pop);

			INSTRUCTION_2(Add);
			INSTRUCTION_2(Sub);
			INSTRUCTION_2(Mul);
			INSTRUCTION_2(Div);

			INSTRUCTION_1(Jmp);
			INSTRUCTION_1(Call);

			INSTRUCTION_1(Print);

			INSTRUCTION_0(DbgRegs);
			INSTRUCTION_0(DbgStack);
			INSTRUCTION_2(DbgStackRange);
		}
	}

	return this->Reg[RegAx];
}
#undef PROG_STEP
#undef INSTRUCTION_0
#undef INSTRUCTION_1
#undef INSTRUCTION_2

inline void RetInt(VajmProgram *this)
{
	this->Reg[RegSp]--;
	this->Reg[RegIp] = this->Memory[this->Reg[RegSp]];
}

inline void ClearRegsInt(VajmProgram *this)
{ memset(this->Reg, 0, RegXCount); }


inline void MovInt(VajmProgram *this)
{ this->Reg[this->Arg[1]] = this->Reg[this->Arg[0]]; }

inline void MovValInt(VajmProgram *this)
{ this->Reg[this->Arg[1]] = this->Arg[0]; }


inline void LoadInt(VajmProgram *this)
{ this->Reg[this->Arg[1]] = this->Memory[this->Arg[0]]; }

inline void StoreInt(VajmProgram *this)
{ this->Memory[this->Arg[1]] = this->Reg[this->Arg[0]]; }

inline void PushInt(VajmProgram *this)
{
	this->Memory[this->Reg[RegSp]] = this->Reg[this->Arg[0]];
	this->Reg[RegSp]++;
}

inline void PopInt(VajmProgram *this)
{
	this->Reg[RegSp]--;
	this->Reg[this->Arg[0]] = this->Memory[this->Reg[RegSp]];
}


inline void AddInt(VajmProgram *this)
{ this->Reg[this->Arg[1]] += this->Reg[this->Arg[0]]; }

inline void SubInt(VajmProgram *this)
{ this->Reg[this->Arg[1]] -= this->Reg[this->Arg[0]]; }

inline void MulInt(VajmProgram *this)
{ this->Reg[this->Arg[1]] *= this->Reg[this->Arg[0]]; }

inline void DivInt(VajmProgram *this)
{ this->Reg[this->Arg[1]] /= this->Reg[this->Arg[0]]; }


inline void JmpInt(VajmProgram *this)
{ this->Reg[RegIp] = this->Arg[0]; }

inline void CallInt(VajmProgram *this)
{
	this->Memory[this->Reg[RegSp]] = this->Reg[RegIp];
	this->Reg[RegSp]++;

	this->Reg[RegIp] = this->Arg[0];
}


inline void PrintInt(VajmProgram *this)
{
	const char *ptr = this->Memory;
	ptr += this->Arg[0];
	puts(ptr);
}


inline void DbgRegsInt(VajmProgram *this)
{
	for (RegKind reg = 0; reg < RegCount; reg++)
		printf("%s: %i\n", RegKindNames[reg], this->Reg[reg]);
	putchar('\n');
}

inline void DbgStackInt(VajmProgram *this)
{
	for (Value i = 0; i < MEM_SIZE; i++)
		printf("[%i] %i\n", i, this->Memory[i]);
	putchar('\n');
}

inline void DbgStackRangeInt(VajmProgram *this)
{
	assert(this->Arg[0] + this->Arg[1] < MEM_SIZE);
	for (Value i = 0; i < this->Arg[1]; i++)
		printf("[%i] %i\n", this->Arg[0] + i, this->Memory[this->Arg[0] + i]);
	putchar('\n');
}