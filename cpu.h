#pragma once
#include "Common.h"
#include "Registers.h";
#include <memory>
class emu;
class CPU
{
public:
	CPU(emu& gbEmu);
	bool isHalted();
	void init();
	void fetchOpcode();
	//unsigned short fetchInstructionData(unsigned short address);
	void executeCurrentOpcode();

	void cycleCPU(int numCycles);
private:
	emu& gbEmu;
	unsigned long long cycles;
	unsigned int programCounter;
	u16 stackPointer;
	ByteRegister a, b, c, d, e,  h, l;
	FlagsRegister f;
	//std::unique_ptr<WordRegister[]> registers;
	WordRegister afReg;
	WordRegister bcReg;
	WordRegister deReg;
	WordRegister hlReg;
	u8 IR;
	u8 IE; 
	u8 currentOpcode;

	u16 readWordFromPC();
	u8 readByteFromPC();
	u8 readByteFromAddress(u16 address);
	void writeByteAtAddress(u16 address, u8 data);

	void opcodeNOP();
	void opcodeJP_A16();

	void opcodeLD_HLD_A();

	void INC_R8(ByteRegister& reg);
	void INC_R16(WordRegister& reg);
	void INC_R16_INDIRECT(WordRegister& reg);
	void DEC_R8(ByteRegister& reg);
	void DEC_R16(WordRegister& reg);
	void DEC_R16_INDIRECT(WordRegister& reg);

	void LD_R16_N16(WordRegister& reg);
	void LD_R8_R8(ByteRegister& reg1, ByteRegister& reg2);
	void LD_R8_N8(ByteRegister& reg);
	void LD_R16_R8(WordRegister& reg, ByteRegister& reg2);
	void LD_R16_INC_A(WordRegister& reg);
	void LD_R16_DEC_A(WordRegister& reg);
	void LD_R16_N8(WordRegister& reg);
	void LD_R8_R16(ByteRegister& reg1, WordRegister& reg2);
	void LD_R8_R16_INC(ByteRegister& reg1, WordRegister& reg2);
	void LD_R8_R16_DEC(ByteRegister& reg1, WordRegister& reg2);
	//void LD_R16_Addr_R8(WordRegister& reg1, ByteRegister& reg2);
	//void LD_R16_Addr_R8(WordRegister& reg1, ByteRegister& reg2);
	//void LD_R8_R16_Addr(WordRegister& reg1, ByteRegister& byte1);
	void ADD_R8(ByteRegister& reg);
	void ADD_N8();

	void SUB_R8(ByteRegister& reg);
	void SUB_N8();

	void AND_R8(ByteRegister& reg);
	void OR_R8(ByteRegister& reg);

	void XOR_R8(ByteRegister& reg);
};

