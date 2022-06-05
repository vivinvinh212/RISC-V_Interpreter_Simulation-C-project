#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#include "hashtable.h"

struct hashtable {
    // TODO: define hashtable struct to use linkedlist as buckets
	struct linkedlist** buckets;
	int number_buc;
	int number_maps;
};

/**
 * Hash function to hash a key into the range [0, max_range)
 */
static int hash(int key, int max_range) {
    // TODO: feel free to write your own hash function (NOT REQUIRED)
    key = (key > 0) ? key : -key;
    return key % max_range;
}

hashtable_t *ht_init(int num_buckets) {
    // TODO: create a new hashtable
    
    hashtable_t *hash_tab = (hashtable_t *)malloc(sizeof(hashtable_t));
    hash_tab->buckets = malloc(num_buckets*sizeof(linkedlist_t *));
    for (int i = 0; i < num_buckets; i++) {
	hash_tab->buckets[i] = ll_init();
    }
    hash_tab->number_buc = num_buckets;
    hash_tab->number_maps = 0;
    return hash_tab;
}

void ht_add(hashtable_t *table, int key, int value) {
    // TODO: create a new mapping from key -> value.
    // If the key already exists, replace the value.
    
    int bucket_num = hash(key, table->number_buc);
    int original_size = ll_size(table->buckets[bucket_num]);
    ll_add(table->buckets[bucket_num], key, value);
    int new_size = ll_size(table->buckets[bucket_num]);
    if (original_size < new_size) {
	table->number_maps++;
    }
}

int ht_get(hashtable_t *table, int key) {
    // TODO: retrieve the value mapped to the given key.
    // If it does not exist, return 0
    
    int bucket_num = hash(key, table->number_buc);
    return ll_get(table->buckets[bucket_num], key);
}

int ht_size(hashtable_t *table) {
    // TODO: return the number of mappings in this hashtable
    
    return table->number_maps;
}
