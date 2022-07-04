#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>



// -----/ DEFINE ZONE /-----

#define CAPACITY 32
#define CAPACITY_LIMIT 8

#define CAP_UP_COEF 2
#define LOW_CAP_UP_COEF 0.75
#define LIM_CAP_UP_COEF 1.1
#define EPS 0.0001

#define CAP_DOWN_COEF 4


// -----/ DECLARATION /-----

struct queue {
	char *data;
	size_t capacity;
	size_t size;
	size_t elem_size;

	size_t front;
};

struct queue *queue_new(size_t elem_size);
int queue_push(struct queue *q, const void *elem);
int queue_pop (struct queue *q, void *elem);
int queue_empty(struct queue const *q);
struct queue *queue_delete(struct queue *q);
void queue_print(struct queue const *q, void (*pf)(void const *data));

static int  up_capacity(struct queue *q, long double cap_up_coef);
static void down_capacity(struct queue *q);
static int is_equal(long double a, long double b);


// -----/ CHANGE CAPACITY /-----

static int  up_capacity(struct queue *q, long double cap_up_coef) {
	char *ch_data;					// changed data point
    size_t new_capacity = (size_t) (cap_up_coef * q->capacity);
    // printf("new_cap: %zd; cap_up_coef: %Lg\n", new_capacity, cap_up_coef);

	ch_data = (char *) realloc(q->data, new_capacity * q->elem_size);

    if (!ch_data && !is_equal(cap_up_coef, LIM_CAP_UP_COEF)) {
        return up_capacity(q, cap_up_coef * LOW_CAP_UP_COEF);
    }

	if (!ch_data) {					// not enough space -> ++capacity
		ch_data = (char *) realloc(q->data, (1 + q->capacity) * q->elem_size);
		
		if (!ch_data)
			return 1;				// ++capacity -> fail
		
		q->data = ch_data;
		++q->capacity;
		return 0;					// ++capacity -> success
	}

	q->data = ch_data;
	q->capacity = new_capacity;
	return 0;						// capacity *= CAP_UP_COEF -> success
}

static void down_capacity(struct queue *q) {
	size_t new_capacity = (q->capacity / CAP_DOWN_COEF) * q->elem_size;
	char *ch_data = malloc(new_capacity);
	if (!ch_data) {
		return;
	}

	memcpy(ch_data, q->data + (q->front * q->elem_size), new_capacity);
	free(q->data);
	q->data = ch_data;
	// q->data = (char *) realloc(q->data, q->size * q->elem_size);
	q->capacity /= CAP_DOWN_COEF;
	q->front = 0;
}

static int is_equal(long double a, long double b) {
    long double diff = a - b;
    return (diff < EPS) && (diff > -EPS);
}


// -----/ QUEUE FUNCTIONS /-----

// Make new queue: q->elem_size <=> elem_size; elements are not initialized;
struct queue *queue_new(size_t elem_size) {
	assert(elem_size > 0);

	struct queue *q = (struct queue *) malloc(sizeof(struct queue));

	if (q) {						// check malloc fail
		q->data = (char *) malloc(CAPACITY * elem_size);

		if (!q->data) {
			free(q);
			return NULL;
		}

		q->capacity = CAPACITY;
		q->size = 0;
		q->elem_size = elem_size;
		q->front = 0;
	}

	return q;
}

// Add to queue q new element elem: ++q->size && return 0 (success) / 1 (fail);
int queue_push(struct queue *q, const void *elem) {
	assert(q);
	assert(elem != NULL);
	
	if (q->size + q->front >= q->capacity && up_capacity(q, CAP_UP_COEF)) {
		return 1;					// fail in up_capacity
	}

	memcpy(q->data + (q->elem_size * ((q->front + q->size) % q->capacity)),
				elem, q->elem_size);
	++q->size;
	return 0;						// success
}

// Remove from queue q top element -> put in *elem; --q->size && return 0 (success) / 1 (fail);
int queue_pop(struct queue *q, void *elem) {
	if (queue_empty(q)) {
		return 1;					// fail
	}

	memcpy(elem, q->data + q->front * q->elem_size, q->elem_size);
	--q->size;
	++q->front;

	if (q->size > CAPACITY_LIMIT && q->capacity > CAP_DOWN_COEF * q->size) {
		down_capacity(q);
	}

	return 0;						// success
}

// If queue is empty -> return 1; else -> return 0;
int queue_empty(struct queue const *q) {
	assert(q);
	return !q->size;
}

// Delete queue q and return NULL;
struct queue *queue_delete(struct queue *q) {
	assert(q);

	if (q->data) {
		free(q->data);
    }
	free(q);

	return NULL;
}

// Print elements in format: [3, 2, 1] if was queue_push(X) where x = 1, 2, 3; [] for empty queue;
// (pf*) - users function to print 1 element;
void queue_print(struct queue const *q, void (*pf)(void const *q)) {
	assert(q);
	assert(pf);
	
	putchar('[');

	if (q->size) {
		pf(q->data + (q->front + q->size - 1) * q->elem_size);
	}
	
	for (ssize_t i = q->front + q->size - 2; i >= 0 && i >= q->front; --i) {
		putchar(','); putchar(' ');
		pf(q->data + i * q->elem_size);
	}

	putchar(']'); putchar('\n');
}