#ifndef VARARRAY_H
#define VARARRAY_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct VarArray VarArray;

/**
 * Create a new variably sized array.
 * 
 * @param size The element size of the array.
 * @param min_space The minimum number of extra element spaces in the array
 * before allocating more space.
 * @param max_space The maximum number of elements to allow for for in the
 * array on allocating more space.
 * 
 * @returns The variably sized array.
 */
VarArray *vararray_create(size_t size, size_t min_space, size_t max_space);

/**
 * Added an element to the end of the array.
 * 
 * @param vararray The variable length array to add the element to.
 * @param element The element to append.
 */
bool vararray_append(VarArray *vararray, void *element);

/**
 * Deallocate and destroy the provided VarArray. Using the VarArray pointer
 * after this operation is performed is undefined.
 * 
 * @param vararray The VarArray to destroy.
 */
void vararray_destroy(VarArray *vararray);

#endif // VARARRAY_H