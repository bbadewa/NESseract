#include "MMU.h"
#include "PPU.h"
#include "ROM.h"
#include "Memory.h"


void TranslateWriteAddress(short address, void* src, short writeSize) {
    //I'm mulling over implementing a jump-table for different
    //ranges of the 6502's address-space, but in the meantime:
    if (address <= NES_MEM_MAX) {
        LoadIntoMemory(src, address, writeSize, SYSTEM_RAM);
    }else if (address <= NES_PPU_MIRROR_MAX) {
        StoreInPPURegister(
            (address - (address < NES_PPU_MIRROR_MIN ? NES_PPU_MIN : NES_PPU_MIRROR_MIN)) % 8,
            *((unsigned char*)(src)));
    } else if (address <= NES_APU_REG_MAX) {
        //TODO NES APU funcs
    } else if (address <= NES_CART_RAM_MAX) {
        LoadIntoMemory(src, address, writeSize, CARTRIDGE_RAM);
    }else if (address <= NES_CART_ROM_MAX) {
        //This has something to do with bankswapping, mapper chips, the whole kit and caboodle...
        //TODO: revisit this later and implement that functionality!!
    }
}

unsigned char ReadFromAddress(unsigned short address) {
    if (address <= NES_MEM_MAX) {
        return GetFromMemory(address, SYSTEM_RAM);
    }else if (address <= NES_PPU_MIRROR_MAX) {
        return GetPPURegister( (address - (address < NES_PPU_MIRROR_MIN ? NES_PPU_MIN : NES_PPU_MIRROR_MIN)) % 8);
    } else if (address <= NES_APU_REG_MAX) {
        //TODO NES APU funcs
    } else if (address <= NES_CART_RAM_MAX) {
        return GetFromMemory(address, CARTRIDGE_RAM);
    } else if (address <= NES_CART_ROM_MAX) {
        return ReadFromROM(address);
    }
}

unsigned short ReadShortFromAddress(unsigned short s) {
    return ReadFromAddress(s+1) << 8 | ReadFromAddress(s);
}


void WriteToAddress(unsigned short address, void* source, short writeSize) {
    switch(address) {
        case NES_APU_CHN_ENABLE:
            break;
        case NES_JOYSTICK_STROBE:
            break;
        case NES_APU_FRAME_COUNT:
            break;
        case NES_SPRITE_DMA:
            break;
        default:
            TranslateWriteAddress(address, source, writeSize);
            break;
    }
}