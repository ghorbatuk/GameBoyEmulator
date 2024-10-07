#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "cartridge.h"
#include <stdio.h>
#include <stdlib.h> 
#include <conio.h>

Cartridge::Cartridge() {
    romData = 0;
    romFileSize = 0;
    
    romHeader = { 0 }; 
}

bool Cartridge::loadCartridge(char* romName)
{
    FILE* file = fopen(romName,"rb");

    if (!file) 
    {
        printf("Could not open file: %s\n", romName);
        return false;
    }
    fseek(file, 0, SEEK_END);
    romFileSize = ftell(file);

    rewind(file);

    romData = (unsigned char*) malloc(romFileSize);

    if(romData)
    {
        fread(romData, romFileSize, 1, file);
    }
   
    fclose(file);


    romHeader = (RomHeader*) (romData + 0x100);
    //romHeader->title[15] = 0;

    printf("ROM: %s loaded successfully\n", romName);
    printf("\t Title:  %s\n", romHeader->title);
    printf("\t Type:  %X - %s\n", romHeader->cartType, getCartType(romHeader->cartType));
    printf("\t ROM size:  %X\n", romHeader->romSize);
    printf("\t Ram size:  %X\n", romHeader->ramSize);
    printf("\t Lic Code:  %X - %s\n", romHeader->oldLicenseeCode, getOldLicenseeCode(romHeader->oldLicenseeCode));
    printf("\t Rom Version:  %X\n", romHeader->romVersion);

    unsigned char checksum = 0;
    for (int i = 0x0134; i <= 0x014C; i++)
    {
        checksum = checksum - romData[i] - 1;
    }
    printf("\t Checksum:  %X  (%s)\n", romHeader->romChecksum, ((checksum & romHeader->romChecksum) == romHeader->romChecksum)?"PASS":"FAIL");

    return true;
}

const char* Cartridge::getOldLicenseeCode(int code)
{
    const char* licenseeCode = "UNKNOWN";
    std::map<int, const char*>::const_iterator pos = oldLicenseeCodes.find(code);
    if (pos != oldLicenseeCodes.end()) {
        licenseeCode = pos->second;
    }

    return licenseeCode;
}

const char* Cartridge::getNewLicenseeCode(int code)
{
    const char* licenseeCode = "UNKNOWN";
    std::map<int, const char*>::const_iterator pos = newLicenseeCodes.find(code);
    if (pos != newLicenseeCodes.end()) {
        licenseeCode = pos -> second;
    }

    return licenseeCode;
}

const char* Cartridge::getCartType(int code)
{
    const char* type = "UNKNOWN";
    std::map<int, const char*>::const_iterator pos = cartTypes.find(code);
    if (pos != cartTypes.end()) {
        type = pos->second;
    }

    return type;
}

unsigned char Cartridge::readRom(unsigned short address)
{
    return romData[address];
}
