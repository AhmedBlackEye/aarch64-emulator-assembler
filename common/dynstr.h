#include <stdlib.h>

typedef struct DynStr *DynStr;

// DynStr ds = dynstr_create( s );
// Create and return a new dynstr, with initial data s
extern DynStr dynstr_create(char *s);

// dynstr_strcat( d, s );
// append string s onto the end of d’s data
extern void dynstr_strcat(DynStr d, char *s);

extern int dynstr_cmp(DynStr d1, DynStr d2);

// int len = dynstr_len( d );
// Return the length of DynStr d.
extern int dynstr_len(DynStr d);

// char *data = dynstr_data( d );
// Return the dynstr’s data; note: not a copy, the actual data.
extern char *dynstr_data(DynStr d);

// void dynstr_free( d );
// When you’ve finished growing your dynstr, you should
// free the DynStr and it’s data chunk.
extern void dynstr_free(DynStr d);