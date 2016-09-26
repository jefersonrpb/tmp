#include <stdio.h>
#include <stdlib.h>
#include "array.h"

array *array_new()
{
    array* data = malloc(sizeof(array));
    array_init(data);
    return data;
}

void array_init(array *data)
{
    data->size = ARRAY_INIT_SIZE;
    data->length = 0;
    data->items = malloc(sizeof(void *) * data->size);
}

static void array_resize(array *data, int size)
{
    void **items = realloc(data->items, sizeof(void *) * size);
    if (items) {
        data->items = items;
        data->size = size;
    }
}

void array_add(array *data, void *item)
{
    if (data->size == data->length) {
        array_resize(data, data->size * 2);
    }
    data->items[data->length++] = item;
}

void array_set(array *data, int index, void *item)
{
    if (index < 0) {
        return;
    }

    if (index > data->length) {
        for (int i = 0; i < index + 1; i++) {
            array_add(data, NULL);
        }
    }

    data->items[index] = item;
}

bool array_has(array *data, int index)
{
    if (index < 0 || index > data->length) {
        return false;
    }
    if (data->items[index] == NULL) {
        return false;
    }
    return true;
}

void *array_get(array *data, int index)
{
    if (index >= 0 && index < data->length) {
        return data->items[index];
    }
    return NULL;
}

void array_delete(array *data, int index)
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
        array_resize(data, data->size / 2);
    }
}

void array_clear(array *data)
{
    array_free(data);
    array_init(data);
}

void array_free(array *data)
{
    for (int i = 0; i < data->length; i++) {
        data->items[i] = NULL;
    }
    free(data->items);
}
