////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#include "c_list.h"

#include <assert.h>
#include <stdlib.h>

#include "c_result.h"

/**
 * Internal node struct. Stores value, next node and dtor function for value.
 */
typedef struct Node
{
    void *value;
    struct Node *next;
    void (*dtor)(void *);
} Node;

/**
 * Internal list struct. Stores a head sentinel node.
 */
typedef struct C_List
{
    Node *head;
} C_List;

/**
 * Internal iterator struct. Stores the node it is referencing.
 */
typedef struct C_ListIter
{
    Node *node;
} C_ListIter;

C_Result c_list_create(C_List **list)
{
    assert(list != NULL);

    C_Result result = C_SUCCESS;

    // allocate the list
    C_List *new_list = (C_List *)calloc(sizeof(C_List), 1u);
    if (new_list == NULL)
    {
        result = C_FAILED_TO_ALLOCATE_LIST;
        goto fail;
    }

    // allocate the head node
    new_list->head = (Node *)calloc(sizeof(Node), 1u);
    if (new_list->head == NULL)
    {
        result = C_FAILED_TO_ALLOCATE_NODE;
        goto fail;
    }

    // assign the list to the user supplied pointer
    *list = new_list;

    return result;

fail:
    // clean up and return error code
    c_list_destroy(new_list);
    return result;
}

void c_list_destroy(C_List *list)
{
    if (list == NULL)
    {
        return;
    }

    Node *cursor = list->head;

    // walk through the linked list
    do
    {
        Node *next = cursor->next;

        // if we have a dtor the call it
        if (cursor->dtor != NULL)
        {
            cursor->dtor(cursor->value);
        }

        // clean up the node and advance
        free(cursor);
        cursor = next;
    } while (cursor != NULL);

    free(list);
}

C_Result c_list_push_back(C_List *list, void *value)
{
    return c_list_push_back_dtor(list, value, NULL);
}

C_Result c_list_push_back_dtor(C_List *list, void *value, void (*dtor)(void *))
{
    assert(list != NULL);

    C_Result result = C_SUCCESS;

    // walk the list looking for rhe last node
    Node *cursor = list->head;
    while (cursor->next != NULL)
    {
        cursor = cursor->next;
    }

    // allocate a new node
    Node *new_node = (Node *)calloc(sizeof(Node), 1u);
    if (new_node == NULL)
    {
        result = C_FAILED_TO_ALLOCATE_NODE;
        goto end;
    }

    // store the supplied data abd wire up the node to the end
    new_node->value = value;
    new_node->dtor = dtor;
    cursor->next = new_node;

end:
    return result;
}

void c_list_remove(C_List *list, const C_ListIter *iter)
{
    assert(list != NULL);
    assert(iter != NULL);

    // walk the list looking for the node
    Node *cursor = list->head;
    while ((cursor->next != NULL) && (cursor->next != iter->node))
    {
        cursor = cursor->next;
    }

    // if we didn't reach the end of the list then we found the node
    if (cursor->next != NULL)
    {
        Node *to_remove = cursor->next;
        Node *next = to_remove->next;

        // if we have a dtor the call it
        if (to_remove->dtor != NULL)
        {
            to_remove->dtor(to_remove->value);
        }

        // free the node and remove it from the list
        free(to_remove);
        cursor->next = next;
    }
}

C_Result c_list_iterator_create(const C_List *list, C_ListIter **iter)
{
    assert(list != NULL);
    assert(iter != NULL);

    C_Result result = C_SUCCESS;

    // allocate the iterator
    C_ListIter *new_iter = (C_ListIter *)calloc(sizeof(C_ListIter), 1u);
    if (new_iter == NULL)
    {
        result = C_FAILED_TO_ALLOCATE_ITERATOR;
        goto end;
    }

    c_list_iterator_reset(list, &new_iter);

    // assign the iterator to the user supplied pointer
    *iter = new_iter;

end:
    return result;
}

void c_list_iterator_destroy(C_ListIter *iter)
{
    free(iter);
}

void c_list_iterator_advance(C_ListIter **iter)
{
    assert(*iter != NULL);

    (*iter)->node = (*iter)->node->next;
}

void c_list_iterator_reset(const C_List *list, C_ListIter **iter)
{
    assert(*iter != NULL);

    (*iter)->node = list->head->next;
}

bool c_list_iterator_at_end(C_ListIter *iter)
{
    assert(iter != NULL);

    return iter->node == NULL;
}

void *c_list_iterator_value(const C_ListIter *iter)
{
    assert(iter != NULL);

    return iter->node->value;
}
