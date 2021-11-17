#include "util/array.h"

#include <assert.h>
#include <string.h>
#include <stdint.h>

struct Array {
    void *buffer;
    size_t size;
    size_t length;
    size_t capacity;
};

Array *array_create(size_t size)
{
    assert(size > 0);

    Array *array = malloc(sizeof(Array));
    if (!array)
        return NULL;

    // Set variables.
    array->buffer = NULL;
    array->size = size;
    array->length = 0;
    array->capacity = 0;

    return array;
}

bool array_at(Array *array, int index, void **element)
{
    // Ensure the index is in range.
    if (!array || index < 0 || index >= array->length)
        return false;

    // Set the pointer to point to the provided element at the index.
    *element = (uint8_t*)array->buffer + array->size * index;

    return true;
}

bool array_front(Array *array, void **element)
{
    // There is no front element if the array length is 0.
    if (!array || !element || array->length == 0)
        return false;

    // Set the element pointer.
    *element = array->buffer;

    return true;
}

bool array_back(Array *array, void **element)
{
    if (!array || array->length == 0)
        return false;

    // Set the pointer to point to the last element.
    *element = (uint8_t*)array->buffer + array->size * (array->length - 1);

    return true;
}

void *array_data(Array *array)
{
    return array->buffer;
}

bool array_empty(Array *array)
{
    return array->length == 0;
}

int array_length(Array *array)
{
    return array->length;
}

bool array_push_back(Array *array, void *element)
{
    if (!array || !element)
        return false;

    // Check whether to allocate more memory.
    if (array->length + 1 > array->capacity) {
        int capacity = array->capacity ? array->capacity << 1 : 1;

        size_t *reallocated = realloc(
            array->buffer,
            capacity * array->size
        );

        if (!reallocated)
            return false;

        array->buffer = reallocated;
        array->capacity = capacity;
    }

    // Copy the element into the next available position.
    memcpy(
        (uint8_t*)array->buffer + array->size * array->length,
        element,
        array->size
    );
    array->length++;

    return true;
}

bool array_pop_back(Array *array, void *element)
{
    if (!array || !element || array->length <= 0)
        return false;

    memcpy(
        element,
        (uint8_t*)array->buffer + array->size * (array->length - 1),
        array->size
    );
    array->length--;

    // Check if we can free memory. Occurs when we have enough elements to fit
    // into the below power of 2 capacity.
    if (array->length <= (array->capacity >> 1)) {
        size_t *reallocated = realloc(
            array->buffer,
            (array->capacity >> 1) * array->size
        );

        if (!reallocated && array->length != 0)
            return false;

        array->capacity >>= 1;
    }

    return true;
}

void array_destroy(Array *array)
{
    free(array->buffer);
    free(array);
}