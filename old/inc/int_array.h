#ifndef INT_ARRAY_H
#define INT_ARRAY_H

#include <stdlib.h>
#include <stdbool.h>

#define INT_ARRAY_INIT_SIZE 4

typedef struct int_array {
    int *items;
    int size;
    int length;
} int_array;

int_array *int_array_new();
void int_array_init(int_array *);
void int_array_add(int_array *, int);
void int_array_set(int_array *, int, int);
int int_array_get(int_array *, int);
void int_array_delete(int_array *, int);
void int_array_free(int_array *);
bool int_array_has(int_array *, int);
void int_array_clear(int_array *);

#endif
