#pragma once
#include "MMU.h"

#define NES_STACK_MIN 0x0100
#define NES_STACK_MAX  0x01FF
#define NES_MEM_MAX  0x07FF

#define ADDR_IN_BOUNDS(address, min,max) (address >= min && address <= max)

enum AccessMode {
    SYSTEM_RAM,
    CARTRIDGE_RAM
};

/*
    Reading the NES memory layout, 
    we have 2KB of internal work ram (0-07FF),
    which this file will handle.
    We're going to have an MMU file (or whatever you 
    decide to call it) that will map memory accesses
    for the rest from 0800-FFFF to their respective whatevers
*/
int InitializeMemory();
int LoadIntoMemory(void* into, short at, short size, enum AccessMode mode);
unsigned char GetFromMemory(short memoryAddress, enum AccessMode mode);
void PushOntoStack(unsigned short data, unsigned short stackPointer, int size);
unsigned short PopFromStack(unsigned short stackPointer, int size);


