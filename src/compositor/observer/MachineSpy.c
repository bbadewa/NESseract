#include"../lib/raygui.h"
#define RAYGUI_IMPLEMENTATION
#include"../lib/raygui.h"

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"../../interpreter/CPU.h"
#include"../../interpreter/ROM.h"

const char* flagNames[8] = {"Carry", "Zero", "Interrupt", "Decimal", "Break", "Constant", "Overflow", "Negative" };
const char* Registers[6] = {"Program Counter", "Stack Pointer", "Accumulator", "Index X", "Index Y", "Flags"};

void* CPURunner(void* nothing) {
    (void)nothing; //For example, nothing. -Mbappe 2026
    while (!WindowShouldClose()) {
        Step();
        sleep(1);
    }
}

int main() {
    LoadROM("../testroms/example.bin");
    CPUInit();
    InitWindow(800,650, "NESseract (MachineSpy 6502 mode)");
    SetTargetFPS(60);

    char currentByte[5];
    char registerStateThisFrame[6][7]; //hehe
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
            
            GuiLabel((Rectangle){250,0,400,50},BASE_6502_EMU ? "Running in stock 6502 mode" : "Running in NTSC NES mode");

            //CPU flags;
            for (int i = 0; i < 8; i++) {
                    GuiSetStyle(DEFAULT, TEXT_SIZE, 12);
                    GuiLabel((Rectangle){10 + (i * 100), 50, 80,50},flagNames[7-i]);
                    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
                    GuiLabel((Rectangle){25 + (i * 100), 25, 50,50},GetCPUFlag(7-i) ? "1" : "0");
            }
            
            //CPU registers;
            for (int i = 0; i < 6; i++) {
                GuiSetStyle(DEFAULT, TEXT_SIZE, 12);
                GuiLabel((Rectangle){10 ,100 + (i * 50), 120, 50},Registers[i]);
                snprintf(registerStateThisFrame[i], 7, "0x%04X", GetRegisterValue(i));
                GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
                GuiLabel((Rectangle){140, 100 + (i * 50), 80, 50},registerStateThisFrame[i]);
            }

            GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
            GuiLabel((Rectangle){10, 375, 300,90}, "Current Instruction");

            GuiSetStyle(DEFAULT, TEXT_SIZE, 36);
            GuiLabel((Rectangle){70, 425, 300,90}, GetLastInstruction());

            if(GuiButton((Rectangle){30,500, 150,45}, "Step") && !haltResult) {
                haltResult = Step();
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