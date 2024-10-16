#include "cpu.h"
#include "emu.h"
#include <assert.h>


CPU::CPU(emu& gbEmu) :
	gbEmu(gbEmu),
	afReg(a,f),
	bcReg(b,c),
	deReg(d,e),
	hlReg(h,l)
{
	init();
}

bool CPU::isHalted()
{
	return false;
}

void CPU::init()
{
	cycles = 0;
	PC.setWord(0x100);
	SP.setWord(0x0);
	IR = 0;
	IE = 0;
	currentOpcode = 0;
}

void CPU::fetchOpcode()
{
	currentOpcode = gbEmu.addressBus.busRead(PC.getWord());
	++PC;
}

void CPU::executeCurrentOpcode()
{
	printf("PC: %X", PC.getWord());
	printf(" OPCODE: %X\n", currentOpcode);

	switch(currentOpcode)
	{
	case 0x00:
		opcodeNOP();
		break;
	case 0x01:
		//LD BC N16
		LD_R16_N16(bcReg);
		break;
	case 0x02:
		//LD (BC), A
		LD_A16_R8(bcReg, a);
		break;
	case 0x03:
		//INC BC
		INC_R16(bcReg);
		break;
	case 0x04:
		//INC B
		INC_R8(b);
		break;
	case 0x05:
		//DEC B
		DEC_R8(b);
		break;
	case 0x06:
		//LD B N8
		LD_R8_N8(bcReg.getHighByteRegister());
		break;	
	case 0x07:
		//RCLA
		rotateAccumulatorLeftCircular();
		break;
	case 0x08:
		//LD (nn) SP
		LD_A16_R16(SP);
		break;
	case 0x09:
		//ADD HL, BC
		ADD_HL_R16(bcReg);
		break;
	case 0x0A:
		//LD A, (BC)
		LD_R8_R16(a, bcReg);
		break;
	case 0x0B:
		//DEC BC
		DEC_R16(bcReg);
		break;
	case 0x0C:
		//INC C
		INC_R8(c);
		break;
	case 0x0D:
		//DEC C
		DEC_R8(c);
		break;
	case 0x0E:
		//LD C N8
		LD_R8_N8(bcReg.getLowByteRegister());
		break;
	case 0x0F:
		//RRCA
		rotateAccumulatorRightCircular();
		break;
	case 0x10:
		//STOP
		STOP();
		break;
	case 0x11:
		//LD DE N16
		LD_R16_N16(deReg);
		break;
	case 0x12:
		//LD (DE), A
		LD_A16_R8(deReg, a);
		break;
	case 0x13:
		//INC DE
		INC_R16(deReg);
		break;
	case 0x14:
		//INC D
		INC_R8(d);
		break;
	case 0x15:
		//DEC D
		DEC_R8(d);
		break;
	case 0x16:
		//LD D, n
		LD_R8_N8(d);
		break;
	case 0x17:
		//RLA
		rotateAccumulatorLeft();
		break;
	case 0x18:
		//JR e
		JR_E();
		break;
	case 0x19:
		//ADD HL, DE
		ADD_HL_R16(deReg);
		break;
	case 0x1A:
		//LD A, (DE)
		LD_R8_R16(a, deReg);
		break;
	case 0x1B:
		//DEC DE
		DEC_R16(deReg);
		break;
	case 0x1C:
		//INC E
		INC_R8(e);
		break;
	case 0x1D:
		//DEC E
		DEC_R8(e);
		break;
	case 0x1E:
		//LD E, n
		LD_R8_N8(e);
		break;
	case 0x1F:
		//RRA
		rotateAccumulatorRight();
		break;
	case 0x20:
	{
		//JR NZ, e
		bool val = f.getZeroFlag();
		val = !f.getZeroFlag();
		JR_CC_E(!f.getZeroFlag());
		break;
	}
		
	case 0x21:
		//LD HL N16
		LD_R16_N16(hlReg);
		break;
	case 0x22:
		//LD [HL+] A
		LD_R16_INC_A(hlReg);
		break;
	case 0x23:
		//INC HL
		INC_R16(hlReg);
		break;
	case 0x24:
		//INC H
		INC_R8(h);
		break;
	case 0x25:
		//DEC H
		DEC_R8(h);
		break;
	case 0x26:
		//LD H, n
		LD_R8_N8(h);
		break;
	case 0x27:
		//DAA
		DAA();
		break;
	case 0x28:
		//JR Z, e
		JR_CC_E(f.getZeroFlag());
		break;
	case 0x29:
		//ADD HL, HL
		ADD_HL_R16(hlReg);
		break;
	case 0x2A:
		//LD A, (HL +)
		LD_R8_R16_INC(a, hlReg);
		break;
	case 0x2B:
		//INC HL
		INC_R16(hlReg);
		break;
	case 0x2C:
		//INC L
		INC_R8(l);
		break;
	case 0x2D:
		//DEC L
		DEC_R8(l);
		break;
	case 0x2E:
		//LD L, n
		LD_R8_N8(l);
		break;
	case 0x2F:
		//CPL
		CPL();
		break;
	case 0x30:
		//JR NC, e
		JR_CC_E(!f.getCarryFlag());
		break;
	case 0x31:
		//LD HL, nn
		LD_R16_N16(hlReg);
		break;
	case 0x32:
		//LD [HL-] A
		LD_R16_DEC_A(hlReg);
		break;
	case 0x33:
		//INC SP
		INC_R16(SP);
		break;
	case 0x34:
		//INC (HL)
		INC_R16_INDIRECT(hlReg);
		break;
	case 0x35:
		//DEC (HL)
		DEC_R16_INDIRECT(hlReg);
		break;
	case 0x36:
		//LD (HL), n
		LD_R16_N8(hlReg);
		break;
	case 0x37:
		//SCF
		SCF();
		break;
	case 0x38:
		//JR C, e
		JR_CC_E(f.getCarryFlag());
		break;
	case 0x39:
		//ADD HL, SP
		ADD_HL_R16(SP);
		break;
	case 0x3A:
		//LD A, (HL -)
		LD_R8_R16_DEC(a, hlReg);
		break;
	case 0x3B:
		//DEC SP
		DEC_R16(SP);
		break;
	case 0x3C:
		//INC A
		INC_R8(a);
		break;
	case 0x3D:
		//DEC A
		DEC_R8(a);
		break;
	case 0x3E:
		//LD A,n
		LD_R8_N8(a);
		break;
	case 0x3F:
		//CCF
		CCF();
		break;
	case 0x40:
		//LD B, B
		LD_R8_R8(b, b);
		break;
	case 0x41:
		//LD B, C
		LD_R8_R8(b, c);
		break;
	case 0x42:
		//LD B, D
		LD_R8_R8(b, d);
		break;
	case 0x43:
		//LD B, E
		LD_R8_R8(b, e);
		break;
	case 0x44:
		//LD B, h
		LD_R8_R8(b, h);
		break;
	case 0x45:
		//LD B, L
		LD_R8_R8(b, l);
		break;
	case 0x46:
		//LD B, (HL)
		LD_R8_R16(b, hlReg);
		break;
	case 0x47:
		//LD B, A
		LD_R8_R8(b, a);
		break;
	case 0x48:
		//LD C, B
		LD_R8_R8(c, b);
		break;
	case 0x49:
		//LD C, C
		LD_R8_R8(c, c);
		break;
	case 0x4A:
		//LD C, D
		LD_R8_R8(c, d);
		break;
	case 0x4B:
		//LD C, E
		LD_R8_R8(c, e);
		break;
	case 0x4C:
		//LD C, H
		LD_R8_R8(c, h);
		break;
	case 0x4D:
		//LD C, L
		LD_R8_R8(c, l);
		break;
	case 0x4E:
		//LD C, (HL)
		LD_R8_R16(c, hlReg);
		break;
	case 0x4F:
		//LD C, A
		LD_R8_R8(c, a);
		break;
	case 0x50:
		//LD D, B
		LD_R8_R8(d, b);
		break;
	case 0x51:
		//LD D, C
		LD_R8_R8(d, c);
		break;
	case 0x52:
		//LD D, D
		LD_R8_R8(d, d);
		break;
	case 0x53:
		//LD D, E
		LD_R8_R8(d, e);
		break;
	case 0x54:
		//LD D, H
		LD_R8_R8(d, h);
		break;
	case 0x55:
		//LD D, L
		LD_R8_R8(d, l);
		break;
	case 0x56:
		//LD D, (HL)
		LD_R8_R16(d, hlReg);
		break;
	case 0x57:
		//LD D, A
		LD_R8_R8(d, a);
		break;
	case 0x58:
		//LD E, B
		LD_R8_R8(e, b);
		break;
	case 0x59:
		//LD E, C
		LD_R8_R8(e, c);
		break;
	case 0x5A:
		//LD E, D
		LD_R8_R8(e, d);
		break;
	case 0x5B:
		//LD E, E
		LD_R8_R8(e, e);
		break;
	case 0x5C:
		//LD E, H
		LD_R8_R8(e, h);
		break;
	case 0x5D:
		//LD E, L
		LD_R8_R8(e, l);
		break;
	case 0x5E:
		//LD E, (HL)
		LD_R8_R16(e, hlReg);
		break;
	case 0x5F:
		//LD E, A
		LD_R8_R8(e, a);
		break;
	case 0x60:
		//LD H, B
		LD_R8_R8(h, b);
		break;
	case 0x61:
		//LD H, C
		LD_R8_R8(h, c);
		break;
	case 0x62:
		//LD H, D
		LD_R8_R8(h, d);
		break;
	case 0x63:
		//LD H, E
		LD_R8_R8(h, e);
		break;
	case 0x64:
		//LD H, H
		LD_R8_R8(h, h);
		break;
	case 0x65:
		//LD H, L
		LD_R8_R8(h, l);
		break;
	case 0x66:
		//LD H, (HL)
		LD_R8_R16(h, hlReg);
		break;
	case 0x67:
		//LD H, A
		LD_R8_R8(h, a);
		break;
	case 0x68:
		//LD L, B
		LD_R8_R8(l, b);
		break;
	case 0x69:
		//LD L, C
		LD_R8_R8(l, c);	
		break;
	case 0x6A:
		//LD L, D
		LD_R8_R8(l, d);		
		break;
	case 0x6B:
		//LD L, E
		LD_R8_R8(l, e);
		break;
	case 0x6C:
		//LD L, H
		LD_R8_R8(l, h);
		break;
	case 0x6D:
		//LD L, L
		LD_R8_R8(l, l);
		break;
	case 0x6E:
		//LD L, (HL)
		LD_R8_R16(l, hlReg);
		break;
	case 0x6F:
		//LD L, A
		LD_R8_R8(l, a);
		break;
	case 0x70:
		//LD (HL), B
		LD_A16_R8(hlReg, b);
		break;
	case 0x71:
		//LD (HL), C
		LD_A16_R8(hlReg, c);
		break;
	case 0x72:
		//LD (HL), D
		LD_A16_R8(hlReg, d);
		break;
	case 0x73:
		//LD (HL), E
		LD_A16_R8(hlReg, e);
		break;
	case 0x74:
		//LD (HL), H
		LD_A16_R8(hlReg, h);
		break;
	case 0x75:
		//LD (HL), L
		LD_A16_R8(hlReg, l);
		break;
	case 0x76:
		//HALT
		HALT();
		break;
	case 0x77:
		//LD (HL), A
		LD_A16_R8(hlReg, a);
		break;
	case 0x78:
		//LD A, B
		LD_R8_R8(a, b);
		break;
	case 0x79:
		//LD A, C
		LD_R8_R8(a, c);
		break;
	case 0x7A:
		//LD A, D
		LD_R8_R8(a, d);
		break;
	case 0x7B:
		//LD A, E
		LD_R8_R8(a, e);
		break;
	case 0x7C:
		//LD A, H
		LD_R8_R8(a, h);
		break;
	case 0x7D:
		//LD A, L
		LD_R8_R8(a, l);
		break;
	case 0x7E:
		//LD A, (HL)
		LD_R8_R16(a, hlReg);
		break;
	case 0x7F:
		//LD A, A
		LD_R8_R8(a, a);
		break;
	case 0x80:
		//ADD B
		ADD_R8(b);
		break;
	case 0x81:
		//ADD C
		ADD_R8(c);
		break;
	case 0x82:
		//ADD D
		ADD_R8(d);
		break;
	case 0x83:
		//ADD E
		ADD_R8(e);
		break;
	case 0x84:
		//ADD H
		ADD_R8(h);
		break;
	case 0x85:
		//ADD L
		ADD_R8(l);
		break;
	case 0x86:
		//ADD (HL)
		ADD_HL();
		break;
	case 0x87:
		//ADD A
		ADD_R8(a);
		break;
	case 0x88:
		//ADC B
		ADC_R8(b);
		break;
	case 0x89:
		//ADC C
		ADC_R8(c);
		break;
	case 0x8A:
		//ADC D
		ADC_R8(d);
		break;
	case 0x8B:
		//ADC E
		ADC_R8(e);
		break;
	case 0x8C:
		//ADC H
		ADC_R8(h);
		break;
	case 0x8D:
		//ADC L
		ADC_R8(l);
		break;
	case 0x8E:
		//ADC (HL)
		ADC_HL();
		break;
	case 0x8F:
		//ADC A
		ADC_R8(a);
		break;
	case 0x90:
		//SUB B
		SUB_R8(b);
		break;
	case 0x91:
		//SUB C
		SUB_R8(c);
		break;
	case 0x92:
		//SUB D
		SUB_R8(d);
		break;
	case 0x93:
		//SUB E
		SUB_R8(e);
		break;
	case 0x94:
		//SUB H
		SUB_R8(h);
		break;
	case 0x95:
		//SUB L
		SUB_R8(l);
		break;
	case 0x96:
		//SUB (HL)
		SUB_HL();
		break;
	case 0x97:
		//SUB A
		SUB_R8(a);
		break;
	case 0x98:
		//SBC B
		SBC_R8(b);
		break;
	case 0x99:
		//SBC C
		SBC_R8(c);
		break;
	case 0x9A:
		//SBC D
		SBC_R8(d);
		break;
	case 0x9B:
		//SBC E
		SBC_R8(e);
		break;
	case 0x9C:
		//SBC H
		SBC_R8(h);
		break;
	case 0x9D:
		//SBC L
		SBC_R8(l);
		break;
	case 0x9E:
		//SBC (HL)
		SBC_HL();
		break;
	case 0x9F:
		//SBC A
		SBC_R8(a);
		break;
	case 0xA0:
		//AND B
		AND_R8(b);
		break;
	case 0xA1:
		//AND C
		AND_R8(c);
		break;
	case 0xA2:
		//AND D
		AND_R8(d);
		break;
	case 0xA3:
		//AND E
		AND_R8(e);
		break;
	case 0xA4:
		//AND H
		AND_R8(h);
		break;
	case 0xA5:
		//AND L
		AND_R8(l);
		break;
	case 0xA6:
		//AND (HL)
		AND_HL();
		break;
	case 0xA7:
		//AND A
		AND_R8(a);
		break;
	case 0xA8:
		//XOR B
		XOR_R8(b);
		break;
	case 0xA9:
		//XOR C
		XOR_R8(c);
		break;
	case 0xAA:
		//XOR D
		XOR_R8(d);
		break;
	case 0xAB:
		//XOR E
		XOR_R8(e);
		break;
	case 0xAC:
		//XOR H
		XOR_R8(h);
		break;
	case 0xAD:
		//XOR L
		XOR_R8(l);
		break;
	case 0xAE:
		//XOR (HL)
		XOR_HL();
		break;
	case 0xAF:
		//XOR A
		XOR_R8(a);
		break;
	case 0xB0:
		//OR B
		OR_R8(b);
		break;
	case 0xB1:
		//OR C
		OR_R8(c);
		break;
	case 0xB2:
		//OR D
		OR_R8(d);
		break;
	case 0xB3:
		//OR E
		OR_R8(e);
		break;
	case 0xB4:
		//OR H
		OR_R8(h);
		break;
	case 0xB5:
		//OR L
		OR_R8(l);
		break;
	case 0xB6:
		//OR (HL)
		OR_HL();
		break;
	case 0xB7:
		//OR A
		OR_R8(a);
		break;
	case 0xB8:
		//CP B
		CP_R8(b);
		break;
	case 0xB9:
		//CP C
		CP_R8(c);
		break;
	case 0xBA:
		//CP d
		CP_R8(d);
		break;
	case 0xBB:
		//CP E
		CP_R8(e);
		break;
	case 0xBC:
		//CP H
		CP_R8(h);
		break;
	case 0xBD:
		//CP L
		CP_R8(l);
		break;
	case 0xBE:
		//CP (HL)
		CP_HL();
		break;
	case 0xBF:
		//CP A
		CP_R8(a);
		break;
	case 0xC0:
		//RET NZ
		RET_Cond(!f.getZeroFlag());
		break;
	case 0xC1:
		//POP BC
		POP(bcReg);
		break;
	case 0xC2:
		//JP NZ,nn
		JP_CC_NN(!f.getZeroFlag());
		break;
	case 0xC3:
		//JP nn
		opcodeJP_A16();
		break;
	case 0xC4:
		//CALL NZ, nn
		CALL_cond(!f.getZeroFlag());
		break;
	case 0xC5:
		//PUSH BC
		PUSH(bcReg);
		break;
	case 0xC6:
		//ADD n
		ADD_N8();
		break;
	case 0xC7:
		//RST 0x00
		RST(RST1);
		break;
	case 0xC8:
		//RET Z
		RET_Cond(f.getCarryFlag());
		break;
	case 0xC9:
		//RET
		RET();
		break;
	case 0xCA:
		//JP Z, nn
		JP_CC_NN(f.getZeroFlag());
		break;
	case 0xCB:
		//Execute $CB intruction
		executeCbOpcode();
		break;
	case 0xCC:
		//CALL Z, nn
		CALL_cond(f.getZeroFlag());
		break;
	case 0xCD:
		//CALL nn
		CALL();
		break;
	case 0xCE:
		//ADC n
		ADC_N8();
		break;
	case 0xCF:
		//RST 0x08;
		RST(RST2);
		break;
	case 0xD0:
		//RET NC
		RET_Cond(!f.getCarryFlag());
		break;
	case 0xD1:
		//POP DE
		POP(deReg);
		break;
	case 0xD2:
		//JP NC, nn
		JP_CC_NN(!f.getCarryFlag());
		break;
	case 0xD4:
		//CALL NC, nn
		CALL_cond(!f.getCarryFlag());
		break;
	case 0xD5:
		//PUSH DE
		PUSH(deReg);
		break;
	case 0xD6:
		//SUB n
		SUB_N8();
		break;
	case 0xD7:
		//RST 0x10;
		RST(RST3);
		break;
	case 0xD8:
		//RET C
		RET_Cond(f.getCarryFlag());
		break;
	case 0xD9:
		//RETI
		RETI();
		break;
	case 0xDA:
		//JP C, nn
		JP_CC_NN(f.getCarryFlag());
		break;
	case 0xDC:
		//CALL C, nn
		CALL_cond(f.getCarryFlag());
		break;
	case 0xDE:
		//SBC n
		SBC_N8();
		break;
	case 0xDF:
		//RST 0x18;
		RST(RST4);
		break;
	case 0xE0:
		//LDH (n), A
		LDH_A8_A();
		break;
	case 0xE1:
		//POP HL
		POP(hlReg);
		break;
	case 0xE2:
		//LDH (C), A
		LDH_RC_A();
		break;
	case 0xE5:
		//PUSH HL
		PUSH(hlReg);
		break;
	case 0xE6:
		//AND n
		AND_N8();
		break;
	case 0xE7:
		//RST 0x20;
		RST(RST5);
		break;
	case 0xE8:
		//ADD SP,e
		ADD_SP_E();
		break;
	case 0xE9:
		//JP HL
		JP_HL();
		break;
	case 0xEA:
		//LD (nn), A
		LD_A16_A();
		break;
	case 0xEE:
		//XOR n
		XOR_N8();
		break;
	case 0xEF:
		//RST 0x28;
		RST(RST6);
		break;
	case 0xF0:
		//LDH A, (n)
		LDH_A_A8();
		break;
	case 0xF1:
		//POP AF
		POP(afReg);
		break;
	case 0xF2:
		//LDH A, (C)
		LDH_A_RC();
		break;
	case 0xF3:
		//DI
		DI();
		break;
	case 0xF5:
		//PUSH AF
		PUSH(afReg);
		break;
	case 0xF6:
		//OR n
		OR_N8();
		break;
	case 0xF7:
		//RST 0x30;
		RST(RST7);
		break;
	case 0xF8:
		//LD HL, SP+e
		loadHLFromAdjustedSP();
		break;
	case 0xF9:
		//LD SP, HL
		loadSPFromHL();
		break;
	case 0xFA:
		//LD A, (nn)
		LD_A_A16();
		break;
	case 0xFB:
		//EI
		EI();
		break;
	case 0xFE:
		//CP n
		CP_N8();
		break;
	case 0xFF:
		//RST 0x38;
		RST(RST8);
		break;
	default:
		printf("UNKNOWN OPCODE: %X\n", currentOpcode);
		exit(-1);
		break;
	}
}

void CPU::cycleCPU(int numCycles)
{
	cycles += numCycles;
}

u16 CPU::readWordFromPC()
{
	u16 lo = gbEmu.addressBus.busRead(PC.getWord());
	u16 hi = gbEmu.addressBus.busRead(PC.getWord() + 1);
	PC += 2;
	return lo | (hi << 8);
}

u16 CPU::readWordFromSP()
{
	u16 lo = gbEmu.addressBus.busRead(SP.getWord());
	u16 hi = gbEmu.addressBus.busRead(SP.getWord() + 1);
	SP += 2;
	return lo | (hi << 8);
}

u8 CPU::readByteFromPC()
{
	u8 value = gbEmu.addressBus.busRead(PC.getWord());
	++PC;
	return value;
}

u8 CPU::readByteFromAddress(u16 address)
{
	return gbEmu.addressBus.busRead(address);
}

void CPU::writeByteAtAddress(u16 address, u8 data)
{
	gbEmu.addressBus.busWrite(address, data);
}



void CPU::opcodeNOP()
{
	cycleCPU(1);

}

void CPU::opcodeJP_A16()
{
	PC.setWord(readWordFromPC());
	cycleCPU(1);
}


void CPU::opcodeLD_HLD_A()
{
	gbEmu.addressBus.busWrite(hlReg.getWord(), afReg.getHighByteRegister().getRegisterValue());
	++hlReg;
}

u8 CPU::INC_8BIT(u8 value)
{
	u8 result = value + 1;

	if (result == 0)
	{
		f.setZeroFlag(true);
	}
	else
	{
		f.setZeroFlag(false);
	}

	if ((1 + (value & 0xf) & 0x10) == 0x10)
	{
		f.setHalfCarryFlag(true);
	}
	else 
	{
		f.setHalfCarryFlag(false);
	}
	f.setSubstractionFlag(false);

	return result;
}

void CPU::INC_R8(ByteRegister& reg)
{
	reg.setRegisterValue(INC_8BIT(reg.getRegisterValue()));
	cycleCPU(1);
}

void CPU::INC_R16(WordRegister& reg)
{
	++reg;
	cycleCPU(2);
}

void CPU::INC_R16_INDIRECT(ByteRegisterPair& reg)
{
	writeByteAtAddress(reg.getWord(), INC_8BIT(readByteFromAddress(reg.getWord())));
	cycleCPU(3);
}

u8 CPU::DEC_8BIT(u8 value)
{
	u8 result = value - 1;
	if (result == 0)
	{
		f.setZeroFlag(true);
	}
	else
	{
		f.setZeroFlag(false);
	}

	if ((1 + (value & 0xf) & 0x10) == 0x10)
	{
		f.setHalfCarryFlag(true);
	}
	else
	{
		f.setHalfCarryFlag(false);
	}

	f.setSubstractionFlag(true);
	return result;
}

void CPU::DEC_R8(ByteRegister& reg)
{
	reg.setRegisterValue(DEC_8BIT(reg.getRegisterValue()));
	//printf("Register value = %X\n", reg.getRegisterValue());
	cycleCPU(1);
}

void CPU::DEC_R16(WordRegister& reg)
{
	--reg;
	cycleCPU(2);
}

void CPU::DEC_R16_INDIRECT(ByteRegisterPair& reg)
{
	writeByteAtAddress(reg.getWord(), DEC_8BIT(readByteFromAddress(reg.getWord())));
	cycleCPU(3);
}

void CPU::DAA()
{
}

void CPU::SCF()
{
	f.setCarryFlag(true);
	f.setHalfCarryFlag(false);
	f.setSubstractionFlag(false);
	cycleCPU(1);
}

void CPU::CPL()
{
	a.setRegisterValue(~a.getRegisterValue());

	f.setSubstractionFlag(true);
	f.setHalfCarryFlag(true);
	cycleCPU(1);
}

void CPU::CCF()
{
	f.setCarryFlag(!f.getCarryFlag());

	f.setHalfCarryFlag(false);
	f.setSubstractionFlag(false);
	cycleCPU(1);
}

void CPU::LD_R16_N16(WordRegister& reg)
{
	reg.setWord(readWordFromPC());
	cycleCPU(3);
}

void CPU::LD_R8_R8(ByteRegister& reg1, ByteRegister& reg2)
{
	reg1.setRegisterValue(reg2.getRegisterValue());	
	cycleCPU(1);
}

void CPU::LD_R8_N8(ByteRegister& reg)
{
	reg.setRegisterValue(readByteFromPC());
	cycleCPU(2);
}

void CPU::LD_A16_R8(ByteRegisterPair& reg, ByteRegister& reg2)
{
	gbEmu.addressBus.busWrite(reg.getWord(), reg2.getRegisterValue());
	cycleCPU(2);
}

void CPU::LD_R16_INC_A(ByteRegisterPair& reg)
{
	gbEmu.addressBus.busWrite(reg.getWord(), a.getRegisterValue());
	++reg;
	cycleCPU(2);
}

void CPU::LD_R16_DEC_A(ByteRegisterPair& reg)
{
	gbEmu.addressBus.busWrite(reg.getWord(), a.getRegisterValue());
	--reg;
	cycleCPU(2);

	//DEBUG
	if (reg.getWord() == 57212)
		printf("stop\n");
	printf("HLREG Value: %d\n", reg.getWord());
}

void CPU::LD_R16_N8(ByteRegisterPair& reg)
{
	gbEmu.addressBus.busWrite(reg.getWord(), readByteFromPC());
	cycleCPU(3);
}

void CPU::LD_R8_R16(ByteRegister& reg1, ByteRegisterPair& reg2)
{
	reg1.setRegisterValue(readByteFromAddress(reg2.getWord()));
	cycleCPU(2);
}

void CPU::LD_R8_R16_INC(ByteRegister& reg1, ByteRegisterPair& reg2)
{
	reg1.setRegisterValue(readByteFromAddress(reg2.getWord()));
	++reg2;
	cycleCPU(2);
}

void CPU::LD_R8_R16_DEC(ByteRegister& reg1, ByteRegisterPair& reg2)
{
	reg1.setRegisterValue(readByteFromAddress(reg2.getWord()));
	--reg2;
	cycleCPU(2);
}

void CPU::LD_A_A16()
{
	u16 address = readWordFromPC();
	a.setRegisterValue(readByteFromAddress(address));
	cycleCPU(4);
}

void CPU::LD_A16_A()
{
	u16 address = readWordFromPC();
	writeByteAtAddress(address, a.getRegisterValue());
	cycleCPU(4);
}

void CPU::LD_A16_R16(WordRegister& reg)
{
	u16 address = readWordFromPC();
	u16 value = reg.getWord();
	writeByteAtAddress(address, (u8)(value >> 8));
	writeByteAtAddress(address+1, (u8)(value & 0x00FF));
	cycleCPU(5);
}

void CPU::LDH_A8_A()
{
	u16 address = readByteFromPC() | 0xFF00;
	writeByteAtAddress(address, a.getRegisterValue());

	cycleCPU(3);
}

void CPU::LDH_A_A8()
{
	u16 address = readByteFromPC() | 0xFF00;
	a.setRegisterValue(readByteFromAddress(address));
	cycleCPU(3);
}

void CPU::LDH_RC_A()
{
	u16 address = c.getRegisterValue() | 0xFF00;
	writeByteAtAddress(address, a.getRegisterValue());
	cycleCPU(2);
}

void CPU::LDH_A_RC()
{
	u16 address = c.getRegisterValue() | 0xFF00;
	a.setRegisterValue(readByteFromAddress(address));
	cycleCPU(2);
}

void CPU::loadSPFromHL()
{
	SP.setWord(hlReg.getWord());
	cycleCPU(2);
}

void CPU::loadHLFromAdjustedSP()
{
	u16 SPValue = SP.getWord();
	char e = (char)readByteFromPC();

	hlReg.setWord(SPValue + e);
	
	if (((e & 0xf) + (SPValue & 0xf) & 0x10) == 0x10)
	{
		f.setHalfCarryFlag(true);
	}
	else
	{
		f.setHalfCarryFlag(false);
	}

	if (((e & 0xff) + (SPValue & 0xff)) > 0xFF)
	{
		f.setCarryFlag(true);
	}
	else
	{
		f.setCarryFlag(false);
	}

	f.setZeroFlag(false);
	cycleCPU(3);
}



u8 CPU::ADD_8BIT(u8 oper1, u8 oper2)
{
	u8 result = oper1 + oper2;

	if (result == 0)
	{
		f.setZeroFlag(true);
	}
	else
	{
		f.setZeroFlag(false);
	}

	if (((oper2 & 0xf) + (oper1 & 0xf) & 0x10) == 0x10)
	{
		f.setHalfCarryFlag(true);
	}
	else
	{
		f.setHalfCarryFlag(false);
	}

	if (((oper2 & 0xff) + (oper1 & 0xff)) > 0xFF)
	{
		f.setCarryFlag(true);
	}
	else
	{
		f.setCarryFlag(false);
	}

	f.setSubstractionFlag(false);

	return result;
}

void CPU::ADD_R8(ByteRegister& reg)
{
	a.setRegisterValue(ADD_8BIT(a.getRegisterValue(), reg.getRegisterValue()));
	cycleCPU(1);
}

void CPU::ADD_N8()
{
	a.setRegisterValue(ADD_8BIT(a.getRegisterValue(), readByteFromPC()));
	cycleCPU(2);
}

void CPU::ADD_HL()
{
	a.setRegisterValue(ADD_8BIT(a.getRegisterValue(), readByteFromAddress(hlReg.getWord())));
	cycleCPU(2);
}

u16 CPU::ADD_16BIT(u16 oper1, u16 oper2)
{
	u16 result = oper1 + oper2;

	if (((oper2 & 0xFFF) + (oper1 & 0xFFF)) > 0xFFF)
	{
		f.setHalfCarryFlag(true);
	}
	else
	{
		f.setHalfCarryFlag(false);
	}

	if (((oper2 & 0xFFFF) + (oper1 & 0xFFFF)) > 0xFFFF)
	{
		f.setCarryFlag(true);
	}
	else
	{
		f.setCarryFlag(false);
	}

	f.setSubstractionFlag(false);

	return result;
}

void CPU::ADD_HL_R16(WordRegister& reg)
{
	hlReg.setWord(ADD_16BIT(hlReg.getWord(), reg.getWord()));
	cycleCPU(2);
}

void CPU::ADD_SP_E()
{
	char e = (char)readByteFromPC();
	u16 SPValue = SP.getWord();
	u16 result = SPValue + e;
	SP.setWord(result);

	if (((e & 0xf) + (SPValue & 0xf) & 0x10) == 0x10)
	{
		f.setHalfCarryFlag(true);
	}
	else
	{
		f.setHalfCarryFlag(false);
	}

	if (((e & 0xff) + (SPValue & 0xff)) > 0xFF)
	{
		f.setCarryFlag(true);
	}
	else
	{
		f.setCarryFlag(false);
	}
    
	f.setZeroFlag(false);
	cycleCPU(4);
}

void CPU::ADC_R8(ByteRegister& reg)
{
	a.setRegisterValue(ADD_8BIT(a.getRegisterValue(), reg.getRegisterValue() + (u8)f.getCarryFlag()));
	cycleCPU(1);
}

void CPU::ADC_N8()
{
	a.setRegisterValue(ADD_8BIT(a.getRegisterValue(), readByteFromPC() + (u8)f.getCarryFlag()));
	cycleCPU(2);
}

void CPU::ADC_HL()
{
	a.setRegisterValue(ADD_8BIT(a.getRegisterValue(), readByteFromAddress(hlReg.getWord()) + (u8)f.getCarryFlag()));
	cycleCPU(2);
}

u8 CPU::SUB_8BIT(u8 oper1, u8 oper2)
{
	u8 result = oper1 - oper2;
	
	if (result == 0)
	{
		f.setZeroFlag(true);
	}
	else 
	{
		f.setZeroFlag(false);
	}

	if (((oper2 & 0xf) + (oper1 & 0xf) & 0x10) == 0x10)
	{
		f.setHalfCarryFlag(true);
	}
	else
	{
		f.setHalfCarryFlag(false);
	}

	if (((oper2 & 0xff) + (oper1 & 0xff)) > 0xFF)
	{
		f.setCarryFlag(true);
	}
	else
	{
		f.setCarryFlag(false);
	}

	f.setSubstractionFlag(true);

	return result;
}

void CPU::SUB_R8(ByteRegister& reg)
{	
	a.setRegisterValue(SUB_8BIT(a.getRegisterValue(), reg.getRegisterValue()));
	cycleCPU(1);
}

void CPU::SUB_N8()
{
	a.setRegisterValue(SUB_8BIT(a.getRegisterValue(), readByteFromPC()));
	cycleCPU(2);
}

void CPU::SUB_HL()
{
	a.setRegisterValue(SUB_8BIT(a.getRegisterValue(), readByteFromAddress(hlReg.getWord())));
	cycleCPU(2);
}

void CPU::SBC_R8(ByteRegister& reg)
{
	a.setRegisterValue(SUB_8BIT(a.getRegisterValue(), reg.getRegisterValue() - (u8) f.getCarryFlag()));
	cycleCPU(1);
}

void CPU::SBC_N8()
{
	a.setRegisterValue(SUB_8BIT(a.getRegisterValue(), readByteFromPC() - (u8)f.getCarryFlag()));
	cycleCPU(2);
}

void CPU::SBC_HL()
{
	a.setRegisterValue(SUB_8BIT(a.getRegisterValue(), readByteFromAddress(hlReg.getWord()) - (u8)f.getCarryFlag()));
	cycleCPU(2);
}

void CPU::CP_R8(ByteRegister& reg)
{
	SUB_8BIT(a.getRegisterValue(), reg.getRegisterValue());
	cycleCPU(1);
}

void CPU::CP_N8()
{
	SUB_8BIT(a.getRegisterValue(), readByteFromPC());
	cycleCPU(2);
}

void CPU::CP_HL()
{
	SUB_8BIT(a.getRegisterValue(), readByteFromAddress(hlReg.getWord()));
	cycleCPU(2);
}

u8 CPU::AND_8BIT(u8 oper1, u8 oper2)
{
	u8 result = oper1 & oper2;
	if (result == 0)
	{
		f.setZeroFlag(true);
	}
	else {
		f.setZeroFlag(false);
	}
	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(true);
	f.setCarryFlag(false);

	return result;
}

void CPU::AND_R8(ByteRegister& reg)
{
	a.setRegisterValue(AND_8BIT(a.getRegisterValue(), reg.getRegisterValue()));
	cycleCPU(1);
}

void CPU::AND_N8()
{
	a.setRegisterValue(AND_8BIT(a.getRegisterValue(), readByteFromPC()));
	cycleCPU(2);
}

void CPU::AND_HL()
{
	a.setRegisterValue(AND_8BIT(a.getRegisterValue(), readByteFromAddress(hlReg.getWord())));
	cycleCPU(2);
}

u8 CPU::OR_8BIT(u8 oper1, u8 oper2)
{
	u8 result = oper1 | oper2;
	if (result == 0)
	{
		f.setZeroFlag(true);
	}
	else {
		f.setZeroFlag(false);
	}
	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);
	f.setCarryFlag(false);
	return result;
}

void CPU::OR_R8(ByteRegister& reg)
{
	a.setRegisterValue(OR_8BIT(a.getRegisterValue(), reg.getRegisterValue()));
	cycleCPU(1);
}

void CPU::OR_N8()
{
	a.setRegisterValue(OR_8BIT(a.getRegisterValue(), readByteFromPC()));
	cycleCPU(2);
}

void CPU::OR_HL()
{
	a.setRegisterValue(OR_8BIT(a.getRegisterValue(), readByteFromAddress(hlReg.getWord())));
	cycleCPU(2);
}

u8 CPU::XOR_8BIT(u8 oper1, u8 oper2)
{
	u8 result = oper1 ^ oper2;

	if (result == 0)
	{
		f.setZeroFlag(true);
	}
	else {
		f.setZeroFlag(false);
	}
	f.setCarryFlag(false);
	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);

	return result;
}

void CPU::XOR_R8(ByteRegister& reg)
{
	a.setRegisterValue(XOR_8BIT(a.getRegisterValue(), reg.getRegisterValue()));
	cycleCPU(1);
}

void CPU::XOR_N8()
{
	a.setRegisterValue(XOR_8BIT(a.getRegisterValue(), readByteFromPC()));
	cycleCPU(2);
}

void CPU::XOR_HL()
{
	a.setRegisterValue(XOR_8BIT(a.getRegisterValue(), readByteFromAddress(hlReg.getWord())));
	cycleCPU(2);
}

void CPU::JR_CC_E(bool condition)
{
	char e = (char)readByteFromPC();
	if (condition)
	{
		PC.setWord(PC.getWord() + e);
		cycleCPU(1);
	}
	cycleCPU(3);
}

void CPU::JR_E()
{
	char e = (char)readByteFromPC();
	PC.setWord(PC.getWord() + e);
	cycleCPU(3);
}

void CPU::JP_CC_NN(bool condition)
{
	u16 address = readWordFromPC();
	if (condition)
	{
		PC.setWord(address);
		cycleCPU(1);
	}
	cycleCPU(3);
}

void CPU::JP_HL()
{
	PC.setWord(hlReg.getWord());
	cycleCPU(1);
}

void CPU::RST(RSTCodes rstCode)
{
	PC.setWord(rstCode);
	cycleCPU(4);
}

void CPU::CALL_cond(bool condition)
{
	u16 address = readWordFromPC();
	if (condition)
	{
		--SP;
		writeByteAtAddress(SP.getWord(), (u8)(PC.getWord() >> 8));
		--SP;
		writeByteAtAddress(SP.getWord(), (u8)(PC.getWord() & 0x00FF));
		PC.setWord(address);
		cycleCPU(3);
	}
	cycleCPU(3);
}

void CPU::CALL()
{
	u16 address = readWordFromPC();
	--SP;
	writeByteAtAddress(SP.getWord(), (u8)(PC.getWord() >> 8));
	--SP;
	writeByteAtAddress(SP.getWord(), (u8)(PC.getWord() & 0x00FF));
	PC.setWord(address);
	cycleCPU(6);
}

void CPU::RET()
{
	PC.setWord(readWordFromSP());
	cycleCPU(4);
}

void CPU::RETI()
{
	RET();
	IME = 1;
}


void CPU::RET_Cond(bool condition)
{
	if (condition)
	{
		PC.setWord(readWordFromSP());
		cycleCPU(3);
	}
	cycleCPU(2);
}

void CPU::PUSH(ByteRegisterPair& reg)
{
	--SP;
	writeByteAtAddress(SP.getWord(), (u8)(reg.getWord() >> 8));
	--SP;
	writeByteAtAddress(SP.getWord(), (u8)(reg.getWord() & 0x00FF));

	cycleCPU(4);
}

void CPU::POP(ByteRegisterPair& reg)
{
	reg.setWord(readWordFromSP());
	cycleCPU(3);
}

void CPU::STOP()
{
	//TODO
	IME = 0;
	cycleCPU(1);
}

void CPU::HALT()
{
	//TODO
	IME = 0;
	cycleCPU(1);
}

void CPU::DI()
{
	IME = 0;
	cycleCPU(1);
}

void CPU::EI()
{
	IME = 1;

	//IME_NEXT = 1;
	cycleCPU(1);
}

void CPU::rotateAccumulatorLeft()
{
	u8 value = a.getRegisterValue();
	u8 highBit = (value >> 7) & (u8)1;
	u8 oldCarryFlag = (u8)f.getCarryFlag();
	f.setCarryFlag(highBit);
	//rotate
	a.setRegisterValue((value << 1) | oldCarryFlag);

	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);
	
	f.setZeroFlag(false);
	
	cycleCPU(1);
}

void CPU::rotateAccumulatorLeftCircular()
{
	//check highest bit
	u8 value = a.getRegisterValue();
	u8 highBit = (value >> 7) & (u8)1;

	f.setCarryFlag(highBit);

	//rotate
	a.setRegisterValue((value << 1) | highBit);

	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);
	f.setZeroFlag(false);

	cycleCPU(1);
}

void CPU::rotateAccumulatorRight()
{
	u8 value = a.getRegisterValue();
	u8 lowBit = value & (u8)1;
	u8 oldCarryFlag = (u8)f.getCarryFlag();
	f.setCarryFlag(lowBit);
	//rotate
	a.setRegisterValue((value >> 1) | (oldCarryFlag << 7));

	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);
	f.setZeroFlag(false);

	cycleCPU(1);
}

void CPU::rotateAccumulatorRightCircular()
{
	//check lowest bit
	u8 value = a.getRegisterValue();
	u8 lowBit = value & (u8)1;

	f.setCarryFlag(lowBit);

	//rotate
	a.setRegisterValue((value >> 1) | (lowBit << 7));

	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);
	f.setZeroFlag(false);

	cycleCPU(1);
}

void CPU::rotateRegisterLeft(ByteRegister& reg)
{
	u8 value = reg.getRegisterValue();
	u8 shiftedBit = (value >> 7) & (u8)1;
	u8 oldCarryFlag = (u8)f.getCarryFlag();
	f.setCarryFlag(shiftedBit);
	//rotate
	reg.setRegisterValue((value << 1) | oldCarryFlag);

	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);
	if (reg.getRegisterValue() == 0) {
		f.setZeroFlag(true);
	}
	else {
		f.setZeroFlag(false);
	}
	cycleCPU(2);
}

void CPU::rotateRegisterLeftCircular(ByteRegister& reg)
{
	//check highest bit
	u8 value = reg.getRegisterValue();
	u8 highBit = (value >> 7) & (u8)1;
	
	f.setCarryFlag(highBit);
	
	//rotate
	reg.setRegisterValue((value << 1) | highBit);
	
	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);
	if (reg.getRegisterValue() == 0) {
		f.setZeroFlag(true);
	}
	else {
		f.setZeroFlag(false);
	}

	cycleCPU(2);
}

void CPU::rotateRegisterRight(ByteRegister& reg)
{
	u8 value = reg.getRegisterValue();
	u8 lowBit = value & (u8)1;

	u8 oldCarryFlag = (u8)f.getCarryFlag();
	f.setCarryFlag(lowBit);
	//rotate
	reg.setRegisterValue((value >> 1) | (oldCarryFlag << 7));

	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);
	if (reg.getRegisterValue() == 0) {
		f.setZeroFlag(true);
	}
	else {
		f.setZeroFlag(false);
	}
	cycleCPU(2);
}

void CPU::rotateRegisterRightCircular(ByteRegister& reg)
{
	//check lowest bit
	u8 value = reg.getRegisterValue();
	u8 lowBit = value & (u8)1;

	f.setCarryFlag(lowBit);

	//rotate
	reg.setRegisterValue((value >> 1) | (lowBit << 7));

	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);
	if (reg.getRegisterValue() == 0) {
		f.setZeroFlag(true);
	}
	else {
		f.setZeroFlag(false);
	}

	cycleCPU(2);
}

void CPU::rotateLeftIndirect()
{
	u8 value = readByteFromAddress(hlReg.getWord());
	u8 highBit = (value >> 7) & (u8)1;
	u8 oldCarryFlag = (u8)f.getCarryFlag();
	f.setCarryFlag(highBit);
	value = (value << 1) | oldCarryFlag;
	writeByteAtAddress(hlReg.getWord(), value);

	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);
	if (value == 0) {
		f.setZeroFlag(true);
	}
	else {
		f.setZeroFlag(false);
	}

	cycleCPU(4);
}

void CPU::rotateLeftCircularIndirect()
{

	//check highest bit
	u8 value = readByteFromAddress(hlReg.getWord());
	u8 highBit = (value >> 7) & (u8)1;

	f.setCarryFlag(highBit);

	//rotate
	value = (value << 1) | highBit;
	writeByteAtAddress(hlReg.getWord(), value);
	
	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);
	if (value == 0) {
		f.setZeroFlag(true);
	}
	else {
		f.setZeroFlag(false);
	}

	cycleCPU(4);
}

void CPU::rotateRightIndirect()
{
	u8 value = readByteFromAddress(hlReg.getWord());
	u8 lowBit = value & (u8)1;
	u8 oldCarryFlag = (u8)f.getCarryFlag();
	f.setCarryFlag(lowBit);
	//rotate
	value = (value >> 1) | (oldCarryFlag << 7);
	writeByteAtAddress(hlReg.getWord(), value);
	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);
	if (value == 0) {
		f.setZeroFlag(true);
	}
	else {
		f.setZeroFlag(false);
	}
	cycleCPU(4);
}

void CPU::rotateRightCircularIndirect()
{
	//check lowest bit
	u8 value = readByteFromAddress(hlReg.getWord());
	u8 lowBit = value & (u8)1;

	f.setCarryFlag(lowBit);

	//rotate
	value = (value >> 1) | (lowBit << 7);

	writeByteAtAddress(hlReg.getWord(), value);
	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);
	if (value == 0) {
		f.setZeroFlag(true);
	}
	else {
		f.setZeroFlag(false);
	}
	cycleCPU(4);

}

void CPU::shiftRegisterLeftArithmetic(ByteRegister& reg)
{
	u8 value = reg.getRegisterValue();
	u8 highBit = (value >> 7) & (u8)1;
	u8 lowBit = value & (u8)1;

	f.setCarryFlag(highBit);

	reg.setRegisterValue((value << 1) | lowBit);

	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);
	if (reg.getRegisterValue() == 0) {
		f.setZeroFlag(true);
	}
	else {
		f.setZeroFlag(false);
	}
	cycleCPU(2);
}

void CPU::shiftLeftArithmeticIndirect()
{
	u8 value = readByteFromAddress(hlReg.getWord());
	u8 highBit = (value >> 7) & (u8)1;
	u8 lowBit = value & (u8)1;

	f.setCarryFlag(highBit);
	value = (value << 1) | lowBit;
	writeByteAtAddress(hlReg.getWord(), value);
	
	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);
	if (value == 0) {
		f.setZeroFlag(true);
	}
	else {
		f.setZeroFlag(false);
	}
	cycleCPU(2);
}

void CPU::shiftRegisterRightArithmetic(ByteRegister& reg)
{
	u8 value = reg.getRegisterValue();
	u8 lowBit = value & (u8)1;
	u8 highBit = (value >> 7) & (u8)1 ;
	f.setCarryFlag(lowBit);

	reg.setRegisterValue((value >> 1)| (highBit << 7));

	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);
	if (reg.getRegisterValue() == 0) {
		f.setZeroFlag(true);
	}
	else {
		f.setZeroFlag(false);
	}
	cycleCPU(2);
}

void CPU::shiftRightArithmeticIndirect()
{
	u8 value = readByteFromAddress(hlReg.getWord());
	u8 lowBit = value & (u8)1;
	u8 highBit = (value >> 7) & (u8)1;
	f.setCarryFlag(lowBit);
	value = (value >> 1) | (highBit << 7);
	writeByteAtAddress(hlReg.getWord(),value);

	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);
	if (value == 0) {
		f.setZeroFlag(true);
	}
	else {
		f.setZeroFlag(false);
	}
	cycleCPU(2);

}

void CPU::shiftRegisterRightLogical(ByteRegister& reg)
{
	u8 value = reg.getRegisterValue();
	u8 shiftedBit = value & (u8)1;

	f.setCarryFlag(shiftedBit);
	
	reg.setRegisterValue((value >> 1));

	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);
	if (reg.getRegisterValue() == 0) {
		f.setZeroFlag(true);
	}
	else {
		f.setZeroFlag(false);
	}
	cycleCPU(2);
}

void CPU::shiftRightLogicalIndirect()
{
	u8 value = readByteFromAddress(hlReg.getWord());
	u8 shiftedBit = value & (u8)1;

	f.setCarryFlag(shiftedBit);
	value = value >> 1;

	writeByteAtAddress(hlReg.getWord(), value);

	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);
	if (value == 0) {
		f.setZeroFlag(true);
	}
	else {
		f.setZeroFlag(false);
	}
	cycleCPU(2);
}

void CPU::swapRegisterNibbles(ByteRegister& reg)
{
	u8 value = reg.getRegisterValue();
	value = ((value & 0xF0) >> 4) | (value << 4);
	reg.setRegisterValue(value);
	if (value == 0) {
		f.setZeroFlag(true);
	}
	else {
		f.setZeroFlag(false);
	}
	f.setCarryFlag(false);
	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);
	cycleCPU(2);
}

void CPU::swapNibblesIndirect()
{
	u8 value = readByteFromAddress(hlReg.getWord());
	value = ((value & 0xF0) >> 4) | (value << 4);
	writeByteAtAddress(hlReg.getWord(), value);
	if (value == 0) {
		f.setZeroFlag(true);
	}
	else {
		f.setZeroFlag(false);
	}
	f.setCarryFlag(false);
	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(false);
	cycleCPU(4);
}

void CPU::setRegisterBit(u8 bitNumber, ByteRegister& reg)
{
	assert(bitNumber < 8);


	u8 value = reg.getRegisterValue();
	reg.setRegisterValue(value | ((u8)1 << bitNumber));

	cycleCPU(2);
}

void CPU::resetRegisterBit(u8 bitNumber, ByteRegister& reg)
{
	assert(bitNumber < 8);
	u8 value = reg.getRegisterValue();

	reg.setRegisterValue(value & ~((u8)1 << bitNumber));

	cycleCPU(2);
}

void CPU::setBitIndirect(u8 bitNumber)
{
	assert(bitNumber < 8);
	u8 value = readByteFromAddress(hlReg.getWord());
	writeByteAtAddress(hlReg.getWord(), value | ((u8)1 << bitNumber));
	cycleCPU(4);
}

void CPU::resetBitIndirect(u8 bitNumber)
{
	assert(bitNumber < 8);
	u8 value = readByteFromAddress(hlReg.getWord());
	writeByteAtAddress(hlReg.getWord(), value & ~((u8)1 << bitNumber));
	cycleCPU(4);
}

void CPU::testRegisterBit(u8 bitNumber, ByteRegister& reg)
{
	assert(bitNumber < 8);
	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(true);
	f.setZeroFlag(!((reg.getRegisterValue() >> bitNumber) & (u8)1));
	cycleCPU(2);
}

void CPU::testBitIndirect(u8 bitNumber)
{
	assert(bitNumber < 8);
	f.setSubstractionFlag(false);
	f.setHalfCarryFlag(true);
	f.setZeroFlag(!((readByteFromAddress(hlReg.getWord()) >> bitNumber) & (u8)1));
	cycleCPU(3);
}

void CPU::executeCbOpcode()
{
	u8 cbOpcode = readByteFromPC();
	switch (cbOpcode)
	{
	case 0x00:
		//RLC B
		rotateRegisterLeftCircular(b);
		break;
	case 0x01:
		//RLC C
		rotateRegisterLeftCircular(c);
		break;
	case 0x02:
		//RLC D
		rotateRegisterLeftCircular(d);
		break;
	case 0x03:
		//RLC E
		rotateRegisterLeftCircular(e);
		break;
	case 0x04:
		//RLC H
		rotateRegisterLeftCircular(h);
		break;
	case 0x05:
		//RLC L
		rotateRegisterLeftCircular(l);
		break;
	case 0x06:
		//RLC (HL)
		rotateLeftCircularIndirect();
		break;
	case 0x07:
		//RLC A
		rotateRegisterLeftCircular(a);
		break;
	case 0x08:
		//RRC B
		rotateRegisterRightCircular(b);
		break;
	case 0x09:
		//RRC C
		rotateRegisterRightCircular(c);
		break;
	case 0x0A:
		//RRC D
		rotateRegisterRightCircular(d);
		break;
	case 0x0B:
		//RRC E
		rotateRegisterRightCircular(e);
		break;
	case 0x0C:
		//RRC H
		rotateRegisterRightCircular(h);
		break;
	case 0x0D:
		//RRC L
		rotateRegisterRightCircular(l);
		break;
	case 0x0E:
		//RRC (HL)
		rotateRightCircularIndirect();
		break;
	case 0x0F:
		//RRC A
		rotateRegisterRightCircular(a);
		break;
	case 0x10:
		//RL B
		rotateRegisterLeft(b);
		break;
	case 0x11:
		//RL C
		rotateRegisterLeft(c);
		break;
	case 0x12:
		//RL D
		rotateRegisterLeft(d);
		break;
	case 0x13:
		//RL E
		rotateRegisterLeft(e);
		break;
	case 0x14:
		//RL H
		rotateRegisterLeft(h);
		break;
	case 0x15:
		//RL L
		rotateRegisterLeft(l);
		break;
	case 0x16:
		//RL (HL)
		rotateLeftIndirect();
		break;
	case 0x17:
		//RL A
		rotateRegisterLeft(a);
		break;
	case 0x18:
		//RR B
		rotateRegisterRight(b);
		break;
	case 0x19:
		//RR C
		rotateRegisterRight(c);
		break;
	case 0x1A:
		//RR D
		rotateRegisterRight(d);
		break;
	case 0x1B:
		//RR E
		rotateRegisterRight(e);
		break;
	case 0x1C:
		//RR H
		rotateRegisterRight(h);
		break;
	case 0x1D:
		//RR L
		rotateRegisterRight(l);
		break;
	case 0x1E:
		rotateRightIndirect();
		break;
	case 0x1F:
		//RR A
		rotateRegisterRight(a);
		break;
	case 0x20:
		//SLA B
		shiftRegisterLeftArithmetic(b);
		break;
	case 0x21:
		//SLA C
		shiftRegisterLeftArithmetic(c);
		break;
	case 0x22:
		//SLA D
		shiftRegisterLeftArithmetic(d);
		break;
	case 0x23:
		//SLA E
		shiftRegisterLeftArithmetic(e);
		break;
	case 0x24:
		//SLA H
		shiftRegisterLeftArithmetic(h);
		break;
	case 0x25:
		//SLA L
		shiftRegisterLeftArithmetic(l);
		break;
	case 0x26:
		//SLA (HL)
		shiftLeftArithmeticIndirect();
		break;
	case 0x27:
		//SLA A
		shiftRegisterLeftArithmetic(a);
		break;
	case 0x28:
		//SRA B
		shiftRegisterRightArithmetic(b);
		break;
	case 0x29:
		//SRA C
		shiftRegisterRightArithmetic(c);
		break;
	case 0x2A:
		//SRA D
		shiftRegisterRightArithmetic(d);
		break;
	case 0x2B:
		//SRA E
		shiftRegisterRightArithmetic(e);
		break;
	case 0x2C: //SRA H
		shiftRegisterRightArithmetic(h);
		break;
	case 0x2D: //SRA L
		shiftRegisterRightArithmetic(l);
		break;
	case 0x2E: //SRA (HL)
		shiftRightArithmeticIndirect();
		break;
	case 0x2F: //SRA A
		shiftRegisterRightArithmetic(a);
		break;		
	case 0x30: //SWAP B
		swapRegisterNibbles(b);
		break;
	case 0x31: //SWAP C
		swapRegisterNibbles(c);
		break;
	case 0x32: //SWAP D
		swapRegisterNibbles(d);
		break;
	case 0x33: //SWAP E
		swapRegisterNibbles(e);
		break;
	case 0x34: //SWAP H
		swapRegisterNibbles(h);
		break;
	case 0x35: //SWAP L
		swapRegisterNibbles(l);
		break;
	case 0x36: //SWAP (HL)
		swapNibblesIndirect();
		break;
	case 0x37: //SWAP A
		swapRegisterNibbles(a);
		break;
	case 0x38: //SRL B
		shiftRegisterRightLogical(b);
		break;
	case 0x39: //SRL C
		shiftRegisterRightLogical(c);
		break;
	case 0x3A: //SRL D
		shiftRegisterRightLogical(d);
		break;
	case 0x3B: //SRL E
		shiftRegisterRightLogical(e);
		break;
	case 0x3C: //SRL H
		shiftRegisterRightLogical(h);
		break;
	case 0x3D: //SRL L
		shiftRegisterRightLogical(l);
		break;
	case 0x3E: //SRL (HL)
		shiftRightLogicalIndirect();
		break;
	case 0x3F: //SRL A
		shiftRegisterRightLogical(a);
		break;
	case 0x40: //BIT 0, B
		testRegisterBit(0, b);
		break;
	case 0x41: //BIT 0, C
		testRegisterBit(0, c);
		break;
	case 0x42: //BIT 0, D
		testRegisterBit(0, d);
		break;
	case 0x43: //BIT 0, E
		testRegisterBit(0, e);
		break;
	case 0x44: //BIT 0, H
		testRegisterBit(0, h);
		break;
	case 0x45: //BIT 0, L
		testRegisterBit(0, l);
		break;
	case 0x46: //BIT 0, (HL)
		testBitIndirect(0);
		break;
	case 0x47: //BIT 0, A
		testRegisterBit(0, a);
		break;
	case 0x48: //BIT 1, B
		testRegisterBit(1, b);
		break;
	case 0x49: //BIT 1, C
		testRegisterBit(1, c);
		break;
	case 0x4A: //BIT 1, D
		testRegisterBit(1, d);
		break;
	case 0x4B: //BIT 1, E
		testRegisterBit(1, e);
		break;
	case 0x4C: //BIT 1, H
		testRegisterBit(1, h);
		break;
	case 0x4D: //BIT 1, L
		testRegisterBit(1, l);
		break;
	case 0x4E: //BIT 1, (HL)
		testBitIndirect(1);
		break;
	case 0x4F: //BIT 1, A
		testRegisterBit(1, a);
		break;
	case 0x50: //BIT 2, B
		testRegisterBit(2, b);
		break;
	case 0x51: //BIT 2, C
		testRegisterBit(2, c);
		break;
	case 0x52: //BIT 2, D
		testRegisterBit(2, d);
		break;
	case 0x53: //BIT 2, E
		testRegisterBit(2, e);
		break;
	case 0x54: //BIT 2, H
		testRegisterBit(2, h);
		break;
	case 0x55: //BIT 2, L
		testRegisterBit(2, l);
		break;
	case 0x56: //BIT 2, (HL)
		testBitIndirect(2);
		break;
	case 0x57: //BIT 2, A
		testRegisterBit(2, a);
		break;
	case 0x58: //BIT 3, B
		testRegisterBit(3, b);
		break;
	case 0x59: //BIT 3, C
		testRegisterBit(3, c);
		break;
	case 0x5A: //BIT 3, D
		testRegisterBit(3, d);
		break;
	case 0x5B: //BIT 3, E
		testRegisterBit(3, e);
		break;
	case 0x5C: //BIT 3, H
		testRegisterBit(3, h);
		break;
	case 0x5D: //BIT 3, L
		testRegisterBit(3, l);
		break;
	case 0x5E: //BIT 3, (HL)
		testBitIndirect(3);
		break;
	case 0x5F: //BIT 3, A
		testRegisterBit(3, a);
		break;
	case 0x60: //BIT 4, B
		testRegisterBit(4, b);
		break;
	case 0x61: //BIT 4, C
		testRegisterBit(4, c);
		break;
	case 0x62: //BIT 4, D
		testRegisterBit(4, d);
		break;
	case 0x63: //BIT 4, E
		testRegisterBit(4, e);
		break; 
	case 0x64: //BIT 4, H
		testRegisterBit(4, h);
		break;
	case 0x65: //BIT 4, L
		testRegisterBit(4, l);
		break;
	case 0x66: //BIT 4, (HL)
		testBitIndirect(4);
		break;
	case 0x67: //BIT 4, A
		testRegisterBit(4, a);
		break;
	case 0x68: //BIT 5, B
		testRegisterBit(5, b);
		break;
	case 0x69: //BIT 5, C
		testRegisterBit(5, c);
		break;
	case 0x6A: //BIT 5, d
		testRegisterBit(5, d);
		break;
	case 0x6B: //BIT 5, E
		testRegisterBit(5, e);
		break;
	case 0x6C: //BIT 5, H
		testRegisterBit(5, h);
		break;
	case 0x6D: //BIT 5, L
		testRegisterBit(5, l);
		break;
	case 0x6E:  //BIT 5, (HL)
		testBitIndirect(5);
		break;
	case 0x6F: //BIT 5, A
		testRegisterBit(5, a);
		break;
	case 0x70: //BIT 6, B
		testRegisterBit(6, b);
		break;
	case 0x71: //BIT 6, C
		testRegisterBit(6, c);
		break;
	case 0x72: //BIT 6, D
		testRegisterBit(6, d);
		break;
	case 0x73: //BIT 6, E
		testRegisterBit(6, e);
		break;
	case 0x74: //BIT 6, H
		testRegisterBit(6, h);
		break;
	case 0x75: //BIT 6, L
		testRegisterBit(6, l);
		break;
	case 0x76: //BIT 6, (HL)
		testBitIndirect(6);
		break;
	case 0x77: //BIT 6, A
		testRegisterBit(6, a);
		break;
	case 0x78: //BIT 7, B
		testRegisterBit(7, b);
		break;
	case 0x79: //BIT 7, C
		testRegisterBit(7, c);
		break;
	case 0x7A: //BIT 7, D
		testRegisterBit(7, d);
		break;
	case 0x7B: //BIT 7, E
		testRegisterBit(7, e);
		break;
	case 0x7C: //BIT 7, L
		testRegisterBit(7, l);
		break;
	case 0x7D: //BIT 7, H
		testRegisterBit(7, h);
		break;
	case 0x7E: //BIT 7, (HL)
		testBitIndirect(7);
		break;
	case 0x7F: //BIT 7, A
		testRegisterBit(7, a);
		break;
	case 0x80: //RES 0, B
		resetRegisterBit(0, b);
		break;
	case 0x81: //RES 0, C
		resetRegisterBit(0, c);
		break;
	case 0x82: //RES 0, D
		resetRegisterBit(0, d);
		break;
	case 0x83: //RES 0, E
		resetRegisterBit(0, e);
		break;
	case 0x84: //RES 0, H
		resetRegisterBit(0, h);
		break;
	case 0x85: //RES 0, L
		resetRegisterBit(0, l);
		break;
	case 0x86: //RES 0, (HL)
		resetBitIndirect(0);
		break;
	case 0x87: //RES 0, A
		resetRegisterBit(0, a);
		break;
	case 0x88: //RES 1, B
		resetRegisterBit(1, b);
		break;
	case 0x89: //RES 1, C
		resetRegisterBit(1, c);
		break;
	case 0x8A: //RES 1, D
		resetRegisterBit(1, d);
		break;
	case 0x8B: //RES 1, E
		resetRegisterBit(1, e);
		break;
	case 0x8C: //RES 1, h
		resetRegisterBit(1, h);
		break;
	case 0x8D: //RES 1, L
		resetRegisterBit(1, l);
		break;
	case 0x8E: //RES 1, (HL)
		resetBitIndirect(1);
		break;
	case 0x8F: //RES 1, A
		resetRegisterBit(1, a);
		break;
	case 0x90: //RES 2, B
		resetRegisterBit(2, b);
		break;
	case 0x91: //RES 2, C
		resetRegisterBit(2, c);
		break;
	case 0x92: //RES 2, D
		resetRegisterBit(2, d);
		break;
	case 0x93: //RES 2, e
		resetRegisterBit(2, e);
		break;
	case 0x94: //RES 2, H
		resetRegisterBit(2, h);
		break;
	case 0x95: //RES 2, L
		resetRegisterBit(2, l);
		break;
	case 0x96: //RES 2, (HL)
		resetBitIndirect(2);
		break;
	case 0x97: //RES 2, A
		resetRegisterBit(2, a);
		break;
	case 0x98: //RES 3, B
		resetRegisterBit(3, b);
		break;
	case 0x99: //RES 3, C
		resetRegisterBit(3, c);
		break;
	case 0x9A: //RES 3, D
		resetRegisterBit(3, d);
		break;
	case 0x9B: //RES 3, E
		resetRegisterBit(3, e);
		break;
	case 0x9C: //RES 3, H
		resetRegisterBit(3, h);
		break;
	case 0x9D: //RES 3, L
		resetRegisterBit(3, l);
		break;
	case 0x9E: //RES 3, (HL)
		resetBitIndirect(3);
		break;
	case 0x9F: //RES 3, A
		resetRegisterBit(3, a);
		break;
	case 0xA0: //RES 4, B
		resetRegisterBit(4, b);
		break;
	case 0xA1: //RES 4, C
		resetRegisterBit(4, c);
		break;
	case 0xA2: //RES 4, D
		resetRegisterBit(4, d);
		break;
	case 0xA3: //RES 4, E
		resetRegisterBit(4, e);
		break;
	case 0xA4: //RES 4, H
		resetRegisterBit(4, h);
		break;
	case 0xA5: //RES 4, L
		resetRegisterBit(4, l);
		break;
	case 0xA6: //RES 4, (HL)
		resetBitIndirect(4);
		break;
	case 0xA7: //RES 4, A
		resetRegisterBit(4, a);
		break;
	case 0xA8: //RES 5, B
		resetRegisterBit(5, b);
		break;
	case 0xA9: //RES 5, C
		resetRegisterBit(5, c);
		break;
	case 0xAA: //RES 5, D
		resetRegisterBit(5, d);
		break;
	case 0xAB: //RES 5, E
		resetRegisterBit(5, e);
		break;
	case 0xAC: //RES 5, H
		resetRegisterBit(5, h);
		break;
	case 0xAD: //RES 5, L
		resetRegisterBit(5, l);
		break;
	case 0xAE: //RES 5, (HL)
		resetBitIndirect(5);
		break;
	case 0xAF: //RES 5, A
		resetRegisterBit(5, a);
		break;
	case 0xB0: //RES 6, B
		resetRegisterBit(6, b);
		break;
	case 0xB1: //RES 6, C
		resetRegisterBit(6, c);
		break;
	case 0xB2: //RES 6, D
		resetRegisterBit(6, d);
		break;
	case 0xB3: //RES 6, E
		resetRegisterBit(6, e);
		break;
	case 0xB4: //RES 6, H
		resetRegisterBit(6, h);
		break;
	case 0xB5: //RES 6, L
		resetRegisterBit(6, l);
		break;
	case 0xB6: //RES 6, (HL)
		resetBitIndirect(6);
		break;
	case 0xB7: //RES 6, A
		resetRegisterBit(6, a);
		break;
	case 0xB8: //RES 7, B
		resetRegisterBit(7, b);
		break;
	case 0xB9: //RES 7, C
		resetRegisterBit(7, c);
		break;
	case 0xBA: //RES 7, D
		resetRegisterBit(7, d);
		break;
	case 0xBB: //RES 7, E
		resetRegisterBit(7, e);
		break;
	case 0xBC: //RES 7, H
		resetRegisterBit(7, h);
		break;
	case 0xBD: //RES 7, L
		resetRegisterBit(7, l);
		break;
	case 0xBE: //RES 7, (HL)
		resetBitIndirect(7);
		break;
	case 0xBF: //RES 7, A
		resetRegisterBit(7, a);
		break;
	case 0xC0: //SET 0, B
		setRegisterBit(0, b);
		break;
	case 0xC1: //SET 0, C
		setRegisterBit(0, c);
		break;
	case 0xC2: //SET 0, D
		setRegisterBit(0, d);
		break;
	case 0xC3: //SET 0, E
		setRegisterBit(0, e);
		break;
	case 0xC4: //SET 0, H
		setRegisterBit(0, h);
		break;
	case 0xC5: //SET 0, L
		setRegisterBit(0, l);
		break;
	case 0xC6: //SET 0, (HL)
		setBitIndirect(0);
		break;
	case 0xC7: //SET 0, A
		setRegisterBit(0, a);
		break;
	case 0xC8: //SET 1, B
		setRegisterBit(1, b);
		break;
	case 0xC9: //SET 1, C
		setRegisterBit(1, c);
		break;
	case 0xCA: //SET 1, D
		setRegisterBit(1, d);
		break;
	case 0xCB: //SET 1, E
		setRegisterBit(1, e);
		break;
	case 0xCC: //SET 1, H
		setRegisterBit(1, h);
		break;
	case 0xCD: //SET 1, L
		setRegisterBit(1, l);
		break;
	case 0xCE: //SET 1, (HL)
		setBitIndirect(1);
		break;
	case 0xCF: //SET 1, A
		setRegisterBit(1, a);
		break;
	case 0xD0: //SET 2, B
		setRegisterBit(2, b);
		break;
	case 0xD1: //SET 2, C
		setRegisterBit(2, c);
		break;
	case 0xD2: //SET 2, D
		setRegisterBit(2, d);
		break;
	case 0xD3: //SET 2, E
		setRegisterBit(2, e);
		break;
	case 0xD4: //SET 2, H
		setRegisterBit(2, h);
		break;
	case 0xD5: //SET 2, L
		setRegisterBit(2, l);
		break;
	case 0xD6: //SET 2, (HL)
		setBitIndirect(2);
		break;
	case 0xD7: //SET 2, A
		setRegisterBit(2, a);
		break;
	case 0xD8: //SET 3, B
		setRegisterBit(3, b);
		break;
	case 0xD9: //SET 3, C
		setRegisterBit(3, c);
		break;
	case 0xDA: //SET 3, D
		setRegisterBit(3, d);
		break;
	case 0xDB: //SET 3, E
		setRegisterBit(3, e);
		break;
	case 0xDC: //SET 3, H
		setRegisterBit(3, h);
		break;
	case 0xDD: //SET 3, L
		setRegisterBit(3, l);
		break;
	case 0xDE: //SET 3, (HL)
		setBitIndirect(3);
		break;
	case 0xDF: //SET 3, A
		setRegisterBit(3, a);
		break;
	case 0xE0: //SET 4, B
		setRegisterBit(4, b);
		break;
	case 0xE1: //SET 4, C
		setRegisterBit(4, c);
		break;
	case 0xE2: //SET 4, D
		setRegisterBit(4, d);
		break;
	case 0xE3: //SET 4, E
		setRegisterBit(4, e);
		break;
	case 0xE4: //SET 4, H
		setRegisterBit(4, h);
		break;
	case 0xE5: //SET 4, L
		setRegisterBit(4, l);
		break;
	case 0xE6: //SET 4, (HL)
		setBitIndirect(4);
		break;
	case 0xE7: //SET 4, A
		setRegisterBit(4, a);
		break;
	case 0xE8: //SET 5, B
		setRegisterBit(5, b);
		break;
	case 0xE9: //SET 5, C
		setRegisterBit(5, c);
		break;
	case 0xEA: //SET 5, D
		setRegisterBit(5, d);
		break;
	case 0xEB: //SET 5, E
		setRegisterBit(5, e);
		break;
	case 0xEC: //SET 5, H
		setRegisterBit(5, h);
		break;
	case 0xED: //SET 5, L
		setRegisterBit(5, l);
		break;
	case 0xEE: //SET 5, (HL)
		setBitIndirect(5);
		break;
	case 0xEF: //SET 5, A
		setRegisterBit(5, a);
		break;
	case 0xF0: //SET 6, B
		setRegisterBit(6, b);
		break;
	case 0xF1: //SET 6, C
		setRegisterBit(6, c);
		break;
	case 0xF2: //SET 6, D
		setRegisterBit(6, d);
		break;
	case 0xF3: //SET 6, E
		setRegisterBit(6, e);
		break;
	case 0xF4: //SET 6, H
		setRegisterBit(6, h);
		break;
	case 0xF5: //SET 6, L
		setRegisterBit(6, l);
		break;
	case 0xF6: //SET 6, (HL)
		setBitIndirect(6);
		break;
	case 0xF7: //SET 6, A
		setRegisterBit(6, a);
		break;
	case 0xF8: //SET 7, B
		setRegisterBit(7, b);
		break;
	case 0xF9: //SET 7, C
		setRegisterBit(7, c);
		break;
	case 0xFA: //SET 7, D
		setRegisterBit(7, d);
		break;
	case 0xFB: //SET 7, E
		setRegisterBit(7, e);
		break;
	case 0xFC: //SET 7, H
		setRegisterBit(7, h);
		break;
	case 0xFD: //SET 7, L
		setRegisterBit(7, l);
		break;
	case 0xFE: //SET 7, (HL)
		setBitIndirect(7);
		break;
	case 0xFF: //SET 7, A
		setRegisterBit(7, a);
		break;
	default:
		break;
	}
}

