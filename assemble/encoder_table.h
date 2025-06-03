#ifndef ENCODER_TABLE_H
#define ENCODER_TABLE_H

#include <stdint.h>

typedef uint32_t (*encode_function)(char **, int);

encode_function lookup_encoder(const char *mnemonic);

#endif 