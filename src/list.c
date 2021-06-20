#include "list.h"

#include <stdlib.h>
#include <string.h>

List *list_create(size_t size)
{
    // Allocate the list, return a pointer to make it easier to pass into
    // the rest of the interface.
    List *list = malloc(sizeof(List));
    if (!list)
        return NULL;

    list->size = size;
    list->head = NULL;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

bool list_push(List *list, void *element, bool front_back)
{
    // If the element is NULL then there's no buffer to copy from.
    if (!element || !list)
        return false;

    // Allocate a new node.
    Node *node = malloc(sizeof(Node));
    if (!node)
        return false;

    // Allocate space for the node's element.
    node->element = malloc(list->size);
    if (!node->element) {
        free(node);
        return false;
    }

    // Copy element to the node element buffer.
    memcpy(node->element, element, list->size);

    // Link the new node into the list. Note that either list->head or
    // list->tail can be NULL.
    if (front_back) {
        node->next = list->head;
        node->last = NULL;
        if (list->head) {
            list->head->last = node;
            list->head = node;
        }
        else {
            list->head = node;
            list->tail = node;
        }
    }
    else {
        node->next = NULL;
        node->last = list->tail;
        if (list->tail) {
            list->tail->next = node;
            list->tail = node;
        }
        else {
            list->head = node;
            list->tail = node;
        }
    }

    return true;
}

bool list_push_front(List *list, void *element) {
    return list_push(list, element, true);
}

bool list_push_back(List *list, void *element) {
    return list_push(list, element, false);
}

bool list_pop_front_back(List *list, void *element, bool front_back)
{
    // If there's no buffer to pop into, or there is no node to pop from, then
    // fail.
    if (!list || !list->head /* || !list->tail */)
        return false;

    // The node that is now at the front or back of the list, and the one
    // that is being popped off.
    Node *new;
    Node *old;
    if (front_back) {
        old = list->head;
        new = old->next;
        if (new)
            new->last = NULL;
    }
    else {
        old = list->tail;
        new = old->last;
        if (new)
            new->next = NULL;
    }

    // Pop node contents into element pointer.
    if (element)
        memcpy(element, old->element, list->size);

    // Free the node.
    free(old->element);
    free(old);

    // Keep track of the new list ends.
    if (front_back) {
        list->head = new;
    }
    else
        list->tail = new;

    return true;
}

bool list_pop_front(List *list, void *element) {
    return list_pop_front_back(list, element, true);
}

bool list_pop_back(List *list, void *element) {
    return list_pop_front_back(list, element, false);
}

bool list_pop(List *list, size_t index, void *element)
{
    // If there's no buffer to pop into, fail.
    if (!element || !list)
        return false;

    // Iterate to the indexed node. If it does not exist, then return false.
    Node *node = list->head;
    for (size_t i = 0; i < index; ++i) {
        if (node->next)
            node = node->next;
        else
            return false;
    }

    // Copy element.
    if (element)
        memcpy(element, node->element, list->size);

    // Reconfigure the list.
    if (node->next && node->last) { // Popping inbetween
        node->next->last = node->last;
        node->last->next = node->next;
    }
    else if (node->next) { // Popping head.
        node->next->last = NULL;
        list->head = node->next;
    }
    else if (node->last) { // Popping tail.
        node->last->next = NULL;
        list->tail = node->last;
    }
    else { // Removed last node.
        list->head = NULL;
        list->tail = NULL;
    }

    // Delete the node.
    free(node->element);
    free(node);

    return true;
}

bool list_at(const List *list, size_t index, void **element)
{
    // If there's no buffer to pop into, fail.
    if (!element || !list)
        return false;

    // Iterate to the indexed node. If it does not exist, then return false.
    Node *node = list->head;
    for (size_t i = 0; i < index; ++i) {
        if (node->next)
            node = node->next;
        else
            return false;
    }

    // Set the pointer to the element.
    *element = node->element;

    return true;
}

bool list_front(const List *list, void **element)
{
    // If there's no buffer to pop into, or the list is empty, fail.
    if (!element || !list || !list->head)
        return false;

    *element = list->head->element;
    return true;
}

bool list_back(const List *list, void **element)
{
    // If there's no buffer to pop into, or the list is empty, fail.
    if (!element || !list || !list->tail)
        return false;

    *element = list->tail->element;
    return true;
}

bool list_insert(List *list, size_t index, void *element)
{
    // If there's no buffer to pop into, or the list is empty, fail.
    if (!element || !list)
        return false;

    // Loop to the node index.
    Node *node = list->head;
    for (size_t i = 0; i < index; ++i) {
        if (node)
            node = node->next;
        else
            return false;
    }

    // If the node was the first node, or NULL, then push the element to the
    // back.
    if (node == list->tail)
        return list_push_back(list, element);

    // If the node was the head then push the element to the front.
    if (node == list->head)
        return list_push_front(list, element);

    // Allocate a new node to insert.
    Node *new = malloc(sizeof(Node));
    if (!new)
        return false;

    new->element = malloc(list->size);
    if (!new->element) {
        free(new);
        return false;
    }

    // Link the node.
    new->next = node;
    new->last = node->last;

    // Fix adjacent nodes.
    new->last->next = node;
    new->next->last = node;

    return true;
}

bool list_find(List *list, void *element, bool(*func)(void*))
{
    if (!list || !func)
        return false;

    Node *node = list->head;
    while (node) {
        if (func(node->element)) {
            memcpy(element, node->element, list->size);
            return true;
        }
        node = node->next;
    }

    return false;
}

void list_destroy(List *list)
{
    Node *node = list->head;
    Node *next = NULL;

    while (node) {
        next = node->next;
        free(node->element);
        free(node);
        node = next;
    }

    free(list);
}
