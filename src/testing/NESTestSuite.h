#include "unity.h"
#include "../interpreter/CPU.h"
#include "../interpreter/ROM.h"

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
    TEST_ASSERT_EQUAL(value,GetCPUFlag(flag));
}

void AssertFlags(unsigned char flagValue) {
    TEST_ASSERT_EQUAL(flagValue,GetRegisterValue(FL));
}

void AssertRegister(unsigned char reg, unsigned short value){
    TEST_ASSERT_EQUAL(value, GetRegisterValue(reg));
}