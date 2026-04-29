#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "8086.h"

bool LoadRom(uint8_t* memory, const char* path, uint32_t start_address) {
    FILE* fptr = fopen(path, "rb");
    if (fptr == NULL) {
        printf("ROM image could not be opened.\n");
        return false;
    }

    // Move to end to get size
    fseek(fptr, 0, SEEK_END);
    long size = ftell(fptr);
    rewind(fptr);

    if (size <= 0) {
        printf("Invalid ROM size!\n");
        fclose(fptr);
        return false;
    }

    if (start_address + (uint32_t)size > TOTAL_MEMORY_SIZE) {
        printf("ROM spills outside of memory!\n");
        fclose(fptr);
        return false;
    }

    size_t read = fread(memory + start_address, 1, size, fptr);
    if (read != size) {
        printf("Failed to read full ROM file (read %zu of %ld bytes).\n", read, size);
        fclose(fptr);
        return false;
    }

    fclose(fptr);
    printf("%zu Bytes read\n", read);
    return true;
}

void ZeroMemory(uint8_t* memory) {
    memset(memory, 0, TOTAL_MEMORY_SIZE);
}

int main() {
    CPU cpu;
    ZeroMemory(cpu.memory);
    LoadRom(cpu.memory, "PCBIOS-REV1.bin", 0x0000); //0x0F000);
    printf("%d\n", TOTAL_MEMORY_SIZE);
    cpu.Reset();
    while(1) {
        cpu.Process();
    }
    return 0;
}