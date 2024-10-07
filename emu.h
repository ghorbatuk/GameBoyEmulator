#pragma once
#include "cartridge.h"
#include "AddressBus.h"
#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include "timer.h"
#include "Ram.h"

/*unsigned char workMemory[8192] = {0};
unsigned char videoMemory[8192] = { 0 };
unsigned char display[160 * 144] = { 0 };
*/


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

	AddressBus addressBus;
	friend class AddressBus;

	Ram ram;

	
	
};

