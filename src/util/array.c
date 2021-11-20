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

    // Allocate array structure.
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

void array_reduce(Array *array)
{
    // Check if we can free memory. Occurs when we have enough elements to fit
    // into the below power of 2 capacity.

    // If there are more elements than there would be capacity, cannot reduce.
    if (array->length > (array->capacity >> 1))
        return;

    // Reduce the capacity until we cannot fit the elements. 
    // @TODO: Do this mathematically.
    int capacity = array->capacity >> 1;
    while (capacity > array->length)
        capacity >>= 1;

    size_t *reallocated = realloc(array->buffer, capacity * array->size);

    // If the reallocation was successful, set the correct values in the array.
    if (reallocated || array->length == 0) {
        array->buffer = reallocated;
        array->capacity = capacity;
    }

    // If the reallocation was unsuccessful, ignore and attempt to reduce on
    // next time.
    return;
}

bool array_allocate(Array *array, int n)
{
    // If there is already capacity for the n elements, already allocated.
    if (array->length + n <= array->capacity)
        return true;

    // Increase the capacity until n elements can be allocated within a power
    // of 2. TODO: Do this mathematically.
    int capacity = array->capacity ? array->capacity << 1 : 1;
    while (capacity < array->length + n)
        capacity <<= 1;

    size_t *reallocated = realloc(
        array->buffer,
        capacity * array->size
    );

    if (!reallocated)
        return false;

    array->buffer = reallocated;
    array->capacity = capacity;

    return true;
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
    // Check the array is not NULL, and that a back element exists.
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

bool array_insert(Array *array, int index, void *element)
{
    // Ensure the array and element is not null, and that there is allocated
    // space for the new element.
    if (!array ||
        !element ||
        index < 0 ||
        index > array->length ||
        !array_allocate(array, 1)
    ) {
        return false;
    }

    // Shift the buffer to the right by one element at the position of 
    // insertion.
    uint8_t *buf = array->buffer;
    for (int i = array->length * array->size; i-- > index * array->size;) {
        buf[i + array->size] = buf[i];
    }

    // Copy the element into the insertion place.
    memcpy(
        (uint8_t*)array->buffer + array->size * index,
        element,
        array->size
    );

    array->length++;
    return true;
}

bool array_push_back(Array *array, void *element)
{
    // Ensure the array is not NULL, the element to push back exists and there
    // is an element slot available.
    if (!array || !element || !array_allocate(array, 1))
        return false;

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
    // Ensure the array is not NULL and there exists an element to pop.
    if (!array || array->length <= 0)
        return false;

    // Pop into the element if it is not NULL.
    if (element)
        memcpy(
            element,
            (uint8_t*)array->buffer + array->size * (array->length - 1),
            array->size
        );

    array->length--;
    array_reduce(array);

    return true;
}

bool array_erase(Array *array, int index)
{
    // Ensure the array is not NULL and the index is in range.
    if (!array || index < 0 || index >= array->length)
        return false;

    uint8_t *buf = array->buffer;
    int end = (array->length - 1) * array->size;

    // Shift the buffer one element to the left at the index, overwriting it's
    // data.
    for (int i = index * array->size; i < end; i++) {
        buf[i] = buf[i + array->size];
    }

    array->length--;
    array_reduce(array);

    return true;
}

bool array_erase_range(Array *array, int index_from, int index_to)
{
    // Ensure the indicies are in range.
    if (!array ||
        index_from < 0 ||
        index_to < index_from ||
        index_to > array->length
    ) {
        return false;
    }

    // Number of elements to erase.
    int n = (index_to - index_from);

    uint8_t *buf = array->buffer;
    int end = (array->length - n) * array->size;
    int offset = n * array->size;

    // Shift the buffer to the left by n elements and overwrite the range
    // of values.
    for (int i = index_from * array->size; i < end; i++) {
        buf[i] = buf[i + offset];
    }

    array->length -= n;
    array_reduce(array);

    return true;
}

void array_apply(Array *array, ArrayApplyFunction func, void *data)
{
    for (int i = 0; i < array->length; i++) {
        func((uint8_t*)array->buffer + i * array->size, data);
    }
}

void array_clear(Array *array)
{
    free(array->buffer);
}

void array_destroy(Array *array)
{
    free(array->buffer);
    free(array);
}
