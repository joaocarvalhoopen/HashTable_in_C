// HashTable in C 
// Just to test my current C programming chopsticks :-)
//
// Author: João Nuno Carvalho
// Date: 2022.08.27
// Description: This is a working implementation of a HashTable or Hashmap or
//              Associative Arrays made in the good ... old C programming
//              language.
//              This implementation, that I made, isn't optimized for speed,
//              I just wanted to put a lot of stuff in it.
//              Like I said, just to test my current chopsticks ... they are fine hehehe 
//
// License: MIT Open Source License
//

#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include <stdio.h>
#include <stdint.h>

#define RET_ERROR_SUCCESS         0
#define RET_ERROR_MINUS_1      ((int32_t) (-1))
#define RET_ERROR_MINUS_2      ((int32_t) (-2))
#define RET_ERROR_MINUS_3      ((int32_t) (-3))
#define RET_ERROR_MINUS_4      ((int32_t) (-4))
#define RET_ERROR_MINUS_5      ((int32_t) (-5))
#define RET_ERROR_MINUS_6      ((int32_t) (-6))
#define RET_ERROR_MINUS_7      ((int32_t) (-7))
#define RET_ERROR_MINUS_8      ((int32_t) (-8))
#define RET_ERROR_MINUS_9      ((int32_t) (-9))
#define RET_ERROR_MINUS_10     ((int32_t) (-10))


void * hmap_ret_element_not_found();

#define RET_ELEMENT_NOT_FOUND  (hmap_ret_element_not_found())

typedef struct HmapIter HmapIter;

typedef struct Pair {
    char * key_str;
    void * value;
} Pair;

typedef struct Hnode {
    Pair * pair;
    struct Hnode * next;
} Hnode;

typedef struct Hashmap {
    Hnode **nodes;
    size_t size_array;
    size_t size_elements;
    char * last_error;
    HmapIter * valid_iterators[10];
} Hashmap;

typedef struct HmapIter {
    Hashmap ** hmap;
    size_t curr_array_index;
    Hnode * next;
} HmapIter;

typedef struct RetTuple {
    void * data_1;
    void * data_2;
    void * data_3;
} RetTuple;

// Function example for the hmap_apply_func_key_value() 
// to process the type (int *). Don't forget that the 
// internal type is (void *), so you can make a Hashtable
// with anytype.
//
// Procedure:
//    1. Define the function.
//    2. Call the hmap_apply_func_key_value( & func_example );
//
static void func_example(Pair * pair_key_value) {
    // char * key_str = pair_key_value->key_str;
    int * value_ptr = (int *) pair_key_value->value;
    // int j = *value_ptr;

    // Increments the hashtable inner value by 100.
    *value_ptr += 100;
}


Hashmap * hmap_new();
Hashmap * hmap_new_min_size(size_t min_size);
RetTuple hmap_delete(Hashmap ** hmap_pp);
const char * hmap_get_last_error(Hashmap ** hmap_pp);
size_t hmap_get_capacity(Hashmap ** hmap_pp);
size_t hmap_get_size(Hashmap ** hmap_pp);
int32_t hmap_insert(Hashmap ** hmap_pp, char * key_str, void * value);
void * hmap_get_pair(Hashmap ** hmap_pp, char * key_str);
void * hmap_set_value(Hashmap ** hmap_pp, char * key_str, void * value);
void * hmap_remove(Hashmap ** hmap_pp, char * key_str);
HmapIter * hmap_get_iterator(Hashmap ** hmap_pp);
Pair * hmap_next(HmapIter * hmap_iter);
int32_t hmap_del_iterator(HmapIter * hmap_iter);
int32_t hmap_invalidate_iterators(Hashmap ** hmap_pp);
int32_t hmap_apply_func_key_value(Hashmap ** hmap_pp, void (*func_ptr)(Pair * pair_key_value) );
int32_t hmap_rehash(Hashmap ** hmap_pp);
size_t hmap_calc_next_prime(size_t num);

#endif  /* _HASHMAP_H_ */