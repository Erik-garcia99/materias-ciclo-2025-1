/******************************************************************************
Prac 2 - AVR ASM OpCode Decoder
*******************************************************************************/
/*garcia chavez erik 01275863
practica 2 microcontroladores
2025-1
*/
#include <stdio.h>
#include <inttypes.h>

const uint8_t flash_mem[] = {
    0x0F, 0xEF, // -> SER R0 1110 1111 0000 1111
    0x10, 0x0F, // -> 0000 1111 0001  0000 -> ADD r17,r16
    0x21, 0x2F, // -> 0010 1111 0010 0001 -> MOV r18,r17
    0x3F, 0xEF, // -> 1110 1111 0011 1111 -> SER r3 - LDI r3, 255
    0x32, 0x02, //-> 0000 0010 00011 0010 -> MULS r3, r2
    0x00, 0x00  // NOP
};

// SER - 1110 1111 DDDD 1111
// ldi - 1111 KKKK 0000 KKKK

const uint16_t inst16_table[] = {

    0x0,    // NOP
    0x9406, // LSR
    0x2c00, // MOV
    0x9C00, // MUL
    0x1c00, // ADC
    0x0c00, // ADD
    0x2000, // AND
    0x0400, // CPC
    0xE000, // LDI
    0xEF0F, // SER
    0x0200  // MULS

};

enum
{
    e_NOP,
    e_LSR,
    e_MOV,
    e_MUL,
    e_ADC,
    e_ADD,
    e_AND,
    e_CPC,
    e_LDI,
    e_SER,
    e_MULS
};

// Op Code struct
typedef union
{
    uint16_t op16; // e.g.: watchdog
    struct
    {
        uint16_t op4 : 4;
        uint16_t d5 : 5;
        uint16_t op7 : 7;
    } type1; // e.g: LSR
    struct
    {
        uint16_t r4 : 4;
        uint16_t d5 : 5;
        uint16_t r1 : 1;
        uint16_t op6 : 6;
    } type2; // e.g.: MOV,MUL,ADC,ADD,AND,CPC
    // TO-DO: Add more types as needed
    struct
    {
        uint16_t k4_L : 4;
        uint16_t d4 : 4;
        uint16_t k4_H : 4;
        uint16_t op4 : 4;
    } type3; // LDI,SER
    struct
    {
        uint16_t r4 : 4;
        uint16_t d : 4;
        uint16_t op8 : 8;
    } type4; // MULS

} Op_Code_t;

int main()
{
    Op_Code_t *instruction;

    printf("- Practica 2: AVR OpCode -\n");
    // Decode the instructions by cycling through the array
    for (uint8_t idx = 0; idx < sizeof(flash_mem); idx += 2)
    {
        // busca la instruccion NOP
        instruction = (Op_Code_t *)&flash_mem[idx];
        uint16_t op = instruction->op16;

        if (instruction->op16 == inst16_table[e_NOP])
        {
            printf("NOP\n");
            continue;
        }

        // LSR
        if ((op & 0xFF0F) == inst16_table[e_LSR])
        {
            printf("LSR %d", instruction->type1.d5);
            continue;
        }

        // ***********section type2***************
        if ((op & 0xFC00) == inst16_table[e_MOV])
        {
            uint8_t d = (op >> 5) & 0x1F;
            uint8_t r = op & 0x1F;

            printf("MOV r%d, r%d\n", d, r);
            continue;
        }

        else if ((op & 0xFC00) == inst16_table[e_MUL])
        {
            uint8_t d = (op >> 5) & 0x1F;
            uint8_t r = op & 0x1F;

            printf("MUL r%d , r%d\n", d, r);
            continue;
        }

        else if ((op & 0xFC00) == inst16_table[e_ADC])
        {

            uint8_t d = (op >> 5) & 0x1F;
            uint8_t r = op & 0x1F;

            printf("ADC r%d, r%d\n", d, r);
            continue;
        }

        // add
        else if ((op & 0xFC00) == inst16_table[e_ADD])
        {
            uint8_t d = (op >> 5) & 0x1F;
            uint8_t r = op & 0x1F;

            printf("ADD r%d,r%d\n", d, r);
            continue;
        }

        // and

        else if ((op & 0xFC00) == inst16_table[e_AND])
        {
            uint8_t d = (op >> 5) & 0x1F;
            uint8_t r = op & 0x1F;

            printf("AND r%d, r%d\n", d, r);
            continue;
        }

        // CPC

        else if ((op & 0xFC00) == inst16_table[e_CPC])
        {
            uint8_t d = (op >> 5) & 0x1F;
            uint8_t r = op & 0x1F;

            printf("CPC r%d, r%d\n", d, r);
            continue;
        }

        // ****************************************

        // LDI, // SER

        else if ((op & 0xF000) == inst16_table[e_LDI])
        {
            uint8_t d = instruction->type3.d4 + 16;
            uint8_t kk = (instruction->type3.k4_H << 4) | (instruction->type3.k4_L);

            if (kk == 0xFF)
            {
                printf("SER r%d\n", d);
                continue;
            }
            else
            {
                printf("LDI r%d, 0x%02X\n", d, kk);
            }

            continue;
        }

        // MULS
        else if ((op & 0x200) == inst16_table[e_MULS])
        {
            uint8_t d = (op >> 4) & 0x0F;
            uint8_t r = op & 0x0F;
            printf("MULS r%d, r%d\n", d + 16, r + 16);
            continue;
        }
    }
}