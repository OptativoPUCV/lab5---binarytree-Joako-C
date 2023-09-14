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
    TreeNode* parent = NULL;

    while (current != NULL) {
        parent = current;
        int comparison = tree->lower_than(key, current->pair->key);

        if (comparison == 0) {
            // Key already exists, update the value if needed.
            free(new_node->pair->key);
            free(new_node->pair->value);
            free(new_node->pair);
            free(new_node);
            return;
        } else if (comparison < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    int comparison = tree->lower_than(key, parent->pair->key);
    if (comparison < 0) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }
    new_node->parent = parent;
    tree->current = new_node;
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

    TreeNode* parent = node->parent;

    if (node->left == NULL && node->right == NULL) {
        if (parent != NULL) {
            if (parent->left == node) {
                parent->left = NULL;
            } else {
                parent->right = NULL;
            }
        } else {
            tree->root = NULL;
        }
        free(node->pair->key);
        free(node->pair->value);
        free(node->pair);
        free(node);
    }

    else if (node->left == NULL || node->right == NULL) {
        TreeNode* child = (node->left != NULL) ? node->left : node->right;

        if (parent != NULL) {
            if (parent->left == node) {
                parent->left = child;
            } else {
                parent->right = child;
            }
            child->parent = parent;
        } else {
            tree->root = child;
            child->parent = NULL;
        }

        free(node->pair->key);
        free(node->pair->value);
        free(node->pair);
        free(node);
    }

    else {
        TreeNode* successor = minimum(node->right);

        node->pair->key = successor->pair->key;
        node->pair->value = successor->pair->value;

        removeNode(tree, successor);
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
    if (tree == NULL || tree->root == NULL || key == NULL) return NULL;

    TreeNode* current = tree->root;
    TreeNode* upper_bound_node = NULL;

    while (current != NULL) {
        int comparison = tree->lower_than(key, current->pair->key);

        if (comparison < 0) {
            upper_bound_node = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (upper_bound_node != NULL) {
        return upper_bound_node->pair;
    }

    return NULL;
}


Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;

    TreeNode* current = tree->root;

    while (current->left != NULL) {
        current = current->left;
    }

    return current->pair;
}


Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL || tree->root == NULL) return NULL;

    TreeNode* current = tree->current;

    if (current->right != NULL) {
        current = current->right;
        while (current->left != NULL) {
            current = current->left;
        }
        tree->current = current;
        return current->pair;
    }

    TreeNode* parent = current->parent;
    while (parent != NULL && current == parent->right) {
        current = parent;
        parent = parent->parent;
    }

    if (parent != NULL) {
        tree->current = parent;
        return parent->pair;
    }

    tree->current = NULL;
    return NULL;
}

