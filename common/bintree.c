#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bintree.h"

typedef void* GVal;
typedef void (*PrintFn)(GVal val);
typedef void (*FreeFn)(GVal val);
typedef int (*CmpFn)(GVal v1, GVal v2);

typedef struct Node *Node;
struct Node {
    GVal val;
    Node left;
    Node right;
};

struct BinTree {
    Node root;
    int size;
    CmpFn cmp;
    PrintFn pval;
    FreeFn fval;
};

Node create_node(GVal val) {
    Node new = malloc(sizeof(struct Node));
    assert(new != NULL);
    new->val = val;
    new->left = NULL;
    new->right = NULL;
    return new;
}

void destroy_node(Node n) {
    free(n);
}

BinTree create_tree(CmpFn cmp, PrintFn pval, FreeFn fval) {
    BinTree new = malloc(sizeof(struct BinTree));
    assert(new != NULL);
    new->root = NULL;
    new->size = 0;
    new->cmp = cmp;
    new->pval = pval;
    new->fval = fval;
    return new;
}

static void destroy_tree_helper(Node n, FreeFn fval) {
    if (n == NULL) return;
    destroy_tree_helper(n->left, fval);
    destroy_tree_helper(n->right, fval);
    fval(n->val);
    destroy_node(n);
}

void destroy_tree(BinTree t) {
    assert(t != NULL);
    destroy_tree_helper(t->root, t->fval);
    free(t);
}

static void print_bintree_helper(Node n, PrintFn pf, int *first) {
    if (n == NULL) return;
    print_bintree_helper(n->left, pf, first);
    
    if (*first) {
        *first = 0;
    } else {
        printf(", ");
    }
    pf(n->val);
    
    print_bintree_helper(n->right, pf, first);
}

void print_bintree(BinTree t) {
    assert(t != NULL);
    printf("[");
    int first = 1;
    print_bintree_helper(t->root, t->pval, &first);
    printf("]\n");
}

static Node bintree_add_helper(Node curr_node, GVal v, BinTree t) {
    if (curr_node == NULL) {
        t->size++;
        return create_node(v);
    }
    int res = t->cmp(v, curr_node->val);
    if (res < 0) {
        curr_node->left = bintree_add_helper(curr_node->left, v, t);
    } else if (res > 0) {
        curr_node->right = bintree_add_helper(curr_node->right, v, t);
    }

    return curr_node;
}

void bintree_add(BinTree t, GVal v) {
    assert(t != NULL);
    t->root = bintree_add_helper(t->root, v, t);
}

static Node remove_max(Node root, Node *max_node) {
    assert(root != NULL);
    if (root->right == NULL) {
        *max_node = root;
        return root->left;
    }
    root->right = remove_max(root->right, max_node);
    return root;
}

static Node bintree_remove_helper(Node curr_node, GVal target_val, BinTree t) {
    if (curr_node == NULL) return NULL;
    int res = t->cmp(target_val, curr_node->val);
    if (res < 0) {
        curr_node->left = bintree_remove_helper(curr_node->left, target_val, t);
        return curr_node;
    } else if (res > 0) {
        curr_node->right = bintree_remove_helper(curr_node->right, target_val, t);
        return curr_node;
    }

    t->size--;
    Node replacement = NULL;
    if (curr_node->left == NULL) {
        replacement = curr_node->right;
    } else if (curr_node->right == NULL) {
        replacement = curr_node->left;
    } else {
        Node max_node;
        curr_node->left = remove_max(curr_node->left, &max_node);
        max_node->left = curr_node->left;
        max_node->right = curr_node->right;
        replacement = max_node;
    }

    t->fval(curr_node->val);
    destroy_node(curr_node);
    return replacement;
}

void bintree_remove(BinTree t, GVal v) {
    assert(t != NULL);
    t->root = bintree_remove_helper(t->root, v, t);
}

static GVal bintree_lookup_helper(Node curr_node, GVal v, BinTree t) {
    if (curr_node == NULL) return NULL;
    int res = t->cmp(v, curr_node->val);
    if (res == 0) {
        return curr_node->val;
    } if (res < 0) {
        return bintree_lookup_helper(curr_node->left, v, t);
    } else {
        return bintree_lookup_helper(curr_node->right, v, t);
    }
}

GVal bintree_lookup(BinTree t, GVal v) {
    assert(t != NULL);
    return bintree_lookup_helper(t->root, v, t);
}