#ifndef LOAD_STORE_INSTRUCTIONS_H
#define LOAD_STORE_INSTRUCTIONS_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Decides whether the given instruction is a load-literal or
 * a single data transfer, and dispatches it accordingly.
 *
 * @param instruction The 32-bit ARM instruction to decode and handle.
 */
void handle_load_store(uint32_t instruction);

#endif
