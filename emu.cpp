#include "emu.h"
#include <iostream>

emu::emu() :
    cpu(*this),
    ppu(*this),
    addressBus(*this),
    cart()
{

}

int emu::run(int agrc, char** argv)
{
    char* programName = argv[1];
    
    if (init(programName)) {
        while (true)
        {
            //Emulate cycle
            cpu.tick();
            ppu.tick();
            //timer.tick()
        }
    }
    else {
        return 1;
    }
    return 0;
}

bool emu::init(char* programName)
{
    bool initSuccess = true;
    

    if (programName == NULL)
    {
        printf("No ROM to load. Exiting...\n");
        initSuccess = false;
    }

    if (!cart.loadCartridge(programName))
    {
        printf("Error loading ROM %s. Exiting...\n", programName);
        initSuccess = false;
    }

    return initSuccess;
}

