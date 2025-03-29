/******************************************************************************
Prac 2 - AVR ASM OpCode Decoder
*******************************************************************************/
#include <stdio.h>
#include <inttypes.h>

const uint8_t flash_mem[] = {
    0x0F, 0xEF, // Primera instrucción 1110 1111 0000 1111 (SER)
    0x10, 0x0F, // Segunda instrucción 0000 1111 0001 0000 (ADD)
    0x21, 0x2F, // Tercera instrucción 0010 1111 0010 0001 (MOV)
    0x3F, 0xEF, // Cuarta instrucción  1110 1111 0011 1111 (SER)
    0x32, 0x02, // Quinta instrucción  0000 0010 0011 0010 (MULS)
    0x00, 0x00, // Sexta instrucción   0000 0000 0000 0000 (NOP)
};

const uint16_t inst16_table[] = {
    0x0000, // NOP
    0x9406, // LSR
    0x2C00, // MOV
    0x9C00, // MUL
    0x1C00, // ADC
    0x0C00, // ADD
    0x2000, // AND
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
    } type2; // e.g.: MOV,MUL,ADC,ADD,AND
    struct
    {
        uint16_t k4_low : 4;
        uint16_t d4 : 4;
        uint16_t k4_high : 4;
        uint16_t op4 : 4;
    } type3; // e.g.: LDI, SER
    struct
    {
        uint16_t r4 : 4;
        uint16_t d4 : 4;
        uint16_t op8 : 8;
    } type4; // e.g.: MULS
} Op_Code_t;

int main()
{
    Op_Code_t *instruction;
    printf("- Practica 2: AVR OpCode -\n");

    // Decode the instructions by cycling through the array
    for (uint8_t idx = 0; idx < sizeof(flash_mem); idx += 2)
    {
        instruction = (Op_Code_t *)&flash_mem[idx];

        // Check NOP first
        if (instruction->op16 == inst16_table[e_NOP])
        {
            printf("NOP\n");
            continue;
        }

        // Check LSR (type1)
        if ((instruction->op16 & 0xFE0F) == inst16_table[e_LSR])
        {
            printf("LSR r%d\n", instruction->type1.d5);
            continue;
        }

        // Check LDI/SER (type3) - ambos empiezan con 1110
        if ((instruction->op16 & 0xF000) == inst16_table[e_LDI])
        {
            uint8_t d = instruction->type3.d4 + 16; // Registros 16-31

            // Verificar si es SER (k = 0xFF)
            if (instruction->type3.k4_low == 0xF && instruction->type3.k4_high == 0xF)
            {
                printf("SER r%d\n", d);
            }
            else
            {
                // Es LDI normal
                uint8_t k = (instruction->type3.k4_high << 4) | instruction->type3.k4_low;
                printf("LDI r%d,0x%02X\n", d, k);
            }
            continue;
        }

        // Check MULS (type4)
        if ((instruction->op16 & 0x0200) == inst16_table[e_MULS])
        {
            uint8_t r = instruction->type4.r4 + 16; // Registros 16-31
            uint8_t d = instruction->type4.d4 + 16; // Registros 16-31
            printf("MULS r%d,r%d\n", d, r);
            continue;
        }

        // Check type2 instructions
        uint16_t type2_opcode = instruction->op16 & 0xFC00;
        if (type2_opcode == inst16_table[e_MOV])
        {
            printf("MOV r%d,r%d\n", instruction->type2.d5, instruction->type2.r4 | (instruction->type2.r1 << 4));
        }
        else if (type2_opcode == inst16_table[e_MUL])
        {
            printf("MUL r%d,r%d\n", instruction->type2.d5, instruction->type2.r4 | (instruction->type2.r1 << 4));
        }
        else if (type2_opcode == inst16_table[e_ADC])
        {
            printf("ADC r%d,r%d\n", instruction->type2.d5, instruction->type2.r4 | (instruction->type2.r1 << 4));
        }
        else if (type2_opcode == inst16_table[e_ADD])
        {
            printf("ADD r%d,r%d\n", instruction->type2.d5, instruction->type2.r4 | (instruction->type2.r1 << 4));
        }
        else if (type2_opcode == inst16_table[e_AND])
        {
            printf("AND r%d,r%d\n", instruction->type2.d5, instruction->type2.r4 | (instruction->type2.r1 << 4));
        }
        else
        {
            printf("unknown\n");
        }
    }

    return 0;
}