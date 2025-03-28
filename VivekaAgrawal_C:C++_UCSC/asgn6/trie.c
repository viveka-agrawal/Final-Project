#include "trie.h"
#include "code.h"
#include <stdlib.h>
#include <inttypes.h>

// Constructor function for creating TrieNode
TrieNode *trie_node_create(uint16_t index) {
    // dynamically allocate memory for TrieNode
    TrieNode *tnode = (TrieNode *) calloc(1, sizeof(TrieNode));

    // Initialize all children node pointers to NULL
    for (uint16_t i = 0; i < ALPHABET; i += 1) {
        tnode->children[i] = NULL;
    }

    // Set the node's code to index
    tnode->code = index;

    // return pointer to created TrieNode
    return tnode;
}

// Destructor function for deleting TrieNode
void trie_node_delete(TrieNode *n) {
    // free the dynamically allocated memory for n
    free(n);
    return;
}

// function to initialize a Trie
TrieNode *trie_create(void) {
    TrieNode *root;

    // call the TrieNode constructor function to initialize a root TrieNode
    root = trie_node_create(EMPTY_CODE);
    return root;
}

// function to reset a Trie to just the root TrieNode
void trie_reset(TrieNode *root) {
    // recursively call trie_delete on each of n's children
    for (uint16_t i = 0; i < ALPHABET; i += 1) {
        if (root->children[i] != NULL) {
            trie_delete(root->children[i]);
        }
    }

    // reset all children node pointers to NULL
    for (uint16_t i = 0; i < ALPHABET; i += 1) {
        root->children[i] = NULL;
    }

    // reset the root's code to EMPTY_CODE
    root->code = EMPTY_CODE;
}

// function to delete a sub-trie
void trie_delete(TrieNode *n) {
    // recursively call trie_delete on each of n's children
    for (uint16_t i = 0; i < ALPHABET; i += 1) {
        if (n->children[i] != NULL) {
            trie_delete(n->children[i]);
        }
    }

    // reset all children node pointers to NULL
    for (uint16_t i = 0; i < ALPHABET; i += 1) {
        n->children[i] = NULL;
    }

    // delete the root node
    trie_node_delete(n);
}

// function to return pointer to child node representing symbol sym
TrieNode *trie_step(TrieNode *n, uint8_t sym) {
    TrieNode *child;

    // set the child pointer to the child node representing the symbol sym
    child = n->children[sym];

    // if symbol does not exist, NULL will be returned since all child pointers were initialized to NULL when TrieNode was created
    return child;
}
