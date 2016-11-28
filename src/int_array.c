#include <stdio.h>
#include <stdlib.h>
#include "int_array.h"

int_array *int_array_new()
{
    int_array* data = malloc(sizeof(int_array));
    int_array_init(data);
    return data;
}

void int_array_init(int_array *data)
{
    data->size = INT_ARRAY_INIT_SIZE;
    data->length = 0;
    data->items = malloc(sizeof(int*) * data->size);

    for (int i = 0; i < data->size; i++) {
        int_array_add(data, -1);
    }
}

static void int_array_resize(int_array *data, int size)
{
    int *items = realloc(data->items, sizeof(int*) * size);
    if (items) {
        data->items = items;
        data->size = size;
    }
}

void int_array_add(int_array *data, int item)
{
    if (data->size == data->length) {
        int_array_resize(data, data->size * 2);
    }
    data->items[data->length++] = item;
}

void int_array_set(int_array *data, int index, int item)
{
    if (index < 0) {
        return;
    }

    if (index > data->length) {
        for (int i = 0; i < index + 1; i++) {
            int_array_add(data, -1);
        }
    }

    data->items[index] = item;
}

bool int_array_has(int_array *data, int index)
{
    if (index < 0 || index > data->length) {
        return false;
    }
    if (data->items[index] == -1) {
        return false;
    }
    return true;
}

int int_array_get(int_array *data, int index)
{
    if (index >= 0 && index < data->length) {
        return data->items[index];
    }
    return -1;
}

void int_array_delete(int_array *data, int index)
{
    if (index < 0 || index >= data->length) {
        return;
    }

    data->items[index] = -1;
    for (int i = index; i < data->length - 1; i++) {
        data->items[i] = data->items[i + 1];
    }
    data->items[data->length - 1] = -1;
    data->length--;

    if (data->length > 0 && data->length <= data->size / 4) {
        int_array_resize(data, data->size / 2);
    }
}

void int_array_clear(int_array *data)
{
    int_array_free(data);
    int_array_init(data);
}

void int_array_free(int_array *data)
{
    for (int i = 0; i < data->length; i++) {
        data->items[i] = -1;
    }
    free(data->items);
}
