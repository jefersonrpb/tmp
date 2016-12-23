#ifndef PTR_ARRAY_H
#define PTR_ARRAY_H

#include <array.h>

typedef struct ptr_array_t {
    void **items;
    int size;
    int length;
} PtrArray ;

PtrArray *ptr_array_new(int size);
void ptr_array_free(PtrArray *data);
void ptr_array_clean(PtrArray *data);
void ptr_array_push(PtrArray *data, void* item);
bool ptr_array_has(PtrArray *data, int index);
void ptr_array_set(PtrArray *data, int index, void* item);
void* ptr_array_get(PtrArray *data, int index);
void ptr_array_delete(PtrArray *data, int index);
void ptr_array_splice(PtrArray *data, int index, int size);

#endif
