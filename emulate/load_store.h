#ifndef LOAD_STORE_H
#define LOAD_STORE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Decides whether the given instruction is a load-literal or
 * a single data transfer, and dispatches it accordingly.
 */
void handle_load_store(uint32_t instruction);

#endif
