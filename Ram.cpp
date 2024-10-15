#include "Ram.h"
#include <memory>
#include <assert.h>

Ram::Ram()
{
    memset(workRam, 0, sizeof(workRam));
    memset(highRam, 0, sizeof(highRam));
    memset(vRam, 0, sizeof(vRam));
}

u8 Ram::readWorkRam(u16 address)
{
    assert(address - 0xC000 > -1 && address - 0xC000 < sizeof(workRam));
    return workRam[address - 0xC000];
}

u8 Ram::readHighRam(u16 address)
{
    if ((address - 0xFF80) >= sizeof(highRam))
        int i = 1;
    if ((address - 0xFF80) < 0)
        int i = 1;
    assert(address - 0xFF80 > -1 && address - 0xFF80 < sizeof(highRam));
    return highRam[address - 0xFF80];
}

u8 Ram::readVRam(u16 address)
{
    assert(address - 0x8000 > -1 && address - 0x8000 < sizeof(vRam));
    return vRam[address - 0x8000];
}

void Ram::writeWorkRam(u16 address, u8 value)
{
    assert(address - 0xC000 > -1 && address - 0xC000 < sizeof(workRam));
    workRam[address - 0xC000] = value;
}

void Ram::writeHighRam(u16 address, u8 value)
{
    if (address - 0xFF80 >= sizeof(highRam))
        int i = 1;
    if (address - 0xFF80 < 0)
        int i = 1;
    assert(address - 0xFF80 > -1 && address - 0xFF80 < sizeof(highRam));
    highRam[address - 0xFF80] = value;
}

void Ram::writeVRam(u16 address, u8 value)
{
    assert(address - 0x8000 > -1 && address - 0x8000 < sizeof(vRam));
    vRam[address - 0x8000] = value;
}
