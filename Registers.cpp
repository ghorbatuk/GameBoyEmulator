#include "Registers.h"

ByteRegister::ByteRegister()
{
	value = 0;
}

u8 ByteRegister::getRegisterValue() const
{
	return value;
}
void ByteRegister::setRegisterValue(u8 value)
{
	this->value = value;
}

ByteRegister& ByteRegister::operator++()
{
	value++;
	return *this;
}


ByteRegisterPair::ByteRegisterPair(ByteRegister& low, ByteRegister& high):
	lowByte(low),
	highByte(high)
{
}

ByteRegister& ByteRegisterPair::getLowByteRegister()
{
	return lowByte;
}
ByteRegister& ByteRegisterPair::getHighByteRegister()
{
	return highByte;
}

u16 ByteRegisterPair::getWord() const
{
	return (u16)lowByte.getRegisterValue() | (u16)(highByte.getRegisterValue() << 8);
}

void ByteRegisterPair::setHighByteValue(u8 value)
{
	highByte.setRegisterValue(value);
}

void ByteRegisterPair::setLowByteValue(u8 value)
{
	lowByte.setRegisterValue(value);
}

void  ByteRegisterPair::setWord(u16 value)
{
	lowByte.setRegisterValue((u8) value);
	highByte.setRegisterValue((u8)(value >> 8));
}

ByteRegisterPair& ByteRegisterPair::operator++()
{
	u16 wordValue = getWord();
	setWord(++wordValue);
	return *this;
}

ByteRegisterPair& ByteRegisterPair::operator--()
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

WordRegister::WordRegister()
{
	value = 0;
}

u16 WordRegister::getWord() const
{
	return value;
}

void WordRegister::setWord(u16 value)
{
	this->value = value;
}

WordRegister& WordRegister::operator++()
{
	value++;
	return *this;
}

WordRegister& WordRegister::operator--()
{
	value--;
	return *this;
}

WordRegister& WordRegister::operator+=(const u16 val)
{
	value += val;
	return *this;
}
