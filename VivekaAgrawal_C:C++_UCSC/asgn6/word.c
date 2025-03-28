#include "word.h"
#include "code.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// WordTable wtable;

// Constructor function for Word
Word *word_create(uint8_t *syms, uint32_t len) {
    Word *w = NULL;

    // dynamically allocate memory for Word
    w = (Word *) calloc(1, sizeof(Word));

    if (syms != NULL) {
        // dynamically allocate memory to store the syms array
        uint8_t *newsyms = (uint8_t *) calloc(len, sizeof(uint8_t));

        // duplicate the syms array in the newsyms array
        memcpy(newsyms, syms, len);

        // set newsyms as the new syms array in the word
        w->syms = newsyms;
    }

    // Set the word's length to the passed in length
    w->len = len;

    // return pointer to created Word
    return w;
}

// function to construct a new Word
Word *word_append_sym(Word *w, uint8_t sym) {
    // variable to store the current length of syms array in word
    uint8_t currlen = 0;

    // check if the current word is NULL
    if (w == NULL) {
        // create a new word
        w = word_create(&sym, 1);
    }

    else {
        // word exists
        // find the length of the current syms array in word

        if (w->syms != NULL) {
            currlen = w->len;
        }

        else {
            currlen = 0;
        }

        // append sym to symbol array in w

        // dynamically allocate memory to store the newsyms array
        uint8_t *newsymsarr = (uint8_t *) calloc((currlen + 1), sizeof(uint8_t));

        // check for NULL syms array
        if (w->syms != NULL) {
            // duplicate the syms array from the word in the newsyms array
            memcpy(newsymsarr, w->syms, currlen);
        }

        // append the new symbol to the newsyms array
        memcpy((newsymsarr + currlen), &sym, 1);

        // increment the current length for appended symbol
        currlen += 1;

        // create the new word with the new syms array
        w = word_create(newsymsarr, currlen);

        // free the dynamically allocated memory for the newsyms array
        free(newsymsarr);
    }

    return w;
}

// Destructor for a Word
void word_delete(Word *w) {
    free(w->syms);
    free(w);
    return;
}

// function to create a new WordTable
WordTable *wt_create(void) {
    // dynamically allocate memory for array of words
    WordTable *wt = (WordTable *) calloc(MAX_CODE, sizeof(Word *));

    // set all word entries to NULL
    for (uint16_t i = 0; i < MAX_CODE; i++) {
        wt[i] = NULL;
    }

    // assign emptyword to index EMPTY_CODE of length 0
    wt[EMPTY_CODE] = word_create(NULL, 0);

    // return WordTable
    return wt;
}

// function to reset a WordTable
void wt_reset(WordTable *wt) {
    // set word at EMPTY_CODE index to the empty word of string length 0
    wt[EMPTY_CODE]->len = 0;

    // make sure all other words in the table are NULL
    for (uint16_t i = START_CODE; i < MAX_CODE; i += 1) {
        wt[i] = NULL;
    }
}

// Destructor for WordTable
void wt_delete(WordTable *wt) {
    // call destructor function for each word in the WordTable
    for (uint16_t i = EMPTY_CODE; i < MAX_CODE; i += 1) {
        if (wt[i] != NULL) {
            word_delete(wt[i]);
        }
    }

    // free the dynamically allocated memory for WordTable
    free(wt);
}
