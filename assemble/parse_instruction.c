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
    int token_count = 0;

    // Extract mnemonic
    char *saveptr = NULL;
    char *mnemonic = strtok_r(line, " \t", &saveptr);
    PANIC_IF(mnemonic == NULL, "Error: missing mnemonic\n. Line: %s", line);

    // Extract operands
    char *arg = strtok_r(NULL, ",", &saveptr);
    while (arg != NULL && token_count < MAX_TOKENS) {
        tokens[token_count++] = ltrim(arg);
        arg = strtok_r(NULL, ",", &saveptr);
    }

    encode_function fn = lookup_encoder(mnemonic);
    PANIC_IF(!fn, "Error: unknown mnemonic '%s'\n", mnemonic);

    return fn(tokens, token_count);
}