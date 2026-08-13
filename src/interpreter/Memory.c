#include"Memory.h"
#include<stdlib.h>
#include<string.h>
void* stack;//remember, the stack grows UPWARDS in memory addreses
char mainMemory[NES_MEM_MAX + 1];
char cartridgeMemory[NES_CART_RAM_MAX - NES_CART_RAM_MIN + 1];
int memSize = NES_MEM_MAX;


int InitializeMemory() {
    stack = mainMemory + NES_STACK_MIN;
    memset(mainMemory, 0, NES_MEM_MAX+1);
    memset(cartridgeMemory, 0, NES_CART_RAM_MAX - NES_CART_RAM_MIN + 1);
    return 0;
}

int LoadIntoMemory(void* into, short at, short size, enum AccessMode mode) {
    (void)into;
    (void)at;
    (void)size;
    (void)mode;
    if (size <= 0) {return -1;}

    char* mem = mode == SYSTEM_RAM ? mainMemory : cartridgeMemory;
    int minAddr = mode == SYSTEM_RAM ? NES_MEM_MIN : NES_CART_RAM_MIN; 
    int maxAddr = mode == SYSTEM_RAM ? NES_MEM_MAX : NES_CART_RAM_MAX;
    if (at + size > maxAddr) {return -1;}
    memcpy_s(mem + (at - minAddr), maxAddr - minAddr + 1, into, size);

    return 0;
}

unsigned char GetFromMemory(short memAddr, enum AccessMode mode) {
    int isSysRam = mode == SYSTEM_RAM;
    char* mem = isSysRam ? mainMemory : cartridgeMemory;
    short lower = isSysRam ? NES_MEM_MIN : NES_CART_RAM_MIN;
    short upper = isSysRam ? NES_MEM_MAX : NES_CART_RAM_MAX;
    if (!ADDR_IN_BOUNDS(memAddr, lower,upper)) {return -1;}
    return (unsigned char) *(mem + memAddr);
}


//WARN: The stack isn't cleared out at all, so if you're putting a thing, be mindful
//of what that thing is, be it a 16 or 8-bit value!
void PushOntoStack(unsigned short data, unsigned short stackPointer, int size) {
    memcpy(mainMemory + NES_STACK_MIN + stackPointer, &data, size);
}

//WARN: The stack isn't cleared out at all, so if you're taking a thing, be mindful
//of what that thing is, be it a 16 or 8-bit value!
unsigned short PopFromStack(unsigned short stackPointer, int size) {
    return *(unsigned short*)(mainMemory + NES_STACK_MIN + (stackPointer - size));
}
