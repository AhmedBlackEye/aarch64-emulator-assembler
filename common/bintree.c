#include <stdint.h>
#include <stdlib.h>

#include "dynstr.h"
#include "debug.h"
#include "assert.h"

typedef struct Node *Node;
struct Node {
    Node left;
    Node right;    
    DynStr dstr;
};

Node make_node(char *str) {
    Node new = malloc(sizeof(struct Node));
    PANIC_IF(new == NULL, "Couldn't allocate memory");
    new->left = NULL;
    new->right = NULL;
    new->dstr  = dynstr_create(str);
    return new;
}


typedef struct BinTreeStr *BinTreeStr;
struct BinTreeStr {
    Node root;
    int size;
};


BinTreeStr make_bintreestr() {
    BinTreeStr new = malloc(sizeof(struct BinTreeStr));
    PANIC_IF(new == NULL, "Couldn't allocate memory");
    new->root = NULL;
    new->size = 0;
    return new;
}

static void destroy_bintreestr_helper(Node node) {
    if(node == NULL) return;

    destroy_bintreestr_helper(node->left);
    destroy_bintreestr_helper(node->right);

    dynstr_free(node->dstr);
    free(node);
}


void destroy_bintreestr(BinTreeStr t) {
    PANIC_IF(t == NULL, "Tree is NULL");
    destroy_bintreestr_helper(t->root);
    free(t);
}

static Node bintreestr_add_helper(Node currnode, char *str) {
    if(currnode == NULL) return make_node(str);

    int cmp = dynstr_cmp(str, currnode->dstr);
    if(cmp <= 0) {
        currnode->left  = bintreestr_add_helper(currnode->left, str);
    } else {
        currnode->right  = bintreestr_add_helper(currnode->right, str);
    }

    return currnode;
}

void bintreestr_add(BinTreeStr t, char *str) {
    PANIC_IF(t == NULL, "Tree is NULL");
    t->root = bintreestr_add_helper(t->root, str);
    t->size++;
}

static void inorder(Node node, DynStr *arr, int *index) {
    if (node == NULL) return;
    inorder(node->left, arr, index);
    arr[(*index)++] = node->dstr;
    inorder(node->right, arr, index);
}

// Test function
void test_bintreestr() {
    BinTreeStr tree = make_bintreestr();
    assert(tree != NULL);
    assert(tree->size == 0);

    bintreestr_add(tree, "banana");
    bintreestr_add(tree, "apple");
    bintreestr_add(tree, "cherry");

    assert(tree->size == 3);

    // Collect nodes in sorted order
    DynStr arr[3];
    int idx = 0;
    inorder(tree->root, arr, &idx);

    assert(strcmp(dynstr_data(arr[0]), "apple") == 0);
    assert(strcmp(dynstr_data(arr[1]), "banana") == 0);
    assert(strcmp(dynstr_data(arr[2]), "cherry") == 0);

    printf("All bintreestr tests passed!\n");

    destroy_bintreestr(tree);  // Formerly destory_bintreestr
}

int main() {
    test_bintreestr();
    return 0;
}
