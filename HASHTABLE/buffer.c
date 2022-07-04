#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -----/ DEFINE ZONE /-----

const long double    UP_CAP_COEFF = 0.8;
const long double  DOWN_CAP_COEFF = 0.4;
const int             HASH1_COEFF = 7;
const int            RESIZE_COEFF = 2;
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


static size_t CAPACITY = 8;
static size_t LIMIT_DOWN_SIZE = 2;


// -----/ DECLARATION /-----

static size_t hash1(K key, size_t capacity);
static size_t hash2(K key);
static int ht_rehash_insert(backet *data, size_t capacity, K key, V val);
static int rehash(hash_table *ht, size_t new_capacity);

hash_table *ht_create(size_t capacity);
hash_table *ht_destroy(hash_table *ht);
size_t ht_find_ind(backet *data, size_t capacity, K key);
int ht_insert(hash_table *ht, K key, V val);
int ht_erase(hash_table *ht, K key);
V   ht_get(hash_table *ht, K key);
size_t ht_size(hash_table *ht);
void ht_print(hash_table *ht);


void ht_operations();
size_t ident_command(char *command);
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

    char command[10] = {'\0'};
    size_t c = 0;
    int ind = 0;
    size_t ht_num = 0;
    char key[21];
    char *val = NULL;
    char *get_val = NULL;
    while ((scanf("%9s", command)) == 1) {
        c = ident_command(command);
        switch (c) {
            case 0:     // create
                // puts("CASE 0:");
                for (size_t i = 0; i < N; ++i) {
                    if (!ht_arr[i]) {
                        scanf("%d", &num_of_ht[i]);
                        ht_arr[i] = ht_create(CAPACITY);
                        break;
                    }
                }
                /*
                for (size_t i = 0; i < N; ++i) {
                    printf("%d %p\n", num_of_ht[i], ht_arr[i]);
                }
                */
                break;
            case 1:     // insert
                // puts("CASE 1:");
                val = (char *) calloc(21, sizeof(char));
                scanf("%d %255s %255s", &ind, key, val);
                ht_num = find_ind(num_of_ht, ind, N);
                if (ht_insert(ht_arr[ht_num], key, val)) {
                    free(val);
                }
                // ht_print(ht_arr[ht_num]);
                break;
            case 2:     // erase
                // puts("CASE 2:");
                scanf("%d %255s", &ind, key);
                ht_num = find_ind(num_of_ht, ind, N);
                ht_erase(ht_arr[ht_num], key);
                break;
            case 3:     // find
                // puts("CASE 3:");
                scanf("%d %255s", &ind, key);
                // printf("%d %s\n", ind, key);
                ht_num = find_ind(num_of_ht, ind, N);
                get_val = ht_get(ht_arr[ht_num], key);
                if (get_val) {
                    printf("%s\n", get_val);
                } else {
                    puts("MISSING");
                }
                break;
            case 4:     // destroy
                // puts("CASE 4:");
                scanf("%d", &ind);
                ht_num = find_ind(num_of_ht, ind, N);
                ht_arr[ht_num] = ht_destroy(ht_arr[ht_num]);
                /*
                for (size_t i = 0; i < N; ++i) {
                    printf("%d %p\n", num_of_ht[i], ht_arr[i]);
                }
                */
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

size_t ident_command(char *command) {
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
    return 10;
}

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

static size_t hash2(K key) {
	return strlen(key) | 1;  // odd value
}

static int ht_rehash_insert(backet *data, size_t capacity, K key, V val) {
	if (!data || !key || !capacity) {
		return 1;
	}
	size_t ind = ht_find_ind(data, capacity, key);
	printf("REHASH_INSERT key, val, ind: %s %s %zd\n", key, val, ind);
	data[ind].key = key;
	data[ind].val = val;
	data[ind].is_del = 0;
	return 0;
}

static int rehash(hash_table *ht, size_t new_capacity) {
	backet *new_data = (backet *) calloc(sizeof(backet), new_capacity);
	if (!new_data) {
		return 1;
	}

	for (size_t i = 0; i < ht->capacity; ++i) {
		printf("REHASH ind, key: %zd %s\n", i, ht->data[i].key);
		if (ht->data[i].key != NULL) {
			if (ht->data[i].is_del) {
				free(ht->data[i].key);
				ht->data[i].key = NULL;
				ht->data[i].is_del = 0;
			} else {
				ht_rehash_insert(new_data, new_capacity, ht->data[i].key, ht->data[i].val);
			}
		}
	}

	free(ht->data);
	ht->data = new_data;
	ht->capacity = new_capacity;
	ht->del_size = 0;
	printf("NEWCAP: %zd\n", new_capacity);
	ht_print(ht);
	return 0;
}


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

/*
A = ht->data[ind].key					// key is exist
B = !strcmp(key, ht->data[ind].key)		// equal
C = ht->data[ind].is_del				// element is deleted

need: !A || (B && C)

A && (!B || !C)
*/

size_t ht_find_ind(backet *data, size_t capacity, K key) {
	size_t h1 = hash1(key, capacity);

	size_t h2 = hash2(key);;

	size_t ind = h1;
	while (data[ind].key && !strcmp(key, data[ind].key) && data[ind].is_del) {
		ind = (ind + h2) % capacity;
	}
	return ind;
}

int ht_insert(hash_table *ht, K key, V val) {
	if (!ht) {
		return 1;
	}

	printf("??? %zd %Lg\n", ht->size, ht->capacity * UP_CAP_COEFF);
	if (ht->size >= ht->capacity * UP_CAP_COEFF) {
		rehash(ht, (size_t) ht->capacity * RESIZE_COEFF);
	}

	size_t h1 = hash1(key, ht->capacity);

	size_t h2 = 0;
	if (ht->data[h1].key) {
		h2 = hash2(key);
	}

	size_t ind = h1;
	while (ht->data[ind].key && strcmp(key, ht->data[ind].key)) {
		ind = (ind + h2) % ht->capacity;
	}

	printf("INS ind: %zd\n", ind);

	if (!ht->data[ind].key) {
		ht->data[ind].key = (char *) calloc((strlen(key) + 1), sizeof(char));
		if (!ht->data[ind].key) {
			return 1;
		}
		
		memcpy(ht->data[ind].key, key, strlen(key));
	}
	// printf("val: %s\n", ht->data[ind].val);
	if (ht->data[ind].val) {
		free(ht->data[ind].val);
		--ht->size;
		puts("!!!");
	}
	ht->data[ind].val = val;

	if (ht->data[ind].is_del) {
		--ht->del_size;
		++ht->size;
	}
	ht->data[ind].is_del = 0;
	++ht->size;

	ht_print(ht);
	return 0;
}

int ht_erase(hash_table *ht, K key) {
	if(!ht) {
		return 1;
	}
	size_t ind = ht_find_ind(ht->data, ht->capacity, key);
	if(!ht->data[ind].key) {
		return 1;
	}
	ht->data[ind].is_del = 1;
	--ht->size;
	++ht->del_size;
	
	printf("??? %zd %Lg\n", ht->size, ht->capacity * DOWN_CAP_COEFF);
	if (ht->size > LIMIT_DOWN_SIZE && ht->size <= ht->capacity * DOWN_CAP_COEFF) {	// ht->size + ht->del_size
		rehash(ht, ht->capacity / RESIZE_COEFF);
	}
	
	ht_print(ht);
	return 0;
}

V ht_get(hash_table *ht, K key) {	// return point (!!!)
	if(!ht) {
		return NULL;
	}
	
	size_t ind = ht_find_ind(ht->data, ht->capacity, key);
	if(!ht->data[ind].key || ht->data[ind].is_del) {
		return NULL;
	}

	// ht_print(ht);
	return ht->data[ind].val;
}

size_t ht_size(hash_table *ht) {
	return ht->size;
}

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
