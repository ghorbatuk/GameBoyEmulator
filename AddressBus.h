#pragma once
#include "Common.h"
class emu;
class AddressBus
{
public:
	AddressBus(emu& gbEmu);
	unsigned char busRead(unsigned short address);
	void busWrite(unsigned short address, unsigned char value);
private:

	u8 readIO(u16 address);
	void writeIO(u16 address, u8 value);

	void dmaTransfer(u8 source);

	emu& gbEmu;
};