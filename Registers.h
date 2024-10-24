#pragma once
#include "Common.h"

class ByteRegister
{
public:
	ByteRegister();
	ByteRegister(const ByteRegister& reg) = delete;
	auto operator=(const ByteRegister&)->ByteRegister & = delete;
	u8 getRegisterValue() const;
	void setRegisterValue(u8 value);
	virtual ByteRegister& operator++();
protected:
	u8 value;
};

class WordRegister 
{
public:
	WordRegister();
	WordRegister(const WordRegister& reg) = delete;
	auto operator=(const WordRegister&)->WordRegister & = delete;
	virtual u16 getWord() const;
	virtual void setWord(u16 value);
	virtual WordRegister& operator++();
	virtual WordRegister& operator--();
	WordRegister& operator+=(const u16 reg);
private:
	u16 value;
};

class ByteRegisterPair : public WordRegister
{
public:
	ByteRegisterPair(ByteRegister&, ByteRegister&);
	ByteRegisterPair(const ByteRegisterPair& reg) = delete;
	auto operator=(const ByteRegisterPair&)->ByteRegisterPair & = delete;
	//u8 getHighByteValue();
	//u8 getLowByteValue();
	u16 getWord() const override;
	void setHighByteValue(u8 value);
	void setLowByteValue(u8 value);
	void setWord(u16 value) override;
	ByteRegisterPair& operator++() override;
	ByteRegisterPair& operator--() override;
	ByteRegister& getLowByteRegister();
	ByteRegister& getHighByteRegister();
private:
	ByteRegister& lowByte;
	ByteRegister& highByte;
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