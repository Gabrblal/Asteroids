#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stddef.h>

struct Node;
struct List;

typedef struct Node Node;
typedef struct List List;

// A node in a doubly linked list.
struct Node {
    Node *next;
    Node *last;
    void *element;
};

/**
 * A doubly linked list / deque.
 */
struct List {
    Node *head;
    Node *tail;
    size_t size;
};

/**
 * Create a new list with a given element size.
 * 
 * @param size The element size of the list.
 * @returns A pointer the list, or NULL if the list was not allocated.
 */
List *list_create(size_t size);

/**
 * Append an element to the front of the list (highest index).
 * 
 * @param list The pointer to the list to push to.
 * @param element A pointer to the element to push onto the list.
 * 
 * @returns False if the memory was unable to be allocated, or a null pointer
 * was passed as an argument, otherwise true.
 */
bool list_push_front(List *list, void *element);

/**
 * Append an element to end of the list in constant time.
 * 
 * @param list The pointer to the list to push to.
 * @param element A pointer to the element to push onto the list.
 * 
 * @return False if the memory was not allocated, or a null pointer was passed
 * as an argument, otherwise true.
 */
bool list_push_back(List *list, void *element);

/**
 * Pop the element at the front of the list, optionally into the provided
 * element, in constant time.
 * 
 * @param list The pointer to the list to pop from.
 * @param element A pointer to an element to copy the list data into. NULL if
 * no data 
 * 
 * @returns False if the list is empty, or a null pointer was passed as an
 * argument.
 */
bool list_pop_front(List *list, void *element);

/**
 * Pop the element at the back of the list into the provided element in constant
 * time.
 * 
 * @param list The pointer to the list to pop from.
 * @param element A pointer to an element to copy the list data into.
 * 
 * @returns False if the list is empty, or a null pointer was passed as an
 * argument.
 */
bool list_pop_back(List *list, void *element);

/**
 * Pop the element at the provided index into the provided element in linear
 * time.
 * 
 * @param list The pointer to the list to pop from.
 * @param index The index of the element being popped.
 * @param element A pointer to an element to copy the list data into.
 * 
 * @returns False if the index is out of range, or a null pointer was passed as
 * an argument.
 */
bool list_pop(List *list, size_t index, void *element);

/**
 * Get a pointer to the element at the provided index in linear time.
 * 
 * @param list The pointer to the list to retrieve the elment from.
 * @param index The index of the element.
 * @param element A pointer to a pointer to an element, that is set to point
 * to the requested element on success.
 * 
 * @returns False if the index is out of range, or a null pointer was passed
 * as an argument.
 */
bool list_at(const List *list, size_t index, void **element);

/**
 * Get a pointer to the front element (at maximum index) in constant time.
 * 
 * @param list The pointer to the list to retrieve the front element from.
 * @param element A pointer to a pointer to an element, that is set to point
 * to the front element on success.
 * 
 * @returns False if the list is empty, or a null pointer was passed as an
 * argument.
 */
bool list_front(const List *list, void **element);

/**
 * Get a pointer to the back element (index 0) in constant time.
 * 
 * @param list The pointer to the list to retrieve the front element from.
 * @param element A pointer to a pointer to an element, that is set to point
 * to the back element on success.
 * 
 * @returns False if the list is empty, or a null pointer was passed as an
 * argument.
 */
bool list_back(const List *list, void **element);

/**
 * Insert an element into the list at the specified index in linear time.
 * 
 * @param list The pointer to the list to insert the element into.
 * @param index The index into which to insert the element.
 * @param element A pointer to an element to copy into the list at the index.
 * 
 * @returns False if the index is out of range, or a nullptr was passed as an
 * argument.
 */
bool list_insert(List *list, size_t index, void *element);

/**
 * Find an element in the list such that the provided search function returns
 * true.
 * 
 * @param list The pointer to the list to search.
 * @param element A optional pointer to a pointer to an element, that is set to
 * point to the found element on success. Ignored if NULL.
 * @param func The search function.
 * 
 * @returns If the element was found.
 */
bool list_find(List *list, void **element, bool(*func)(void*));

/**
 * Apply a function to all elements in the list from front to back.
 * 
 * @param list The pointer to the list to search.
 * @param element A pointer to an element to copy into the successful element,
 * if any.
 * @param func The search function.
 * 
 * @returns If the function was applied to all elements.
 */
bool list_apply(List *list, void(*func)(void*));

/**
 * Destroy a list. Using the list after calling this function on it is
 * undefined.
 * 
 * @param list The list to destroy.
 */
void list_destroy(List *list);

#endif // LIST_H
