#include "set.h"

static const uint32_t mask = 0x0000000f; // Sets are numbered bits 0 through 31
    // setting initial value to set size of 4 corresponding to the 4 sorting methods

Set set_empty(void) { // returns an empty set
    return 0; // sets all bits in the set equal to 0
} // end of set_empty function

Set set_universal(void) { // returns a set where all members are part of the set
    return 0xffffffff; // sets all bits in the set equal to 1
} // end of set_universal function

bool set_member(
    Set s, uint8_t x) { // returns a bool indicating the presence of the given value x in the set s
    return (s & (0x1 << (x & mask)));
} // end of set_member function

Set set_insert(Set s, uint8_t x) { // inserts x into set s
    return (s | (0x1 << (x & mask))); // return set s with the bit corresponding to x set to 1
} // end of set_insert function

Set remove_set(Set s, uint8_t x) { // removes x from from set s
    return (s & ~(0x1 << (x & mask))); // return set s with the bit corresponding to x cleared to 0
} // end of remove_set function

Set union_set(Set s, Set t) {
    return (s | t); // returns the union of set s and set t
} // end of union_set function

Set intersect_set(Set s, Set t) {
    return (s & t); // returns the intersect of set s and set t
} // end of intersect_set function

Set difference_set(Set s, Set t) {
    return (s & ~t); // returns the elements of set s which are not in set t
} // end of difference_set

Set complement_set(Set s) {
    return ~s; // returns the complement of set s
} // end of complement_set function
