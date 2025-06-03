#include <stdio.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "debug.h"
#include "sym_table.h"
#include "parse_instruction.h"
#include "global.h"

#define MAX_NUM_LINES 10000
#define MAX_LINE_SIZE 256
#define MAX_LABEL_SIZE 256

static char *trim_ws(char *str)
{
    while (isspace(*str)) str++;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end))
        end--;
    *(end + 1) = '\0';

    return str;
}

bool get_label(char *line, char *label_buf)
{
    char *start = line;
    while (isspace(*start)) start++;

    char *colon = strchr(start, ':');
    if (!colon) {
        return false;
    }

    size_t len = colon - start;
    PANIC_IF(len == 0 || len >= MAX_LABEL_SIZE, 
             "Invalid label size: len=%zu", len);

    for (size_t i = 0; i < len; i++) {
        PANIC_IF(!isalpha((unsigned char)start[i]),
                 "Invalid label: only alphabetical characters allowed\n"
                 "Line: %s", line);
    }

    strncpy(label_buf, start, len);
    label_buf[len] = '\0';

    return true;
}

bool get_directive_val(char *line, uint32_t *num) {
    while (isspace(*line)) line++;

    if (strncmp(line, ".int", 4) != 0) {
        return false;
    }
    line += 4;

    while (isspace(*line)) {
        line++;
    }
    PANIC_IF(*line == '\0', "Missing operand for .int directive\nLine: %s", line);

    char *endptr;
    unsigned long val = strtoul(line, &endptr, 0);
    PANIC_IF(line == endptr, "Invalid .int operand: %s", line);

    while (isspace(*endptr)) endptr++;
    PANIC_IF(*endptr != '\0', "Extra characters after .int operand: %s", endptr);

    *num = (uint32_t)val;
    return true;

}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *input_path = argv[1];
    const char *output_path = argv[2];

    FILE *in_file = fopen(input_path, "r");
    if (!in_file)
    {
        perror("Failed to open input file");
        return EXIT_FAILURE;
    }

    FILE *out_file = fopen(output_path, "wb");
    if (!out_file)
    {
        perror("Failed to open output file");
        fclose(in_file);
        return EXIT_FAILURE;
    }

    // First pass: build symbol table
    uint32_t address = 0;
    char line[MAX_LINE_SIZE];
    char label_buf[MAX_LABEL_SIZE];

    while (fgets(line, sizeof(line), in_file)) {
        char *trimmed = trim_ws(line);
        if (*trimmed == '\0') {
            continue;  // Skip empty lines
        }

        if (get_label(trimmed, label_buf)) {
            symtab_define(label_buf, address);
        } else if (get_directive_val(trimmed, NULL)) {
            address++;
        } else {
            address++;
        }
    }

    // Second pass: generate machine code
    rewind(in_file);
    while (fgets(line, sizeof(line), in_file)) {
        char *trimmed = trim_ws(line);
        if (*trimmed == '\0') {
            continue;  // Skip empty lines
        }

        // Skip pure label lines
        if (get_label(trimmed, label_buf)) {
            continue;
        }

        uint32_t bin;
        if (get_directive_val(trimmed, &bin)) {
            fwrite(&bin, sizeof(bin), 1, out_file);
            curr_instr_addr += 0x4;
        } else {
            bin = parse_instr(trimmed);
            fwrite(&bin, sizeof(bin), 1, out_file);
            curr_instr_addr += 0x4;
        }
    }

    fclose(in_file);
    fclose(out_file);

    return EXIT_SUCCESS;
}

