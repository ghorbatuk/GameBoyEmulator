#include "cpu.h"
#include "emu.h"


CPU::CPU(emu& gbEmu) :
	gbEmu(gbEmu),
	registers(new WordRegister[6])
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
	registers[AF].setWord(0);
	registers[BC].setWord(0);
	registers[DE].setWord(0);
	registers[HL].setWord(0);
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
		//opcodeLD_BC_A();
		break;
	case 0x02:
		//opcodeINC_BC();
		break;
	case 0x03:
		programCounter++;
		break;
	case 0x06:
		//LD B N8
		LD_R8_N8(registers[BC].getHighByteRegister());
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
		LD_R8_N8(registers[BC].getLowByteRegister());
		break;
	case 0x21:
		//LD HL N16
		LD_R16_N16(registers[HL]);
		break;
	case 0x32:
		opcodeLD_HLD_A();
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
		XOR_R8(registers[AF].getHighByteRegister());
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
	gbEmu.addressBus.busWrite(registers[HL].getWord(), registers[AF].getHighByteRegister().getRegisterValue());
	++registers[HL];
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


void CPU::XOR_R8(ByteRegister& reg)
{
	u8 valueA = registers[AF].getHighByteRegister().getRegisterValue();
	u8 valueR = reg.getRegisterValue();

	registers[AF].getHighByteRegister().setRegisterValue(valueA ^ valueR);

	if (registers[AF].getHighByteRegister().getRegisterValue() == 0)
	{
		registers[AF].getLowByteRegister().setRegisterValue(0x8); //set z flag to 1 and n,h,c flags to 0
	}
	cycleCPU(1);
}

