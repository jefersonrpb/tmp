#include "ptr_array.h"

#define PTR_ARRAY_INIT_SIZE 4

static void ptr_array_init(PtrArray *data, int size)
{
    data->size = size;
    data->length = 0;
    data->items = calloc(data->size, sizeof(void *));
}

static void ptr_array_resize(PtrArray *data, int size)
{
    void **items = realloc(data->items, sizeof(void *) * size);
    if (!items) {
        printf("eror on realloc array\n");
        exit(1);
    }
    memset(items + data->size, 0, size - data->size);
    data->items = items;
    data->size = size;
}

PtrArray *ptr_array_new(int size)
{
    PtrArray* data = malloc(sizeof(PtrArray));
    ptr_array_init(data, size);
    return data;
}

void ptr_array_add(PtrArray *data, void *item)
{
    if (data->length == data->size) {
        ptr_array_resize(data, data->size + PTR_ARRAY_INIT_SIZE);
    }
    data->items[data->length++] = item;
}

void ptr_array_set(PtrArray *data, int index, void *item)
{
    if (index < 0) {
        return;
    }

    if (index >= data->size) {
        ptr_array_resize(data, index + PTR_ARRAY_INIT_SIZE);
    }

    if (index >= data->length) {
        data->length = index + 1;
    } 

    data->items[index] = item;
}

bool ptr_array_has(PtrArray *data, int index)
{
    if (index < 0 || index > data->length) {
        return false;
    }
    if (data->items[index] == NULL) {
        return false;
    }
    return true;
}

void *ptr_array_get(PtrArray *data, int index)
{
    if (index >= 0 && index < data->length) {
        return data->items[index];
    }
    return NULL;
}

void ptr_array_delete(PtrArray *data, int index)
{
    if (index < 0 || index >= data->length) {
        return;
    }

    data->items[index] = NULL;
    for (int i = index; i < data->length - 1; i++) {
        data->items[i] = data->items[i + 1];
    }
    data->items[data->length - 1] = NULL;
    data->length--;
}

void ptr_array_free(PtrArray *data)
{
    free(data->items);
}

void ptr_array_clean(PtrArray *data)
{
    ptr_array_free(data);
    ptr_array_init(data, data->size);
}
