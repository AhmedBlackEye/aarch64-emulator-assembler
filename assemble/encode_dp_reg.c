#include <stdint.h>
#include <stdio.h>  
#include <string.h>
#include <stdlib.h>
#include "encode_dp_reg.h"
#include "debug.h"

static uint32_t encode_multiply(const char **tokens,
    int size,
    uint8_t  x
);

static uint32_t encode_bit_logic (const char **tokens, int size,
                                  uint8_t opc, uint8_t n, INSTRUCTION_TYPE type);

uint32_t encode_and(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 0, 0, STANDARD);
}

uint32_t encode_bic(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 0, 1, STANDARD);
}

uint32_t encode_orr(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 1, 0, STANDARD);
}

uint32_t encode_orn(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 1, 1, STANDARD);
}

uint32_t encode_eor(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 2, 0, STANDARD);
}

uint32_t encode_eon(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 2, 1, STANDARD);
}

uint32_t encode_ands(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 3, 0, STANDARD);
}

uint32_t encode_bics(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 3, 1, STANDARD);
}

uint32_t encode_tst(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 3, 0, RD_ZR);
}

uint32_t encode_mvn(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 1, 1, RN_ZR);
}

uint32_t encode_mov(const char ** tokens, int size) {
    return encode_bit_logic(tokens, size, 1, 0, RN_ZR);
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
    uint8_t n,
    INSTRUCTION_TYPE type
) {

    PANIC_IF(!(size == 3 || size == 5 || size == 2 || size == 4),"encode_bit_logic: unexpected number of tokens: %d", size);

    uint8_t sf = (tokens[0][0] == 'x') ? 1 : 0;

    uint8_t shift = 0;
    uint8_t operand = 0;

    if (size == 4 || size == 5) {

        const char *shift_token = tokens[size - 2];

        if (strcmp(shift_token, "lsl") == 0) {
            shift = 0;
        } 
        else if (strcmp(shift_token, "lsr") == 0) {
            shift = 1;
        } 
        else if (strcmp(shift_token, "asr") == 0) {
            shift = 2;
        } 
        else if (strcmp(shift_token, "ror") == 0) {
            shift = 3;
        }
        else {
            PANIC("Unknown shift operation: %s\n", shift_token);
        }

        operand = atoi(tokens[size - 1] + 1);  
    }

    uint8_t rd, rn, rm;
    if (type == RD_ZR) {
        rd = 31;
        rn = atoi(tokens[0] + 1);
        rm = atoi(tokens[1] + 1);
    } else if (type == RN_ZR) {
        rd = atoi(tokens[0] + 1);
        rn = 31;
        rm = atoi(tokens[1] + 1);
    } else {
        rd = atoi(tokens[0] + 1);
        rn = atoi(tokens[1] + 1);
        rm = atoi(tokens[2] + 1);
    }

    uint32_t instr = 0;
    instr |= sf << 31;
    instr |= opc << 29;
    instr |= 0b0101 << 24;
    instr |= shift << 22; 
    instr |= n << 21;
    instr |= rm << 16;
    instr |= operand << 10;
    instr |= rn << 5;
    instr |= rd;
    return instr;
}

static uint32_t encode_multiply(const char **tokens,
    int size,
    uint8_t  x
) {

    PANIC_IF(!(size == 3 || size == 4),"encode_multiply: unexpected number of tokens: %d", size);

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
    uint8_t  opc,
    INSTRUCTION_TYPE type
) {

    PANIC_IF(!(size == 3 || size == 5 || size == 2 || size == 4),"encode_arith_reg: unexpected number of tokens: %d", size);

    uint8_t sf = (tokens[0][0] == 'x') ? 1 : 0;

    uint8_t shift = 0x0;
    uint8_t operand = 0x0;

    uint8_t offset = (size == 2 || size == 4) ? 0 : 1;

    if (size == 4 || size == 5) {
        const char *shift_token = tokens[size - 2];

        if (strcmp(shift_token, "lsl") == 0) {
            shift = 0x0;
        }
        else if (strcmp(shift_token, "lsr") == 0) {
            shift = 0x1;
        }
        else if (strcmp(shift_token, "asr") == 0) {
            shift = 0x2;
        }
        else {
            PANIC("Unknown shift operation: %s\n", shift_token);
        }

        operand = atoi(tokens[size - 1] + 1);
    }

    uint8_t rd, rn, rm;
    if (type == RD_ZR) {
        rd = 31;
        rn = atoi(tokens[0] + 1);
        rm = atoi(tokens[1] + 1);
    } else if (type == RN_ZR) {
        rd = atoi(tokens[0] + 1);
        rn = 31;
        rm = atoi(tokens[1] + 1);
    } else {
        rd = atoi(tokens[0] + 1);
        rn = atoi(tokens[1] + 1);
        rm = atoi(tokens[2] + 1);
    }

    uint32_t instr = 0;
    instr |= sf << 31;
    instr |= opc << 29;
    instr |= 0b1011 << 24;
    instr |= shift << 22;
    instr |= rm << 16;
    instr |= operand << 10;
    instr |= rn << 5;
    instr |= rd;
    return instr;
}