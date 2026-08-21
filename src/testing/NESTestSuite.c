#include "NESTestSuite.h"

void LoadBIN(char *binpath)
{
    LoadROM(binpath);
}

void RunUntilHalt()
{
    while (Step()) {
        
    }
}

void RunUntilPCValue(unsigned short value) {
    while(GetRegisterValue(PC) <= value) {
        Step();
    }
}

void AssertFlag(unsigned char flag, int value) {
    assert(GetCPUFlag(flag) == value);
}

void AssertFlags(unsigned char flagValue) {
    assert(GetRegisterValue(FL) == flagValue);
}

void AssertRegister(unsigned char reg, unsigned short value){
    assert(GetRegisterValue(reg) == value);
}