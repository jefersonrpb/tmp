#include <stdio.h>
#include <stdlib.h>
#include "ptr_array.h"

ptr_array *ptr_array_new()
{
    ptr_array* data = malloc(sizeof(ptr_array));
    ptr_array_init(data);
    return data;
}

void ptr_array_init(ptr_array *data)
{
    data->size = PTR_ARRAY_INIT_SIZE;
    data->length = 0;
    data->items = malloc(sizeof(void *) * data->size);
}

static void ptr_array_resize(ptr_array *data, int size)
{
    void **items = realloc(data->items, sizeof(void *) * size);
    if (items) {
        data->items = items;
        data->size = size;
    }
}

void ptr_array_add(ptr_array *data, void *item)
{
    if (data->size == data->length) {
        ptr_array_resize(data, data->size * 2);
    }
    data->items[data->length++] = item;
}

void ptr_array_set(ptr_array *data, int index, void *item)
{
    if (index < 0) {
        return;
    }

    if (index > data->length) {
        for (int i = 0; i < index + 1; i++) {
            ptr_array_add(data, NULL);
        }
    }

    data->items[index] = item;
}

bool ptr_array_has(ptr_array *data, int index)
{
    if (index < 0 || index > data->length) {
        return false;
    }
    if (data->items[index] == NULL) {
        return false;
    }
    return true;
}

void *ptr_array_get(ptr_array *data, int index)
{
    if (index >= 0 && index < data->length) {
        return data->items[index];
    }
    return NULL;
}

void ptr_array_delete(ptr_array *data, int index)
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

    if (data->length > 0 && data->length <= data->size / 4) {
        ptr_array_resize(data, data->size / 2);
    }
}

void ptr_array_clear(ptr_array *data)
{
    ptr_array_free(data);
    ptr_array_init(data);
}

void ptr_array_free(ptr_array *data)
{
    for (int i = 0; i < data->length; i++) {
        data->items[i] = NULL;
    }
    free(data->items);
}
