#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>



// -----/ DEFINE ZONE /-----

#define CAPACITY_LIMIT 8

#define CAP_UP_COEF 2
#define LOW_CAP_UP_COEF 0.75
#define LIM_CAP_UP_COEF 1.1
#define EPS 0.0001

#define CAP_DOWN_COEF 4


// -----/ DECLARATION /-----

struct vector {
	char *data;
	size_t size;
	size_t capacity;
	size_t elem_size;
};

struct vector *vector_new(size_t elems, size_t elem_size);
int vector_push(struct vector *v, void const *elem);
int vector_pop(struct vector *v, void *elem);
int vector_empty(struct vector const *v);
struct vector *vector_delete(struct vector *v);
void vector_print(struct vector const *v, void (*pf)(void const *data));
int vector_set(struct vector *v, size_t index, void const *elem);
int vector_get(struct vector const *v, size_t index, void *elem);
int vector_resize(struct vector *v, size_t new_size);
size_t vector_size(struct vector const *v);

static int  up_capacity(struct vector *v, long double cap_up_coef);
static void down_capacity(struct vector *v);
static int is_equal(long double a, long double b);


// -----/ CHANGE CAPACITY /-----

static int  up_capacity(struct vector *v, long double cap_up_coef) {
	char *ch_data;					// changed data point
    size_t new_capacity = (size_t) (cap_up_coef * v->capacity);
    // printf("new_cap: %zd; cap_up_coef: %Lg\n", new_capacity, cap_up_coef);

	ch_data = (char *) realloc(v->data, new_capacity * v->elem_size);

    if (!ch_data && !is_equal(cap_up_coef, LIM_CAP_UP_COEF)) {
        return up_capacity(v, cap_up_coef * LOW_CAP_UP_COEF);
    }

	if (!ch_data) {					// not enough space -> ++capacity
		ch_data = (char *) realloc(v->data, (1 + v->capacity) * v->elem_size);
		
		if (!ch_data)
			return 1;				// ++capacity -> fail
		
		v->data = ch_data;
		++v->capacity;
		return 0;					// ++capacity -> success
	}

	v->data = ch_data;
	v->capacity = new_capacity;
	return 0;						// capacity *= CAP_UP_COEF -> success
}

// capacity = v->size
static void down_capacity(struct vector *v) {
	v->data = (char *) realloc(v->data, v->size * v->elem_size);
	v->capacity = v->size;
}

static int is_equal(long double a, long double b) {
    long double diff = a - b;
    return (diff < EPS) && (diff > -EPS);
}


// -----/ VECTOR FUNCTIONS /-----

// Make new vector: v->size <=> elems; v->elem_size <=> elem_size; elements are not initialized;
struct vector *vector_new(size_t elems, size_t elem_size) {
	assert(elem_size > 0);
	assert(elems >= 0);
	
	struct vector *v = (struct vector *) malloc(sizeof(struct vector));

	if (v) {
		v->size = elems;

		if (elems <= 8) {
			elems = 8;
		}

		v->data = (char *) malloc(elems * elem_size);
			
		if (!v->data) {
			free(v);
		return NULL;
		}
		
		v->capacity = elems;
		v->elem_size = elem_size;
	}

	return v;
}

// Add to vector v new element elem: ++v->size && return 0 (success) / 1 (fail);
int vector_push(struct vector *v, void const *elem) {
	assert(v);
	assert(elem != NULL);
	
	if (v->size >= v->capacity && up_capacity(v, CAP_UP_COEF)) {
			return 1;			// fail in up_capacity
	}

	++v->size;
	vector_set(v, v->size - 1, elem);
	return 0;					// success
}

// Remove from vector v last element -> put in *elem; --v->size && return 0 (success) / 1 (fail);
int vector_pop(struct vector *v, void *elem) {
	if (vector_get(v, v->size - 1, elem))
		return 1;				// fail

	if (v->size > CAPACITY_LIMIT && v->capacity > CAP_DOWN_COEF * v->size) {
		down_capacity(v);
	}
	--v->size;

	return 0;					// success
}

// If vector v is empty -> return 1; else -> return 0;
int vector_empty(struct vector const *v) {
	assert(v);
	return !v->size;
}

// Delete vector v and return NULL;
struct vector *vector_delete(struct vector *v) {
	assert(v);

	if (v->data)
		free(v->data);
	free(v);

	return NULL;
}

// Print elements in format: [v1, v2, ...]; (*pf) - users function to print 1 element;
void vector_print(struct vector const *v, void (*pf)(void const *data)) {
	assert(v);
	assert(pf);

	putchar('[');

	if (v->size)
		pf(v->data);

	for (size_t i = 1; i < v->size; ++i) {
		putchar(','); putchar(' ');
		pf(v->data + i * v->elem_size);
	}

	putchar(']'); putchar('\n');
}

// Set v[index] = elem && return 0 (success) / 1 (fail);
int vector_set(struct vector *v, size_t index, void const *elem) {
	assert(v);

	if(0 > index || index >= v->size)
		return 1;				// fail
	
	memcpy(v->data + (v->elem_size * index), elem, v->elem_size);
	return 0;					// success
}

// Set elem = v[index] && return 0 (success) / 1 (fail);
int vector_get(struct vector const *v, size_t index, void *elem) {
	assert(v);

	if (0 > index || index >= v->size)
		return 1;				// fail

	memcpy(elem, v->data + (v->elem_size * index), v->elem_size);
	return 0;					// success
}

// Change v->size => v->new_size;
int vector_resize(struct vector *v, size_t new_size) {
	assert(v);
	
	if (0 > new_size)
		return 1;				// fail

	if (v->size == new_size)
		return 0;				// no operations -> success

	char *ch_data = NULL;		// changed data pointer
	if (new_size > v->size) {
		if (new_size <= 8) {
			new_size = 8;
		}
		
		ch_data = (char *) realloc(v->data, new_size * v->elem_size);
		
		if (!ch_data)			// not enough space -> return 0;
			return 1;			// fail

		v->capacity = new_size;
		v->data = ch_data;
	}
	
	v->size = new_size;
	return 0;					// success
}

// return v->size
size_t vector_size(struct vector const *v) {
	assert(v);
	return v->size;
}

