#pragma once
#include "cartridge.h"
#include "AddressBus.h"
#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include "timer.h"
#include "Ram.h"

class emu
{
public:
	emu();
	int run(int agrc, char **argv);

private:
	bool init(char* programName);
	Cartridge cart;
	CPU cpu;
	friend class CPU;

	PPU ppu;
	friend class PPU;

	AddressBus addressBus;
	friend class AddressBus;

	Ram ram;

	
	
};

