#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include "encode_arith.h"
#include "encode_dp_reg.h"
#include "encode_dp_imm.h"
#include "encoder_table.h"
#include "encode_load_store.h"

typedef uint32_t (*encode_function)(char **, int);

typedef struct {
    const char *mnemonic;
    encode_function fn;
} enc_tab_element;

static const enc_tab_element encode_table[] = {
    { "add",   encode_add  },
    { "adds",  encode_adds },
    { "sub",   encode_sub  },
    { "subs",  encode_subs },
    { "cmp",   encode_cmp  },
    { "cmn",   encode_cmn  },
    { "neg",   encode_neg  },
    { "negs",  encode_negs },
    { "and",   encode_and  },
    { "bic",   encode_bic  },
    { "orr",   encode_orr  },
    { "orn",   encode_orn  },
    { "eor",   encode_eor  },
    { "eon",   encode_eon  },
    { "ands",  encode_ands },
    { "bics",  encode_bics },
    { "tst",   encode_tst  },
    { "mvn",   encode_mvn  },
    { "mov",   encode_mov  },
    { "madd",  encode_madd },
    { "msub",  encode_msub },
    { "mul",   encode_mul  },
    { "mneg",  encode_mneg },
    { "movn",  encode_movn },
    { "movz",  encode_movz },
    { "movk",  encode_movk },
    { "ldr",   encode_load },
    { "str",   encode_store},
};

encode_function lookup_encoder(const char *mnemonic)
{
    for (const enc_tab_element *e = encode_table; e->mnemonic; ++e)
        if (strcmp(e->mnemonic, mnemonic) == 0)
            return e->fn;
    return NULL;  
};
