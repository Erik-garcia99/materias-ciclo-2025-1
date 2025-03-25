/******************************************************************************
Prac 2 - AVR ASM OpCode Decoder
*******************************************************************************/

#include <stdio.h>
#include <inttypes.h>

const uint8_t flash_mem[] = {
    0x0F, 0xEF, // -> SER R0 1110 1111 0000 1111
    0x10, 0x0F, // -> 0000 1111 0001 0000 -> ADD r17,r16
    0x21, 0x2F, // -> 0010 1111 0010 0001 -> MOV r18,r17
    0x3F, 0xEF, // -> 1110 1111 0011 1111 -> SER r3 - LDI r3, 255
    0x32, 0x02, //-> 0000 0010 00011 0010 -> MULS r3, r2
    0x00, 0x00  // NOP
};

// SER - 1110 1111 DDDD 1111
// ldi - 1111 KKKK 0000 KKKK

const uint16_t inst16_table[] = {

    {0x0},    // NOP
    {0x9406}, // LSR
    {0x2c00}, // MOV
    {0x9C00}, // MUL
    {0x1c00}, // ADC
    {0x0c00}, // ADD
    {0x2000}, // AND
    {0x0400}, // CPC
    {0xE000}, // LDI
    {0xEF0F}, // SER

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
    e_SER
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

} Op_Code_t;

int main()
{
    Op_Code_t *instruction;
    printf("- Practica 2: AVR OpCode -\n");
    // Decode the instructions by cycling through the array
    for (uint8_t idx = 0; idx < sizeof(flash_mem); idx += 2)
    {
        instruction = (Op_Code_t *)&flash_mem[idx];

        if (instruction->op16 == inst16_table[e_NOP])
        {
            printf("NOP\n");
        }
        else
        {
            printf("unknown\n");
        }
    }
}