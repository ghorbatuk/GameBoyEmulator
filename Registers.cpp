#include "Registers.h"

ByteRegister::ByteRegister()
{
	value = 0;
}

u8 ByteRegister::getRegisterValue()
{
	return value;
}
void ByteRegister::setRegisterValue(u8 value)
{
	this->value = value;
}


WordRegister::WordRegister(ByteRegister& low, ByteRegister& high):
	lowByte(low),
	highByte(high)
{
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
	highByte.setRegisterValue(value);
}

void WordRegister::setLowByteValue(u8 value)
{
	lowByte.setRegisterValue(value);
}

void  WordRegister::setWord(u16 value)
{
	lowByte.setRegisterValue((u8) value);
	highByte.setRegisterValue((u8)(value >> 8));
}

WordRegister& WordRegister::operator++()
{
	u16 wordValue = getWord();
	setWord(++wordValue);
	return *this;
}

WordRegister& WordRegister::operator--()
{
	u16 wordValue = getWord();
	setWord(--wordValue);
	return *this;
}

void FlagsRegister::setCarryFlag(bool flagValue)
{
	if (flagValue)
	{
		value = value | ((u8)1 << c);
	}
	else {
		value = value & ~((u8)1 << c);
	}
}

void FlagsRegister::setHalfCarryFlag(bool flagValue)
{
	if (flagValue)
	{
		value = value | ((u8)1 << h);
	}
	else {
		value = value & ~((u8)1 << h);
	}
}

void FlagsRegister::setSubstractionFlag(bool flagValue)
{
	if (flagValue)
	{
		value = value | ((u8)1 << n);
	}
	else {
		value = value & ~((u8)1 << n);
	}
}

void FlagsRegister::setZeroFlag(bool flagValue)
{
	if (flagValue)
	{
		value = value | ((u8)1 << z);
	}
	else {
		value = value & ~((u8)1 << z);
	}
}

bool FlagsRegister::getCarryFlag()
{
	return (value >> c) & (u8)1;
}

bool FlagsRegister::getHalfCarryFlag()
{
	return (value >> h) & (u8)1;;
}

bool FlagsRegister::getSubstractionFlag()
{
	return (value >> n) & (u8)1;;
}

bool FlagsRegister::getZeroFlag()
{
	return (value >> z) & (u8)1;;
}
