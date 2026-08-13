#pragma once
#define NTSC_BASELINE_CPU_CYCLES 29658

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


unsigned char GetPPURegister(enum PPURegister PPUReg);
void StoreInPPURegister(enum PPURegister PPUReg, unsigned char data);


