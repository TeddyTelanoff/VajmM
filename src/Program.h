#pragma once

#include <assert.h>
#include <memory.h>

#define PROGRAM_SIZE 0xFF + 1
#define MEM_SIZE 0xFF + 1

typedef unsigned char Value;

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
	IntKernel,

	IntDbgRegs,
	IntDbgMem,
	IntDbgMemRange,

	IntCount,
} IntKind;

typedef enum
{
	KernelExit,
	KernelPrint,
} KernelKind;

typedef struct
{
	_Bool Exiting;
	Value Arg[4];
	Value Reg[RegCount];
	Value Memory[MEM_SIZE];
	const Value *Prog;
} VajmProgram;

inline void ExitInt(VajmProgram *);
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
inline void KernelInt(VajmProgram *);

inline void DbgRegsInt(VajmProgram *);
inline void DbgMemInt(VajmProgram *);
inline void DbgMemRangeInt(VajmProgram *);


#define PROG_STEP() this->Prog[this->Reg[RegIp]++]
#define INSTRUCTION_0(name) case Int##name: name##Int(this); break
#define INSTRUCTION_1(name) case Int##name: this->Arg[0] = PROG_STEP(); name##Int(this); break
#define INSTRUCTION_2(name) case Int##name: this->Arg[0] = PROG_STEP(); this->Arg[1] = PROG_STEP(); name##Int(this); break
inline int ExecuteProgram(const Value prog[PROGRAM_SIZE])
{
	VajmProgram *this = &(VajmProgram) { .Prog = prog };

	IntKind $int;
	while (!this->Exiting)
	{
		switch ($int = PROG_STEP())
		{
			INSTRUCTION_0(Exit);
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
			INSTRUCTION_1(Kernel);

			INSTRUCTION_0(DbgRegs);
			INSTRUCTION_0(DbgMem);
			INSTRUCTION_2(DbgMemRange);
		}
	}

	return this->Reg[RegAx];
}
#undef INSTRUCTION_0
#undef INSTRUCTION_1
#undef INSTRUCTION_2

inline void ExitInt(VajmProgram *this)
{ this->Exiting = 1; }

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

inline void KernelInt(VajmProgram *this)
{
	switch (this->Arg[0])
	{
	case KernelExit:
		ExitInt(this);
		break;
	case KernelPrint:
	{
		const char *ptr = this->Memory;
		ptr += PROG_STEP();
		puts(ptr);
		break;
	}

	default:
		puts("INVALID KERNEL: 0x%x", this->Arg[0]);
		break;
	}
}


inline void DbgRegsInt(VajmProgram *this)
{
	for (RegKind reg = 0; reg < RegCount; reg++)
		printf("%s: %i\n", RegKindNames[reg], this->Reg[reg]);
	putchar('\n');
}

inline void DbgMemInt(VajmProgram *this)
{
	for (short i = 0; i < MEM_SIZE; i++)
		printf("[%i] %i\n", i, this->Memory[i]);
	putchar('\n');
}

inline void DbgMemRangeInt(VajmProgram *this)
{
	assert(this->Arg[0] + this->Arg[1] < MEM_SIZE);
	for (short i = 0; i < this->Arg[1]; i++)
		printf("[%i] %i\n", this->Arg[0] + i, this->Memory[this->Arg[0] + i]);
	putchar('\n');
}
#undef PROG_STEP