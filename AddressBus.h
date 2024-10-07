#pragma once
//#include "emu.h"
class emu;
class AddressBus
{
public:
	AddressBus(emu& gbEmu);
	unsigned char busRead(unsigned short address);
	void busWrite(unsigned short address, unsigned char value);
private:

	emu& gbEmu;
};