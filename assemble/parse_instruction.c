#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include "encoder_table.h"
#include "debug.h"

#define MAX_TOKENS 5
#define BRACKET_BUFFER_SIZE 256

static char *ltrim(char *s)
{
    while (isspace((unsigned char)*s))
        s++;
    return s;
}

uint32_t parse_instr(char *line)
{
    char *tokens[MAX_TOKENS];
    int num_tokens = 0;

    char *saveptr = NULL;
    char *mnemonic = strtok_r(line, " \t", &saveptr);
    PANIC_IF(mnemonic == NULL, "parse_instr: no mnemonic read");

    char *arg = strtok_r(NULL, " \t,", &saveptr);
    while (arg != NULL && num_tokens < MAX_TOKENS)
    {
        if (arg[0] == '[')
        {
            // Use a static buffer to safely build the bracketed expression
            static char bracket_buffer[BRACKET_BUFFER_SIZE];
            int buf_len = 0;
            
            // Copy the first part
            int arg_len = strlen(arg);
            PANIC_IF(arg_len >= BRACKET_BUFFER_SIZE, "Bracketed operand too long");
            strcpy(bracket_buffer, arg);
            buf_len = arg_len;
            
            // Check if this token already contains the closing bracket
            if (!strchr(bracket_buffer, ']'))
            {
                // Need to collect more tokens until we find the closing bracket
                char *next;
                while ((next = strtok_r(NULL, " \t,", &saveptr)) != NULL)
                {
                    // Add a comma and space, then the next token
                    int next_len = strlen(next);
                    PANIC_IF(buf_len + 2 + next_len >= BRACKET_BUFFER_SIZE, 
                             "Bracketed operand too long");
                    
                    strcat(bracket_buffer, ", ");
                    strcat(bracket_buffer, next);
                    buf_len += 2 + next_len;
                    
                    if (strchr(next, ']'))
                    {
                        break; // Found the closing bracket
                    }
                }
                
                if (!strchr(bracket_buffer, ']'))
                {
                    PANIC("parse_instr: unmatched [ in bracketed operand");
                }
            }
            
            tokens[num_tokens++] = ltrim(bracket_buffer);
        }
        else
        {
            tokens[num_tokens++] = ltrim(arg);
        }

        arg = strtok_r(NULL, " \t,", &saveptr);
    }

    PANIC_IF(num_tokens < 1, "parse_instr: insufficient operands for %s", mnemonic);

    encode_function fn = lookup_encoder(mnemonic);
    return fn((const char **)tokens, num_tokens);
}