#pragma once
#include "Common.h"
#include "Registers.h"
#include <queue>
#include <list>

class emu;

static const int LINE_PER_FRAME = 154;
static const int TICKS_PER_LINE = 456;
static const int SCREEN_HEIGHT = 144;
static const int SCREEN_WIDTH = 160;

enum lcdControlBits {
	bgWindowEnablePriority = 0,
	objEnable = 1,
	objSize = 2,
	bgTileMap = 3,
	bgWindowTiles = 4,
	windowEnable = 5,
	windowTileMap = 6,
	lcdPpuEnable = 7
};

enum PPUMode {
	H_BLANK_MODE,
	V_BLANK_MODE,
	OAM_SCAN_MODE,
	DRAW_PIXEL_MODE
};

enum Colors {
	WHITE,
	LIGHT_GRAY,
	DARK_GRAY,
	BLACK
};
class PPU
{
public:
	PPU(emu& gbEmu);
	void init();
	void tick();
	
	u8 readVRam(u16 address);
	void writeVRam(u16 address, u8 value);

	u8 readOAM(u16 address);
	void writeOAM(u16 address, u8 value);

	u8 getLcdControlValue();
	u8 getObjectSize();
	u8 getLcdStatus();
	u8 getLcdY();
	u8 getLcdYCompare();
	u8 getBackgroundPaletteValue();
	u8 getObjPalette0Value();
	u8 getObjPalette1Value();
	u8 getScrollX();
	u8 getScrollY();
	u8 getWindowX();
	u8 getWindowY();
	u8 getDMATransfer();

	void setLcdControlValue(u8 value);
	void setLcdStatus(u8 value);
	void setLcdY(u8 value);
	void setLcdYCompare(u8 value);
	void setBackgroundPaletteValue(u8 value);
	void setObjPalette0Value(u8 value);
	void setObjPalette1Value(u8 value);
	void setScrollX(u8 value);
	void setScrollY(u8 value);
	void setWindowX(u8 value);
	void setWindowY(u8 value);
	void setDMATransfer(u8 value);


	void oamScan();
	void draw();
	void vBlank();
	void hBlank();

	u8 getCurrentPpuMode();
	void setCurrentPpuMode(PPUMode newMode);
private:
	emu& gbEmu;
	int lineTicks;

	std::list<u8*> lineSprites;

	std::queue<int> backgroundFifo;
	std::queue<int> objectFifo;

	ByteRegister lcdControl;
	ByteRegister lcdStatus;
	ByteRegister lcdY;
	ByteRegister lcdYCompare;
	ByteRegister scrollX;
	ByteRegister scrollY;
	ByteRegister windowX;
	ByteRegister windowY;
	ByteRegister backgroundPalette;
	ByteRegister objectPalette0;
	ByteRegister objectPalette1;
	ByteRegister dmaTransfer;

	u8 objectAtributeMemory[4*40];
	u8 vRam[8192];
	
	void increment_lcdY();

};

