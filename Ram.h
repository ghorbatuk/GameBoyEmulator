#pragma once
#include "Common.h"

class Ram
{
public:
	Ram();
	u8 readWorkRam(u16 address);
	u8 readHighRam(u16 address);

	void writeWorkRam(u16 address, u8 value);
	void writeHighRam(u16 address, u8 value);
private:
	u8 workRam[8192];
	u8 highRam[127];
};

