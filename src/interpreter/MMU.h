#pragma once
#include "Memory.h"
/*
Since the NES' 2KB workram is only a fraction of what can
be addressed by the NES, we have to shove an MMU in between
to actually translate addresses dependent on wherever they're
supposed to go.

Please re-read: https://www.nesdev.org/wiki/CPU_memory_map
*/

#define NES_MEM_MIN 0x0000
#define NES_MEM_MAX 0x07FF

#define NES_PPU_MIN 0x2000
#define NES_PPU_MAX 0x2007

#define NES_PPU_MIRROR_MIN 0x2008
#define NES_PPU_MIRROR_MAX 0x3FFF

/*
Note: attempting to read from this range
should cause some weird behaviours.
TODO, shove some weird error message in there
in the meantime, but read https://www.nesdev.org/wiki/Open_bus_behavior
later
*/
#define NES_APU_REG_MIN 0x4000
#define NES_APU_REG_MAX 0x4013

#define NES_SPRITE_DMA 0x4014


//Reading and writing from this range has special behaviour.
//Consult https://www.nesdev.org/wiki/2A03
#define NES_APU_CHN_ENABLE 0x4015
#define NES_JOYSTICK_STROBE 0x4016
#define NES_APU_FRAME_COUNT 0x4017

//0x4018-F is typically disabled. See https://www.nesdev.org/wiki/CPU_Test_Mode

//0x4020-0x5FFF is typically unmanaged. May be mapped to anything cartridge-internal.

#define NES_CART_RAM_MIN 0x6000
#define NES_CART_RAM_MAX 0x7FFF

#define NES_CART_ROM_MIN 0x8000
#define NES_CART_ROM_MAX 0xFFFF

void WriteToAddress(unsigned short address, void* source, short writeSize);
unsigned char ReadFromAddress(unsigned short address); //Rectify this all over, because the RP2A03 6502 can only do one byte per read.
unsigned short ReadShortFromAddress(unsigned short address);

