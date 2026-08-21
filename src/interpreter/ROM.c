#include "ROM.h"
#include "CPU.h"
#include<stdio.h>
#include<stdlib.h>

void* ROM;

int LoadROM(char* filepath) {
    FILE* romfile = fopen(filepath, "rb");
    if (romfile == NULL) {return -1;}
   

    if (fseek(romfile, 0, SEEK_END) != 0) {
        fclose(romfile);
        return -1;
    }
    size_t romSize = ftell(romfile);
    void* myRom = malloc(romSize);
    rewind(romfile);

    fread(myRom,sizeof(char), romSize, romfile);
    fclose(romfile);
    ROM = myRom;
    printf("Successfully loaded ROM \"%s\"\n", filepath);
    return 0;
}

unsigned char ReadFromROM(unsigned short idx) { 
    return *((unsigned char*)ROM + (idx -  (GetEmulationMode() ? 0x8000 : 0x7FF0)));
}

void UnloadROM() {
    free(ROM);
}