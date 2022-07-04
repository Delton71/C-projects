#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -----/ DEFINE ZONE /-----

const long double    UP_CAP_COEFF = 0.8;
const long double  DOWN_CAP_COEFF = 0.4;
const int             HASH1_COEFF = 7;		// only prime number
const int            RESIZE_COEFF = 2;		// only 2^n

static size_t CAPACITY = 64;			// used in ht_create function
static size_t LIMIT_DOWN_SIZE = 16;		// used in ht_erase  function

typedef char * K;
typedef char * V;

typedef struct {
	K     key;
	V     val;
 	bool  is_del;			// used to identify deleted items
} backet;

typedef struct {
	backet *data;
	size_t  capacity;
	size_t  size;			// count of undeleted elements
	size_t  del_size;		// count of deleted elements
} hash_table;


// -----/ DECLARATION /-----

static size_t hash1(K key, size_t capacity);
static size_t hash2(K key);
static int ht_rehash_insert(backet *data, size_t capacity, K key, V val);
static int rehash(hash_table *ht, size_t new_capacity);

hash_table *ht_create(size_t capacity);
hash_table *ht_destroy(hash_table *ht);
size_t ht_find_ind(backet *data, size_t capacity, K key, bool is_erased);
int ht_insert(hash_table *ht, K key, V val);
int ht_erase(hash_table *ht, K key);
V   ht_get(hash_table *ht, K key);
size_t ht_size(hash_table *ht);
void ht_print(hash_table *ht);


// M-HASHTABLE functions
void ht_operations();
size_t identify_command(char *command);
size_t find_ind(int arr[], int elem, size_t N);


// -----/ M-HASHTABLE /-----

int main() {
    ht_operations();
    return 0;
}

void ht_operations() {
    size_t N = 10;
    scanf("%zd", &N);
    int num_of_ht[10];
    hash_table *ht_arr[10];


    for (size_t i = 0; i < N; ++i) {
        ht_arr[i] = NULL;
        num_of_ht[i] = 2147483647;
    }

    char command[21] = {'\0'};
    size_t c = 0;
    int ind = 0;
    size_t ht_num = 0;
    char key[21];
    char *val = NULL;
    char *get_val = NULL;
    while ((scanf("%20s", command)) == 1) {
        c = identify_command(command);
        switch (c) {
            case 0:     // create
                for (size_t i = 0; i < N; ++i) {
                    if (!ht_arr[i]) {
                        scanf("%d", &num_of_ht[i]);
                        ht_arr[i] = ht_create(CAPACITY);
                        break;
                    }
                }
                break;
            case 1:     // insert
                val = (char *) calloc(21, sizeof(char));
                scanf("%d %20s %20s", &ind, key, val);
                ht_num = find_ind(num_of_ht, ind, N);
                if (ht_insert(ht_arr[ht_num], key, val)) {
                    free(val);
                }
                break;
            case 2:     // erase
                scanf("%d %20s", &ind, key);
                ht_num = find_ind(num_of_ht, ind, N);
                ht_erase(ht_arr[ht_num], key);
                break;
            case 3:     // find
                scanf("%d %20s", &ind, key);
                ht_num = find_ind(num_of_ht, ind, N);
                get_val = ht_get(ht_arr[ht_num], key);
                if (get_val) {
                    printf("%s\n", get_val);
                } else {
                    puts("MISSING");
                }
                break;
            case 4:     // destroy
                scanf("%d", &ind);
                ht_num = find_ind(num_of_ht, ind, N);
                ht_arr[ht_num] = ht_destroy(ht_arr[ht_num]);
                break;
            default:
                puts("No command(((");
                break;
        }
    }

    for (size_t i = 0; i < N; ++i) {
        ht_arr[i] = ht_destroy(ht_arr[i]);
    }
}

// Used to identify the given command
size_t identify_command(char *command) {
    if (!strcmp(command, "create")) {
        return 0;
    }
    if (!strcmp(command, "insert")) {
        return 1;
    }
    if (!strcmp(command, "erase")) {
        return 2;
    }
    if (!strcmp(command, "find")) {
        return 3;
    }
    if (!strcmp(command, "destroy")) {
        return 4;
    }
    return 100;		// not a command
}

// Used to find index in hash_table's array
size_t find_ind(int arr[], int elem, size_t N) {
    size_t reserve = 0;
    for (size_t i = 0; i < N; ++i) {
        if (arr[i] == elem) {
            return i;
        }
        if (arr[i] == 2147483647) {
            reserve = i;
        }
    }
    return reserve;
}




// -----/ HASH FUNCTIONS /-----

static size_t hash1(K key, size_t capacity) {
	size_t len = strlen(key);
	size_t res = 0;
	for (int i = 0; i < len; ++i) {
		res = ((res + key[i]) * HASH1_COEFF) % capacity;
	}
	return res;
}

// Function which return only odd value
static size_t hash2(K key) {
	return strlen(key) | 1;  		// odd value
}

// Insert elements (with is_del == false) in new_data from rehash function
static int ht_rehash_insert(backet *data, size_t capacity, K key, V val) {
	if (!data || !capacity) {
		return 1;
	}

	size_t h1 = hash1(key, capacity);

	size_t h2 = 0;
	if (data[h1].key) {
		h2 = hash2(key);
	}

	size_t ind = h1;
	while (data[ind].key && strcmp(key, data[ind].key)) {
		ind = (ind + h2) % capacity;
	}

	data[ind].key = key;
	data[ind].val = val;
	data[ind].is_del = false;
	return 0;
}

// Change capacity and rebuild ht->data
// Rehash all not erased elements (is_del == false), free all erased elements; return new_data
static int rehash(hash_table *ht, size_t new_capacity) {
	backet *new_data = (backet *) calloc(new_capacity, sizeof(backet));
	if (!new_data) {
		return 1;
	}

	for (size_t i = 0; i < ht->capacity; ++i) {
		if (ht->data[i].key != NULL) {
			if (ht->data[i].is_del) {
				if (ht->data[i].val) {
					free(ht->data[i].val);
				}
				if (ht->data[i].key) {
					free(ht->data[i].key);
				}
			} else if (ht_rehash_insert(new_data, new_capacity, ht->data[i].key, ht->data[i].val)) {
				return 1;			// fail in ht_rehash_insert
			}
		}
	}

	free(ht->data);
	ht->data = new_data;
	ht->capacity = new_capacity;
	ht->del_size = 0;
	return 0;
}

// Create new hash_table *ht with given capacity
hash_table *ht_create(size_t capacity) {
	hash_table *ht = (hash_table *) malloc(sizeof(hash_table));
	if (!ht) {
		return NULL;
	}
	ht->data = (backet *) calloc(capacity, sizeof(backet));
	if (!ht->data) {
		free(ht);
		return NULL;
	}

	ht->capacity = capacity;
	ht->size = 0;
	ht->del_size = 0;

	return ht;
}

// Destroy hash_table ht; return NULL
hash_table *ht_destroy(hash_table *ht) {
    if (!ht) {
		return NULL;
	}
	
	for (size_t i = 0; i < ht->capacity; ++i) {
		if (ht->data[i].key) {
			free(ht->data[i].key);
		}
		if (ht->data[i].val) {
			free(ht->data[i].val);
		}
	}
	free(ht->data);
	free(ht);
	return NULL;
}

// Find index from key with using h1 && h2 functions
// Flag is_erased used because of erase_function that requires an additional ... || data[ind].is_del
size_t ht_find_ind(backet *data, size_t capacity, K key, bool is_erased) {
	size_t h1 = hash1(key, capacity);

	size_t h2 = hash2(key);

	size_t ind = h1;
	while (data[ind].key && (strcmp(key, data[ind].key) || (is_erased && data[ind].is_del))) {
		ind = (ind + h2) % capacity;
	}
	return ind;
}

// Insert (key, val) in hash_table
int ht_insert(hash_table *ht, K key, V val) {
	if (!ht) {
		return 1;
	}

	if (ht->size + ht->del_size >= ht->capacity * UP_CAP_COEFF) {
		if (rehash(ht, (size_t) ht->capacity * RESIZE_COEFF)) {
			return 1;			// fail in rehash
		}
	}

	size_t ind = ht_find_ind(ht->data, ht->capacity, key, false);

	if (!ht->data[ind].key) {
		ht->data[ind].key = (char *) calloc((strlen(key) + 1), sizeof(char));
		if (!ht->data[ind].key) {
			return 1;			// fail in calloc key
		}
		
		memcpy(ht->data[ind].key, key, strlen(key));
	}

	if (ht->data[ind].is_del) {
		--ht->del_size;
		++ht->size;
	}
	if (ht->data[ind].val) {
		free(ht->data[ind].val);
		--ht->size;
	}

	ht->data[ind].val = val;
	ht->data[ind].is_del = false;
	++ht->size;
	return 0;
}

// Erase (key, val) from hash_table using the key
int ht_erase(hash_table *ht, K key) {
	if(!ht) {
		return 1;
	}

	size_t ind = ht_find_ind(ht->data, ht->capacity, key, true);

	if(!ht->data[ind].key) {
		return 1;
	}
	ht->data[ind].is_del = true;
	--ht->size;
	++ht->del_size;
	
	if (ht->size > LIMIT_DOWN_SIZE && ht->size <= ht->capacity * DOWN_CAP_COEFF) {
		if (rehash(ht, ht->capacity / RESIZE_COEFF)) {
			return 1;			// fail in rehash
		}
	}
	
	return 0;
}

// Return a pointer to a value by the key
V ht_get(hash_table *ht, K key) {
	if(!ht) {
		return NULL;
	}
	
	size_t ind = ht_find_ind(ht->data, ht->capacity, key, false);

	if(!ht->data[ind].key || ht->data[ind].is_del) {
		return NULL;
	}

	return ht->data[ind].val;
}

// Return size of hash_table
size_t ht_size(hash_table *ht) {
	return ht->size;
}

// Print hash_table data in format for non-null element: |index in data array, key, value, flag is_del|
void ht_print(hash_table *ht) {
	if (!ht) {
		return;
	}

	printf("PRINT CAP, SIZE, DEL_SIZE: %zd %zd %zd\n", ht->capacity, ht->size, ht->del_size);
	for (size_t i = 0; i < ht->capacity; ++i) {
		if (ht->data[i].key) {
			printf("|%zd %s %s %d| ", i, ht->data[i].key,
						ht->data[i].val, ht->data[i].is_del);
		}
	}
	putchar('\n');
}
