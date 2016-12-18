#ifndef PTR_ARRAY_H
#define PTR_ARRAY_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct ptr_array_t {
    void **items;
    int size;
    int length;
} PtrArray ;

PtrArray *ptr_array_new();
void ptr_array_add(PtrArray *, void *);
void ptr_array_set(PtrArray *, int, void *);
void *ptr_array_get(PtrArray *, int);
void ptr_array_delete(PtrArray *, int);
void ptr_array_free(PtrArray *);
bool ptr_array_has(PtrArray *, int);
void ptr_array_clean(PtrArray *);

#endif
