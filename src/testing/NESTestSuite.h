#include <assert.h>
#include "../interpreter/CPU.h"
#include "../interpreter/ROM.h"

void LoadBIN(char* binpath);

void RunUntilHalt();

void RunUntilPCValue(unsigned short value);

void AssertFlag(unsigned char flag, int value);

void AssertFlags(unsigned char flagValue);

void AssertRegister(unsigned char reg, unsigned short value);

