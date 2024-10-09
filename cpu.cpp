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
	programCounter = 0x100;
	stackPointer = 0;
	IR = 0;
	IE = 0;
	currentOpcode = 0;
}

void CPU::fetchOpcode()
{
	currentOpcode = gbEmu.addressBus.busRead(programCounter);
	programCounter++;
}

void CPU::executeCurrentOpcode()
{
	printf("PC: %X", programCounter);
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
		//opcodeINC_BC();
		break;
	case 0x03:
		programCounter++;
		break;
	case 0x06:
		//LD B N8
		LD_R8_N8(bcReg.getHighByteRegister());
		break;	
	case 0x08:
		programCounter += 3;
		break;
	case 0x0B:
		programCounter++;
		break;
	case 0x0C:
		programCounter++;
		break;
	case 0x0D:
		programCounter++;
		break;
	case 0x0E:
		//LD C N8
		LD_R8_N8(bcReg.getLowByteRegister());
		break;
	case 0x11:
		//LD DE N16
		LD_R16_N16(deReg);
		break;
	case 0x21:
		//LD HL N16
		LD_R16_N16(hlReg);
		break;
	case 0x22:
		//LD [HL+] A
		LD_R16_INC_A(hlReg);
		break;
	case 0x31:
		//LD HL N16
		//LD_R16_N16(registers[SP]);
		break;
	case 0x32:
		//LD [HL-] A
		LD_R16_DEC_A(hlReg);
		break;
	case 0x66:
		programCounter++;
		break;
	case 0x73:
		programCounter++;
		break;
	case 0x83:
		programCounter++;
		break;
	case 0x88:
		programCounter++;
		break;
	case 0x89:
		programCounter++;
		break;
	case 0xAF:
		//XOR A
		XOR_R8(afReg.getHighByteRegister());
		break;
	case 0xC3:
		opcodeJP_A16();

		break;
	case 0xCC:
		programCounter += 3;
		break;
	case 0xCE:
		programCounter += 2;
		break;
	case 0xDC:
		programCounter += 2;
		break;
	case 0xDD:
		programCounter += 2;
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
	u16 lo = gbEmu.addressBus.busRead(programCounter);
	u16 hi = gbEmu.addressBus.busRead(programCounter + 1);
	programCounter += 2;
	return lo | (hi << 8);
}

u8 CPU::readByteFromPC()
{
	u8 value = gbEmu.addressBus.busRead(programCounter);
	programCounter++;
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
	programCounter = readWordFromPC();
	cycleCPU(1);
}


void CPU::opcodeLD_HLD_A()
{
	gbEmu.addressBus.busWrite(hlReg.getWord(), afReg.getHighByteRegister().getRegisterValue());
	++hlReg;
}

void CPU::INC_R8(ByteRegister& reg)
{
	u8 oldValue = reg.getRegisterValue();
	reg.setRegisterValue(oldValue + 1);

	if (reg.getRegisterValue() == 0)
	{
		f.setZeroFlag(true);
	}

	if ((1 + (oldValue & 0xf) & 0x10) == 0x10)
	{
		f.setHalfCarryFlag(true);
	}

	f.setSubstractionFlag(false);
	cycleCPU(1);
}

void CPU::INC_R16(WordRegister& reg)
{
	++reg;
	cycleCPU(2);
}

void CPU::INC_R16_INDIRECT(WordRegister& reg)
{
	u8 oldValue = readByteFromAddress(reg.getWord());
	u8 result = oldValue + 1;
	
	writeByteAtAddress(reg.getWord(), result);

	if (result == 0)
	{
		f.setZeroFlag(true);
	}

	if ((1 + (oldValue & 0xf) & 0x10) == 0x10)
	{
		f.setHalfCarryFlag(true);
	}

	f.setSubstractionFlag(false);
	cycleCPU(3);

}

void CPU::DEC_R8(ByteRegister& reg)
{
	u8 oldValue = reg.getRegisterValue();
	reg.setRegisterValue(oldValue - 1);

	if (reg.getRegisterValue() == 0)
	{
		f.setZeroFlag(true);
	}

	if ((1 + (oldValue & 0xf) & 0x10) == 0x10)
	{
		f.setHalfCarryFlag(true);
	}

	f.setSubstractionFlag(true);
	cycleCPU(1);
}

void CPU::DEC_R16(WordRegister& reg)
{
	--reg;
	cycleCPU(2);
}

void CPU::DEC_R16_INDIRECT(WordRegister& reg)
{
	u8 oldValue = readByteFromAddress(reg.getWord());
	u8 result = oldValue - 1;

	writeByteAtAddress(reg.getWord(), result);

	if (result == 0)
	{
		f.setZeroFlag(true);
	}

	if ((1 + (oldValue & 0xf) & 0x10) == 0x10)
	{
		f.setHalfCarryFlag(true);
	}

	f.setSubstractionFlag(true);
	cycleCPU(3);
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

void CPU::LD_R16_R8(WordRegister& reg, ByteRegister& reg2)
{
	gbEmu.addressBus.busWrite(reg.getWord(), reg2.getRegisterValue());
	cycleCPU(2);
}

void CPU::LD_R16_INC_A(WordRegister& reg)
{
	gbEmu.addressBus.busWrite(reg.getWord(), afReg.getHighByteRegister().getRegisterValue());
	++reg;
	cycleCPU(2);
}

void CPU::LD_R16_DEC_A(WordRegister& reg)
{
	gbEmu.addressBus.busWrite(reg.getWord(), afReg.getHighByteRegister().getRegisterValue());
	--reg;
	cycleCPU(2);
}

void CPU::LD_R16_N8(WordRegister& reg)
{
	gbEmu.addressBus.busWrite(reg.getWord(), readByteFromPC());
	cycleCPU(3);
}

void CPU::LD_R8_R16(ByteRegister& reg1, WordRegister& reg2)
{
	reg1.setRegisterValue(readByteFromAddress(reg2.getWord()));
	cycleCPU(2);
}

void CPU::LD_R8_R16_INC(ByteRegister& reg1, WordRegister& reg2)
{
	reg1.setRegisterValue(readByteFromAddress(reg2.getWord()));
	++reg2;
	cycleCPU(2);
}

void CPU::LD_R8_R16_DEC(ByteRegister& reg1, WordRegister& reg2)
{
	reg1.setRegisterValue(readByteFromAddress(reg2.getWord()));
	--reg2;
	cycleCPU(2);
}


u8 CPU::ADD_8BIT(u8 oper1, u8 oper2)
{
	u8 result = oper1 + oper2;

	if (result == 0)
	{
		f.setZeroFlag(true);
	}

	if (((oper2 & 0xf) + (oper1 & 0xf) & 0x10) == 0x10)
	{
		f.setHalfCarryFlag(true);
	}

	if (((oper2 & 0xff) + (oper1 & 0xff)) > 0xFF)
	{
		f.setCarryFlag(true);
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
	cycleCPU(1);
}

u8 CPU::SUB_8BIT(u8 oper1, u8 oper2)
{
	u8 result = oper1 - oper2;
	
	if (result == 0)
	{
		f.setZeroFlag(true);
	}

	if (((oper2 & 0xf) + (oper1 & 0xf) & 0x10) == 0x10)
	{
		f.setHalfCarryFlag(true);
	}

	if (((oper2 & 0xff) + (oper1 & 0xff)) > 0xFF)
	{
		f.setCarryFlag(true);
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
	cycleCPU(1);
}

u8 CPU::AND_8BIT(u8 oper1, u8 oper2)
{
	u8 result = oper1 & oper2;
	if (result == 0)
	{
		f.setZeroFlag(true);
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

u8 CPU::OR_8BIT(u8 oper1, u8 oper2)
{
	u8 result = oper1 | oper2;
	if (result == 0)
	{
		f.setZeroFlag(true);
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

u8 CPU::XOR_8BIT(u8 oper1, u8 oper2)
{
	u8 result = oper1 ^ oper2;

	if (result == 0)
	{
		f.setZeroFlag(true);
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

