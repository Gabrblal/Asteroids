#ifndef ARRAY_H
#define ARRAY_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct Array Array;

/**
 * Create a new variably sized array.
 * 
 * @param size The element size of the array.
 * @returns Pointer to the variably sized array on success, or false on failure.
 */
Array *array_create(size_t size);

/**
 * Get an element from the array at the provided index, and assign it to
 * the provided element pointer.
 * 
 * Does bounds checking.
 * 
 * @param array The array to index.
 * @param index The index of the array
 * @param element Pointer to an array element, to set to the element at the
 * provided index in the array on success, or NULL on out of bounds.
 * 
 * @returns True on success, false on out of bounds error or failure.
 */
bool array_at(Array *array, int index, void **element);

/**
 * Get the element at the front / start of an array.
 * 
 * @param array The array to get the element from.
 * @param element Pointer to an array element, to set to the element at the
 * front of the array on success, or NULL on out of bounds.
 * 
 * @returns True on success, false on out of bounds error or failure.
 */
bool array_front(Array *array, void **element);

/**
 * Get the element at the back / end of an array.
 * 
 * @param array The array to get the element from.
 * @param element Pointer to an array element, to set to the element at the
 * end of the array on success, or NULL on out of bounds.
 * 
 * @returns True on success, false on out of bounds error or failure.
 */
bool array_back(Array *array, void **element);

/**
 * Get a pointer to the underlying data buffer of the array.
 * 
 * @param array The array to get the data buffer of.
 * @returns Pointer to the data buffer.
 */
void *array_data(Array *array);

/**
 * Check if an array is empty.
 * 
 * @param array The array to check for emptiness.
 * @returns True if the array is empty, otherwise false.
 */
bool array_empty(Array *array);

/**
 * Return the length of an array, the number of elements in it.
 * 
 * @param array The array to check the length of.
 * @returns The length of the array.
 */
int array_length(Array *array);

/**
 * Delete all data in the array and make the list empty.
 * 
 * @param array The array to clear of all data.
 */
void array_clear(Array *array);

/**
 * Insert an element into the array at the provided index.
 * 
 * @param array The array to insert the value into.
 * @param index The index into which the element should be inserted.
 * @param element The element to insert into the array.
 * 
 * @returns True on success, false on out of bounds or failure.
 */
bool array_insert(Array *array, int index, void *element);

/**
 * Append an element to the end / back of the array.
 * 
 * @param array The array to add the element to.
 * @param element The element to append.
 * 
 * @returns True on success, false on failure.
 */
bool array_push_back(Array *array, void *element);

/**
 * Copy the last element into the provided element, and then remove it from
 * the array.
 * 
 * @param array The array to pop the last element from.
 * @param element Pointer to an array element to set the last element to, or
 * NULL on out of bounds.
 * 
 * @returns True on success, false on out of bounds or failure.
 */
bool array_pop_back(Array *array, void *element);

/**
 * Erase an element from the array at the provided index.
 * 
 * @param array The array to erase to element from.
 * @param index The index of the element in the array to erase.
 * 
 * @returns True on success, false on failure.
 */
bool array_erase(Array *array, int index);

/**
 * Erase a range of elements from the array.
 * 
 * @param array The array to erase to elements from.
 * @param index_from The left hand side index to erase from (included).
 * @param index_to The right hand side index to erase to (excluded).
 * 
 * @returns True on success, false on failure.
 */
bool array_erase_range(Array *array, int index_from, int index_to);

/**
 * Function prototype to provide to array_apply.
 * 
 * @param element The element in the array being applied.
 * @param data The optional data forwarded to the function from array_apply.
 */
typedef void(ArrayApplyFunction)(void *element, void *data);

/**
 * Apply a function to all elements in the array.
 * 
 * @param array The array to apply the function to.
 * @param func The function to apply to all elements in the array.
 * @param data Optional data to forward to the applied function.
 */
void array_apply(Array *array, ArrayApplyFunction func, void *data);

/**
 * Deallocate and destroy the provided Array. Using the Array pointer
 * after this operation is performed is undefined.
 * 
 * @param array The Array to destroy.
 */
void array_destroy(Array *array);

#endif // ARRAY_H
