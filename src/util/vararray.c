#include "util/vararray.h"

#include <assert.h>
#include <string.h>

struct VarArray {
    uint8_t *buffer;
    size_t size;
    size_t length;
    size_t space;
    size_t min_space;
    size_t max_space;
};

VarArray *vararray_create(size_t size, size_t min_space, size_t max_space)
{
    assert(max_space > min_space);
    assert(size > 0);

    VarArray *vararray = malloc(sizeof(VarArray));
    if (!vararray)
        return NULL;

    vararray->buffer = malloc(size * max_space);
    if (!vararray->buffer) {
        free(vararray);
        return NULL;
    }

    // Set variables.
    vararray->size = size;
    vararray->length = 0;
    vararray->space = max_space;
    vararray->max_space = max_space;
    vararray->min_space = min_space;

    return vararray;
}

bool vararray_append(VarArray *vararray, void *element)
{
    if (!vararray || !element)
        return false;

    // Verify the constraint min_size <= space <= max_size
    if (vararray->space - 1 < vararray->min_space) {

        // Reallocate the buffer to satisfy the constraint.
        int difference = vararray->max_space - (vararray->min_space + 1);
        uint8_t *buffer = realloc(
            vararray->buffer,
            vararray->size * (vararray->length + difference)
        );

        // If we were unable to realloc, indicate failure.
        if (!buffer)
            return false;

        // Set new values.
        vararray->buffer = buffer;
        vararray->space =  vararray->max_space;
    }
    else {
        vararray->space--;
    }

    memcpy(
        vararray->buffer + vararray->size * vararray->length,
        element,
        vararray->size
    );
    vararray->length++;

    return true;
}

void vararray_destroy(VarArray *vararray)
{
    free(vararray->buffer);
    free(vararray);
}