#pragma once

/*
This CPU is based off of the NTSC-variant NES' RP2A03 chip's CPU, which, in turn, is based
off of the MOS 6502, without the decimal mode.
*/

#define NES_EMU 0
#define MOS_6502_EMU 1

//Registers.
#define PC  0x00 //Register for Program counter.
#define SP  0x01 //Register for Stack pointer.
#define ACC 0x02 //Register for Accumulator.
#define RX  0x03 //Register for Index X
#define RY  0x04 //Register for Index Y
#define FL  0x05 //Register for Flags.

//CPU Flags, with the value being the distance from right.
#define CARRY_FLAG 0x00
#define ZERO_FLAG 0x01
#define INT_FLAG 0x02
#define DECIMAL_FLAG 0x03
#define BREAK_FLAG 0x04
#define CONST_FLAG 0x05 //Always 1
#define OVERFLOW_FLAG 0x06
#define NEGATIVE_FLAG 0x07

enum ReadMode {
    Opcode = 0,
    Op1 = 1, 
    Op2 = 2
};

struct InstructionData {

    //For test harnessing.
    char fullInstruction[16];
  

    //The value to put in the destination after cpuCyclesToBurn
    unsigned short value; 
   
    //The type of destination. 0x00 for register, 0xFF for memory.
    unsigned char destinationType; 

    //the index of the destination (memory loc for 0xFF, register # for 0x00)
    unsigned short destIdx;

    /*
    However many CPU cycles to burn before doing the above. If zero, then immediately.
    Important: cpuCyclesToBurn DOES NOT include the amount of CPU cycles required
    to read the instruction and its ops. 
    It DOES, however, denote however many further reads are carried out by the CPU after reading the
    instruction and its ops. 
    */
    unsigned char cpuCyclesToBurn; 
};

struct InstructionData newInstructionData();

typedef unsigned char Register;

void MovePC(short position);
int Step();
unsigned short GetRegisterValue(Register r);
void SetRegister(Register r, unsigned short value);
void SetCPUFlag (unsigned char flag, unsigned char value);
void SetHaltFlag();
void SetEmulationMode(unsigned char mode);
unsigned char GetEmulationMode();
const char* GetLastInstruction();
unsigned char GetCPUFlag(unsigned char flag);
unsigned char GetCurrentOpcode();
unsigned char GetLastByte();
unsigned char GetHiByte();
unsigned char GetLoByte();
void CPUInit();
unsigned int totalCPUCycles();
