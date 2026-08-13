#include "PPU.h"
#include "CPU.h"

unsigned char control;
unsigned char mask;
unsigned char status;
unsigned char OAMaddr;
unsigned char OAMdata;
unsigned char scroll;
unsigned char addr;
unsigned char data;


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
            return data;
        break;

        default:
        return -1;
    }
}

void StoreInPPURegister(enum PPURegister PPUReg, unsigned char data)
{
    if (totalCPUCycles() < NTSC_BASELINE_CPU_CYCLES) {return;}
    PPUReg++;
    data++;
}
