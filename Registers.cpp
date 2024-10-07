#include "Registers.h"

u8 ByteRegister::getRegisterValue() 
{
	return value;
}
void ByteRegister::setRegisterValue(u8 value)
{
	this->value = value;
}



WordRegister::WordRegister()
{
	lowByte.setRegisterValue(0);
	highByte.setRegisterValue(0);
}


ByteRegister& WordRegister::getLowByteRegister()
{
	return lowByte;
}
ByteRegister& WordRegister::getHighByteRegister()
{
	return highByte;
}

u16 WordRegister::getWord()
{
	return (u16)lowByte.getRegisterValue() | (u16)(highByte.getRegisterValue() << 8);
}

void WordRegister::setHighByteValue(u8 value)
{
	//registerData = (registerData & 0x00FF) | ((u16)value << 8);
	highByte.setRegisterValue(value);
}

void WordRegister::setLowByteValue(u8 value)
{
	lowByte.setRegisterValue(value);
	//registerData = (registerData & 0xFF00) | (u16)value;
}

void  WordRegister::setWord(u16 value)
{
	lowByte.setRegisterValue((u8) value);
	highByte.setRegisterValue((u8)(value >> 8));
	//registerData = value;
}

WordRegister& WordRegister::operator++()
{
	u16 wordValue = getWord();
	setWord(++wordValue);
	return *this;
}
