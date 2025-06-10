#ifndef BINTREE_H
#define BINTREE_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Generic value type - can hold any data type
 */
typedef void* GVal;

/**
 * @brief Function pointer type for printing values
 * @param val The value to print
 */
typedef void (*PrintFn)(GVal val);

/**
 * @brief Function pointer type for freeing values
 * @param val The value to free
 */
typedef void (*FreeFn)(GVal val);

/**
 * @brief Function pointer type for comparing values
 * @param v1 First value to compare
 * @param v2 Second value to compare
 * @return Negative if v1 < v2, 0 if v1 == v2, positive if v1 > v2
 */
typedef int (*CmpFn)(GVal v1, GVal v2);

/**
 * @brief Opaque pointer to a binary tree
 */
typedef struct BinTree *BinTree;

/**
 * @brief Creates a new binary search tree
 * @param cmp Comparison function for ordering elements
 * @param pval Function for printing values
 * @param fval Function for freeing values
 * @return Pointer to the new tree, or NULL if allocation fails
 */
BinTree create_tree(CmpFn cmp, PrintFn pval, FreeFn fval);

/**
 * @brief Destroys a binary tree and frees all memory
 * @param t The tree to destroy (must not be NULL)
 */
void destroy_tree(BinTree t);

/**
 * @brief Adds a value to the binary tree
 * 
 * If the value already exists in the tree (based on the comparison function),
 * the duplicate is ignored and not added.
 * 
 * @param t The tree to add to (must not be NULL)
 * @param v The value to add
 */
void bintree_add(BinTree t, GVal v);

/**
 * @brief Removes a value from the binary tree
 * 
 * If the value is not found, the tree remains unchanged.
 * 
 * @param t The tree to remove from (must not be NULL)
 * @param v The value to remove
 */
void bintree_remove(BinTree t, GVal v);

/**
 * @brief Looks up a value in the binary tree
 *  
 * @param t The tree to add to (must not be NULL)
 * @param v The value to add
 * @return Pointer to the new value, or NULL if fails
 */
GVal bintree_lookup(BinTree t, GVal v);

/**
 * @brief Prints the tree in sorted order
 * 
 * Performs an in-order traversal and prints all values using the
 * print function provided during tree creation.
 * 
 * @param t The tree to print (must not be NULL)
 */
void print_bintree(BinTree t);

/**
 * @brief Gets the number of elements in the tree
 * @param t The tree to query (must not be NULL)
 * @return The number of elements in the tree
 */
int bintree_size(BinTree t);

/**
 * @brief Checks if the tree is empty
 * @param t The tree to check (must not be NULL)
 * @return 1 if empty, 0 if not empty
 */
int bintree_is_empty(BinTree t);

#endif /* BINTREE_H */