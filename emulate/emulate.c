#include <stdio.h>

#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

#include "global.h"
#include "debug.h"
#include "handle_instr.h"

void write_output(FILE *out)
{
    fprintf(out, "Registers:\n");
    for (int i = 0; i < NUM_REG - 1; i++)
    {
        // PRIx64 makes sure the correct format specifier is selected "llx" or "lx" depending on platform
        fprintf(out, "X%02d = %016" PRIx64 "\n", i, read_reg(i, true));
    }
    fprintf(out, "PC = %016" PRIx64 "\n", pc);

    fprintf(out, "PSTATE : %c%c%c%c\n",
            pstate.N ? 'N' : '-',
            pstate.Z ? 'Z' : '-',
            pstate.C ? 'C' : '-',
            pstate.V ? 'V' : '-');

    fprintf(out, "Non-zero memory:\n");
    for (uint32_t addr = 0; addr < MAX_MEMORY_SIZE; addr += INSTR_SIZE)
    {
        uint32_t value = read_mem(addr, false);
        if (value != 0)
        {
            fprintf(out, "0x%08" PRIx32 " : 0x%08" PRIx32 "\n", addr, value);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3)
    {
        fprintf(stderr, "Usage: %s <input_file> [output_file]\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *input_path = argv[1];
    const char *output_path = (argc == 3) ? argv[2] : NULL;

    FILE *in_file = fopen(input_path, "rb");
    if (!in_file)
    {
        perror("Failed to open input file");
        return EXIT_FAILURE;
    }

    FILE *out_file = output_path ? fopen(output_path, "wb") : stdout;
    if (!out_file)
    {
        perror("Failed to open output file");
        fclose(in_file);
        return EXIT_FAILURE;
    }

    // loads memory
    fread(&memory[0], 1, MAX_MEMORY_SIZE, in_file);
    if (ferror(in_file))
    {
        perror("Error reading from input file");
        exit(EXIT_FAILURE);
    }
    uint32_t instr;
    while (true)
    {
        instr = read_mem(pc, false);
        if (instr == HALT_INSTR)
        {
            break;
        }
        // increment PC by the instruction size before handling a new instruction.
        pc += INSTR_SIZE;
        handle_instr(instr);
    }

    write_output(out_file);

    fclose(in_file);
    if (output_path)
    {
        fclose(out_file);
    }

    return EXIT_SUCCESS;
}
