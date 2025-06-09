#include <stdlib.h>
#include <string.h>

#include "dynstr.h"
#include "assert.h"

struct DynStr
{
    char *data;
    int cap;
    int len;
};

// DynStr ds = dynstr_create( s );
// Create and return a new dynstr, with initial data s
DynStr dynstr_create(char *s)
{
    DynStr new = malloc(sizeof(struct DynStr));
    assert(new != NULL);
    int len = strlen(s);
    int cap = 10 + len;
    new->data = malloc(cap * sizeof(char));
    assert(new->data != NULL);
    strcpy(new->data, s);
    new->len = len;
    new->cap = cap;
    return new;
}
// dynstr_strcat( d, s );
// append string s onto the end of d’s data
void dynstr_strcat(DynStr d, char *s)
{
    int len = strlen(s);
    d->len += len;
    if (d->len >= d->cap)
    { // need more space?
        d->cap += 100 + len;
        d->data = realloc(d->data, d->cap * sizeof(char));
        assert(d->data != NULL);
    }
    strcat(d->data, s);
}

int dynstr_cmp(DynStr d1, DynStr d2)
{
    return strcmp(d1->data, d2->data);
}

// int len = dynstr_len( d );
// Return the length of DynStr d.
int dynstr_len(DynStr d)
{
    return d->len;
}
// char *data = dynstr_data( d );
// Return the dynstr’s data; note: not a copy, the actual data.
char *dynstr_data(DynStr d)
{
    return d->data;
}

// void dynstr_free( d );
// When you’ve finished growing your dynstr, you should
// free the DynStr and it’s data chunk.
void dynstr_free(DynStr d)
{
    free(d->data);
    free(d);
}