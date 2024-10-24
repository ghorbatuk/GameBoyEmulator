#pragma once
#include "Common.h"
#include "Registers.h";
#include <memory>



namespace interrupts {
	const u16 VBLANK = 0x40;
	const u16 LCDC_STAT = 0x48;
	const u16 TIMER = 0x50;
	const u16 SERIAL = 0x58;
	const u16 GAMEPAD = 0x60;
}

class emu;
class CPU
{
public:
	ByteRegister interruptFlag;
	ByteRegister interruptEnable;

	CPU(emu& gbEmu);
	bool isHalted();
	void init();
	void tick();
	
private:
	enum RSTCodes {
		RST1 = 0x00,
		RST2 = 0x08,
		RST3 = 0x10,
		RST4 = 0x18,
		RST5 = 0x20,
		RST6 = 0x28,
		RST7 = 0x30,
		RST8 = 0x38
	};

	emu& gbEmu;
	unsigned long long cycles;
	WordRegister PC;
	WordRegister SP;
	ByteRegister a, b, c, d, e, h, l;
	FlagsRegister f;
	ByteRegisterPair afReg;
	ByteRegisterPair bcReg;
	ByteRegisterPair deReg;
	ByteRegisterPair hlReg;
	u8 IR;
	u8 IE;
	bool IME;
	u8 currentOpcode;

	void stackPush8(u8 value);
	void stackPush16(u16 value);
	void fetchOpcode();
	void executeCurrentOpcode();

	void cycleCPU(int numCycles);

	u16 readWordFromPC();
	u16 readWordFromSP();
	u8 readByteFromPC();
	u8 readByteFromSP();
	u8 readByteFromAddress(u16 address);
	void writeByteAtAddress(u16 address, u8 data);
	void checkInterrupts();
	void handleInterrupt(u8 interruptBit, u8 interruptAddress );

	void opcodeNOP();
	void opcodeJP_A16();

	void opcodeLD_HLD_A();

	u8 INC_8BIT(u8 value);
	void INC_R8(ByteRegister& reg);
	void INC_R16(WordRegister& reg);
	void INC_R16_INDIRECT(ByteRegisterPair& reg);
	u8 DEC_8BIT(u8 value);
	void DEC_R8(ByteRegister& reg);
	void DEC_R16(WordRegister& reg);
	void DEC_R16_INDIRECT(ByteRegisterPair& reg);

	void DAA();
	void SCF();
	void CPL();
	void CCF();

	void LD_R16_N16(WordRegister& reg);
	void LD_R8_R8(ByteRegister& reg1, ByteRegister& reg2);
	void LD_R8_N8(ByteRegister& reg);
	void LD_A16_R8(ByteRegisterPair& reg, ByteRegister& reg2);
	void LD_R16_INC_A(ByteRegisterPair& reg);
	void LD_R16_DEC_A(ByteRegisterPair& reg);
	void LD_R16_N8(ByteRegisterPair& reg);
	void LD_R8_R16(ByteRegister& reg1, ByteRegisterPair& reg2);
	void LD_R8_R16_INC(ByteRegister& reg1, ByteRegisterPair& reg2);
	void LD_R8_R16_DEC(ByteRegister& reg1, ByteRegisterPair& reg2);
	void LD_A_A16();
	void LD_A16_A();
	void LD_A16_R16(WordRegister&);

	void LDH_A8_A();
	void LDH_A_A8();
	void LDH_RC_A();
	void LDH_A_RC();

	void loadSPFromHL();
	void loadHLFromAdjustedSP();


	//void LD_R16_Addr_R8(WordRegister& reg1, ByteRegister& reg2);
	//void LD_R16_Addr_R8(WordRegister& reg1, ByteRegister& reg2);
	//void LD_R8_R16_Addr(WordRegister& reg1, ByteRegister& byte1);
	u8 ADD_8BIT(u8 oper1, u8 oper2);
	void ADD_R8(ByteRegister& reg);
	void ADD_N8();
	void ADD_HL();

	u16 ADD_16BIT(u16 oper1, u16 oper2);
	void ADD_HL_R16(WordRegister& reg);
	void ADD_SP_E();

	void ADC_R8(ByteRegister& reg);
	void ADC_N8();
	void ADC_HL();

	u8 SUB_8BIT(u8 oper1, u8 oper2);
	void SUB_R8(ByteRegister& reg);
	void SUB_N8();
	void SUB_HL();

	void SBC_R8(ByteRegister& reg);
	void SBC_N8();
	void SBC_HL();

	void CP_R8(ByteRegister& reg);
	void CP_N8();
	void CP_HL();

	u8 AND_8BIT(u8 oper1, u8 oper2);
	void AND_R8(ByteRegister& reg);
	void AND_N8();
	void AND_HL();

	

	u8 OR_8BIT(u8 oper1, u8 oper2);
	void OR_R8(ByteRegister& reg);
	void OR_N8();
	void OR_HL();

	u8 XOR_8BIT(u8 oper1, u8 oper2);
	void XOR_R8(ByteRegister& reg);
	void XOR_N8();
	void XOR_HL();

	void JR_CC_E(bool condition);
	void JR_E();
	void JP_CC_NN(bool condition);
	void JP_HL();

	void RST(RSTCodes rstCode);

	void CALL_cond(bool condition);
	void CALL();

	void RET();
	void RETI();
	void RET_Cond(bool condition);

	void PUSH(ByteRegisterPair& reg);
	void POP(ByteRegisterPair& reg);

	void STOP();
	void HALT();
	void DI();
	void EI();

	void rotateAccumulatorLeft();
	void rotateAccumulatorLeftCircular();
	void rotateAccumulatorRight();
	void rotateAccumulatorRightCircular();

	void rotateRegisterLeft(ByteRegister& reg);
	void rotateRegisterLeftCircular(ByteRegister& reg);
	void rotateRegisterRight(ByteRegister& reg);
	void rotateRegisterRightCircular(ByteRegister& reg);

	void rotateLeftIndirect();
	void rotateLeftCircularIndirect();
	void rotateRightIndirect();
	void rotateRightCircularIndirect();

	void shiftRegisterLeftArithmetic(ByteRegister& reg);
	void shiftLeftArithmeticIndirect();
	void shiftRegisterRightArithmetic(ByteRegister& reg);
	void shiftRightArithmeticIndirect();

	void shiftRegisterRightLogical(ByteRegister& reg);
	void shiftRightLogicalIndirect();

	void swapRegisterNibbles(ByteRegister& reg);
	void swapNibblesIndirect();

	void setRegisterBit(u8 bitNumber, ByteRegister& reg);
	void resetRegisterBit(u8 bitNumber, ByteRegister& reg);
	void setBitIndirect(u8 bitNumber);
	void resetBitIndirect(u8 bitNumber);

	void testRegisterBit(u8 bitNumber, ByteRegister& reg);
	void testBitIndirect(u8 bitNumber);

	void executeCbOpcode();
};
