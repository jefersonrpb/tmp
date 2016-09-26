#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>
#include <stdbool.h>

#define ARRAY_INIT_SIZE 4

#define ARRAY_INIT(data) array data; array_init(&data)
#define ARRAY_ADD(data, item) array_add(&data, (void *) item)
#define ARRAY_SET(data, id, item) array_set(&data, id, (void *) item)
#define ARRAY_GET(data, id) array_get(&data, id)
#define ARRAY_DELETE(data, id) array_delete(&data, id)
#define ARRAY_FREE(data) array_free(&data)

typedef struct array {
    void **items;
    int size;
    int length;
} array;

static void array_resize(array *, int);
array *array_new();
void array_init(array *);
void array_add(array *, void *);
void array_set(array *, int, void *);
void *array_get(array *, int);
void array_delete(array *, int);
void array_free(array *);
bool array_has(array *, int);
void array_clear(array *);

#endif
