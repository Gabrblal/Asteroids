#include "util/array.h"

#include <assert.h>
#include <string.h>
#include <stdint.h>

struct Array {
    uint8_t *buffer;
    size_t size;
    size_t length;
    size_t space;
    size_t min_space;
    size_t max_space;
};

Array *array_create(size_t size, size_t min_space, size_t max_space)
{
    assert(max_space > min_space);
    assert(size > 0);

    Array *array = malloc(sizeof(Array));
    if (!array)
        return NULL;

    array->buffer = malloc(size * max_space);
    if (!array->buffer) {
        free(array);
        return NULL;
    }

    // Set variables.
    array->size = size;
    array->length = 0;
    array->space = max_space;
    array->max_space = max_space;
    array->min_space = min_space;

    return array;
}

bool array_push_back(Array *array, void *element)
{
    if (!array || !element)
        return false;

    // Verify the constraint min_size <= space <= max_size
    if (array->space - 1 < array->min_space) {

        // Reallocate the buffer to satisfy the constraint.
        int difference = array->max_space - (array->min_space + 1);
        uint8_t *buffer = realloc(
            array->buffer,
            array->size * (array->length + difference)
        );

        // If we were unable to realloc, indicate failure.
        if (!buffer)
            return false;

        // Set new values.
        array->buffer = buffer;
        array->space =  array->max_space;
    }
    else {
        array->space--;
    }

    memcpy(
        array->buffer + array->size * array->length,
        element,
        array->size
    );
    array->length++;

    return true;
}

void array_destroy(Array *array)
{
    free(array->buffer);
    free(array);
}