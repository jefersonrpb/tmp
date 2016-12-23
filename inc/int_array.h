#ifndef PTR_ARRAY_H
#define PTR_ARRAY_H

#include <array.h>

typedef struct int_array_t {
    void **items;
    int size;
    int length;
} IntArray ;

IntArray *int_array_new(int size);
void int_array_free(IntArray *data);
void int_array_clean(IntArray *data);
void int_array_push(IntArray *data, int item);
bool int_array_has(IntArray *data, int index);
void int_array_set(IntArray *data, int index, int item);
int int_array_get(IntArray *data, int index);
void int_array_delete(IntArray *data, int index);
void int_array_splice(IntArray *data, int index, int size);

#endif
