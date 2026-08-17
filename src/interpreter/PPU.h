#pragma once
#define NTSC_BASELINE_CPU_CYCLES 29658
#define NES_OUTPUT_WIDTH  256
#define NES_OUTPUT_HEIGHT 240 //Technically, since we're using NTSC standards, this should be 224, but I want the full output.
enum PPURegister {
    PPUCTRL,
    PPUMASK,
    PPUSTATUS,
    OAMADDR,
    OAMDATA,
    PPUSCROLL,
    PPUADDR,
    PPUDATA
};

void PPUInit();
void* GetPPUOutput();
unsigned char GetPPURegister(enum PPURegister PPUReg);
void StoreInPPURegister(enum PPURegister PPUReg, unsigned char data);


