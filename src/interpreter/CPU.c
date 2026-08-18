#include"CPU.h"
#include "MMU.h"
#include "fptrs.h"
#include<stdio.h>

unsigned char lowByte;
unsigned char highByte;
unsigned char opcode;

unsigned short programCounter;
unsigned short IRQ;
unsigned short NMI;
unsigned char stackPointer;
unsigned char accumulator;
unsigned char indexX;
unsigned char indexY;
unsigned char haltFlag = 0; //This is for 6502-based testing purposes, we'll set this when we hit a BRK instruction. 
unsigned char cpuFlags = 32;
unsigned char flagEscrow = 32; //This will be set during an instruction's duration and propagated to cpuFlags at the end.
unsigned int cycles = 0;
unsigned int sleepCycles;
unsigned int bytesLeft;
enum ReadMode currentReadMode;
struct InstructionData escrow;
void Fetch();
void Execute(unsigned char opcode);
void ProcessEscrow();
char lastInstruction[16];

const char* GetLastInstruction() {
    return lastInstruction;
}

unsigned char GetLastByte() {
    return ReadFromAddress(programCounter); 
}

void CPUInit()
{
    if (BASE_6502_EMU) {
        programCounter = 0x8000;
    } else {
        NMI = ReadShortFromAddress(0xFFFA);
        printf("Acquired non-maskable interrupt vector: 0x%04X\n", NMI);
        unsigned short resetVector = ReadShortFromAddress(0xFFFC);
        printf("Acquired reset vector: 0x%04X\n", resetVector);
        programCounter = resetVector;
        IRQ = ReadShortFromAddress(0xFFFE);
        printf("Acquired external IRQ vector: 0x%04X\n", IRQ);
        cycles = 6; //Does CPU cycle accuracy even matter at this stage? The PPU hasn't woken up yet, so....
    }

    snprintf(lastInstruction, 16, "N/A\0");
    currentReadMode = Opcode;
    bytesLeft = 1;
    sleepCycles = 0;

}

int Step() {
    if (haltFlag) {return haltFlag;}
    printf("--- CYCLE %d ---\n", cycles);
    if (sleepCycles == 0) {
        if (bytesLeft) {
            bytesLeft--;
            switch(currentReadMode) {
                case Opcode:
                    opcode = ReadFromAddress(programCounter);
                    bytesLeft = determineInstructionSize(opcode) - 1;
                break;
                case Op1:
                    highByte = ReadFromAddress(programCounter);
                break;
                case Op2:
                    lowByte = ReadFromAddress(programCounter);
                break;
            }
            programCounter++; 
            currentReadMode++;
            currentReadMode %= 3;
        }
        if (bytesLeft == 0) {
            Execute(opcode); 
            currentReadMode = Opcode;
        }
    } else {
        printf("Sleeping for %d extra cycles to emulate accuracy... \n", sleepCycles);
        sleepCycles--;
        if (!sleepCycles) {
            ProcessEscrow();
        }
    }
    cycles++;
    return haltFlag;
}


void ProcessEscrow() {
    if (escrow.destIdx != 0xFFFF) {
        if (escrow.destinationType == 0x00) {
            switch(escrow.destIdx) {
                case ACC:
                    accumulator = escrow.value;
                    break;
                case RX:
                    indexX = escrow.value;
                    break;
                case RY:
                    indexY = escrow.value;
                    break;
                case FL:
                    cpuFlags = escrow.value;
                    break;
                case SP:
                    stackPointer= escrow.value;
                    break;
                case PC:
                    programCounter = escrow.value;
                    break;
                default: 
                    break; 
            }
        } else if (escrow.destinationType == 0x0F) { //Special condition for RTI (PC + Flag register.)
            programCounter = escrow.value;
            flagEscrow = escrow.destIdx;
        } else {
            unsigned char cVal = escrow.value;
            WriteToAddress(escrow.destIdx, &cVal, 1);
        }
        escrow = newInstructionData();
    }
    cpuFlags = flagEscrow;
}

unsigned int totalCPUCycles()
{
    return cycles;
}

void SetHaltFlag() {
    haltFlag = 1;
}

void Execute(unsigned char opcode) {
    escrow = ExecuteInstruction(opcode);
    sleepCycles = escrow.cpuCyclesToBurn;
    bytesLeft = 1;
    if(haltFlag) {programCounter--;}
    sprintf(lastInstruction, "%s", escrow.fullInstruction);
    if (sleepCycles == 0) {
        ProcessEscrow();
    }
}

unsigned short GetRegisterValue(Register r) {
    switch(r) {
        case ACC:
            return accumulator;
            break;
        case RX:
            return indexX;
            break;
        case RY:
            return indexY;
            break;
        case FL:
            return cpuFlags;
            break;
        case SP:
            return stackPointer;
            break;
        case PC:
            return programCounter;
            break;
        default: 
            return 0;
            break;
    }
}

void SetRegister(Register r, unsigned short value) {
    switch(r) {
        case ACC:
            accumulator = value;
            break;
        case RX:
            indexX = value;
            break;
        case RY:
            indexY = value;
            break;
        case FL:
            cpuFlags = value;
            break;
        case SP:
            stackPointer = value;
            break;
        case PC:
            programCounter = value;
            break;
        default:
            break;
    }
}

struct InstructionData newInstructionData() {
    struct InstructionData id;
    id.value = 0;
    id.destIdx = 0xFFFF;
    id.destinationType = 0;
    id.cpuCyclesToBurn = 0;
    return id;
}


void SetCPUFlag(unsigned char flag, unsigned char value) {
    if (value) {
        flagEscrow |= (value << flag);
    } else {
        flagEscrow &= ~(1 << flag);
    }
}

unsigned char GetCPUFlag(unsigned char flag) {
    return (cpuFlags >> flag) & 0x1;
}

unsigned char GetHiByte() {return highByte;}
unsigned char GetLoByte() {return lowByte;}
unsigned char GetCurrentOpcode() {
    return opcode;
}

