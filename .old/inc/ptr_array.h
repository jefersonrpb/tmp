#ifndef PTR_ARRAY_H
#define PTR_ARRAY_H

#include <stdlib.h>
#include <stdbool.h>

#define PTR_ARRAY_INIT_SIZE 4

#define PTR_ARRAY_INIT(data) array data; array_init(&data)
#define PTR_ARRAY_ADD(data, item) array_add(&data, (void *) item)
#define PTR_ARRAY_SET(data, id, item) array_set(&data, id, (void *) item)
#define PTR_ARRAY_GET(data, id) array_get(&data, id)
#define PTR_ARRAY_DELETE(data, id) array_delete(&data, id)
#define PTR_ARRAY_FREE(data) array_free(&data)

typedef struct ptr_array {
    void **items;
    int size;
    int length;
} ptr_array;

ptr_array *ptr_array_new();
void ptr_array_init(ptr_array *);
void ptr_array_add(ptr_array *, void *);
void ptr_array_set(ptr_array *, int, void *);
void *ptr_array_get(ptr_array *, int);
void ptr_array_delete(ptr_array *, int);
void ptr_array_free(ptr_array *);
bool ptr_array_has(ptr_array *, int);
void ptr_array_clear(ptr_array *);

#endif
