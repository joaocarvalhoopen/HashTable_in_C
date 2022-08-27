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

#include "hashmap.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h> 

/*

typedef struct HmapIter HmapIter;

typedef struct Pair {
    char * key_str;
    void * value;
} Pair;

typedef struct Hnode {
    Pair * pair;
    struct Hnode * next;
} Hnode;

typedef strcut Curr_Iterator {
    HmapIter * hmap_iter[10];
}

typedef struct hmap {
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
    void * data_3
} RetTuple;

*/

#define HMAP_MIN_SIZE_PRIME  (11)

// Private
static int32_t hmap_calc_hash_4_string(Hashmap ** hmap_pp, char * key_str);

static RetTuple hmap_delete_optional_ptr(Hashmap ** hmap_pp, bool dont_free_the_outer_pointer);
static int32_t hmap_insert_key_copy_option(Hashmap ** hmap_pp, char * key_str, void * value, bool copy_keys_str);


void * hmap_ret_element_not_found() {
    static const char * singleton_address = "_NO_ELEMENT!_";
    return ((void *) singleton_address);
}

Hashmap * hmap_new() {
    Hashmap * hmap = hmap_new_min_size(HMAP_MIN_SIZE_PRIME);
    return hmap;
}

Hashmap * hmap_new_min_size(size_t min_size) {
    if (min_size <= 11) {
        min_size = HMAP_MIN_SIZE_PRIME;
        // printf("\nhmap nmin_size: %ld \n", min_size_x);
    } else {
        min_size = hmap_calc_next_prime(min_size);
        // printf("\nhmap nmin_size: %ld \n", min_size_x);
    }
    
    // Allocate Hmap structure.
    Hashmap * hmap = malloc(sizeof(Hashmap));
    if (hmap == NULL) {
        return NULL;
    }
    hmap->size_array = min_size;
    hmap->last_error = NULL;
    hmap->size_elements = 0;
    // Allocate Hnode * structure (dynamic array).
    // hmap->nodes = malloc(hmap->size_array * sizeof(Hnode **));
    hmap->nodes = malloc(hmap->size_array * sizeof(Hnode *));
    if (hmap->nodes == NULL) {
        free(hmap);
        hmap = NULL;
        return NULL;
    }

    // Initialize the nodes array pointers to NULL.
    for(size_t i = 0; i < hmap->size_array; i++) {
        hmap->nodes[i] = NULL;
    }


    size_t const max_size_valid_iterators = sizeof(hmap->valid_iterators) 
                                          / sizeof(hmap->valid_iterators[0]);

    // Initialize the valid_iterators array pointers to NULL.
    for(size_t i = 0; i < max_size_valid_iterators; i++) {
        hmap->valid_iterators[i] = NULL;
    }

    return hmap;
}

// Returns (data_1, data_2, data_3)
//
// data_1: The array with the pointers to the Pair elements that have to
// be free() by the calling function.
//
// data_2: size_t of the data_1 returned table.
//
// data_3: error_code
//
// Note: The struture RetTuple is copied by value.
RetTuple hmap_delete(Hashmap ** hmap_pp) {
    const bool DONT_FREE_THE_OUTER_POINTER = false;
    RetTuple res = hmap_delete_optional_ptr(hmap_pp, DONT_FREE_THE_OUTER_POINTER);
    return res;
}

// Returns (data_1, data_2, data_3)
//
// data_1: The array with the pointers to the Pair elements that have to
// be free() by the calling function.
//
// data_2: size_t of the data_1 returned table.
//
// data_3: error_code
//
// Note: The struture RetTuple is copied by value.
static RetTuple hmap_delete_optional_ptr(Hashmap ** hmap_pp, bool dont_free_the_outer_pointer) {
    int32_t * ret_error = malloc(sizeof(uint32_t *));
    // Validates the pointer.
    if (hmap_pp == NULL || *hmap_pp == NULL) {
        if (ret_error != NULL) {
            *ret_error = RET_ERROR_MINUS_1;
        }
        RetTuple ret_tuple = {.data_1 = NULL, .data_2 = NULL, .data_3 = ret_error };
        return ret_tuple;
    }
    // Pass one indirection of pointer.
    Hashmap * hmap = *hmap_pp;

    if (hmap->last_error != NULL) {
        hmap->last_error = NULL;
    }

    hmap_invalidate_iterators(hmap_pp);

    Pair ** ret_ptr_array = NULL; 
    size_t * ret_ptr_size = malloc(sizeof(size_t)); 
    if (ret_ptr_size != NULL) {
        *ret_ptr_size = hmap->size_elements;
    }

    // Delete one by one the list elements nodes and allocates memory to return
    // ptrs array to those Pairs has a void ** that's an "Pair *var[]" . 
    if (hmap->size_elements != 0) {
        ret_ptr_array = malloc(sizeof(Pair *) * hmap->size_elements);
        // TODO: Test the ret_ptr_array != NULL case.
        size_t index_ret = 0;
        size_t index_array = 0;
        while( (index_array < hmap->size_array) && (hmap->size_elements != 0) ) {
            if (hmap->nodes[index_array] != NULL) {              
                Hnode * prev_node = NULL;

                Hnode * hnode = hmap->nodes[index_array];
                hmap->nodes[index_array] = NULL;

                for( ; hnode != NULL; hnode = hnode->next ) {
                        
                        ret_ptr_array[index_ret] = hnode->pair;
                        index_ret++;
                        if (prev_node != NULL) {
                            free(prev_node);
                            prev_node = NULL;
                            // Decrements the number of elements.
                            hmap->size_elements--;
                        }
                        prev_node = hnode;
                }
                if (prev_node != NULL) {
                    free(prev_node);
                    prev_node = NULL;
                    // Decrements the number of elements.
                    hmap->size_elements--;
                }
            }
            index_array++;
        }
    }

    free(hmap->nodes);
    hmap->nodes = NULL;
    free(hmap);
    hmap = NULL;
    if (!dont_free_the_outer_pointer) {
        // free(hmap_pp);
        hmap_pp = NULL;
    }

    // Returns (data_1, data_2, data_3)
    //
    // data_1: The array with the pointers to the Pair elements that have to
    // be free() by the calling function.
    //
    // data_2: size_t of the data_1 returned table.
    //
    // data_3: error_code

    if (ret_error != NULL) {
        *ret_error = RET_ERROR_SUCCESS;
    }
    // Note: The struture RetTuple is copied by value.
    RetTuple ret_tuple = {.data_1 = ret_ptr_array, .data_2 = ret_ptr_size, .data_3 = ret_error };
    return ret_tuple;
}

const char * hmap_get_last_error(Hashmap ** hmap_pp) {    
    // Validates the pointer.
    if (hmap_pp == NULL || *hmap_pp == NULL) {
        const char * HMAP_ERROR = "ERROR: In hmap_get_error(), hmp parameter is NULL!"; 
        return HMAP_ERROR;
    }
    Hashmap * hmap = *hmap_pp;
    return hmap->last_error;
}

size_t hmap_get_capacity(Hashmap ** hmap_pp) {
    // Validates the pointer.
    if (hmap_pp == NULL || *hmap_pp == NULL) {
        return 0;
    }
    Hashmap * hmap = *hmap_pp;
    return hmap->size_array;
}

size_t hmap_get_size(Hashmap ** hmap_pp) {
    // Validates the pointer.
    if (hmap_pp == NULL || *hmap_pp == NULL) {
        return 0;
    }
    Hashmap * hmap = *hmap_pp;
    return hmap->size_elements;
}

int32_t hmap_insert(Hashmap ** hmap_pp, char * key_str, void * value) {
    const bool copy_keys_str = true; 
    int32_t res = hmap_insert_key_copy_option( hmap_pp, key_str, value, copy_keys_str);
    return res;
}

static int32_t hmap_insert_key_copy_option(Hashmap ** hmap_pp, char * key_str, void * value, bool copy_keys_str) {
    // Validates the pointer.
    if (hmap_pp == NULL || *hmap_pp == NULL) {
        return RET_ERROR_MINUS_1;
    }
    Hashmap* hmap = *hmap_pp;
    if (key_str == NULL) {
        hmap->last_error = "Error: In hmap_insert(), key parameter is NULL.";
        return RET_ERROR_MINUS_2;
    }
    if (value == NULL) {
        hmap->last_error = "Error: In hmap_insert(), value parameter is NULL.";
        return RET_ERROR_MINUS_3;
    }
    // Test if we need a bigger hashtable, we need to rehash.
    if (hmap->size_elements + 1 >= hmap->size_array * 1.5) {
        // IMPORTANT: This will create a new Hashmap and copy the contents of the current to it.
        //            But the pointer do hmap will be the same because it's a indirect pointer. 
        hmap_rehash(hmap_pp);
        // Important note: We have to get it again because inside the hmap_rehash()
        // we mantained the hmap_pp but changed the *hmap_pp, that points to the Struct Hashmap.
        hmap = *hmap_pp;
    }
    int32_t hash_res = hmap_calc_hash_4_string(hmap_pp, key_str);
    if (hash_res < 0) {
        hmap->last_error = "Error: In hmap_calc_hash_4_string() inside hmap_insert(), key_str parameter is NULL.";
        return RET_ERROR_MINUS_4;
    }

    size_t index_array = ((size_t) hash_res) % hmap->size_array;
    Hnode * curr_node = hmap->nodes[index_array];
    Hnode * last_node = NULL;

    // While it's not the last element, check if the element already exists and advance to the last element.
    while (curr_node != NULL) {
        if (strcmp(curr_node->pair->key_str, key_str) == 0) {
            // Key already exists.
            hmap->last_error = "Error: In hmap_insert(), key_str already exists.";
            return RET_ERROR_MINUS_5;
        }
        last_node = curr_node;
        curr_node = curr_node->next;
    }

    // Add a new node at the end of the linked list.
    Hnode * new_node = malloc(sizeof(Hnode));
    if (new_node == NULL) {
        hmap->last_error = "Error: In hmap_insert(), internal error in malloc() new_node.";
        return RET_ERROR_MINUS_6;
    }
    new_node->next = NULL;

    // Create a Pair.
    Pair * new_pair = malloc(sizeof(Pair));
    if (new_pair == NULL) {
        hmap->last_error = "Error: In hmap_insert(), internal error in malloc() new pair.";
        return RET_ERROR_MINUS_7;
    }
    new_node->pair = new_pair;

    if (copy_keys_str) {
        new_pair->key_str = strdup(key_str);
    } else {
        new_pair->key_str = key_str;
    }

    new_pair->value = value;    

    hmap_invalidate_iterators(hmap_pp);

    // Insert the node in the linked list.
    if (hmap->nodes[index_array] == NULL) {
        hmap->nodes[index_array] = new_node;
    } else {
        // curr_node->next = new_node;
        last_node->next = new_node;
    }
    // Increments the number of elements.
    hmap->size_elements++;
    return RET_ERROR_SUCCESS;
}

// Returns the internal pair that can be used to update the
// value inside the hashmap ex : ((int* ) pair->value) += 1.
void * hmap_get_pair(Hashmap ** hmap_pp, char * key_str) {
    // Validates the pointer.
    if (hmap_pp == NULL || *hmap_pp == NULL) {
        return NULL;
    }
    Hashmap * hmap = *hmap_pp;
    if (key_str == NULL) {
        hmap->last_error = "Error: In hmap_get(), key parameter is NULL.";
        return NULL;
    }
    int32_t hash_res = hmap_calc_hash_4_string(hmap_pp, key_str);
    if (hash_res < 0) {
        hmap->last_error = "Error: In hmap_calc_hash_4_string() inside hmap_get(), key_str parameter is NULL.";
        return NULL;
    }

    size_t index_array = ((size_t) hash_res) % hmap->size_array;
    Hnode * curr_node = hmap->nodes[index_array];

    // While it's not the last element, check if the element exists.
    while (curr_node != NULL) {
        if (strcmp(curr_node->pair->key_str, key_str) == 0) {
            // Key exists.
            // Returns the internal pair that can be used to update the
            // value inside the hashmap ex : ((int* ) pair->value) += 1.
            return curr_node->pair;
        }
        curr_node = curr_node->next;
    }

    // Returns a singleton to a fixed address that is always the same and unique address.
    // In this way the return of this function can be tested for error with NULL,
    // and can be tested for RET_ELEMENT_NOT_FOUND if the element doesn't exist in Hashmap.
    return RET_ELEMENT_NOT_FOUND;
}

// If the key_str is found, set the value for this key and returns the previous value,
// so that the programmer using this hashmap can free it's memory or use it for other
// thing in the same program.
void * hmap_set_value(Hashmap ** hmap_pp, char * key_str, void * value) {
    // Validates the pointer.
    if (hmap_pp == NULL || *hmap_pp == NULL) {
        return NULL;
    }
    Hashmap * hmap = *hmap_pp;
    if (key_str == NULL) {
        hmap->last_error = "Error: In hmap_get(), key parameter is NULL.";
        return NULL;
    }
    if (value == NULL) {
        hmap->last_error = "Error: In hmap_get(), value parameter is NULL.";
        return NULL;
    }
    int32_t hash_res = hmap_calc_hash_4_string(hmap_pp, key_str);
    if (hash_res < 0) {
        hmap->last_error = "Error: In hmap_calc_hash_4_string() inside hmap_get(), key_str parameter is NULL.";
        return NULL;
    }

    size_t index_array = ((size_t) hash_res) % hmap->size_array;
    Hnode * curr_node = hmap->nodes[index_array];

    // While it's not the last element, check if the element exists.
    while (curr_node != NULL) {
        if (strcmp(curr_node->pair->key_str, key_str) == 0) {
            // Key exists.

            void * ret_old_value = curr_node->pair->value;
            // Updates to the new value.
            curr_node->pair->value = value;
            
            // The key_str was found, returns the previous old value, so that the
            // programmer using this hashmap can free it's memory or use it for other
            // thing in the same program.
            return ret_old_value;
        }
        curr_node = curr_node->next;
    }

    // Returns a singleton to a fixed address that is always the same and unique address.
    // In this way the return of this function can be tested for error with NULL,
    // and can be tested for RET_ELEMENT_NOT_FOUND if the element doesn't exist in Hashmap.
    return RET_ELEMENT_NOT_FOUND;
}

void * hmap_remove(Hashmap ** hmap_pp, char * key_str) {
    // Validates the pointer.
    if (hmap_pp == NULL || *hmap_pp == NULL) {
        return NULL;
    }
    Hashmap * hmap = *hmap_pp;
    if (key_str == NULL) {
        hmap->last_error = "Error: In hmap_remove(), key parameter is NULL.";
        return NULL;
    }
    int32_t hash_res = hmap_calc_hash_4_string(hmap_pp, key_str);
    if (hash_res < 0) {
        hmap->last_error = "Error: In hmap_calc_hash_4_string() inside hmap_remove(), key_str parameter is NULL.";
        return NULL;
    }

    size_t index_array = ((size_t) hash_res) % hmap->size_array;
    
    Hnode * curr_node = hmap->nodes[index_array];
    Hnode * prev_node = curr_node;
    // While it's not the last element, check if the element exists.
    while (curr_node != NULL) {
        // Key exists.
        if (strcmp(curr_node->pair->key_str, key_str) == 0) {
            if (curr_node == hmap->nodes[index_array]) {
                hmap->nodes[index_array] = curr_node->next;
            } else  {
                // In the linked list, bypasses the curr_node that we are removing.
                prev_node->next = curr_node->next; 
            }
            curr_node->next = NULL;
            Pair * pair = curr_node->pair;

            hmap_invalidate_iterators(hmap_pp);

            // Decrements the number of elements.
            hmap->size_elements--;
            free(curr_node);
            curr_node = NULL;
            return pair;
        }
        prev_node = curr_node;
        curr_node = curr_node->next;
    }

    // Returns a singleton to a fixed address that is always the same and unique address.
    // In this way the return of this function can be tested for error with NULL,
    // and can be tested for RET_ELEMENT_NOT_FOUND if the element doesn't exist in Hashmap.
    return RET_ELEMENT_NOT_FOUND;
}

HmapIter * hmap_get_iterator(Hashmap ** hmap_pp) {
    // Validates the pointer.
    if (hmap_pp == NULL || *hmap_pp == NULL) {
        return NULL;
    }
    Hashmap * hmap = *hmap_pp;

    /*    
    typedef struct HmapIter {
        Hashmap ** hmap;
        size_t curr_array_index;
        Hnode * next;
    } HmapIter;
    */

    HmapIter * hmap_iter = malloc(sizeof(HmapIter));
    if (hmap_iter == NULL) {
        hmap->last_error = "Error: Hashmap in func hmap_get_iterator(), while doing a internal malloc.";
        return NULL;
    }

    hmap_iter->hmap = hmap_pp;
    hmap_iter->next = NULL;
    hmap_iter->curr_array_index = 0;

    size_t const max_size_valid_iterators = sizeof(hmap->valid_iterators) 
                                          / sizeof(hmap->valid_iterators[0]);

    bool flag_found_empty_slot = false;
    size_t valid_iterators_index = 0;
    for(size_t i = 0; i < max_size_valid_iterators; i++) {
        if (hmap->valid_iterators[i] == NULL) {
            flag_found_empty_slot = true;
            valid_iterators_index = i;
            hmap->valid_iterators[i] = hmap_iter;
            break;
        }
    }
    if (!flag_found_empty_slot) {
        free(hmap_iter);
        hmap_iter = NULL;
        hmap->last_error = "Error: Hashmap in hmap_get_iterator(), Currently there was no empty slot for this iterator.";
        return NULL;
    }

    // Seek to the next first element inside the Hashmap.
    size_t array_index = 0;
    hmap_iter->next = (hmap->nodes[array_index]);

    while((array_index < hmap->size_array)
           && hmap_iter->next != NULL) {

        if (hmap_iter->next != NULL) {
            return hmap_iter;
        }
        array_index++;
        hmap_iter->next = (hmap->nodes[array_index]);
    }

    if ((hmap_iter->curr_array_index >= hmap->size_array)
         && hmap_iter->next == NULL) {
        // It will an iterator to a empty hmap that will be
        // invalidated so we take it out of the valid_iterator array.
        hmap->valid_iterators[valid_iterators_index] = NULL;
        // This will be tested inside the call to hmap_next(). 
    }

    // The Hashmap is empty and in this case the iterator returns that
    // it doesn't have any values in the hamp_next() first call,
    // it will return NULL in the first use.
    return hmap_iter;
}

Pair * hmap_next(HmapIter * hmap_iter) {
    // Validates the pointer.
    if (hmap_iter == NULL) {
        return NULL;
    }
    
    // There is a double pointer indirection in here.
    Hashmap * hmap = *(hmap_iter->hmap);

    if ((hmap_iter->curr_array_index == hmap->size_array)
           && hmap_iter->next != NULL) {
        // The hashmap is empty
        // or there are no more elements after calling hmap_next sometimes,
        // or the iterator was invalidated.
        return NULL; 
    }

    size_t const max_size_valid_iterators = sizeof(hmap->valid_iterators) 
                                          / sizeof(hmap->valid_iterators[0]);

    bool flag_iterator_is_valid = false;
    // size_t valid_iterators_index = 0;
    for(size_t i = 0; i < max_size_valid_iterators; i++) {
        if (hmap->valid_iterators[i] == hmap_iter) {
            flag_iterator_is_valid = true;
            // valid_iterators_index = i;
            break;
        }
    }

    if (!flag_iterator_is_valid) {
        return NULL;
    }

    // Saves the current ret value;
    
    void * curr_ret_pair = NULL;
    if (  hmap_iter != NULL 
        && hmap_iter->next != NULL
        && hmap_iter->next->pair != NULL) {
        
        curr_ret_pair = hmap_iter->next->pair;  
    } else {
        return NULL;
    }

    // Seek the state to the next position if there is one,
    // to prepare to the next call to hmap_next().    
    while(true) {
        if ((hmap_iter->curr_array_index >= hmap->size_array)
            && hmap_iter->next != NULL) {
           
           return NULL;
        }
        // Prepares the next call to hmap_next().
        if ((hmap_iter->next != NULL)
            && (hmap_iter->next->next != NULL)) {

            hmap_iter->next = hmap_iter->next->next;
            break;
        }
        hmap_iter->curr_array_index++;
        if (hmap_iter->curr_array_index < hmap->size_array) {
            hmap_iter->next = (hmap->nodes[hmap_iter->curr_array_index]);
            if (hmap_iter->next != NULL) {
                break;
            }
        } else {
            hmap_iter->next = NULL;
            break;
        }
    }

    // Return the next value seeked to the position in the previous moment.
    return curr_ret_pair;
}

int32_t hmap_del_iterator(HmapIter * hmap_iter) {
    // Validates the pointer.
    if (hmap_iter == NULL) {
        return RET_ERROR_MINUS_1;
    }
    
    // There is a double pointer indirection in here.
    Hashmap * hmap = *(hmap_iter->hmap);

    size_t const max_size_valid_iterators = sizeof(hmap->valid_iterators) 
                                          / sizeof(hmap->valid_iterators[0]);

    for(size_t i = 0; i < max_size_valid_iterators; i++) {
        if (hmap->valid_iterators[i] == hmap_iter) {
            hmap->valid_iterators[i] = NULL;
        }
    }

    hmap_iter->hmap = NULL;
    hmap_iter->next = NULL;
    free(hmap_iter);
    hmap_iter = NULL;

    return RET_ERROR_SUCCESS;
}

int32_t hmap_invalidate_iterators(Hashmap ** hmap_pp) {
    // Validates the pointer.
    if (hmap_pp == NULL || *hmap_pp == NULL) {
        return RET_ERROR_MINUS_1;
    }
    Hashmap * hmap = *hmap_pp;

    size_t const max_size_valid_iterators = sizeof(hmap->valid_iterators) 
                                          / sizeof(hmap->valid_iterators[0]);

    for(size_t i = 0; i < max_size_valid_iterators; i++) {
        hmap->valid_iterators[i] = NULL;
    }
    return RET_ERROR_SUCCESS;
}

int32_t hmap_apply_func_key_value(Hashmap ** hmap_pp, void (*func_ptr)(Pair * pair_key_value) ) {
    // Validates the pointer.
    if (hmap_pp == NULL || *hmap_pp == NULL) {
        return RET_ERROR_MINUS_1;
    }
    Hashmap * hmap = *hmap_pp;
    if (func_ptr == NULL) {
        hmap->last_error = "Error: hmap_apply_func_key_value() parameter fun_ptr is NULL. ";
        return RET_ERROR_MINUS_2;
    }
    
    // Creating the iterator.
    HmapIter * hmap_iter = hmap_get_iterator(hmap_pp); 
    if (hmap_iter == NULL) {
        hmap->last_error = "Error: hmap_apply_func_key_value() while creating the iterator. ";
        return RET_ERROR_MINUS_3;
    }

    // Iterating the iterator.
    Pair * pair = NULL; 
    int j = 0;
    while( (pair = hmap_next(hmap_iter)) != NULL ) {
        // printf("Hashmap Iterator  pseudo_index: %d  key: %s  value: %d\n", j, pair->key_str, *((int *) pair->value));
        
        // Call the function pointer with the pair[key_str, value].
        (*func_ptr) (pair);
        
        j++;
    }
 
    // Deleting the iterator from memory.
    int32_t res_error = hmap_del_iterator(hmap_iter);
    // printf("Deleting the %d iterator It's NULL: %s \n", (int) i, (res_error == RET_ERROR_MINUS_1)? "NULL" : "NOT_NULL");
    if (res_error != RET_ERROR_SUCCESS) {
        return RET_ERROR_MINUS_4;
    }

    return RET_ERROR_SUCCESS;
}

int32_t hmap_rehash(Hashmap ** hmap_pp) {
    // Validates the pointer.
    if (hmap_pp == NULL || *hmap_pp == NULL) {
        return RET_ERROR_MINUS_1;
    }
    // Hashmap * hmap = *hmap_pp;
    // Creates new 4 x bigger hashmap
    Hashmap * new_hmap = hmap_new_min_size(hmap_get_capacity(hmap_pp) * 4);
    if (new_hmap == NULL) {
        return RET_ERROR_MINUS_2;
    }
    // Deletes the old, smaller hashmap.
    const bool DONT_FREE_THE_OUTER_POINTER = true;
    RetTuple ret_tuple = hmap_delete_optional_ptr(hmap_pp, DONT_FREE_THE_OUTER_POINTER);
    // If there was no error.
    if (*((int32_t *)ret_tuple.data_3) == RET_ERROR_SUCCESS) {
        const bool COPY_KEYS_STR = false;
        // Get the element that where inside and free the memory of the Pair capsule,
        // not the content of the pair.
        size_t num_elemts = *((size_t *)(ret_tuple.data_2));
        if ( num_elemts > 0) {
            for(size_t i = 0; i < num_elemts; i++) {
                Pair * pair_tmp = ((Pair **)(ret_tuple.data_1))[i];
                
                // Insert the key_str and value into the bigger new_hmap, without
                // copying the keys, only the pointer.
                int32_t res = hmap_insert_key_copy_option(&new_hmap, pair_tmp->key_str, pair_tmp->value, COPY_KEYS_STR);
                if (res != RET_ERROR_SUCCESS) {
                    new_hmap->last_error = "Error: inside hmap_rehash() in a insert() while inserting the old content inside the new_hmap .";
                    return RET_ERROR_MINUS_2;
                }
                
                // Free the pair capsule, not it's content.
                free(pair_tmp);
                ((Pair **)ret_tuple.data_1)[i] = NULL;
            }
        } 
    }

    // Free the allocated memory inside the ret:tuple :-)
    if (ret_tuple.data_3 != NULL) { free(ret_tuple.data_3); ret_tuple.data_3 = NULL; }
    if (ret_tuple.data_2 != NULL) { free(ret_tuple.data_2); ret_tuple.data_2 = NULL; }
    if (ret_tuple.data_1 != NULL) { free(ret_tuple.data_1); ret_tuple.data_1 = NULL; }


    // Change the Outer Pointer of the original hmap.
    // free(*hmap_pp); // The free was done inside the delete.
    *hmap_pp = NULL; 
    *hmap_pp = new_hmap;
    
    // hmap_pp = new_hmap;
    
    new_hmap = NULL; 

    return RET_ERROR_SUCCESS;
}

size_t hmap_calc_next_prime(size_t num) {
    bool flag_prime_found = false; 
    size_t n = num;
    if (n % 2 == 0) {
        n++;
    }
    while (true) {
        bool flag_is_prime = true;
        size_t m = n / 2;    
        for(size_t i = 2; i <= m; i+=2) {    
            if(n % i == 0) {
                flag_is_prime = false;
                break;
            }
        }
        flag_prime_found = flag_is_prime;
        if (flag_prime_found)
            return n;
        n += 2;
    }
}

// private
static int32_t hmap_calc_hash_4_string(Hashmap ** hmap_pp, char * key_str) {
    // Validates the pointer.
    if (hmap_pp == NULL || *hmap_pp == NULL) {
        return -1;
    }
    Hashmap * hmap = *hmap_pp;
    if (key_str == NULL) {
        hmap->last_error = "Error in hmap_calc_hash_4_string(), key_str is NULL.";
        return -2;
    }
    size_t acum = 0;
    while(*key_str != '\0') {
        acum += *key_str;
        key_str++;
    }
    return acum;
}


