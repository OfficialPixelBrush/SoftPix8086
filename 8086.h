#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

union MainRegister {
    uint16_t x;
    struct {
        uint8_t l;
        uint8_t h;
    };
};

typedef union MainRegister MainRegister;
typedef uint16_t IndexRegister;
typedef uint16_t ProgramCounter;
typedef uint16_t SegmentRegister;
typedef uint16_t Flags;

typedef uint32_t Address;

enum Flag {
    CF =  0, // Carry Flag
    PF =  2, // Parity Flag
    AF =  4, // Half-carry Flag
    ZF =  6, // Zero Flag
    SF =  7, // Sign Flag
    TF =  8, // Trap Flag
    IF =  9, // Interrupt Flag
    DF = 10, // Direction Flag
    OF = 11  // Overflow Flag
};

typedef enum Flag Flag;

enum Opcode {
    // 0x0X
    ADD_rm8_r8  = 0x00,
    ADD_rm16_r16,
    ADD_r8_rm8,
    ADD_r16_rm16,
    ADD_AL_d8,
    ADD_AL_d16,
    PUSH_ES     = 0x06,
    POP_ES      = 0x07,
    OR_rm8_r8   = 0x08,
    OR_rm16_r16,
    OR_r8_rm8,
    OR_r16_rm16,
    OR_AL_d8,
    OR_AL_d16,
    PUSH_CS     = 0x0E,
    POP_CS      = 0x0F,
    // 0x1X
    ADC_rm8_r8  = 0x10,
    ADC_rm16_r16,
    ADC_r8_rm8,
    ADC_r16_rm16,
    ADC_AL_d8,
    ADC_AL_d16,
    PUSH_SS     = 0x16,
    POP_SS      = 0x17,
    SBB_rm8_r8  = 0x18,
    SBB_rm16_r16,
    SBB_r8_rm8,
    SBB_r16_rm16,
    SBB_AL_d8,
    SBB_AL_d16,
    PUSH_DS     = 0x1E,
    POP_DS      = 0x1F,
    // 0x2X
    AND_rm8_r8  = 0x20,
    AND_rm16_r16,
    AND_r8_rm8,
    AND_r16_rm16,
    AND_AL_d8,
    AND_AL_d16,
    ES          = 0x26,
    DAA         = 0x27,
    SUB_rm8_r8  = 0x28,
    SUB_rm16_r16,
    SUB_r8_rm8,
    SUB_r16_rm16,
    SUB_AL_d8,
    SUB_AL_d16,
    CS          = 0x2E,
    DAS         = 0x2F,
    // 0x3X
    XOR_rm8_r8  = 0x30,
    XOR_rm16_r16,
    XOR_r8_rm8,
    XOR_r16_rm16,
    XOR_AL_d8,
    XOR_AL_d16,
    SS          = 0x36,
    AAA         = 0x37,
    CMP_rm8_r8  = 0x38,
    CMP_rm16_r16,
    CMP_r8_rm8,
    CMP_r16_rm16,
    CMP_AL_d8,
    CMP_AL_d16,
    DS          = 0x3E,
    AAS         = 0x3F,
    // 0x4X
    INC_AX      = 0x40,
    INC_CX      = 0x41,
    INC_DX      = 0x42,
    INC_BX      = 0x43,
    INC_SP      = 0x44,
    INC_BP      = 0x45,
    INC_SI      = 0x46,
    INC_DI      = 0x47,
    DEC_AX      = 0x48,
    DEC_CX      = 0x49,
    DEC_DX      = 0x4A,
    DEC_BX      = 0x4B,
    DEC_SP      = 0x4C,
    DEC_BP      = 0x4D,
    DEC_SI      = 0x4E,
    DEC_DI      = 0x4F,
    // 0x5X
    PUSH_AX     = 0x50,
    PUSH_CX     = 0x51,
    PUSH_DX     = 0x52,
    PUSH_BX     = 0x53,
    PUSH_SP     = 0x54,
    PUSH_BP     = 0x55,
    PUSH_SI     = 0x56,
    PUSH_DI     = 0x57,
    POP_AX      = 0x58,
    POP_CX      = 0x59,
    POP_DX      = 0x5A,
    POP_BX      = 0x5B,
    POP_SP      = 0x5C,
    POP_BP      = 0x5D,
    POP_SI      = 0x5E,
    POP_DI      = 0x5F,
    // 0x6X - Empty
    // 0x7X
    JO_rel8     = 0x70,
    JNO_rel8    = 0x71,
    JB_rel8     = 0x72,
    JC_rel8     = 0x72, // Alt
    JNB_rel8    = 0x73,
    JNC_rel8    = 0x73, // Alt
    JE_rel8     = 0x74,
    JNE_rel8    = 0x75,
    JBE_rel8    = 0x76,
    JNBE_rel8   = 0x77,
    JS_rel8     = 0x78,
    JNS_rel8    = 0x79,
    JP_rel8     = 0x7A,
    JNP_rel8    = 0x7B,
    JL_rel8     = 0x7C,
    JNL_rel8    = 0x7D,
    JLE_rel8    = 0x7E,
    JNLE_rel8   = 0x7F,
    // 0x8X
    // 0x9X
    NOP         = 0x90,

    CALL_seg_a16= 0x9A,
    WAIT        = 0x9B,
    PUSHF       = 0x9C,
    POPF        = 0x9D,
    SAHF        = 0x9E,
    LAHF        = 0x9F,
    // 0xAX
    MOV_AL_addr = 0xA0,
    MOV_AX_addr = 0xA1,
    MOV_addr_AL = 0xA2,
    MOV_addr_AX = 0xA3,
    // 0xBX
    MOV_AL_d8   = 0xB0,
    MOV_CL_d8   = 0xB1,
    MOV_DL_d8   = 0xB2,
    MOV_BL_d8   = 0xB3,
    MOV_AH_d8   = 0xB4,
    MOV_CH_d8   = 0xB5,
    MOV_DH_d8   = 0xB6,
    MOV_BH_d8   = 0xB7,
    MOV_AX_d16  = 0xB8,
    MOV_CX_d16  = 0xB9,
    MOV_DX_d16  = 0xBA,
    MOV_BX_d16  = 0xBB,
    MOV_SP_d16  = 0xBC,
    MOV_BP_d16  = 0xBD,
    MOV_SI_d16  = 0xBE,
    MOV_DI_d16  = 0xBF,
    // 0xCX
    inval_0xc1  = 0xC1,
    inval_0xc2  = 0xC2,
    RET_d16     = 0xC3,
    RET         = 0xC4,
    // 0xDX
    // 0xEX
    CALL_rel16  = 0xE8,
    JMP_rel16   = 0xE9,
    JMP_reg_a16 = 0xEA,
    JMP_rel8    = 0xEB,
    // 0xFX
    HLT         = 0xF4,
    CLI         = 0xFA,
    STI         = 0xFB,
};

typedef enum Opcode Opcode;

// 1 Mebibyte, 1024 Kibibytes, 1048575 Bytes
#define TOTAL_MEMORY_SIZE (1 << (16 + 4)) -1

struct CPU {
    uint8_t memory[TOTAL_MEMORY_SIZE];
    // Out-facing registers
    MainRegister a, b, c, d;
    IndexRegister sp, bp, si, di;
    ProgramCounter ip;
    SegmentRegister es, cs, ss, ds;
    Flags flags;

    bool ReadFlag(Flag flag) {
        return (flags >> flag) & 0x1;
    }
    void WriteFlag(Flag flag, bool value) {
        flags = (flags & ~(1 << flag)) | (value << flag);
    }
    Address GetAddress(SegmentRegister& reg) {
        Address addr = (uint32_t(reg) << 4) + uint32_t(ip);
        //printf("0x%05X - (0x%04X:0x%04X)\n", addr & 0xFFFFF, reg & 0xFFFFF, ip & 0xFFFFF);
        return addr;
    }

    uint8_t ReadByte() {
        uint8_t val = memory[GetAddress(cs)];
        ip++;
        return val;
    }

    void WriteByte(uint8_t value, uint32_t address) {
        memory[address] = value;
    }

    uint16_t ReadWord() {
        return ((uint16_t)ReadByte() << 8 | (uint16_t)ReadByte());
    }

    void WriteWord(uint16_t value, uint32_t address) {
        WriteByte((uint8_t)((value & 0xFF) >> 8), address);
        WriteByte((uint8_t)((value & 0xFF)), address+1);
    }

    void ByteAdd(uint8_t& a, uint8_t& b) {
        a = a + b;
    }

    void ByteSubtract(uint8_t& a, uint8_t& b) {
        a = a - b;
    }

    void Reset() {
        ip = 0x005B;
        cs = 0x0000; //0xFFFF;
        ds = 0x0000;
        es = 0x0000;
        ss = 0x0000;
        flags = 0x0002;
    }

    void PrintFlags() {
        if (ReadFlag(OF)) {
            printf("O");
        } else {
            printf("o");
        }
        if (ReadFlag(DF)) {
            printf("D");
        } else {
            printf("d");
        }
        if (ReadFlag(IF)) {
            printf("I");
        } else {
            printf("i");
        }
        if (ReadFlag(TF)) {
            printf("T");
        } else {
            printf("t");
        }
        if (ReadFlag(SF)) {
            printf("S");
        } else {
            printf("s");
        }
        if (ReadFlag(ZF)) {
            printf("Z");
        } else {
            printf("z");
        }
        if (ReadFlag(AF)) {
            printf("A");
        } else {
            printf("a");
        }
        if (ReadFlag(PF)) {
            printf("P");
        } else {
            printf("p");
        }
        if (ReadFlag(CF)) {
            printf("C");
        } else {
            printf("c");
        }
        printf("\n");
    }

    void PrintRegisters() {
        printf("AX: %04X - BX: %04X - CX: %04X - DX: %04X\n", a.x, b.x, c.x, d.x);
        //printf("SP: %04X - BP: %04X - SI: %04X - DI: %04X\n", sp, bp, si, di);
        //printf("ES: %04X - CS: %04X - SS: %04X - DS: %04X\n", es, cs, ss, ds);
        printf("IP: %04X - Flags: ", ip);
        PrintFlags();
    }

    int cycle = 0;
    bool Process() {
        printf("\nCycle #%d\n", cycle++);
        uint8_t opcode = ReadByte() & 0xFF;
        printf("Opcode: 0x%02X ", opcode & 0xFF);
        switch(opcode) {
            case INC_AX:
                a.x++;
                break;
            case INC_BX:
                b.x++;
                break;
            case INC_CX:
                b.x++;
                break;
            case INC_DX:
                d.x++;
                break;
            case INC_SP:
                sp++;
                break;
            case INC_BP:
                bp++;
                break;
            case INC_SI:
                si++;
                break;
            case INC_DI:
                di++;
                break;
            case DEC_AX:
                a.x--;
                break;
            case DEC_BX:
                b.x--;
                break;
            case DEC_CX:
                b.x--;
                break;
            case DEC_DX:
                d.x--;
                break;
            case DEC_SP:
                sp--;
                break;
            case DEC_BP:
                bp--;
                break;
            case DEC_SI:
                si--;
                break;
            case DEC_DI:
                di--;
                break;
            case JMP_rel8:
                printf("(JMP_rel8)");
                ip = ip + (int8_t)ReadByte();
                break;
            case CLI:
                printf("(CLI)");
                // clear interrupt
                break;
            case SAHF:
                printf("(SAHF)");
                flags = a.h;
                break;
            case MOV_AH_d8:
                printf("(MOV_AH_d8)");
                a.h = ReadByte();
                break;
            case JC_rel8: {
                printf("(JC_rel8)");
                int8_t val = (int8_t)ReadByte();
                if (ReadFlag(CF))
                    ip = ip + val;
                break;
            }
            case JNC_rel8: {
                printf("(JNC_rel8)");
                int8_t val = (int8_t)ReadByte();
                if (!ReadFlag(CF))
                    ip = ip + val;
                break;
            }
            case JE_rel8: {
                printf("(JE_rel8)");
                int8_t val = (int8_t)ReadByte();
                if (ReadFlag(ZF))
                    ip = ip + val;
                break;
            }
            case JNE_rel8: {
                printf("(JNE_rel8)");
                int8_t val = (int8_t)ReadByte();
                if (!ReadFlag(ZF))
                    ip = ip + val;
                break;
            }
            case JP_rel8: {
                printf("(JP_rel8)");
                int8_t val = (int8_t)ReadByte();
                if (ReadFlag(PF))
                    ip = ip + val;
                break;
            }
            case JNP_rel8: {
                printf("(JNP_rel8)");
                int8_t val = (int8_t)ReadByte();
                if (!ReadFlag(PF))
                    ip = ip + val;
                break;
            }
            case JS_rel8: {
                printf("(JS_rel8)");
                int8_t val = (int8_t)ReadByte();
                if (ReadFlag(SF))
                    ip = ip + val;
                break;
            }
            case JNS_rel8: {
                printf("(JNS_rel8)");
                int8_t val = (int8_t)ReadByte();
                if (!ReadFlag(SF))
                    ip = ip + val;
                break;
            }
            case LAHF:
                printf("(LAHF)");
                a.h = flags & 0xFF;
            case HLT:
                printf("(HLT)");
                while(1) {}
                break;
            case NOP:
            default:
                printf("(NOP)");
                break;
        }
        printf("\n");
        PrintRegisters();
        sleep(1);
        return true;
    }
};

typedef struct CPU CPU;