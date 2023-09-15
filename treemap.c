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


void insertTreeMap(TreeMap * tree, void* key, void * value){
  TreeNode* current=tree->root;
  TreeNode* newNode=createTreeNode(key, value);

  while(current!=NULL){
    if(is_equal(tree, key, current->pair->key)){
      return;
    }else if(tree->lower_than(key, current->pair->key)){
      if (current->left==NULL){
        current->left=newNode;
        newNode->parent=current;
        tree->current=newNode;
        return;
      }else{
        current=current->left;
      }
    }else{
      if (current->right==NULL){
        current->right=newNode;
        newNode->parent=current;
        tree->current=newNode;
        return;
      }else{
        current=current->right;
      }
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

  TreeNode* parent = node->parent;

  if (node->left == NULL && node->right == NULL) {
    if (parent != NULL) {
        if (parent->left == node) {
          parent->left = NULL;
        } 
        else {
          parent->right = NULL;
        }
    } else {
      if (tree->root == node) {
        tree->root = NULL;
            }
        }
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
            // Si el nodo es la raíz del árbol, actualiza la raíz al hijo.
            if (tree->root == node) {
                tree->root = child;
            }
            child->parent = NULL;
        }

        free(node);
    }

    // Caso 3: El nodo a eliminar tiene dos hijos.
    else {
        TreeNode* successor = minimum(node->right);

        // Copia el valor del sucesor al nodo actual.
        node->pair->key = successor->pair->key;
        node->pair->value = successor->pair->value;

        // Llamamos a removeNode recursivamente para eliminar el sucesor.
        removeNode(tree, successor);
    }
}



void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}


Pair * searchTreeMap(TreeMap * tree, void* key){
  if (tree==NULL || tree->root==NULL){
    return NULL;
  }
  TreeNode* current=tree->root;
  while (current!=NULL){
    if (is_equal(tree, key, current->pair->key)){
      tree->current=current;
      return tree->current->pair; 
    } else if(tree->lower_than(key, current->pair->key)){
      current=current->left;
    } else{
      current=current->right;
    }
  }
  return NULL;
}


Pair* upperBound(TreeMap * tree, void* key) {
  TreeNode* current = tree->root;
  TreeNode* ubNode = NULL;

  while (current != NULL) 
  {
    if (is_equal(tree, current->pair->key, key)) 
    {
      return current->pair;
    } else if (tree->lower_than(key, current->pair->key)) {
      ubNode = current;
      current = current->left;
    } else {
      current = current->right;
    }
  }

  if (ubNode == NULL) 
  {
    return NULL;
  } else {
      return ubNode->pair;
  }

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

