#include "Ram.h"
#include <memory>

Ram::Ram()
{
    memset(workRam, 0, sizeof(workRam));
    memset(highRam, 0, sizeof(highRam));
}

u8 Ram::readWorkRam(u16 address)
{
    return workRam[address - 0xD000];
}

u8 Ram::readHighRam(u16 address)
{
    return highRam[address - 0xFF80];
}

void Ram::writeWorkRam(u16 address, u8 value)
{
    workRam[address - 0xD000] = value;
}

void Ram::writeHighRam(u16 address, u8 value)
{
    highRam[address - 0xFF80] = value;
}
