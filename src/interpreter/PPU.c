#include "PPU.h"
#include "CPU.h"
#include<stdlib.h>

unsigned char control;
unsigned char mask;
unsigned char status;
unsigned char OAMaddr;
unsigned char OAMdata;
unsigned char scroll;
unsigned char addr;
unsigned char ppuData;

/*
    The PPU has two quirky registers: PPUSCROLL and PPUADDR.
    They're quirky in the sense that they're really kinda 16-bit registers, but ... not really?
    They take two writes at the same time, with the first write determining their high bits, and 
    the second write determining the low bits. 
*/
int addrWrite; 
int scrollWrite; 

void* output;

void PPUInit() {
    /*
        We're going to do something really crude up until I have a better idea:
        Each pixel in the 256x240 space outputted by this PPU is going to be represented by an RGB value.
    */
    output = malloc(sizeof(char) * 3 * NES_OUTPUT_HEIGHT * NES_OUTPUT_WIDTH);
}

unsigned char GetPPURegister(enum PPURegister PPUReg)
{
    switch(PPUReg) {
        case PPUCTRL:
            return control;
        break;       
        case PPUMASK:
            return mask;
        break;
        case PPUSTATUS:
            return status;
        break;
        case OAMADDR:
            return OAMaddr;
        break;
        case OAMDATA:
            return OAMdata;
        break;
        case PPUSCROLL:
            return scroll;
        break;
        case PPUADDR:
            return addr;
        break;
        case PPUDATA:
            return ppuData;
        break;

        default:
        return -1;
    }
}

void StoreInPPURegister(enum PPURegister PPUReg, unsigned char data)
{
    if (totalCPUCycles() < NTSC_BASELINE_CPU_CYCLES) {return;}
    switch(PPUReg) {
        case PPUCTRL:
            control = data;
        break;       
        case PPUMASK:
            mask = data;
        break;
        case PPUSTATUS:
            status = data;
        break;
        case OAMADDR:
            OAMaddr = data;
        break;
        case OAMDATA:
            OAMdata = data;
        break;
        case PPUSCROLL:
            scroll = data;
        break;
        case PPUADDR:
            addr = data;
        break;
        case PPUDATA:
            ppuData = data;
        break;

        default:
        break;
    }
}
