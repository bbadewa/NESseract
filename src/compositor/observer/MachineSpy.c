#define RAYGUI_IMPLEMENTATION
#include"../lib/raylib.h"
#include"../lib/raygui.h"

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"../../interpreter/CPU.h"
#include"../../interpreter/ROM.h"

const char* flagNames[8] = {"Carry", "Zero", "Interrupt", "Decimal", "Break", "Constant", "Overflow", "Negative" };
const char* Registers[6] = {"Program Counter", "Stack Pointer", "Accumulator", "Index X", "Index Y", "Flags"};

char novelFlags = 0;
char novelRegisters = 0;
char flagRegThisCycle;
char registerStateThisCycle[6][7]; //hehe

void InitRegisterState() {
    char def[7] = "0x0000";
    for (int i = 0; i < 6; i++) {
        strncpy_s(registerStateThisCycle[i], 7, def, 7);
    }
}

int UpdateCPUState() {
    char tempflags = 0;
    char tempRegister[7];

    int cpuState = Step();
    novelFlags = 0;
    novelRegisters = 0;
    tempflags = GetRegisterValue(FL);
    //CPU flags;
    for (int i = 0; i < 8; i++) {
        if (((flagRegThisCycle >> i) & 1) != ((tempflags >> i) & 1)) { //Check new flags against last cycle's flags
            novelFlags |= (1 << i);
        }
    }

    //CPU registers;
    for (int i = 0; i < 6; i++) {
        snprintf(tempRegister, 7, "0x%04X", GetRegisterValue(i));
        if (strncmp(tempRegister, registerStateThisCycle[i], 7) != 0) {
            strncpy(registerStateThisCycle[i], tempRegister, 7);
            novelRegisters |= (1 << i);
        }
        
    }
    flagRegThisCycle = tempflags;
    return cpuState;
}

void* CPURunner(void* nothing) {
    (void)nothing; //For example, nothing. -Mbappe 2026
    
    while (!WindowShouldClose()) {
        usleep(3000);
        UpdateCPUState();
    } 

}

int main() {
    InitRegisterState();
    SetTraceLogLevel(LOG_NONE);
    LoadROM("../testroms/coin/5_Instructions1.nes");
    CPUInit();
    InitWindow(800,650, "NESseract (MachineSpy RP2A03 mode)");
    SetTargetFPS(60);

    char currentByte[5];
    
    unsigned char haltResult = 0;

    pthread_t CPUThreadHandle;
    if (pthread_create(&CPUThreadHandle, NULL, CPURunner, NULL)) {
        printf("Failed to create CPU thread!\n");
        return -1;
    }

    while(!WindowShouldClose()) {
        BeginDrawing();
            GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
            DrawRectangle(0,0,800,800,RAYWHITE);
            
            GuiLabel((Rectangle){250,0,400,50}, GetEmulationMode() ? "Running in stock 6502 mode" : "Running in NTSC NES mode");

            //CPU flags;
            for (int i = 0; i < 8; i++) {
                    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt((novelFlags >> i) & 1 ? RED : GRAY));
                    GuiSetStyle(DEFAULT, TEXT_SIZE, 12);
                    GuiLabel((Rectangle){10 + (i * 100), 50, 80,50},flagNames[7-i]);
                    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
                    GuiLabel((Rectangle){25 + (i * 100), 25, 50,50},(flagRegThisCycle >> i) & 1 ? "1" : "0");
            }
            GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(GRAY));
            //CPU registers;
            for (int i = 0; i < 6; i++) {
                GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt((((novelRegisters >> i) & 1) && i != 0) ? RED : GRAY));
                GuiSetStyle(DEFAULT, TEXT_SIZE, 12);
                GuiLabel((Rectangle){10 ,100 + (i * 50), 120, 50},Registers[i]);
                GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
                GuiLabel((Rectangle){140, 100 + (i * 50), 80, 50},registerStateThisCycle[i]);
            }

            GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(GRAY));
            GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
            GuiLabel((Rectangle){10, 375, 300,90}, "Current Instruction");

            GuiSetStyle(DEFAULT, TEXT_SIZE, 36);
            GuiLabel((Rectangle){70, 425, 300,90}, GetLastInstruction());

            if(GuiButton((Rectangle){30,500, 150,45}, "Step") && !haltResult) {
                haltResult = UpdateCPUState();
            }
            GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
            GuiLabel((Rectangle){10, 520, 300,90}, "Current Byte");
            snprintf(currentByte,5, "0x%02X", GetLastByte());
            GuiSetStyle(DEFAULT, TEXT_SIZE, 36);
            GuiLabel((Rectangle){70, 580, 300,90}, currentByte);


        EndDrawing();
    }
    CloseWindow();
    
}