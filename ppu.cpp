#include "ppu.h"
#include "emu.h"
#include "Common.h"
#include <cassert>


PPU::PPU(emu& gbEmu):
	gbEmu(gbEmu)
{
    memset(objectAtributeMemory, 0, sizeof(objectAtributeMemory));
    memset(vRam, 0, sizeof(vRam));
    init();
}

void PPU::init()
{
    lineTicks = 0;
    
    setCurrentPpuMode(OAM_SCAN_MODE);

}

void PPU::tick()
{
    lineTicks++;
    u8 currentMode = getCurrentPpuMode();
    switch (currentMode)
    {
    case OAM_SCAN_MODE:
        oamScan();
        break;
    case DRAW_PIXEL_MODE:
        draw();
        break;
    case H_BLANK_MODE:
        hBlank();
        break;
    case V_BLANK_MODE:
        vBlank();
        break;
    }
}

u8 PPU::readVRam(u16 address)
{
    assert(address - 0x8000 > -1 && address - 0x8000 < sizeof(vRam));
    return vRam[address - 0x8000];
}

void PPU::writeVRam(u16 address, u8 value)
{
    assert(address - 0x8000 > -1 && address - 0x8000 < sizeof(vRam));
    vRam[address - 0x8000] = value;
}

u8 PPU::readOAM(u16 address)
{
    assert(address - 0xFE00 > -1 && address - 0xFE00 < sizeof(objectAtributeMemory));
    return objectAtributeMemory[address - 0xFE00];
}

void PPU::writeOAM(u16 address, u8 value)
{
    assert(address - 0xFE00 > -1 && address - 0xFE00 < sizeof(objectAtributeMemory));
    objectAtributeMemory[address - 0xFE00] = value;
}

u8 PPU::getLcdControlValue()
{
    return lcdControl.getRegisterValue();
}

u8 PPU::getObjectSize()
{
    //check bit 2
    //0 means 8x8 object, 1 means 8x16 object
    return (lcdControl.getRegisterValue() & 0x4) != 0;
}

u8 PPU::getLcdStatus()
{
    return lcdStatus.getRegisterValue();
}

u8 PPU::getLcdY()
{
    return lcdY.getRegisterValue();;
}

u8 PPU::getLcdYCompare()
{
    return lcdYCompare.getRegisterValue();;
}

u8 PPU::getBackgroundPaletteValue()
{
    return backgroundPalette.getRegisterValue();
}

u8 PPU::getObjPalette0Value()
{
    return objectPalette0.getRegisterValue();
}

u8 PPU::getObjPalette1Value()
{
    return objectPalette1.getRegisterValue();
}

u8 PPU::getScrollX()
{
    return scrollX.getRegisterValue();
}

u8 PPU::getScrollY()
{
    return scrollY.getRegisterValue();
}

u8 PPU::getWindowX()
{
    return windowX.getRegisterValue();
}

u8 PPU::getWindowY()
{
    return windowY.getRegisterValue();
}

u8 PPU::getDMATransfer()
{
    return dmaTransfer.getRegisterValue();
}

void PPU::setLcdControlValue(u8 value)
{
    lcdControl.setRegisterValue(value);
}

void PPU::setLcdStatus(u8 value)
{
    lcdStatus.setRegisterValue(value);
}

void PPU::setLcdY(u8 value)
{
    lcdY.setRegisterValue(value);
}

void PPU::setLcdYCompare(u8 value)
{
    lcdYCompare.setRegisterValue(value);
}

void PPU::setBackgroundPaletteValue(u8 value)
{
    backgroundPalette.setRegisterValue(value);
}

void PPU::setObjPalette0Value(u8 value)
{
    objectPalette0.setRegisterValue(value);
}

void PPU::setObjPalette1Value(u8 value)
{
    objectPalette1.setRegisterValue(value);
}

void PPU::setScrollX(u8 value)
{
    scrollX.setRegisterValue(value);
}

void PPU::setScrollY(u8 value)
{
    scrollY.setRegisterValue(value);
}

void PPU::setWindowX(u8 value)
{
    windowX.setRegisterValue(value);
}

void PPU::setWindowY(u8 value)
{
    windowY.setRegisterValue(value);
}

void PPU::setDMATransfer(u8 value)
{
    dmaTransfer.setRegisterValue(value);
}

void PPU::oamScan()
{
    if (lineTicks >= 80) {
        //set mode to draw
        setCurrentPpuMode(DRAW_PIXEL_MODE);
    }

    if (lineTicks == 1) {
        lineSprites.clear();
        
        u8 lcdY = getLcdY();
        
        //read OAM ram
        for (int i = 0; i < sizeof(objectAtributeMemory); i += 4)
        {
            u8 spriteYPosition = objectAtributeMemory[i];
            u8 spriteXPosition = objectAtributeMemory[i + 1];

            if (lineSprites.size() >= 10) {
                //limit of 10 sprites per line reached
                break;
                
            }

            //get sprite height
            u8 spriteHeight = getObjectSize() ? 16 : 8;

            if (lcdY + 16 >= spriteYPosition && lcdY + 16 < spriteYPosition + spriteHeight && spriteXPosition > 0)
            {
                lineSprites.push_back(&objectAtributeMemory[i]);
            }

            //Sort list in order of value of X
            lineSprites.sort(std::greater<u8*>());

        }

    }
}

void PPU::draw()
{
    if (lineTicks >= 80 + 172)
    {
        setCurrentPpuMode(H_BLANK_MODE);
    }
}

void PPU::vBlank()
{
    if (lineTicks >= TICKS_PER_LINE)
    {
        increment_lcdY();

        if (getLcdY() >= LINE_PER_FRAME)
        {
            setCurrentPpuMode(OAM_SCAN_MODE);
            setLcdY(0);
           //set window line = 0
        }
        lineTicks = 0;
    }
}

void PPU::hBlank()
{
    if (lineTicks >= 80 + 376 - 172)
    {
        setCurrentPpuMode(V_BLANK_MODE);
    }
}

u8 PPU::getCurrentPpuMode()
{
    return (u8)lcdControl.getRegisterValue() & 0b11;
}

void PPU::setCurrentPpuMode(PPUMode newMode)
{
    lcdControl.setRegisterValue((lcdControl.getRegisterValue() & 0xFC) | (u8)newMode);
}

void PPU::increment_lcdY()
{
   ++lcdY;

   if (lcdY.getRegisterValue() == lcdYCompare.getRegisterValue())
   {
       //set LYC=LY int STAT register
       lcdStatus.setRegisterValue(lcdStatus.getRegisterValue() | ((u8)1 << 2));
       //request LCD stat interrupt
       gbEmu.addressBus.busWrite(0xFF0F, gbEmu.addressBus.busRead(0xFF0F) | 2);
   }
   else 
   {
       //unset LYC=LY int STAT register
       lcdStatus.setRegisterValue(lcdStatus.getRegisterValue() & ~((u8)1 << 2));
   }
}
