#include"fptrs.h"
#include "MMU.h"
#include<stdio.h>
#include"string.h"
char operandFormatting[8];
/*
    There are certain instructions that basically do the same thing that I can figure out upon execution, so
    I can just take a few function pointers (Store, No-Op, Load, Branch, Compare, Or, And, etc.) and then at execution
    time, based on the opcode position, figure out what the operands are supposed to be (Should I OR these two numbers or 
    a register? Should I store this number in the X or Y register? Should I load this immediate or absolute value?)
*/
struct InstructionData (*fptrPool[256])(unsigned char) = {
    BRK,ORA,INV,INV,NOP,ORA,ASL,INV,PUS,ORA,ASL,INV,NOP,ORA,ASL,INV,
    BRA,ORA,INV,INV,NOP,ORA,ASL,INV,CLR,ORA,NOP,INV,NOP,ORA,ASL,INV,
    JSR,AND,INV,INV,BIT,AND,ROL,INV,PUL,AND,ROL,INV,BIT,AND,ROL,INV,
    BRA,AND,INV,INV,NOP,AND,ROL,INV,SET,AND,NOP,INV,NOP,AND,ROL,INV,
    RTI,EOR,INV,INV,NOP,EOR,LSR,INV,PUS,EOR,LSR,INV,JMP,EOR,LSR,INV,
    BRA,EOR,INV,INV,NOP,EOR,LSR,INV,CLR,EOR,NOP,INV,NOP,EOR,LSR,INV,
    RTS,ADC,INV,INV,NOP,ADC,ROR,INV,PUL,ADC,ROR,INV,JMP,ADC,ROR,INV,
    BRA,ADC,INV,INV,NOP,ADC,ROR,INV,SET,ADC,NOP,INV,NOP,ADC,ROR,INV,
    NOP,STO,NOP,INV,STO,STO,STO,INV,DER,NOP,TRA,INV,STO,STO,STO,INV,
    BRA,STO,INV,INV,STO,STO,STO,INV,TRA,STO,TRA,INV,INV,STO,INV,INV,
    LOD,LOD,LOD,INV,LOD,LOD,LOD,INV,TRA,LOD,TRA,INV,LOD,LOD,LOD,INV,
    BRA,LOD,INV,INV,LOD,LOD,LOD,INV,CLR,LOD,TRA,INV,LOD,LOD,LOD,INV,
    CMP,CMP,NOP,INV,CMP,CMP,DEC,INV,INR,CMP,DER,INV,CMP,CMP,DEC,INV,
    BRA,CMP,INV,INV,NOP,CMP,DEC,INV,CLR,CMP,NOP,INV,NOP,CMP,DEC,INV,
    CMP,SBC,NOP,INV,CMP,SBC,INC,INV,INR,SBC,NOP,INV,CMP,SBC,INC,INV,
    BRA,SBC,INV,INV,NOP,SBC,INC,INV,SET,SBC,NOP,INV,NOP,SBC,INC,INV

};

char ProperInstructionNames[256][4] = {
    "BRK","ORA","KIL","SLO","NOP","ORA","ASL","SLO","PHP","ORA","ASL","ANC","NOP","ORA","ASL","SLO",
    "BPL","ORA","KIL","SLO","NOP","ORA","ASL","SLO","CLC","ORA","NOP","SLO","NOP","ORA","ASL","SLO",
    "JSR","AND","KIL","RLA","BIT","AND","ROL","RLA","PLP","AND","ROL","ANC","BIT","AND","ROL","RLA",
    "BMI","AND","KIL","RLA","NOP","AND","ROL","RLA","SEC","AND","NOP","RLA","NOP","AND","ROL","RLA",
    "RTI","EOR","KIL","SRE","NOP","EOR","LSR","SRE","PHA","EOR","LSR","ALR","JMP","EOR","LSR","SRE",
    "BVC","EOR","KIL","SRE","NOP","EOR","LSR","SRE","CLI","EOR","NOP","SRE","NOP","EOR","LSR","SRE",
    "RTS","ADC","KIL","RRA","NOP","ADC","ROR","RRA","PLA","ADC","ROR","ARR","JMP","ADC","ROR","RRA",
    "BVS","ADC","KIL","RRA","NOP","ADC","ROR","RRA","SEI","ADC","NOP","RRA","NOP","ADC","ROR","RRA",
    "NOP","STA","NOP","SAX","STY","STA","STX","SAX","DEY","NOP","TXA","XAA","STY","STA","STX","SAX",
    "BCC","STA","KIL","AHX","STY","STA","STX","SAX","TYA","STA","TXS","TAS","SHY","STA","SHX","AHX",
    "LDY","LDA","LDX","LAX","LDY","LDA","LDX","LAX","TAY","LDA","TAX","LAX","LDY","LDA","LDX","LAX",
    "BCS","LDA","KIL","LAX","LDY","LDA","LDX","LAX","CLV","LDA","TSX","LAS","LDY","LDA","LDX","LAX",
    "CPY","CMP","NOP","DCP","CPY","CMP","DEC","DCP","INY","CMP","DEX","AXS","CPY","CMP","DEC","DCP",
    "BNE","CMP","KIL","DCP","NOP","CMP","DEC","DCP","CLD","CMP","NOP","DCP","NOP","CMP","DEC","DCP",
    "CPX","SBC","NOP","ISC","CPX","SBC","INC","ISC","INX","SBC","NOP","SBC","CPX","SBC","INC","ISC",
    "BEQ","SBC","KIL","ISC","NOP","SBC","INC","ISC","SED","SBC","NOP","ISC","NOP","SBC","INC","ISC"
};

struct InstructionData ExecuteInstruction(unsigned char opcode)
{
    struct InstructionData escrow = fptrPool[opcode](opcode);  
    sprintf(escrow.fullInstruction, "%s %s\n", ProperInstructionNames[opcode], operandFormatting);
    memset(operandFormatting, 0, 8);
    return escrow;
}

int determineInstructionSize(char opcode)
{
    char highNibble = (opcode | 0x0F) >> 4;
    char lowNibble = (opcode & 0x0F);

    switch(lowNibble) {
        case 0x0C:
        case 0x0D:
        case 0x0E:
        case 0x0F:
            return 3;
            break;
        case 0x9:
            return 2 + (highNibble & 0x01);
            break;
        case 0x1:
            return 2;
            break;
        case 0x0:
            if (opcode == 0x20) {return 3;}
            return 1 + (highNibble & 0x01 ? 2 : 0);
            break;
        case 0x8:
        case 0xA:
            return 1;
            break;
        default:
            return 2;
            break;

    }
}

/*
    We need some helper functions in order to define some of the memory
    locations defined by some of the functions, and to add CPU cycles based
    on registers indexed.

    The 6502 can fetch one byte and do operations all in one CPU cycle.
    So, we can dynamically determine how many CPU cycles to tack on in the reads.

*/

/*
    We're gonna double-up certain instructions in the interest of keeping implementations
    short and sweet, so:
*/
unsigned short GetTransferRegisters(unsigned char opcode) {
    switch(opcode) {
        case 0x8A: //TXA
            return ((unsigned short)RX << 8) | ((unsigned short)ACC);
        break;
        case 0xAA: //TAX
            return ((unsigned short)ACC << 8) | ((unsigned short)RX);
        break;
        case 0xA8: //TAY
            return ((unsigned short)ACC << 8) | ((unsigned short)RY);
        break;
        case 0xBA: //TSX
            return ((unsigned short)SP << 8) | ((unsigned short)RX);
        break;
        case 0x9A: //TXS
            return ((unsigned short)RX << 8) | ((unsigned short)SP);
        break;
        case 0x98: //TYA
            return ((unsigned short)RY << 8) | ((unsigned short)ACC);
        break;
        default:
            return 0;
        break;
    }
}

/*
The 6502 takes an extra cycle to process reads that jump a "page boundary".
What that means is that if your base is some 0xAABB and adding some idx to it
causes the upper bytes to add or decrement 1, then that "crosses the page boundary."
So, to figure that out off the cuff:

base: The base address.
addition: what was added to it.

TODO: This may be a bit crude, and will likely not cover all of our usecases.
Consider re-factoring.
*/
unsigned char hasCrossedPageBoundary(unsigned short base, unsigned short addition) {
    return (base & 0xFF00) == ((base + addition) & 0xFF00);
}

/*
    Reads from the zero page (register value + idx). This is for zp/zpx/zpy reads.
    r: The register to index by. If less than zero, then non-indexed.
    idx: The number to add to the register value.
    Returns a byte read from that address.
*/
unsigned char ReadZeroPage(Register r, unsigned char idx, struct InstructionData* out) {
    unsigned char effectiveAddress = ((unsigned char)GetRegisterValue(r)) + idx;
    out->cpuCyclesToBurn += (GetCurrentOpcode() & 0x01);
    return ReadFromAddress(effectiveAddress);
}

/*
    Reads from memory (register value + idx). This is for izx/izy reads.
    r: The register to index by. If less than zero, then non-indexed.
    idx: The number to add to the register value.
    Returns a byte read from that address.
*/
unsigned char ReadIndirectIndexed(Register r, unsigned char idx, struct InstructionData* out) {
    unsigned char opcode = GetCurrentOpcode();
    unsigned short zeroPageLoc = ReadShortFromAddress((unsigned char)GetRegisterValue(r) + idx);
    if (opcode & 0x01 && opcode != 0x91) {
        out->cpuCyclesToBurn += hasCrossedPageBoundary(GetRegisterValue(r), idx);
    }
    return ReadFromAddress(zeroPageLoc); 
}

/*
    Reads from memory (register value + idx.) This is for abs/abx/aby/ind reads.
    r: The register to index by. If less than zero, then non-indexed.
    idx: The number to add to the register value.
    Returns a byte read from that address.
*/
unsigned char ReadAbsolute(Register r, unsigned short idx, struct InstructionData* out) {
    unsigned short effectiveAddress = GetRegisterValue(r) + idx;
    out->cpuCyclesToBurn++;
    return ReadFromAddress(effectiveAddress);
}


/*
    The 6502 ISA has a buncha patterns that can be used to discern what
    type of CPU read or register op we're doing based on the bits of the opcode.
    So, in the interest of not losing my mind:
*/


/*
   Decodes action registers (read: the register that we are doing an action to, de-generalizing
   STA,STX,STY from STO, CMP, CPX,CPY from CMP).
   Returns the action register (not the value, so feed this into GetRegisterValue() to actually
   get any use from it!)
*/
unsigned char GetActionRegisterFromOpcode(unsigned char opcode) {
    char upper = (opcode & 0xF0) >> 4;
    char lower = (opcode & 0x0F);

    if (fptrPool[opcode] == STO || fptrPool[opcode] == LOD || 
        (fptrPool[opcode] == CMP && upper != 0x0E)) { //Exception: CPX in the 0xEX row handled later
            //Thinking about this a bit, across the opcode matrix, the pattern for these three (and many other tiny exceptions)
            //goes Y-A-X register. So:
            switch (lower % 4) {
                case 0: 
                    return RY;
                    break;
                case 1: 
                    return ACC;
                    break;
                case 2: 
                    return RX;
                break;
                case 3:
                    return -1;
                    break;
            }
        }


    if ((0x4 <= lower && lower <= 0x6) || (0xC <= lower && lower <= 0xF)) { //Zero Page / Absolute Read sections
            //The rule's pretty simplistic otherwise. 
            return ACC;
    }

    if (lower == 0) {
        return  -1 + (upper & 0x01); //Every odd instruction in the 0xX0 column is PC-relative.
    }
    if (lower == 1) {
        return  ACC;
    }
    if (lower == 9) {
        return ACC;
    }

    


    return -1;
    
}

/*
    Read Registers will be used for Reads and Writes, because the heuristic
    is the same across LOD/STO instructions and what not. 
*/
unsigned char GetReadRegisterFromOpcode(unsigned char opcode) {
    /*
        For valid 6502 instructions, we have four lo-nibble ranges for which we are using registers to read from system memory:
        0xX1, 0xX4-0xX6, 0x9, 0xXC-0xXE.
    */
    char upper = (opcode & 0xF0) >> 4;
    char lower = (opcode & 0x0F);

    switch(lower) {
        case 0x1:
            return (upper & 0x1) ? RY : RX;
        break;

        case 4:
        case 5:
        case 6:
            if (opcode == 0x96 || opcode == 0xB6) {return RY;}
            return (upper & 0x1) ? RX : -1;
        break;

        case 9:
            return (upper & 0x1) ? RY : -1;
        break;
        
        case 0xC:
        case 0xD:
        case 0xE:
            if (opcode == 0xBE) {return RY;}
            return (upper & 0x1) ? RX : -1;
        break;

        default:
            return -1;
        break;
    }
    
}

/*
    Decides whether a branch should be taken based on the branch instruction.
    Returns 1 if the branch should be taken, 0 if not.
*/
unsigned char shouldBranch(unsigned char opcode) {
    char upper = (opcode & 0xF0) >> 4;
    switch(upper) {
        case 0x1: //Branch on Result Plus
            return !GetCPUFlag(NEGATIVE_FLAG);
        break;
        case 0x3: //Branch on Result Minus
            return GetCPUFlag(NEGATIVE_FLAG);
        break;
        case 0x5: //Branch on Overflow Clear
            return !GetCPUFlag(OVERFLOW_FLAG);
        break;
        case 0x7: //Branch on Overflow Set
            return GetCPUFlag(OVERFLOW_FLAG);
        break;
        case 0x9: //Branch on Carry Clear
            return !GetCPUFlag(CARRY_FLAG);            
        break;
        case 0xB: //Branch on Carry Set
            return GetCPUFlag(CARRY_FLAG); 
        break;
        case 0xD: //Branch on Not Equal (to zero)
            return !GetCPUFlag(ZERO_FLAG); 
        break;
        case 0xF: //Branch on Equal (to zero)
            return GetCPUFlag(ZERO_FLAG); 
        break;
        default:
            return -1;
        break;
    }
}

/*
    Gets the write location from a certain opcode, and increments the cpuCyclesToBurn
    in the corresponding InstructionData accordingly.
*/
unsigned short GetWriteFromOpcode(unsigned char opcode, struct InstructionData* out) {
    char upper = (opcode & 0xF0) >> 4;
    char lower = (opcode & 0x0F);
    char upperOdd = upper & 0x1;
    char lowerOdd = lower & 0x1;
    (void)out;
    unsigned short absAddr = ((unsigned short)GetLoByte()) << 8 | ((unsigned short)GetHiByte()); //endianness....

    if (lower == 0x08 || lower == 0x0A) {
        return 0x0;
    }
    if (lower >= 0x04 && lower <= 0x07) { //zp/zpx/zpy
        snprintf(operandFormatting, 8, "$%02x", GetHiByte());
        return 
        (lowerOdd ? ((unsigned char)GetRegisterValue(GetReadRegisterFromOpcode(opcode))) : 0) + GetHiByte();
    }
    if (lower >= 0x0C && lower <= 0x0F) { //abs/abx/aby
        snprintf(operandFormatting, 8, "$%04x", absAddr);
        return absAddr;
    }
    if (lower == 0x9) { //imm/aby
        snprintf(operandFormatting, 8, upperOdd ? "$%04x" : "$%02x", upperOdd ? absAddr : GetHiByte());
        return upperOdd ? absAddr : GetHiByte();
    }
    if (lower == 0x0) { //abs/rel
        snprintf(operandFormatting, 8, "$%04x", absAddr);
        return (upperOdd ? GetRegisterValue(PC) : 0) + absAddr;
    }
    if (lower == 0x1) { //izx/izy
        snprintf(operandFormatting, 8, "$%02x", GetHiByte());
        unsigned char ptr = GetReadRegisterFromOpcode(opcode) + GetHiByte();
        return ReadShortFromAddress(ptr);
    }
    return 0x0;
}

/*
    Gets the read bytes from a certain opcode, and increments the cpuCyclesToBurn
    in the corresponding InstructionData accordingly.
*/
unsigned char GetReadFromOpcode(unsigned char opcode, struct InstructionData* out) {
    char upper = (opcode & 0xF0) >> 4;
    char lower = (opcode & 0x0F);
    unsigned short absAddr = ((unsigned short)GetLoByte()) << 8 | ((unsigned short)GetHiByte()); //endianness....

    switch (lower) {
        case 0x08:
        case 0x0A: //Inter-register/stack operations
            return 0;
            break;

        case 0x04:
        case 0x05:
        case 0x06: //zp/zpx/zpy
            snprintf(operandFormatting, 8, "$%02x", GetHiByte());
            return ReadZeroPage(GetReadRegisterFromOpcode(opcode), GetHiByte(), out);
        break;

        case 0x0C:
        case 0x0D:
        case 0x0E: //abs/abx/aby
            snprintf(operandFormatting, 8, "$%04x", absAddr);
            return ReadAbsolute(GetReadRegisterFromOpcode(opcode),absAddr, out);        
        break;
        case 0x9: //imm/aby
            snprintf(operandFormatting, 8, (upper & 0x1) ? "$%04x" : "$%02x", (upper & 0x1) ? absAddr : GetHiByte());
            return (upper & 0x1) ? ReadAbsolute(GetReadRegisterFromOpcode(opcode), absAddr, out) : GetHiByte();
        break;
        case 0x0: //abs/rel
            snprintf(operandFormatting, 8, "$%04x", absAddr);
            return ReadAbsolute((upper & 0x1) ? PC : -1, absAddr, out);
        break;
        case 0x1: //izx/izy
            snprintf(operandFormatting, 8, "$%02x", GetHiByte());
            out->cpuCyclesToBurn += 4;
            return ReadIndirectIndexed(GetReadRegisterFromOpcode(opcode), GetHiByte(), out);   
        break;
        case 0x2: //The fact that there is ONE valid instruction in this column HAS to be ragebait.
            return GetHiByte();
        break;

        default:
        return 0;

    }

}

/*
For the general structure of all instructions here:
We're going to compute the value or instruction all at once, then return a struct from each instruction
telling the CPU how many cycles to idle for, where to put a value, and what the value is.
This way, we can avoid having to implement the digital logic in software and instead deal with the instruction
however we want.
*/


struct InstructionData ORA(unsigned char opcode) {
    struct InstructionData id = newInstructionData();
    
    id.destinationType = 0x00;
    id.destIdx = ACC;
    id.value = GetRegisterValue(ACC) | GetReadFromOpcode(opcode, &id);
    return id;
}

struct InstructionData BRK(unsigned char opcode)
{
    (void)opcode;
    SetHaltFlag();
    return newInstructionData();
}

struct InstructionData LOD(unsigned char opcode) {
    struct InstructionData id = newInstructionData();
    id.destinationType = 0x00;
    id.destIdx = GetActionRegisterFromOpcode(opcode);
    id.value = GetReadFromOpcode(opcode, &id);
    SetCPUFlag(ZERO_FLAG, id.value == 0);
    SetCPUFlag(NEGATIVE_FLAG, id.value >> 7);
    return id;
}

struct InstructionData EOR(unsigned char opcode) {
    struct InstructionData id = newInstructionData();
    
    id.destinationType = 0x00;
    id.destIdx = ACC;
    id.value = GetRegisterValue(ACC) ^ GetReadFromOpcode(opcode, &id);
    SetCPUFlag(ZERO_FLAG, GetRegisterValue(ACC) == 0);
    SetCPUFlag(NEGATIVE_FLAG, GetRegisterValue(ACC) >> 7);
    return id;
}

struct InstructionData STO(unsigned char opcode) {
    struct InstructionData id = newInstructionData();
    id.destinationType = 0xFF;
    id.destIdx = GetWriteFromOpcode(opcode, &id);
    id.value = GetRegisterValue(GetActionRegisterFromOpcode(opcode));
    return id;
}

struct InstructionData INC(unsigned char opcode) {
    struct InstructionData id = newInstructionData();
    id.destinationType = 0xFF;
    id.destIdx = GetWriteFromOpcode(opcode, &id);
    id.value = ReadFromAddress(id.destIdx) + 1;
    id.cpuCyclesToBurn++; //Because we employed a read, that'll be one more cycle
    SetCPUFlag(ZERO_FLAG, id.value == 0);
    SetCPUFlag(NEGATIVE_FLAG, id.value >> 7);
    return id;
}

struct InstructionData DEC(unsigned char opcode) {
    struct InstructionData id = newInstructionData();
    id.destinationType = 0xFF;
    id.destIdx = GetWriteFromOpcode(opcode, &id);
    id.value = ReadFromAddress(id.destIdx) - 1;
    SetCPUFlag(ZERO_FLAG, id.value == 0);
    SetCPUFlag(NEGATIVE_FLAG, id.value >> 7);
    id.cpuCyclesToBurn++; //Same deal here
    return id;
}

struct InstructionData ASL(unsigned char opcode) {
    struct InstructionData id = newInstructionData();
    id.destinationType = 0xFF;
    id.destIdx = GetWriteFromOpcode(opcode, &id);
    id.value = ReadFromAddress(id.destIdx) << 1; 
    SetCPUFlag(ZERO_FLAG, GetRegisterValue(ACC) == 0);
    SetCPUFlag(NEGATIVE_FLAG, id.value >> 7);
    id.cpuCyclesToBurn++;  //and here...
    return id;
}

struct InstructionData BRA(unsigned char opcode) {
    struct InstructionData id = newInstructionData();
    id.destinationType = 0x00;
    id.destIdx = shouldBranch(opcode) ? PC : -1; 
    id.value = GetRegisterValue(PC) + GetReadFromOpcode(opcode, &id) - 1; //Slowly starting to regret using the program counter to read all instruction bytes...
    return id;                                                            //Fortunately, all conditional branches are the same size, so we can do this with little consequence!
}

struct InstructionData JSR(unsigned char opcode) {
    struct InstructionData id = newInstructionData();
    (void)opcode;
    PushOntoStack(GetRegisterValue(PC), GetRegisterValue(SP),2); //TODO create a thing for pushing values onto the stack
    SetRegister(SP, GetRegisterValue(SP)+2);
    id.destinationType = 0x00;
    id.destIdx = PC;
    unsigned short additive = ((unsigned short)GetLoByte() << 8) | ((unsigned short)GetHiByte());
    id.value = NES_CART_ROM_MIN + additive;
    id.cpuCyclesToBurn = 3;
    return id;
}

struct InstructionData NOP(unsigned char opcode) {
    (void)opcode;
    return newInstructionData();
}

struct InstructionData BIT(unsigned char opcode) {
    struct InstructionData id = newInstructionData();
    unsigned char andTest = GetRegisterValue(ACC) & GetReadFromOpcode(opcode, &id);
    //Set Negative
    SetCPUFlag(NEGATIVE_FLAG, (0x80 & andTest) >> 7);
    //Set Overflow
    SetCPUFlag(OVERFLOW_FLAG, (0x40 & andTest) >> 6);
    return id;
}

struct InstructionData ROL(unsigned char opcode) {
    struct InstructionData id = newInstructionData();
    char oper = opcode == 0x2A ? GetRegisterValue(ACC) : GetReadFromOpcode(opcode, &id);
    unsigned char carryTemp = GetCPUFlag(CARRY_FLAG);
    SetCPUFlag(CARRY_FLAG, oper >> 7);
    oper = oper << 1;
    oper &= carryTemp;

    id.destinationType = opcode == 0x2A ? 0x00 : 0xFF;
    id.destIdx = opcode == 0x2A ? ACC : GetWriteFromOpcode(opcode, &id);
    id.cpuCyclesToBurn++; //Dummy read.
    return id;
}
struct InstructionData ROR(unsigned char opcode) {
    struct InstructionData id = newInstructionData();
    char oper = opcode == 0x2A ? GetRegisterValue(ACC) : GetReadFromOpcode(opcode, &id);
    unsigned char carryTemp = GetCPUFlag(CARRY_FLAG);
    SetCPUFlag(CARRY_FLAG, oper & 0x1);
    oper = oper >> 1;
    oper &= (carryTemp << 7);

    id.destinationType = opcode == 0x2A ? 0x00 : 0xFF;
    id.destIdx = opcode == 0x2A ? ACC : GetWriteFromOpcode(opcode, &id);
    id.cpuCyclesToBurn++; //Dummy read.
    return id;
}
/*
    This instruction requires pulling both the CPU flag Register
    and the Program Counter off the stack. We're either gonna have to figure out something tricksy
    (i.e, making a secret third location involving both the Program Counter and the CPU flag register, 
    where we shove that into the destinationType and then the PC value is in the value short while the 
    CPU flag register value is in the destIdx), or we just have to refactor our way of processing this, 
    preferably using pointers or whatever.

    *Sigh* We're going with the tricksy solution, aren't we?
*/
struct InstructionData RTI(unsigned char opcode) {
    (void)opcode;
    struct InstructionData id = newInstructionData();
    id.destinationType = 0x0F; //PC + Flag Register
    id.destIdx = ((unsigned char)PopFromStack(GetRegisterValue(SP), 1));
    SetRegister(SP, GetRegisterValue(SP) - 1);
    id.value = PopFromStack(GetRegisterValue(SP), 2); //TODO: this might be wrong...? Come back and reexamine
    SetRegister(SP, GetRegisterValue(SP) - 2);
    id.cpuCyclesToBurn = 5;
    return id;
}
struct InstructionData RTS(unsigned char opcode) {
    (void)opcode;
    struct InstructionData id = newInstructionData();
    id.destinationType = 0x00; 
    id.destIdx = PC;
    id.value = PopFromStack(GetRegisterValue(SP), 2); //TODO: this might be wrong...? Come back and reexamine
    SetRegister(SP, GetRegisterValue(SP) - 2);
    id.cpuCyclesToBurn = 5;
    return id;
}
struct InstructionData AND(unsigned char opcode) {
    struct InstructionData id = newInstructionData();
    
    id.destinationType = 0x00;
    id.destIdx = ACC;
    id.value = GetRegisterValue(ACC) & GetReadFromOpcode(opcode, &id);
    SetCPUFlag(ZERO_FLAG, GetRegisterValue(ACC) == 0);
    SetCPUFlag(NEGATIVE_FLAG, GetRegisterValue(ACC) >> 7);
    return id;
}
struct InstructionData ADC(unsigned char opcode) {
    struct InstructionData id = newInstructionData();
    id.destinationType = 0x00;
    id.destIdx = ACC;
    unsigned char regValue = GetRegisterValue(ACC);
    unsigned char carryFlagValue = GetCPUFlag(CARRY_FLAG);
    unsigned char read = GetReadFromOpcode(opcode, &id);
    id.value = (unsigned char)(regValue + carryFlagValue + read);
    SetCPUFlag(ZERO_FLAG, id.value == 0);
    SetCPUFlag(OVERFLOW_FLAG, 0); //TODO: this and the below are incorrect. Come back and fix this later.
    SetCPUFlag(CARRY_FLAG, 0);
    return id;
}
struct InstructionData JMP(unsigned char opcode) {
    struct InstructionData id = newInstructionData();
    id.destinationType = 0x00;
    id.destIdx = PC;
    id.value = GetRegisterValue(PC) + GetReadFromOpcode(opcode, &id);
    return id;
}
struct InstructionData TRA(unsigned char opcode) {
    struct InstructionData id = newInstructionData();
    unsigned short sourceAndDest = GetTransferRegisters(opcode);
    id.destinationType = 0x00;
    id.destIdx = 0x00FF & sourceAndDest;
    id.value = GetRegisterValue((0xFF00 & sourceAndDest) >> 8);
    id.cpuCyclesToBurn = 1;
    return id;
}
struct InstructionData CMP(unsigned char opcode) {
    struct InstructionData id =  newInstructionData();
    unsigned char isACmp = opcode & 0x1; //CMP is implemented across the standard instruction set on odd low-nibbles, so we can do this with no consequence.
    unsigned char regVal = GetRegisterValue(ACC);
    if (!isACmp) {
        regVal = GetRegisterValue((opcode & 0xF0) == 0xE0 ? RX : RY);
    }
    unsigned char memVal = GetReadFromOpcode(opcode, &id);
    SetCPUFlag(CARRY_FLAG, regVal >= memVal);
    SetCPUFlag(ZERO_FLAG, regVal == memVal);
    SetCPUFlag(NEGATIVE_FLAG, (regVal - memVal) >> 7);

    return id;
}
struct InstructionData CLR(unsigned char opcode) {
    unsigned char flag;
    switch(opcode) {
        case 0x18: //CLC
            flag = INT_FLAG;
        break;
        case 0xD8: //CLD
            flag = DECIMAL_FLAG;
        break;
        case 0x58: //CLI
            flag = INT_FLAG;
        break;
        case 0xB8: //CLV
            flag = OVERFLOW_FLAG;
        break;
        default:
        break;
    }
    SetCPUFlag(flag, 0);
    struct InstructionData id = newInstructionData();
    id.cpuCyclesToBurn++;
    return id;
}
struct InstructionData SBC(unsigned char opcode) {
    (void)opcode;
    return newInstructionData();
}
struct InstructionData SET(unsigned char opcode) {
    unsigned char flag;
    switch(opcode) {
        case 0x78: //SEI
            flag = INT_FLAG;
        break;
        case 0xF8: //SED
            flag = DECIMAL_FLAG;
        break;
        case 0x38: //SEC
            flag = CARRY_FLAG;
        break;
        default:
        break;
    }
    SetCPUFlag(flag, 1);
    struct InstructionData id = newInstructionData();
    id.cpuCyclesToBurn++;
    return id;
}
struct InstructionData LSR(unsigned char opcode) {
    struct InstructionData id = newInstructionData();
    unsigned char isReg = opcode == 0x4A;
    id.destinationType = isReg ? 0x00 : 0xFF;
    id.destIdx = isReg ? ACC : GetWriteFromOpcode(opcode, &id);
    unsigned char oper = isReg ? GetRegisterValue(ACC) : GetReadFromOpcode(opcode, &id);
    unsigned char carryValue = oper & 0x1; //Clears out the upper seven bits. 
    oper >>= 1;
    SetCPUFlag(CARRY_FLAG, carryValue);
    id.value = oper;
    id.cpuCyclesToBurn++; //Baseline, one more cycle. 
    return id;
}
struct InstructionData PUS(unsigned char opcode) {
    struct InstructionData id = newInstructionData(); 
    PushOntoStack(GetRegisterValue(opcode == 0x08 ? FL : ACC), GetRegisterValue(SP), 1);
    SetRegister(SP, GetRegisterValue(SP) + 1);
    id.cpuCyclesToBurn = 2;
    return id;
}
struct InstructionData PUL(unsigned char opcode) {
    struct InstructionData id = newInstructionData(); 
    SetRegister((opcode == 0x28 ? FL : ACC), PopFromStack(GetRegisterValue(SP) & 0x00FF, 1));
    SetRegister(SP, GetRegisterValue(SP) - 1);
    id.cpuCyclesToBurn = 3;
    return id;
}
struct InstructionData INR(unsigned char opcode) {
    struct InstructionData id = newInstructionData();
    unsigned char reg = (opcode == 0xE8 ? RX : RY);
    id.destinationType = 0x00;
    id.destIdx = reg;
    id.value =  GetRegisterValue(reg) + 1;
    SetCPUFlag(ZERO_FLAG, GetRegisterValue(ACC) == 0);
    SetCPUFlag(NEGATIVE_FLAG, GetRegisterValue(ACC) >> 7);
    id.cpuCyclesToBurn = 1;
    return id;
}
struct InstructionData DER(unsigned char opcode) {
    struct InstructionData id = newInstructionData();
    unsigned char reg = (opcode == 0xCA ? RX : RY);
    id.destinationType = 0x00;
    id.destIdx = reg;
    id.value = GetRegisterValue(reg) - 1;
    SetCPUFlag(ZERO_FLAG, GetRegisterValue(reg) == 0);
    SetCPUFlag(NEGATIVE_FLAG, GetRegisterValue(reg) >> 7);
    id.cpuCyclesToBurn = 1;
    return id;
}
struct InstructionData INV(unsigned char opcode) {
    snprintf(operandFormatting, 8, "INVALID");
    (void)opcode;
    return newInstructionData();
}