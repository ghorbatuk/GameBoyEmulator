#include "AddressBus.h"
#include "emu.h"

AddressBus::AddressBus(emu& gbEmu) :
	gbEmu(gbEmu)
{
}

unsigned char AddressBus::busRead(unsigned short address)
{

    if (address < 0x4000)
    {
        //16 KiB ROM bank 00
        return gbEmu.cart.readRom(address);
    }
    if (address < 0x8000)
    {
        //16 KiB ROM Bank 01–NN
        return 0;
    }
    if (address < 0xA000)
    {
        //8 KiB Video RAM (VRAM)
        return 0;
    }
    if (address < 0xC000)
    {
        //8 KiB External RAM
        return 0;
    }
    if (address < 0xD000)
    {
        //4 KiB Work RAM (WRAM)
        return gbEmu.ram.readWorkRam(address);
    }
    if (address < 0xE000)
    {
        //4 KiB Work RAM (WRAM)
        return gbEmu.ram.readWorkRam(address);
    }
    if (address < 0xFE00)
    {
        //Echo RAM
        return 0;
    }
    if (address < 0xFEA0)
    {
        //Object Attribute Memory (OAM)
        return 0;
    }
    if (address < 0xFF00)
    {
        //NOT USABLE
        return 0;
    }
    if (address < 0xFF80)
    {
        //I/O Registers
        return 0;
    }
    if (address < 0xFFFF)
    {
        //High RAM (HRAM)
        return gbEmu.ram.readHighRam(address);
    }
    if (address == 0xFFFF)
    {
        //Interrupt Enable Register
        return 0;
    }

    return 0;
}

void AddressBus::busWrite(unsigned short address, unsigned char value)
{
    
    if (address < 0x4000)
    {
        //16 KiB ROM bank 00
        return;
    }
    if (address < 0x8000)
    {
        //16 KiB ROM Bank 01–NN
        return;
    }
    if (address < 0xA000)
    {
        //8 KiB Video RAM (VRAM)
        return;
    }
    if (address < 0xC000)
    {
        //8 KiB External RAM
        return;
    }
    if (address < 0xD000)
    {
        //4 KiB Work RAM (WRAM)
        gbEmu.ram.writeWorkRam(address, value);
        return;
    }
    if (address < 0xE000)
    {
        //4 KiB Work RAM (WRAM)
        gbEmu.ram.writeWorkRam(address, value);
        return;
    }
    if (address < 0xFE00)
    {
        //Echo RAM
        return;
    }
    if (address < 0xFEA0)
    {
        //Object Attribute Memory (OAM)
        return;
    }
    if (address < 0xFF00)
    {
        //NOT USABLE
        return;
    }
    if (address < 0xFF80)
    {
        //I/O Registers
        return;
    }
    if (address < 0xFFFF)
    {
        //High RAM (HRAM)
        gbEmu.ram.writeHighRam(address, value);
        return;
    }
    if (address == 0xFFFF)
    {
        //Interrupt Enable Register
        return;
    }
    
}
