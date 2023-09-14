#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap* createTreeMap(int (*lower_than)(void* key1, void* key2)) {
    if (lower_than == NULL) return NULL;
    
    TreeMap* newTreeMap = (TreeMap*)malloc(sizeof(TreeMap));
    if (newTreeMap == NULL) return NULL;

    newTreeMap->root = newTreeMap->current = NULL;
    newTreeMap->lower_than = lower_than;

    return newTreeMap;
}


void insertTreeMap(TreeMap* tree, void* key, void* value) {
    if (tree == NULL || key == NULL) return;

    TreeNode* new_node = createTreeNode(key, value);

    if (new_node == NULL) return;

    if (tree->root == NULL) {
        tree->root = tree->current = new_node;
        return;
    }

    TreeNode* current = tree->root;

    while (current != NULL) {
        int comparison = tree->lower_than(key, current->pair->key);

        if (comparison == 0) {
            current->pair->value = value;
            free(new_node);
            return;
        } else if (comparison < 0) {
            if (current->left == NULL) {
                current->left = new_node;
                new_node->parent = current;
                tree->current = new_node;
                return;
            }
            current = current->left;
        } else {
            if (current->right == NULL) {
                current->right = new_node;
                new_node->parent = current;
                tree->current = new_node;
                return;
            }
            current = current->right;
        }
    }
}


TreeNode* minimum(TreeNode* x) {
    if (x == NULL) return NULL;

    while (x->left != NULL) {
        x = x->left;
    }
  
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (tree == NULL || node == NULL || tree->root == NULL) return;

    // Caso 1: El nodo es un nodo hoja (no tiene hijos).
    if (node->left == NULL && node->right == NULL) {
        if (node->parent != NULL) {
            if (node->parent->left == node) {
                node->parent->left = NULL;
            } else {
                node->parent->right = NULL;
            }
        } else {
            // El nodo es la raíz del árbol.
            tree->root = NULL;
        }
        free(node->pair->key);
        free(node->pair->value);
        free(node->pair);
        free(node);
    }

    // Caso 2: El nodo tiene un hijo.
    else if (node->left == NULL || node->right == NULL) {
        TreeNode* child = (node->left != NULL) ? node->left : node->right;

        if (node->parent != NULL) {
            if (node->parent->left == node) {
                node->parent->left = child;
            } else {
                node->parent->right = child;
            }
            child->parent = node->parent;
        } else {
            // El nodo es la raíz del árbol.
            tree->root = child;
            child->parent = NULL;
        }

        free(node->pair->key);
        free(node->pair->value);
        free(node->pair);
        free(node);
    }

    // Caso 3: El nodo tiene dos hijos.
    else {
        TreeNode* successor = minimum(node->right);

        node->pair->key = successor->pair->key;
        node->pair->value = successor->pair->value;

        // Actualiza el puntero del padre del sucesor.
        if (successor->parent->left == successor) {
            successor->parent->left = NULL;
        } else {
            successor->parent->right = NULL;
        }

        free(successor->pair->key);
        free(successor->pair->value);
        free(successor->pair);
        free(successor);
    }
}



void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}


Pair* searchTreeMap(TreeMap* tree, void* key) {
    if (tree == NULL || key == NULL || tree->root == NULL) return NULL;

    TreeNode* current = tree->root;
    TreeNode* found_node = NULL;
  
    while (current != NULL) {
        int comparison = tree->lower_than(key, current->pair->key);

        if (comparison == 0) {
            found_node = current;
            break;
        } else if (comparison < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (found_node != NULL) {
        tree->current = found_node;
        return found_node->pair;
    }

    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
