#pragma once
#include "Common.h"

class ByteRegister
{
public:
	u8 getRegisterValue();
	void setRegisterValue(u8 value);
private:
	u8 value;
};

enum Registers {
	AF,
	BC,
	DE,
	HL,
	SP,
	PC
};

class WordRegister
{
public:
	WordRegister();
	//u8 getHighByteValue();
	//u8 getLowByteValue();
	u16 getWord();
	void setHighByteValue(u8 value);
	void setLowByteValue(u8 value);
	void setWord(u16 value);
	WordRegister& operator++();
	ByteRegister& getLowByteRegister();
	ByteRegister& getHighByteRegister();
private:
	ByteRegister lowByte;
	ByteRegister highByte;
	//u16 registerData;

};

class FlagsRegister : public ByteRegister{

};