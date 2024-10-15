#include "cpu.h"
#include "emu.h"


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

void CPU::executeCbOpcode()
{
}

