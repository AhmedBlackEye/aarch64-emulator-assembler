#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "encoder_table.h"
#include "debug.h"

#define MAX_TOKENS 5

static char *ltrim(char *s) {
    while (isspace((unsigned char)*s)) s++;
    return s;
}

uint32_t parse_instr(char *line) {
    char *tokens[MAX_TOKENS];
    int num_tokens = 0;

    char *saveptr = NULL;
    char *mnemonic = strtok_r(line, " \t", &saveptr);
    PANIC_IF(mnemonic == NULL, "parse_instr: no mnemonic read");

    char *arg = strtok_r(NULL, ",", &saveptr);
    while (arg != NULL && num_tokens < MAX_TOKENS) {
        tokens[num_tokens++] = ltrim(arg);
        arg = strtok_r(NULL, ",", &saveptr);
    }

    PANIC_IF(num_tokens < 1, "parse_instr: insufficient operands for %s", mnemonic);

    encode_function fn = lookup_encoder(mnemonic);
    return fn((const char **)tokens, num_tokens);
}
