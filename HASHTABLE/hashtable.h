#ifndef HASHTABLE_H
#define HASHTABLE_H


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -----/ DEFINE ZONE /-----

typedef char * K;
typedef char * V;

typedef struct {
	K     key;
	V     val;
 	bool  is_del;
} backet;

typedef struct {
	backet *data;
	size_t  capacity;
	size_t  size;
	size_t  del_size;
} hash_table;


// -----/ DECLARATION /-----

hash_table *ht_create(size_t capacity);
hash_table *ht_destroy(hash_table *ht);
size_t ht_find_ind(backet *data, size_t capacity, K key);
int ht_insert(hash_table *ht, K key, V val);
int ht_erase(hash_table *ht, K key);
V   ht_get(hash_table *ht, K key);
size_t ht_size(hash_table *ht);
void ht_print(hash_table *ht);


#endif /* HASHTABLE_H */