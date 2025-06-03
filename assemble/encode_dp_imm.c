#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "encode_dp_imm.h"
#include "global.h"

static uint32_t encode_wmove(const char **tokens,
    int size,
    uint8_t  opc
);

uint32_t encode_movn(const char ** tokens, int size) {
    return encode_wmove(tokens, size, 0);
}

uint32_t encode_movz(const char ** tokens, int size) {
    return encode_wmove(tokens, size, 2);
}

uint32_t encode_movk(const char ** tokens, int size) {
    return encode_wmove(tokens, size, 3);
}

// Fix to work for cmp etc
uint32_t encode_arith_imm(const char **tokens,
    int size,
    uint8_t  opc
) {
    uint8_t sh = 0;
    if (size == 5 && STR_EQUAL(tokens[4], "#12")) {
        sh = 1;
    }

    uint8_t sf = (tokens[0][0] == 'x') ? 1 : 0;

    uint16_t imm_12 = atoi(tokens[2] + 1);

    uint8_t rd = (size == 3) ? 31 : atoi(tokens[0] + 1);

    uint32_t instr = 0;
    instr |= sf << 31;
    instr |= opc << 29;
    instr |= 0b100 << 26;
    instr |= 0b010 << 23; 
    instr |= sh << 22;
    instr |= imm_12 << 10;
    instr |= atoi(tokens[1] + 1) << 5;
    instr |= rd;
    return instr;
}

static uint32_t encode_wmove(const char **tokens,
    int size,
    uint8_t  opc
) {

    uint8_t hw = 0;
    if (size == 4) {
        hw = atoi(tokens[3] + 1) / 16;
    }

    uint8_t sf = (tokens[0][0] == 'x') ? 1 : 0;

    uint16_t imm_16 = atoi(tokens[1] + 1);

    uint32_t instr = 0;
    instr |= sf << 31;
    instr |= opc << 29;
    instr |= 0b100 << 26;
    instr |= 0b101 << 23; 
    instr |= hw << 21;
     
    instr |= imm_16 << 5;
    instr |= atoi(tokens[0] + 1);
    return instr;
}