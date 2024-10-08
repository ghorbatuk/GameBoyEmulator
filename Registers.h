#pragma once
#include "Common.h"

class ByteRegister
{
public:
	ByteRegister();
	ByteRegister(const ByteRegister& reg) = delete;
	auto operator=(const ByteRegister&)->ByteRegister & = delete;
	u8 getRegisterValue();
	void setRegisterValue(u8 value);
protected:
	u8 value;
};



class WordRegister
{
public:
	
	WordRegister(ByteRegister&, ByteRegister&);
	WordRegister(const WordRegister& reg) = delete;
	auto operator=(const WordRegister&)->WordRegister & = delete;
	//u8 getHighByteValue();
	//u8 getLowByteValue();
	u16 getWord();
	void setHighByteValue(u8 value);
	void setLowByteValue(u8 value);
	void setWord(u16 value);
	WordRegister& operator++();
	WordRegister& operator--();
	ByteRegister& getLowByteRegister();
	ByteRegister& getHighByteRegister();
private:
	ByteRegister& lowByte;
	ByteRegister& highByte;
	//u16 registerData;

};

class FlagsRegister : public ByteRegister{
	enum flags {
		c = 4,
		h = 5,
		n = 6,
		z = 7
	};
public:
	void setCarryFlag(bool flagValue);
	void setHalfCarryFlag(bool flagValue);
	void setSubstractionFlag(bool flagValue);
	void setZeroFlag(bool flagValue);

	bool getCarryFlag();
	bool getHalfCarryFlag();
	bool getSubstractionFlag();
	bool getZeroFlag();

private:

};