#pragma once
#include "CPU.h"

struct InstructionData ExecuteInstruction(unsigned char opcode);
int determineInstructionSize(char opcode);

struct InstructionData ORA(unsigned char opcode); //OR with A
struct InstructionData BRK(unsigned char opcode);
struct InstructionData LOD(unsigned char opcode); //Universal loader (LDX, LDY, LDA)
struct InstructionData EOR(unsigned char opcode); //Exclusive OR with A
struct InstructionData STO(unsigned char opcode); //Universal storer (STA, STX, STY)
struct InstructionData INC(unsigned char opcode); //Increment memory loc
struct InstructionData DEC(unsigned char opcode); //Decrement memory loc
struct InstructionData ASL(unsigned char opcode); //Unsigned shift left
struct InstructionData BRA(unsigned char opcode); //Universal branch (BCC,BCS,BEQ,BNE,BPL,BMI,BVC,BVS)
struct InstructionData JSR(unsigned char opcode); //Jump to Subroutine
struct InstructionData NOP(unsigned char opcode); //No-op
struct InstructionData BIT(unsigned char opcode); //Bitwise analysis (A ^ imm)
struct InstructionData ROL(unsigned char opcode); //Rotate left (push Carry flag into LSB, push MSB into Carry flag)
struct InstructionData ROR(unsigned char opcode); //Rotate right (push Carry flag into MSB, push LSB into Carry flag)
struct InstructionData RTI(unsigned char opcode); //Return from interrupt routine
struct InstructionData RTS(unsigned char opcode); //Return from subroutine
struct InstructionData AND(unsigned char opcode); //Logical AND with A
struct InstructionData ADC(unsigned char opcode); //Add with carry
struct InstructionData JMP(unsigned char opcode); //Unconditional jump
struct InstructionData TRA(unsigned char opcode); //Universal register transfer (TAX, TAY, TSX, TXA, TXS, TYA)
struct InstructionData CMP(unsigned char opcode); //CMP, CPY, CPX roll-into-one
struct InstructionData CLR(unsigned char opcode); //Clear CPU flag (CLC, CLI, CLD, CLV)
struct InstructionData SBC(unsigned char opcode); //Subtract w/ carry
struct InstructionData SET(unsigned char opcode); //Set CPU flag (SEC, SEI, SED)
struct InstructionData LSR(unsigned char opcode); //Logical right shift (set Carry to LSB)
struct InstructionData PUS(unsigned char opcode); //Push register to the stack (PHA, PHP)
struct InstructionData PUL(unsigned char opcode); //Pull register from the stack (PLA, PLP)
struct InstructionData INR(unsigned char opcode); //Increment Register (INX,INY)
struct InstructionData DER(unsigned char opcode); //Decrement Register (DEX,DEY)
struct InstructionData INV(unsigned char opcode); //Invalid operation opcode
struct InstructionData HLT(unsigned char opcode);






