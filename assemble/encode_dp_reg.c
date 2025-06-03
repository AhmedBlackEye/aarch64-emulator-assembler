#include <stdint.h>
#include <stdio.h>  
#include <string.h>
#include <stdlib.h>
#include "encode_dp_reg.h"

static uint32_t encode_multiply(const char **tokens,
    int size,
    uint8_t  x
);

static uint32_t encode_bit_logic (const char **tokens, int size,
                                  uint8_t opc, uint8_t n);

uint32_t encode_and(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 0, 0);
}

uint32_t encode_bic(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 0, 1);
}

uint32_t encode_orr(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 1, 0);
}

uint32_t encode_orn(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 1, 1);
}

uint32_t encode_eor(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 2, 0);
}

uint32_t encode_eon(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 2, 1);
}

uint32_t encode_ands(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 3, 0);
}

uint32_t encode_bics(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 3, 1);
}

// Maybe remove and just call encode_ands for example
uint32_t encode_tst(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 3, 0);
}

uint32_t encode_mvn(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 1, 1);
}

uint32_t encode_mov(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 1, 0);
}

uint32_t encode_madd(const char ** tokens, int size) {
    return encode_multiply(tokens, size, 0);
}

uint32_t encode_msub(const char ** tokens, int size) {
    return encode_multiply(tokens, size, 1);
}

uint32_t encode_mul(const char ** tokens, int size) {
    return encode_multiply(tokens, size, 0);
}

uint32_t encode_mneg(const char ** tokens, int size) {
    return encode_multiply(tokens, size, 1);
}

static uint32_t encode_bit_logic(const char **tokens,
    int size,
    uint8_t  opc,
    uint8_t n
) {

    uint8_t sf = (tokens[0][0] == 'x') ? 1 : 0;

    uint8_t shift = 0;
    uint8_t operand = 0;

    if (size == 5) {
        if (strcmp(tokens[3], "lsl") == 0) {
            shift = 0;
        } 
        else if (strcmp(tokens[3], "lsr") == 0) {
            shift = 1;
        } 
        else if (strcmp(tokens[3], "asr") == 0) {
            shift = 2;
        } 
        else if (strcmp(tokens[3], "ror") == 0) {
            shift = 3;
        }
        else {
            //Put panic here
        }

        operand = atoi(tokens[4] + 1);  
    }

    uint32_t instr = 0;
    instr |= sf << 31;
    instr |= opc << 29;
    instr |= 0b0101 << 24;
    instr |= shift << 22; 
    instr |= n << 21;
    instr |= atoi(tokens[2] + 1) << 16;
    instr |= operand << 10;
    instr |= atoi(tokens[1] + 1) << 5;
    instr |= atoi(tokens[0] + 1);
    return instr;
}

static uint32_t encode_multiply(const char **tokens,
    int size,
    uint8_t  x
) {

    uint8_t ra = (size == 4) ? atoi(tokens[3] + 1) : 31;

    uint8_t sf = (tokens[0][0] == 'x') ? 1 : 0;

    uint32_t instr = 0;
    instr |= sf << 31;
    instr |= 0xD8 << 21;
    instr |= atoi(tokens[2] + 1) << 16;
    instr |= x << 15;
    instr |= ra << 10;
    instr |= atoi(tokens[1] + 1) << 5;
    instr |= atoi(tokens[0] + 1);
    return instr;
}

uint32_t encode_arith_reg(const char **tokens,
    int size,
    uint8_t  opc
) {

    uint8_t sf = (tokens[0][0] == 'x') ? 1 : 0;

    uint8_t shift = 0x0;
    uint8_t operand = 0x0;

    if (size == 5) {
        if (strcmp(tokens[3], "lsl") == 0) {
            shift = 0x0;
        }
        else if (strcmp(tokens[3], "lsr") == 0) {
            shift = 0x1;
        }
        else if (strcmp(tokens[3], "asr") == 0) {
            shift = 0x2;
        }

        operand = atoi(tokens[4] + 1);
    }

    uint32_t instr = 0;
    instr |= sf << 31;
    instr |= opc << 29;
    instr |= 0b1011 << 24;
    instr |= shift << 22;
    instr |= atoi(tokens[2] + 1) << 16;
    instr |= operand << 10;
    instr |= atoi(tokens[1] + 1) << 5;
    instr |= atoi(tokens[0] + 1);
    return instr;
}