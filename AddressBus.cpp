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
    else if (address < 0x8000)
    {
        //16 KiB ROM Bank 01–NN
        return gbEmu.cart.readRom(address);
    }
    else if (address < 0xA000)
    {
        //8 KiB Video RAM (VRAM)
        return 0;
    }
    else if (address < 0xC000)
    {
        //8 KiB External RAM
        return 0;
    }
    else if (address < 0xD000)
    {
        //4 KiB Work RAM (WRAM)
        return gbEmu.ram.readWorkRam(address);
    }
    else if (address < 0xE000)
    {
        //4 KiB Work RAM (WRAM)
        return gbEmu.ram.readWorkRam(address);
    }
    else if (address < 0xFE00)
    {
        //Echo RAM
        return 0;
    }
    else if (address < 0xFEA0)
    {
        //Object Attribute Memory (OAM)   
        return gbEmu.ppu.readOAM(address);;
    }
    else if (address < 0xFF00)
    {
        //NOT USABLE
        return 0;
    }
    else if (address < 0xFF80)
    {
        //I/O Registers
        return readIO(address);
    }
    else if (address < 0xFFFF)
    {
        //High RAM (HRAM)
        return gbEmu.ram.readHighRam(address);
    }
    else if (address == 0xFFFF)
    {
        //Interrupt Enable Register
        return gbEmu.cpu.interruptEnable.getRegisterValue();
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
    else if (address < 0x8000)
    {
        //16 KiB ROM Bank 01–NN
        return;
    }
    else if (address < 0xA000)
    {
        //8 KiB Video RAM (VRAM)
        return;
    }
    else if (address < 0xC000)
    {
        //8 KiB External RAM
        return;
    }
    else if (address < 0xD000)
    {
        //4 KiB Work RAM (WRAM)
        gbEmu.ram.writeWorkRam(address, value);
        return;
    }
    else if (address < 0xE000)
    {
        //4 KiB Work RAM (WRAM)
        gbEmu.ram.writeWorkRam(address, value);
        return;
    }
    else if (address < 0xFE00)
    {
        //Echo RAM
        return;
    }
    else if (address < 0xFEA0)
    {
        //Object Attribute Memory (OAM)
        gbEmu.ppu.writeOAM(address, value);
        return;
    }
    else if (address < 0xFF00)
    {
        //NOT USABLE
        return;
    }
    else if (address < 0xFF80)
    {
        //I/O Registers
        writeIO(address, value);
        return;
    }
    else if (address < 0xFFFF)
    {
        //High RAM (HRAM)
        gbEmu.ram.writeHighRam(address, value);
        return;
    }
    else if (address == 0xFFFF)
    {
        //Interrupt Enable Register
        gbEmu.cpu.interruptEnable.setRegisterValue(value);
    }
    
}


u8 AddressBus::readIO(u16 address)
{
    if (address == 0xFF00)
    {
        //Gamepad
        return 0;
    }
    else if (address == 0xFF01)
    {
        //Serial transfer Data
        return 0;
    }
    else if (address == 0xFF02)
    {
        //Serial transfer control
        return 0;
    }
    else if (address == 0xFF04)
    {
        //Divider Register
        return 0;
    }
    else if (address == 0xFF05)
    {
        //Timer Counter
        return 0;
    }
    else if (address == 0xFF06)
    {
        //Timer Modulo
        return 0;
    }
    else if (address == 0xFF07)
    {
        //Timer Control
        return 0;
    }
    else if (address == 0xFF0F)
    {
        //Interupts flag
        return gbEmu.cpu.interruptFlag.getRegisterValue();
    }
    else if (address == 0xFF10)
    {
        //NR10: Sound Channel 1 sweep
        return 0;
    }
    else if (address == 0xFF11)
    {
        //NR11: Sound Channel 1 length timer and duty cycle
        return 0;
    }
    else if (address == 0xFF12)
    {
        //NR12: Sound Channel 1 volume & envelope
        return 0;
    }
    else if (address == 0xFF13)
    {
        //NR13: Sound Channel 1 period low [write-only]
        return 0;
    }
    else if (address == 0xFF14)
    {
        //NR14: Sound Channel 1 period high and control
        return 0;
    }
    else if (address == 0xFF16)
    {
        //NR21: Sound Channel 2 length timer and duty cycle
        return 0;
    }
    else if (address == 0xFF17)
    {
        //NR22: Sound Channel 2 volume & envelope
        return 0;
    }
    else if (address == 0xFF18)
    {
        //NR23: Sound Channel 2 period low [write-only]
        return 0;
    }
    else if (address == 0xFF19)
    {
        //NR24: Sound Channel 2 period high and control
        return 0;
    }
    else if (address == 0xFF1A)
    {
        //NR30: Sound Channel 3 DAC enable
        return 0;
    }
    else if (address == 0xFF1B)
    {
        //NR31: Sound Channel 3 length timer [write-only]
        return 0;
    }
    else if (address == 0xFF1C)
    {
        //NR32: Sound Channel 3 output level
        return 0;
    }
    else if (address == 0xFF1D)
    {
        //NR33: Sound Channel 3 period low [write-only]
        return 0;
    }
    else if (address == 0xFF1D)
    {
        //NR34: Sound Channel 3 period high and control
        return 0;
    }
    else if (address == 0xFF20)
    {
        //NR41: Sound Channel 4 length timer [write-only]
        return 0;
    }
    else if (address == 0xFF21)
    {
        //NR42: Sound Channel 4 volume and envelope
        return 0;
    }
    else if (address == 0xFF22)
    {
        //NR43: Sound Channel 4 frequency abd randomness
        return 0;
    }
    else if (address == 0xFF23)
    {
        //NR44: Sound Channel 4 control
        return 0;
    }
    else if (address >= 0xFF30 && address <= 0xFF3F) 
    {
        //Wave Pattern RAM
        return 0;
    }
    else if (address == 0xFF40)
    {
        //LCD Control
        return gbEmu.ppu.getLcdControlValue();
    }
    else if (address == 0xFF41)
    {
        //LCD status
        return gbEmu.ppu.getLcdStatus();
    }
    else if (address == 0xFF42)
    {
        //SCY : Background viewport Y coordinate
        return gbEmu.ppu.getScrollY();
    }
    else if (address == 0xFF43)
    {
        //SCX : Background viewport X coordinate
        return gbEmu.ppu.getScrollX();
    }
    else if (address == 0xFF44)
    {
        //LCD Y coordinate [read-only]
        return gbEmu.ppu.getLcdY();
    }
    else if (address == 0xFF46)
    {
        //DMA transfer
        return gbEmu.ppu.getDMATransfer();
    }
    else if (address == 0xFF45)
    {
        //LY compare
        return gbEmu.ppu.getLcdYCompare();
    }
    else if (address == 0xFF47)
    {
        //Background palette data
        return gbEmu.ppu.getBackgroundPaletteValue();
    }
    else if (address == 0xFF48)
    {
        //Object palette 0 data
        return gbEmu.ppu.getObjPalette0Value();
    }
    else if (address == 0xFF49)
    {
        //Object palette 1 data
        return gbEmu.ppu.getObjPalette1Value();
    }

    else if (address == 0xFF4A)
    {
        //Window Y position
        return gbEmu.ppu.getWindowY();
    }
    else if (address == 0xFF4B)
    {
        //Window X position
        return gbEmu.ppu.getWindowX();;
     }
    return 0;
}

void AddressBus::writeIO(u16 address, u8 value)
{
    if (address == 0xFF00)
    {
        //Gamepad
        return;
    }
    else if (address == 0xFF01)
    {
        //Serial transfer Data
        return;
    }
    else if (address == 0xFF02)
    {
        //Serial transfer control
        return;
    }
    else if (address == 0xFF04)
    {
        //Divider Register
        return;
    }
    else if (address == 0xFF05)
    {
        //Timer Counter
        return;
    }
    else if (address == 0xFF06)
    {
        //Timer Modulo
        return;
    }
    else if (address == 0xFF07)
    {
        //Timer Control
        return;
    }
    else if (address == 0xFF0F)
    {
        //Interupts flag
        gbEmu.cpu.interruptFlag.setRegisterValue(value);
        
    }
    else if (address == 0xFF10)
    {
        //NR10: Sound Channel 1 sweep
        return;
    }
    else if (address == 0xFF11)
    {
        //NR11: Sound Channel 1 length timer and duty cycle
        return;
    }
    else if (address == 0xFF12)
    {
        //NR12: Sound Channel 1 volume & envelope
        return;
    }
    else if (address == 0xFF13)
    {
        //NR13: Sound Channel 1 period low [write-only]
        return;
    }
    else if (address == 0xFF14)
    {
        //NR14: Sound Channel 1 period high and control
        return;
    }
    else if (address == 0xFF16)
    {
        //NR21: Sound Channel 2 length timer and duty cycle
        return;
    }
    else if (address == 0xFF17)
    {
        //NR22: Sound Channel 2 volume & envelope
        return;
    }
    else if (address == 0xFF18)
    {
        //NR23: Sound Channel 2 period low [write-only]
        return;
    }
    else if (address == 0xFF19)
    {
        //NR24: Sound Channel 2 period high and control
        return;
    }
    else if (address == 0xFF1A)
    {
        //NR30: Sound Channel 3 DAC enable
        return;
    }
    else if (address == 0xFF1B)
    {
        //NR31: Sound Channel 3 length timer [write-only]
        return;
    }
    else if (address == 0xFF1C)
    {
        //NR32: Sound Channel 3 output level
        return;
    }
    else if (address == 0xFF1D)
    {
        //NR33: Sound Channel 3 period low [write-only]
        return;
    }
    else if (address == 0xFF1D)
    {
        //NR34: Sound Channel 3 period high and control
        return;
    }
    else if (address == 0xFF20)
    {
        //NR41: Sound Channel 4 length timer [write-only]
        return;
    }
    else if (address == 0xFF21)
    {
        //NR42: Sound Channel 4 volume and envelope
        return;
    }
    else if (address == 0xFF22)
    {
        //NR43: Sound Channel 4 frequency abd randomness
        return;
    }
    else if (address == 0xFF23)
    {
        //NR44: Sound Channel 4 control
        return;
    }
    else if (address >= 0xFF30 && address <= 0xFF3F)
    {
        //Wave Pattern RAM
        return;
    }
    else if (address == 0xFF40)
    {
        //LCD Control
        gbEmu.ppu.setLcdControlValue(value);
        return;
    }
    else if (address == 0xFF41)
    {
        //LCD status
        gbEmu.ppu.setLcdStatus(value);
    }
    else if (address == 0xFF42)
    {
        //SCY : Background viewport Y coordinate
        gbEmu.ppu.setScrollY(value);
    }
    else if (address == 0xFF43)
    {
        //SCX : Background viewport X coordinate
        gbEmu.ppu.setScrollX(value);
    }
    else if (address == 0xFF44)
    {
        //LCD Y coordinate [read-only]
        //Writing resets the counter
        gbEmu.ppu.setLcdY(0);
        return;
    }

    else if (address == 0xFF45)
    {
        //LY compare
        gbEmu.ppu.setLcdYCompare(value);
    }
    else if (address == 0xFF46)
    {
        //DMA transfer
       
        gbEmu.ppu.setDMATransfer(value);
        dmaTransfer(value);
    }
    else if (address == 0xFF47)
    {
        //Background palette data
        gbEmu.ppu.setBackgroundPaletteValue(value);
    }
    else if (address == 0xFF48)
    {
        //Object palette 0 data
        gbEmu.ppu.setObjPalette0Value(value);
    }
    else if (address == 0xFF49)
    {
        //Object palette 1 data
        gbEmu.ppu.setObjPalette1Value(value);
    }

    else if (address == 0xFF4A)
    {
        //Window Y position
        gbEmu.ppu.setWindowY(value);
    }
    else if (address == 0xFF4B)
    {
        //Window X position
        gbEmu.ppu.setWindowX(value);
    }
    return;
}

void AddressBus::dmaTransfer(u8 source)
{
    u16 sourceAddress = (u16)source * 0x100;

    for (int i = 0x0; i <= 0x9F; i++)
    {
        gbEmu.ppu.writeOAM(0xFE00 + i, busRead(sourceAddress + i));
    }
}
