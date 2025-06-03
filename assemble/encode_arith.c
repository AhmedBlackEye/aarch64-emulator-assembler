#include "encode_dp_imm.h"
#include "encode_dp_imm.c"

uint32_t encode_add(char **tokens, int size) {
    if (tokens[2][0] == '#') {
        return encode_arith_imm(tokens, size, 0);
    }
    else {
        return encode_arith_reg(tokens, size, 0);
    }
}

uint32_t encode_adds(char **tokens, int size) {
    if (tokens[2][0] == '#') {
        return encode_arith_imm(tokens, size, 1);
    }
    else {
        return encode_arith_reg(tokens, size, 1);
    }
}

uint32_t encode_sub(char **tokens, int size) {
    if (tokens[2][0] == '#') {
        return encode_arith_imm(tokens, size, 2);
    }
    else {
        return encode_arith_reg(tokens, size, 2);
    }
}

uint32_t encode_subs(char **tokens, int size) {
    if (tokens[2][0] == '#') {
        return encode_arith_imm(tokens, size, 3);
    }
    else {
        return encode_arith_reg(tokens, size, 3);
    }
}

uint32_t encode_cmp(char **tokens, int size) {
    if (tokens[1][0] == '#') {
        return encode_arith_imm(tokens, size, 3);
    }
    else {
        return encode_arith_reg(tokens, size, 3);
    }
}

uint32_t encode_cmn(char **tokens, int size) {
    if (tokens[1][0] == '#') {
        return encode_arith_imm(tokens, size, 1);
    }
    else {
        return encode_arith_reg(tokens, size, 1);
    }
}

uint32_t encode_neg(char **tokens, int size) {
    if (tokens[1][0] == '#') {
        return encode_arith_imm(tokens, size, 2);
    }
    else {
        return encode_arith_reg(tokens, size, 2);
    }
}

uint32_t encode_negs(char **tokens, int size) {
    if (tokens[1][0] == '#') {
        return encode_arith_imm(tokens, size, 3);
    }
    else {
        return encode_arith_reg(tokens, size, 3);
    }
}