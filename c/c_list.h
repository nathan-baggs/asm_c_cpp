////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stdbool.h>

#include "c_result.h"

/**
 * Simple linked list data structure which can store void*.
 */

/**
 * Handle to internal list data.
 */
typedef struct C_List C_List;

/**
 * Handle to internal list iterator data.
 */
typedef struct C_ListIter C_ListIter;

/**
 * Create a new list.
 *
 * @param list
 *   Out parameter for created list.
 *
 * @returns
 *   C_SUCCESS on success
 *   Another Result type on error
 */
C_Result c_list_create(C_List **list);

/**
 * Destroy a list.
 *
 * Will call dtor function for each node (if supplied).
 *
 * @param list
 *   List to destroy.
 */
void c_list_destroy(C_List *list);

/**
 * Push a new value to the end of the list.
 *
 * @param list
 *   List to add to.
 *
 * @param value
 *   Value to add.
 *
 * @returns
 *   C_SUCCESS on success
 *   Another Result type on error
 */
C_Result c_list_push_back(C_List *list, void *value);

/**
 * Push a new value to the end of the list, with a dtor function which will get called when the node is removed or the
 * list is destroyed.
 *
 * @param list
 *   List to add to.
 *
 * @param value
 *   Value to add.
 *
 * @param dtor
 *   Function to call to cleanup value.
 *
 * @returns
 *   C_SUCCESS on success
 *   Another Result type on error
 */
C_Result c_list_push_back_dtor(C_List *list, void *value, void (*dtor)(void *));

/**
 * Remove the node referenced by an iterator from a list.
 *
 * @param list
 *   List to remove node from.
 *
 * @param iter
 *   Iterator to node to remove.
 */
void c_list_remove(C_List *list, const C_ListIter *iter);

/**
 * Create a new iterator to the first node.
 *
 * Note this will be NULL if the list is empty.
 *
 * @param list
 *   List to create iterator in.
 *
 * @param iter
 *  Out parameter for created iterator.
 *
 * @returns
 *   C_SUCCESS on success
 *   Another Result type on error
 */
C_Result c_list_iterator_create(const C_List *list, C_ListIter **iter);

/**
 * Destroy an iterator.
 *
 * @param iter
 *   Iterator to destroy.
 */
void c_list_iterator_destroy(C_ListIter *iter);

/**
 * Advance an iterator to the next node.
 *
 * @param iter
 *   Out parameter for advanced iterator.
 */
void c_list_iterator_advance(C_ListIter **iter);

/**
 * Reset an iterator back to the start of the list.
 *
 * @param list
 *   List to reset iterator in.
 *
 * @param iter
 *   Out paramater for reset iterator.
 */
void c_list_iterator_reset(const C_List *list, C_ListIter **iter);

/**
 * Check if an iterator is one past the end of the list.
 *
 * @param iter
 *   Iterator to check.
 *
 * @returns
 *   True if iterator is one past the end of the list, otherwise false.
 */
bool c_list_iterator_at_end(C_ListIter *iter);

/**
 * Get the value the iterator is referencing.
 *
 * @param iter
 *   Iterator to get value of.
 *
 * @returns
 *   Value at iterator.
 */
void *c_list_iterator_value(const C_ListIter *iter);
