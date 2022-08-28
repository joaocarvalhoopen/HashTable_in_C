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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hashmap.h"

uint32_t test_00() {
    const char * test_name = "0. Tests the calculation of primes for rehashing.";
    printf("\nStarted test: %s\n", test_name);

    size_t res = hmap_calc_next_prime(1);
    assert( (res == 1) && "Error: hmap_calc_next_prime(1) = 1" );

    res = hmap_calc_next_prime(2);
    assert( (res == 3) && "Error: hmap_calc_next_prime(2) = 3" );

    res = hmap_calc_next_prime(3);
    assert( (res == 3) && "Error: hmap_calc_next_prime(3) = 3" );

    res = hmap_calc_next_prime(4);
    assert( (res == 5) && "Error: hmap_calc_next_prime(4) = 5" );

    res = hmap_calc_next_prime(5);
    assert( (res == 5) && "Error: hmap_calc_next_prime(5) = 5" );

    res = hmap_calc_next_prime(6);
    assert( (res == 7) && "Error: hmap_calc_next_prime(6) = 7" );

    res = hmap_calc_next_prime(7);
    assert( (res == 7) && "Error: hmap_calc_next_prime(7) = 7" );

    printf("\n\tPassed test: %s\n", test_name);  
    return RET_ERROR_SUCCESS;
}

uint32_t test_01() {
    const char * test_name = "1. Create and delete empty Hashmap hmap_1 .";
    printf("\nStarted test: %s\n", test_name);

    // Test small hashmap.    
    Hashmap * hmap_1 = hmap_new();
    assert( hmap_1 != NULL && "Error: hmap_1 in hmap_new()" );
    assert( (hmap_get_capacity(&hmap_1) == 11) && "Error: hmap_1 in hmap_new() hmap_get_capacity() should be 11.");
    
    RetTuple ret_tuple = hmap_delete(&hmap_1);
    assert( *( (int32_t *) ret_tuple.data_3) == 0 && "Error: hmap_1 in hmap_delete() ." );
    if (ret_tuple.data_3 != NULL) { free(ret_tuple.data_3); ret_tuple.data_3 = NULL; }
    if (ret_tuple.data_2 != NULL) { free(ret_tuple.data_2); ret_tuple.data_2 = NULL; }
    if (ret_tuple.data_1 != NULL) { free(ret_tuple.data_1); ret_tuple.data_1 = NULL; }

    // Test big hashmap
    Hashmap * hmap_2 = hmap_new_min_size(1000000);
    assert( hmap_2 != NULL && "Error: hmap_1 in hmap_new()" );
    assert( hmap_get_capacity(&hmap_2) > 1000000 && "Error: hmap_1 in hmap_new_min_size() hmap_get_capacity() should be > 1_000_000.");

    ret_tuple = hmap_delete(&hmap_2);
    assert( *( (int32_t *) ret_tuple.data_3) == 0 && "Error: hmap_1 in hmap_delete() ." );
    if (ret_tuple.data_3 != NULL) { free(ret_tuple.data_3); ret_tuple.data_3 = NULL; }
    if (ret_tuple.data_2 != NULL) { free(ret_tuple.data_2); ret_tuple.data_2 = NULL; }
    if (ret_tuple.data_1 != NULL) { free(ret_tuple.data_1); ret_tuple.data_1 = NULL; }

    printf("\n\tPassed test: %s\n", test_name);  
    return RET_ERROR_SUCCESS;
}


uint32_t test_02() {
    const char * test_name = "2. Create, insert, get_pair and delete Hashmap hmap_2.";
    printf("\nStarted test: %s\n", test_name);

    Hashmap * hmap_2 = hmap_new();
    assert((hmap_2 != NULL) && "Error: hmap_2 in hmap_new() .");

    char * key_str = "bla";
    int * value = malloc(sizeof(int));
    *value =  33;
    int32_t res = hmap_insert( & hmap_2, key_str, (void *) value);
    assert(res == RET_ERROR_SUCCESS && "Error: hmap_2 in hmap_insert() .");

    // SubTest A: get_pair that exists.
    Pair * pair_1 = hmap_get_pair( & hmap_2, key_str);
    assert(pair_1 != NULL && "Error: hmap_2 in hmap_get_pair() [SubTest A] .");
    assert(pair_1 != RET_ELEMENT_NOT_FOUND && "Error: hmap_2 in hmap_get_pair() [SubTest A] The element wasn't found.");
    assert(strcmp(pair_1->key_str, key_str) == 0 && "Error: hmap_2 in hmap_get_pair() [SubTest A] The Pair key isn't the same.");
    assert(pair_1->value == value && "Error: hmap_2 in hmap_get_pair() [SubTest A] The Pair value isn't hasn't the same address.");

    // SubTest B: get_pair that doesn't exist.
    char * key_str_dont_exists = "babalu";
    Pair * pair_2 = hmap_get_pair( & hmap_2, key_str_dont_exists);
    assert(pair_2 != NULL && "Error: hmap_2 in hmap_get_pair() [SubTest B] .");
    assert(pair_2 == RET_ELEMENT_NOT_FOUND && "Error: hmap_2 in hmap_get_pair() [SubTest B] The element that doesn't exit was found.");
    

    // Delete the Hashmap.
    RetTuple ret_tuple = hmap_delete( & hmap_2);
    assert( *( (int32_t *) ret_tuple.data_3) == 0 && "Error: hmap_2 in hmap_delete()" );
    // If there was no error.
    if (*((int32_t *)ret_tuple.data_3) == RET_ERROR_SUCCESS) {
        // If there were elements we have to free there memory.
        size_t num_elemts = *((size_t *)(ret_tuple.data_2));
        if ( num_elemts > 0) {
            for(size_t i = 0; i < num_elemts; i++) {
                Pair * pair_tmp = ((Pair **)(ret_tuple.data_1))[i];
                // Free the key.
                free(pair_tmp->key_str);
                pair_tmp->key_str = NULL;
                free(pair_tmp->value);
                pair_tmp->value = NULL;
                free(pair_tmp);
                ((Pair **)ret_tuple.data_1)[i] = NULL;
            }
        } 
    }
    if (ret_tuple.data_3 != NULL) { free(ret_tuple.data_3); ret_tuple.data_3 = NULL; }
    if (ret_tuple.data_2 != NULL) { free(ret_tuple.data_2); ret_tuple.data_2 = NULL; }
    if (ret_tuple.data_1 != NULL) { free(ret_tuple.data_1); ret_tuple.data_1 = NULL; }


    printf("\n\tPassed test: %s\n", test_name);

    return RET_ERROR_SUCCESS;
}

uint32_t test_03() {
    const char * test_name = "3. Create, insert, get_pair, remove, get_pair and delete Hashmap hmap_3.";
    printf("\nStarted test: %s\n\n", test_name);

    Hashmap * hmap_3 = hmap_new();
    assert((hmap_3 != NULL) && "Error: hmap_3 in hmap_new() .");
    assert((hmap_get_size( & hmap_3) == 0) && "Error: hmap_3 in hmap_new() hmap_get_size() != 0 .");


    // Get_pair that doesn't exist in empty Hashmap.
    char * key_str_do_not_exist = "do_not_exist";
    Pair * pair = hmap_get_pair( & hmap_3, key_str_do_not_exist);
    assert(pair != NULL && "Error: hmap_3 in hmap_get_pair() [SubTest A] .");
    assert(pair == RET_ELEMENT_NOT_FOUND && "Error: hmap_3 in hmap_get_pair() [SubTest A] The element that doesn't exit was found.");

    // Inserts element "bla".
    char * key_str_bla = "bla";
    int * value_bla = malloc(sizeof(int));
    *value_bla =  33;
    int32_t res = hmap_insert( & hmap_3, key_str_bla, (void *) value_bla);
    assert(res == RET_ERROR_SUCCESS && "Error: hmap_3 in hmap_insert() .");
    assert((hmap_get_size( & hmap_3) == 1) && "Error: hmap_3 in hmap_insert() hmap_get_size() != 1 .");


    // Tests if element "bla" exists.
    Pair * pair_1 = hmap_get_pair( & hmap_3, key_str_bla);
    assert(pair_1 != NULL && "Error: hmap_3 in hmap_get_pair() [SubTest B] .");
    assert(pair_1 != RET_ELEMENT_NOT_FOUND && "Error: hmap_3 in hmap_get_pair() [SubTest B] The element wasn't found.");
    assert(strcmp(pair_1->key_str, key_str_bla) == 0 && "Error: hmap_3 in hmap_get_pair() [SubTest B] The Pair key isn't the same.");
    assert(pair_1->value == value_bla && "Error: hmap_3 in hmap_get_pair() [SubTest B] The Pair value isn't hasn't the same address.");

    // Inserts element "tata".
    char * key_str_tata = "tata";
    int * value_tata = malloc(sizeof(int));
    *value_tata =  66;
    int32_t res_2 = hmap_insert( & hmap_3, key_str_tata, (void *) value_tata);
    assert(res_2 == RET_ERROR_SUCCESS && "Error: hmap_3 in hmap_insert() .");
    assert((hmap_get_size( & hmap_3) == 2) && "Error: hmap_3 in hmap_insert() hmap_get_size() != 2 .");


    // Tests if element "tata" exists.
    Pair * pair_2 = hmap_get_pair( & hmap_3, key_str_tata);
    assert(pair_2 != NULL && "Error: hmap_3 in hmap_get_pair() [SubTest C] .");
    assert(pair_2 != RET_ELEMENT_NOT_FOUND && "Error: hmap_3 in hmap_get_pair() [SubTest C] The element wasn't found.");
    assert(strcmp(pair_2->key_str, key_str_tata) == 0 && "Error: hmap_3 in hmap_get_pair() [SubTest C] The Pair key isn't the same.");
    assert(pair_2->value == value_tata && "Error: hmap_3 in hmap_get_pair() [SubTest C] The Pair value isn't hasn't the same address.");

    // Tries to Removes element "do_not_exist".
    Pair * pair_3 = hmap_remove( & hmap_3, key_str_do_not_exist);
    assert(pair_3 != NULL && "Error: hmap_3 in hmap_remove() .");
    assert(pair_3 == RET_ELEMENT_NOT_FOUND && "Error: hmap_3 in hmap_remove() The element *do_not_exist* wasn't found.");
    assert((hmap_get_size( & hmap_3) == 2) && "Error: hmap_3 in hmap_remove() hmap_get_size() != 2 .");


    // Test if element "do_not_exist" doesn't exist.
    Pair * pair_4 = hmap_get_pair( & hmap_3, key_str_do_not_exist);
    assert(pair_4 != NULL && "Error: hmap_3 in hmap_get_pair() [SubTest D] .");
    assert(pair_4 == RET_ELEMENT_NOT_FOUND && "Error: hmap_3 in hmap_get_pair() [SubTest D] The element that doesn't exit was found.");


    // Removes element "bla".
    Pair * pair_5 = hmap_remove( & hmap_3, key_str_bla);
    assert(pair_5 != NULL && "Error: hmap_3 in hmap_remove() .");
    assert(pair_5 != RET_ELEMENT_NOT_FOUND && "Error: hmap_3 in hmap_remove() The element *bla* was found.");
    assert(pair_5->key_str != NULL && "Error: hmap_3 in hmap_remove() The element *bla* the key is not NULL .");
    assert(pair_5->value != NULL && "Error: hmap_3 in hmap_remove() The element *bla* the value is not NULL .");
    assert((hmap_get_size( & hmap_3) == 1) && "Error: hmap_3 in hmap_remove() hmap_get_size() != 1 .");

    free(pair_5->key_str);
    pair_5->key_str = NULL;
    free(pair_5->value);
    pair_5->value = NULL;
    free(pair_5);
    pair_5 = NULL;

    // Tests if element "bla" exists after remove.
    Pair * pair_6 = hmap_get_pair( & hmap_3, key_str_bla);
    assert(pair_6 != NULL && "Error: hmap_3 in hmap_get_pair() [SubTest B] .");
    assert(pair_6 == RET_ELEMENT_NOT_FOUND && "Error: hmap_3 in hmap_get_pair() [SubTest B] The element *bla* was found.");


    // Removes element "tata".
    Pair * pair_7 = hmap_remove( & hmap_3, key_str_tata);
    assert(pair_7 != NULL && "Error: hmap_3 in hmap_remove() .");
    assert(pair_7 != RET_ELEMENT_NOT_FOUND && "Error: hmap_3 in hmap_remove() The element *bla* was found.");
    assert(pair_7->key_str != NULL && "Error: hmap_3 in hmap_remove() The element *bla* the key is not NULL .");
    assert(pair_7->value != NULL && "Error: hmap_3 in hmap_remove() The element *bla* the value is not NULL .");
    assert((hmap_get_size( & hmap_3) == 0) && "Error: hmap_3 in hmap_remove() hmap_get_size() != 0 .");

    free(pair_7->key_str);
    pair_7->key_str = NULL;
    free(pair_7->value);
    pair_7->value = NULL;
    free(pair_7);
    pair_7 = NULL;

    // Tests if element "tata" exists after remove.
    Pair * pair_8 = hmap_get_pair( & hmap_3, key_str_tata);
    assert(pair_8 != NULL && "Error: hmap_3 in hmap_get_pair() [SubTest ] .");
    assert(pair_8 == RET_ELEMENT_NOT_FOUND && "Error: hmap_3 in hmap_get_pair() [SubTest ] The element *tata* was found.");


    // Test get_pair that doesn't exist.
    Pair * pair_9 = hmap_get_pair( & hmap_3, key_str_do_not_exist);
    assert(pair_9 != NULL && "Error: hmap_3 in hmap_get_pair()  .");
    assert(pair_9 == RET_ELEMENT_NOT_FOUND && "Error: hmap_3 in hmap_get_pair() The element that doesn't exit was found.");


    // Generate 100 elements for the hashmap, so that it will have to grow automatically.
    char key_str_bla_xx[40];
    for(size_t i = 0; i < 20; i++) {
        // Inserts element "bla_XX".
        sprintf(key_str_bla_xx, "bla_%d", (int) i);   // 10 means decimal
        int * value_bla_xx = malloc(sizeof(int));
        assert(value_bla_xx != NULL && "Error in malloc() .");
        *value_bla_xx =  i+1;
        int32_t res = hmap_insert( & hmap_3, key_str_bla_xx, (void *) value_bla_xx);
        assert(res == RET_ERROR_SUCCESS && "Error: hmap_3 in hmap_insert() .");
        int num_elements = hmap_get_size( & hmap_3);
        printf("hmap_get_size( & hmap_3) == i, size_elements: %d,  i+1: %d\n", num_elements, (int) i+1);
        assert((hmap_get_size( & hmap_3) == i+1) && "Error: hmap_3 in hmap_insert() hmap_get_size() != 1 .");
    }

    // Test bla_18
    char * key_str_bla_18 = "bla_18";
    int value_18 = 19; 
    Pair * pair_10 = hmap_get_pair( & hmap_3, key_str_bla_18);
    assert(pair_10 != NULL && "Error: hmap_3 in hmap_get_pair() [SubTest ] .");
    assert(pair_10 != RET_ELEMENT_NOT_FOUND && "Error: hmap_3 in hmap_get_pair() [SubTest ] The element *bla_55* wasn't found.");
    assert(strcmp(pair_10->key_str, key_str_bla_18) == 0 && "Error: hmap_3 in hmap_get_pair() [SubTest C] The Pair key isn't the same.");
    assert(*((int * ) (pair_10->value)) == value_18 && "Error: hmap_3 in hmap_get_pair() [SubTest C] The Pair value doesn't have the same address.");


    // Delete the Hashmap.
    RetTuple ret_tuple = hmap_delete( & hmap_3);
    assert( *( (int32_t *) ret_tuple.data_3) == 0 && "Error: hmap_3 in hmap_delete()" );
    // If there was no error.
    if (*((int32_t *)ret_tuple.data_3) == RET_ERROR_SUCCESS) {
        // If there were elements we have to free there memory.
        size_t num_elemts = *((int32_t *)ret_tuple.data_2);
        if ( num_elemts > 0) {
            for(size_t i = 0; i < num_elemts; i++) {
                Pair * pair = ((Pair **)ret_tuple.data_1)[i];
                // Free the key.
                free(pair->key_str);
                pair->key_str = NULL;
                free(pair->value);
                pair->value = NULL;
                free(pair);
                ((Pair **)ret_tuple.data_1)[i] = NULL;
            }
        } 
    }

    if (ret_tuple.data_3 != NULL) { free(ret_tuple.data_3); ret_tuple.data_3 = NULL; }
    if (ret_tuple.data_2 != NULL) { free(ret_tuple.data_2); ret_tuple.data_2 = NULL; }
    if (ret_tuple.data_1 != NULL) { free(ret_tuple.data_1); ret_tuple.data_1 = NULL; }

    printf("\n\tPassed test: %s\n", test_name);

    return RET_ERROR_SUCCESS;
}

uint32_t test_04() {
    const char * test_name = "4. Create, insert, get_pair, set_value, and delete Hashmap hmap_4.";
    printf("\nStarted test: %s\n\n", test_name);

    Hashmap * hmap_4 = hmap_new();
    assert((hmap_4 != NULL) && "Error: hmap_4 in hmap_new() .");
    assert((hmap_get_size( & hmap_4) == 0) && "Error: hmap_4 in hmap_new() hmap_get_size() != 0 .");

    // set_value for a key_str that doesn't exist in empty Hashmap.
    char * key_str_do_not_exist = "do_not_exist";
    int * value_dummy = malloc(sizeof(int));
    assert(value_dummy != NULL && "Error: hmap_4 in hmap_new(), error while allocating value_dummy."); 
    *value_dummy = 22;
    int * ret_value = (int *) hmap_set_value( & hmap_4, key_str_do_not_exist, value_dummy );
    assert(ret_value != NULL && "Error: hmap_4 in hmap_get_pair() [SubTest A] .");
    assert(ret_value == RET_ELEMENT_NOT_FOUND && "Error: hmap_4 in hmap_get_pair() [SubTest A] The element that doesn't exit was found.");
    free(value_dummy);
    value_dummy = NULL;

    // Inserts element "cemelhas".
    char * key_str_cemelhas = "cemelhas";
    int * value_cemelhas = malloc(sizeof(int));
    assert(value_cemelhas != NULL && "Error: hmap_4 in hmap_new(), error while allocating value_cemelhas."); 
    *value_cemelhas =  7;
    int32_t res = hmap_insert( & hmap_4, key_str_cemelhas, (void *) value_cemelhas);
    assert(res == RET_ERROR_SUCCESS && "Error: hmap_4 in hmap_insert() .");
    assert((hmap_get_size( & hmap_4) == 1) && "Error: hmap_4 in hmap_insert() hmap_get_size() != 1 .");

    // set_value for a key_str that do exist in Hashmap.
    char * key_str_cemelhas_new = "cemelhas";
    int * value_cemelhas_new = malloc(sizeof(int));
    assert(value_cemelhas_new != NULL && "Error: hmap_4 in hmap_new(), error while allocating value_cemelhas."); 
    *value_cemelhas_new = 11;
    int * ret_value_cemelhas_old = (int *) hmap_set_value( & hmap_4, key_str_cemelhas_new, value_cemelhas_new);
    assert(ret_value_cemelhas_old != NULL && "Error: hmap_4 in hmap_get_pair() [SubTest A] .");
    assert(ret_value_cemelhas_old != RET_ELEMENT_NOT_FOUND && "Error: hmap_4 in hmap_get_pair() [SubTest A] The element that doesn't exit was found.");
    assert(*ret_value_cemelhas_old == 7 && "Error: hmap_4 in hmap_get_pair() [SubTest A] The element that doesn't exit was found.");
    free(ret_value_cemelhas_old);
    ret_value_cemelhas_old = NULL;

    // Generate 100 set_values for the same element in the hashmap, this mode needs the malloc and free, the next doesn't.   
    char * key_str_bbb = "cemelhas"; 
    for(size_t i = 0; i < 10; i++) {
        // Set element "cemelhas".
        int * value_bbb_xx = malloc(sizeof(int));
        assert(value_bbb_xx != NULL && "Error in malloc() .");
        *value_bbb_xx = i;
        int * ret_value_bbb_old = (int *) hmap_set_value( & hmap_4, key_str_bbb, value_bbb_xx);
        assert(ret_value_bbb_old != NULL && "Error: hmap_4 in hmap_get_pair() [SubTest A] .");
        assert(ret_value_bbb_old != RET_ELEMENT_NOT_FOUND && "Error: hmap_4 in hmap_get_pair() [SubTest A] The element that do exit was found.");
        assert( ((*ret_value_bbb_old == 11) || (*ret_value_bbb_old == (int)i - 1)) && "Error: hmap_4 in hmap_get_pair() [SubTest A] The element that doesn't exit was found.");
        free(ret_value_bbb_old);
        ret_value_bbb_old = NULL;
        // The size_elements is unchanged.
        assert((hmap_get_size( & hmap_4) == 1) && "Error: hmap_4 in hmap_insert() hmap_get_size() .");
        printf("hmap_set_value( & hmap_4) == i, size_elements: %d,  i+1: %d\n", (int) hmap_get_size( & hmap_4), (int) i);
    }

    //  **A FASTER WAY of doing the same thing** Generate 100 set_values but with out the setting and memory management,
    //  just changing the object inside for the same element in the hashmap, this mode doesn't need malloc's and free's.   
    char * key_str_fast = "cemelhas"; 
    // Get's the pointer to the internal object (void *) casted to a (int *), the element exists.
    Pair * pair_1 = hmap_get_pair( & hmap_4, key_str_fast);
    assert(pair_1 != NULL && "Error: hmap_4 in hmap_get_pair() [SubTest B] .");
    assert(pair_1 != RET_ELEMENT_NOT_FOUND && "Error: hmap_4 in hmap_get_pair() [SubTest B] The element wasn't found.");
    assert(strcmp(pair_1->key_str, key_str_fast) == 0 && "Error: hmap_4 in hmap_get_pair() [SubTest B] The Pair key isn't the same.");
    assert(*((int *) pair_1->value) == 9 && "Error: hmap_4 in hmap_get_pair() [SubTest B] The Pair value isn't 99.");

    for(size_t i = 0; i < 10; i++) {
        assert( ((*((int *) pair_1->value) == 9) || (*((int *) pair_1->value) == (int)i-1) )
                && "Error: hmap_4 in hmap_get_pair() [SubTest B] The Pair value isn't 99, or the value imediatly before.");
        // Updates the value inside the hashtable.
        *((int *) pair_1->value) = i;

        // Just a simple check.        
        assert((hmap_get_size( & hmap_4) == 1) && "Error: hmap_4 in hmap_insert() hmap_get_size() .");
        printf("hmap_fast_set_value ptr    *((int *) pair_1->value) = i;  , internal value: %d,  loop i: %d\n",
               *((int *) pair_1->value), (int) i);
    }

    // Delete the Hashmap.
    RetTuple ret_tuple = hmap_delete( & hmap_4);
    assert( *( (int32_t *) ret_tuple.data_3) == 0 && "Error: hmap_4 in hmap_delete()" );
    // If there was no error.
    if (*((int32_t *)ret_tuple.data_3) == RET_ERROR_SUCCESS) {
        // If there were elements we have to free there memory.
        size_t num_elemts = *((int32_t *)ret_tuple.data_2);
        if ( num_elemts > 0) {
            for(size_t i = 0; i < num_elemts; i++) {
                Pair * pair = ((Pair **)ret_tuple.data_1)[i];
                // Free the key.
                free(pair->key_str);
                pair->key_str = NULL;
                free(pair->value);
                pair->value = NULL;
                free(pair);
                ((Pair **)ret_tuple.data_1)[i] = NULL;
            }
        } 
    }

    if (ret_tuple.data_3 != NULL) { free(ret_tuple.data_3); ret_tuple.data_3 = NULL; }
    if (ret_tuple.data_2 != NULL) { free(ret_tuple.data_2); ret_tuple.data_2 = NULL; }
    if (ret_tuple.data_1 != NULL) { free(ret_tuple.data_1); ret_tuple.data_1 = NULL; }

    printf("\n\tPassed test: %s\n", test_name);

    return RET_ERROR_SUCCESS;
}


// ############################################
// #   BEGIN     ##############################
// ############################################


uint32_t test_05() {
    const char * test_name = "5. Create, insert, test iterators :-), invalidate iterators with Insert and delete Hashmap hmap_5.";
    printf("\nStarted test: %s\n\n", test_name);

    Hashmap * hmap_5 = hmap_new();
    assert((hmap_5 != NULL) && "Error: hmap_5 in hmap_new() .");
    assert((hmap_get_size( & hmap_5) == 0) && "Error: hmap_5 in hmap_new() hmap_get_size() != 0 .");



    // Generate 100 elements for the hashmap, so that it will have to grow automatically.
    char key_str_bla_xx[40];
    // for(size_t i = 0; i < 20; i++) {
    for(size_t i = 0; i < 15; i++) {
        // Inserts element "bla_XX".
        sprintf(key_str_bla_xx, "bla_%d", (int) i);   // 10 means decimal
        int * value_bla_xx = malloc(sizeof(int));
        assert(value_bla_xx != NULL && "Error in malloc() .");
        *value_bla_xx =  i;
        int32_t res = hmap_insert( & hmap_5, key_str_bla_xx, (void *) value_bla_xx);
        assert(res == RET_ERROR_SUCCESS && "Error: hmap_3 in hmap_insert() .");
        int num_elements = hmap_get_size( & hmap_5);
        printf("Insert hmap_get_size( & hmap_5) == i, size_elements: %d,  i: %d\n", num_elements, (int) i+1);
        assert((hmap_get_size( & hmap_5) == i+1) && "Error: hmap_5 in hmap_insert() hmap_get_size() != 1 .");
    }



    // Creating the iterator.
    printf("\n\n *****  Creating the iterator. *****\n\n");
    HmapIter * hmap_iter = hmap_get_iterator(& hmap_5); 
    assert((hmap_5 != NULL) && "Error: hmap_5 in hmap_get_iterator() .");
    
    // Iterating the iterator.
    Pair * pair = NULL; 
    int i = 0;
    while( (pair = hmap_next(hmap_iter)) != NULL ) {
        printf("Hashmap Iterator  pseudo_index: %d  key: %s  value: %d\n", i, pair->key_str, *((int *) pair->value));
        // Order is not preserved in a hashmap :-)
        //assert(*value == i && "Error: Hmap_5 in test iterator, in values while iterating the iterator. ");
        i++;
    }

    int32_t res_error = hmap_del_iterator(hmap_iter);
    assert(res_error != RET_ERROR_MINUS_1 && "Error: Hmap_5 error while deleting the iterator, RET_ERROR_MINUS_1");
    assert(res_error == RET_ERROR_SUCCESS && "Error: Hmap_5 error while deleting the iterator, not RET_ERROR_SUCCESS");



    // Creating 11 iterators at the same time.
    printf("\n\n *****  Creating 11 iterators at the same time. *****\n\n");
    const size_t NUM_ITERATORS = 12;
    HmapIter * hmap_iter_arr[NUM_ITERATORS];
    for(size_t i = 0; i < NUM_ITERATORS; i++) {
        printf("Creating the %d iterator \n", (int) i);

        // Creating the iterator.
        HmapIter * hmap_iter = hmap_get_iterator(& hmap_5); 
        assert((hmap_5 != NULL) && "Error: hmap_5 in hmap_get_iterator() .");
        
        // Save the iterators so that they will exist all at the same time, the
        // hmap_del_iterator() will be done a later point, so they exist all in
        // memory at the same time, to test the max limit of 10 slots for iterators.
        hmap_iter_arr[i] = hmap_iter;

        // Iterating the iterator.
        Pair * pair = NULL; 
        int j = 0;
        while( (pair = hmap_next(hmap_iter)) != NULL ) {
            printf("Hashmap Iterator  pseudo_index: %d  key: %s  value: %d\n", j, pair->key_str, *((int *) pair->value));
            
            // Order is not preserved in a hashmap :-)
            // assert(*value == i && "Error: Hmap_5 in test iterator, in values while iterating the iterator. ");
            j++;
        }
    }

    // Deleting the iterators from memory.
    for(size_t i = 0; i < NUM_ITERATORS; i++) {
    
        HmapIter * hmap_iter = hmap_iter_arr[i];

        int32_t res_error = hmap_del_iterator(hmap_iter);
        printf("Deleting the %d iterator It's NULL: %s \n", (int) i, (res_error == RET_ERROR_MINUS_1)? "NULL" : "NOT_NULL");
        // assert(res_error != RET_ERROR_MINUS_1 && "Error: Hmap_5 error while deleting the iterator, RET_ERROR_MINUS_1");
        // assert(res_error == RET_ERROR_SUCCESS && "Error: Hmap_5 error while deleting the iterator, not RET_ERROR_SUCCESS");    
    }




    // Test the invalidation of the iterators when we make a insertion or removal
    // on the 5 created iterator, we are creating 11 iterators at the same time.
    printf("\n\n *****  // Test the invalidation of the iterators when we make a insertion *****\n\n");
    const size_t NUM_ITERATORS_2 = 12;
    HmapIter * hmap_iter_arr_2[NUM_ITERATORS_2];
    for(size_t i = 0; i < NUM_ITERATORS_2; i++) {
        printf("Creating the %d iterator \n", (int) i);

        // Creating the iterator.
        HmapIter * hmap_iter = hmap_get_iterator(& hmap_5); 
        assert((hmap_5 != NULL) && "Error: hmap_5 in hmap_get_iterator() .");
        
        // Save the iterators so that they will exist all at the same time, the
        // hmap_del_iterator() will be done a later point, so they exist all in
        // memory at the same time, to test the max limit of 10 slots for iterators.
        hmap_iter_arr_2[i] = hmap_iter;

        if (i == 5) {
            size_t num_elemnts_point_A = hmap_get_size( & hmap_5);
            // Inserts element "PIZZA".
            char * key_str_pizza = "PIZZA";
            int * value_pizza = malloc(sizeof(int));
            assert(value_pizza != NULL && "Error: hmap_5 in hmap_new(), error while allocating value_pizza."); 
            *value_pizza = 155;
            int32_t res = hmap_insert( & hmap_5, key_str_pizza, (void *) value_pizza);
            size_t num_elemnts_point_B = hmap_get_size( & hmap_5);
            assert(res == RET_ERROR_SUCCESS 
                   && "Error: hmap_5 in hmap_insert() that has the objective of invalidating the 6 iterators that are already created.");
            assert((num_elemnts_point_A + 1 == num_elemnts_point_B) && "Error: hmap_5 in hmap_insert() hmap_get_size() != 1 .");
        }

        // Iterating the iterator.
        Pair * pair = NULL; 
        int j = 0;

        if (i == 5) {
            pair = hmap_next(hmap_iter);
            if (pair == NULL) {
                printf("\n##########  Invalid iterator!  ##########\n\n");
            }
        } else {
            while( (pair = hmap_next(hmap_iter)) != NULL ) {        
                printf("Hmap_next() %d iterator It's NULL: %s \n", (int) i, (pair == NULL)? "NULL" : "NOT_NULL");
    
                // printf("Hashmap Iterator  pseudo_index: %d  value: %d\n", i, *value);                
                j++;
            }
        }
    }


    // Deleting the iterators from memory.
    for(size_t i = 0; i < NUM_ITERATORS_2; i++) {
    
        HmapIter * hmap_iter = hmap_iter_arr_2[i];

        int32_t res_error = hmap_del_iterator(hmap_iter);
        printf("Deleting the %d iterator It's NULL: %s \n", (int) i, (res_error == RET_ERROR_MINUS_1)? "NULL" : "NOT_NULL");
        // assert(res_error != RET_ERROR_MINUS_1 && "Error: Hmap_5 error while deleting the iterator, RET_ERROR_MINUS_1");
        // assert(res_error == RET_ERROR_SUCCESS && "Error: Hmap_5 error while deleting the iterator, not RET_ERROR_SUCCESS");    
    }



    // Test the Apply function &func_example to Hashmap to sum 100 to each value of the hashmap.
    int32_t res_error_1 = hmap_apply_func_key_value(& hmap_5, & func_example);
    assert(res_error_1 == RET_ERROR_SUCCESS 
           && "Error: hmap_apply_func_key_value() Hmap_5, error while applying the function &func_example to the hashmap. ");

    // With an iterator, test if the all the value are greater then 100.

    // Creating the iterator.
    HmapIter * hmap_iter_2 = hmap_get_iterator(& hmap_5); 
    assert(hmap_iter_2 != NULL);
    
    // Iterating the iterator.
    Pair * pair_2 = NULL; 
    int j = 0;
    while( (pair_2 = hmap_next(hmap_iter_2)) != NULL ) {
        printf("Hashmap Iterator  pseudo_index: %d  key: %s  value: %d\n", j, pair_2->key_str, *((int *) pair_2->value));
        
        // Test if they are larger than 100.
        assert( *((int*)(pair_2->value)) >= 100 );

        j++;
    }
 
    // Deleting the iterator from memory.
    int32_t res_error_2 = hmap_del_iterator(hmap_iter_2);
    // printf("Deleting the %d iterator It's NULL: %s \n", (int) i, (res_error == RET_ERROR_MINUS_1)? "NULL" : "NOT_NULL");
    assert(res_error_2 == RET_ERROR_SUCCESS);
    

    // Delete the Hashmap.
    RetTuple ret_tuple = hmap_delete( & hmap_5);
    assert( *( (int32_t *) ret_tuple.data_3) == 0 && "Error: hmap_3 in hmap_delete()" );
    // If there was no error.
    if (*((int32_t *)ret_tuple.data_3) == RET_ERROR_SUCCESS) {
        // If there were elements we have to free there memory.
        size_t num_elemts = *((int32_t *)ret_tuple.data_2);
        if ( num_elemts > 0) {
            for(size_t i = 0; i < num_elemts; i++) {
                Pair * pair = ((Pair **)ret_tuple.data_1)[i];
                // Free the key.
                free(pair->key_str);
                pair->key_str = NULL;
                free(pair->value);
                pair->value = NULL;
                free(pair);
                ((Pair **)ret_tuple.data_1)[i] = NULL;
            }
        } 
    }

    if (ret_tuple.data_3 != NULL) { free(ret_tuple.data_3); ret_tuple.data_3 = NULL; }
    if (ret_tuple.data_2 != NULL) { free(ret_tuple.data_2); ret_tuple.data_2 = NULL; }
    if (ret_tuple.data_1 != NULL) { free(ret_tuple.data_1); ret_tuple.data_1 = NULL; }

    printf("\n\tPassed test: %s\n", test_name);

    return RET_ERROR_SUCCESS;
}


// ############################################
// #   END       ##############################
// ############################################



int main() {
    printf("...Hashmap test battery begin...\n\n");

    test_00(); // "0. Tests the calculation of primes for rehashing."

    test_01(); // "1. Create and delete empty Hashmap hmap_1 ."  
    
    test_02(); // "2. Create, insert, get_pair and delete Hashmap hmap_2."
    
    test_03(); // "3. Create, insert, get_pair, remove, get_pair and delete Hashmap hmap_3."
    
    test_04(); // "4. Create, insert, get_pair, set_value, and delete Hashmap hmap_4."
    
    test_05(); // "5. Create, insert, test iterators :-), invalidate iterators with Insert and delete Hashmap hmap_5."

    printf("\n... hashmap test end\n");
}