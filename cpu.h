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
	std::unique_ptr<WordRegister[]> registers;
	//WordRegister afReg;
	//WordRegister bcReg;
	//WordRegister deReg;
	//WordRegister hlReg;
	u8 IR;
	u8 IE; 
	u8 currentOpcode;

	u16 readWordFromPC();
	u8 readByteFromPC();

	void opcodeNOP();
	void opcodeJP_A16();
	//void opcodeLD_BC_A();
	//void opcodeLD_DE_A();
	//void opcodeLD_HL_incr_A();
	//void opcodeLD_HL_decr_A();

	//void opcodeLD_BC_N16();
	//void opcodeLD_DE_N16();
	//void opcodeLD_SP_N16();
	
	//void opcodeLD_HLI_A();
	void opcodeLD_HLD_A();


	void LD_R16_N16(WordRegister& reg);
	void LD_R8_R8(ByteRegister& reg1,  ByteRegister& reg2);
	void LD_R8_N8(ByteRegister& reg);
	//void LD_R16_Addr_R8(WordRegister& reg1, ByteRegister& reg2);
	//void LD_R16_Addr_R8(WordRegister& reg1, ByteRegister& reg2);
	//void LD_R8_R16_Addr(WordRegister& reg1, ByteRegister& byte1);

	void XOR_R8(ByteRegister& reg);
};

