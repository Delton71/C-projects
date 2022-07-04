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

struct stack {
	char *data;
	size_t capacity;
	size_t size;
	size_t elem_size;
};

struct stack *stack_new(size_t);
int stack_push(struct stack *, const void *);
int stack_pop (struct stack *, void *);
int stack_top (struct stack *, void *);
int stack_empty(struct stack const *);
struct stack *stack_delete(struct stack *);

static int  up_capacity(struct stack *, long double cap_up_coef);
static void down_capacity(struct stack *);
static int is_equal(long double a, long double b);


// -----/ CHANGE CAPACITY /-----

static int  up_capacity(struct stack *st, long double cap_up_coef) {
	char *ch_data;					// changed data point
    size_t new_capacity = (size_t) (cap_up_coef * st->capacity);
    // printf("new_cap: %zd; cap_up_coef: %Lg\n", new_capacity, cap_up_coef);

	ch_data = (char *) realloc(st->data, new_capacity * st->elem_size);

    if (!ch_data && !is_equal(cap_up_coef, LIM_CAP_UP_COEF)) {
        return up_capacity(st, cap_up_coef * LOW_CAP_UP_COEF);
    }

	if (!ch_data) {					// not enough space -> ++capacity
		ch_data = (char *) realloc(st->data, (1 + st->capacity) * st->elem_size);
		
		if (!ch_data)
			return 1;				// ++capacity -> fail
		
		st->data = ch_data;
		++st->capacity;
		return 0;					// ++capacity -> success
	}

	st->data = ch_data;
	st->capacity = new_capacity;
	return 0;						// capacity *= CAP_UP_COEF -> success
}

static void down_capacity(struct stack *st) {
	st->data = (char *) realloc(st->data, st->size * st->elem_size);
	st->capacity = st->size;
}

static int is_equal(long double a, long double b) {
    long double diff = a - b;
    return (diff < EPS) && (diff > -EPS);
}

// -----/ STACK FUNCTIONS /-----

struct stack *stack_new(size_t elem_size) {
	assert(elem_size > 0);

	struct stack *st = (struct stack *) malloc(sizeof(struct stack));

	if (st) {						// check malloc fail
		st->data = (char *) malloc(CAPACITY * elem_size);

		if (!st->data) {
			free(st);
			return NULL;
		}

		st->capacity = CAPACITY;
		st->size = 0;
		st->elem_size = elem_size;
	}

	return st;
}

// Add to stack st new element elem: ++st->size && return 0 (success) / 1 (fail);
int stack_push(struct stack *st, const void *elem) {
	assert(st);
	assert(elem != NULL);
	
	if (st->size >= st->capacity && up_capacity(st, CAP_UP_COEF)) {
		return 1;					// fail in up_capacity
	}

	memcpy(st->data + (st->elem_size * st->size), elem, st->elem_size);
	++st->size;
	return 0;						// success
}

// Remove from stack st top element -> put in *elem; --st->size && return 0 (success) / 1 (fail);
int stack_pop(struct stack *st, void *elem) {
	if (stack_top(st, elem))
		return 1;					// fail

	if (st->size > CAPACITY_LIMIT && st->capacity > CAP_DOWN_COEF * st->size) {
		down_capacity(st);
	}
	--st->size;

	return 0;						// success
}

// Get from stack st top element -> put in *elem; return 0 (success) / 1 (fail);
int stack_top(struct stack *st, void *elem) {
	assert(st);
	assert(elem != NULL);

	if (stack_empty(st))			// stack is empty -> fail
		return 1;

	memcpy(elem, st->data + (st->elem_size * (st->size - 1)), st->elem_size);
	return 0;						// success
}

// If stack is empty -> return 1; else -> return 0;
int stack_empty(struct stack const *st) {
	assert(st);
	return !st->size;
}

// Delete stack st and return NULL;
struct stack *stack_delete(struct stack *st) {
	assert(st);

	if (st->data)
		free(st->data);
	free(st);

	return NULL;
}

// Print elements in format: [st1, st2, ...]; (pf*) - users function to print 1 element;
void stack_print(struct stack const *st, void (*pf)(void const *st)) {
	assert(st);
	assert(pf);

	putchar('[');

	if (st->size)
		pf(st->data);

	for (size_t i = 1; i < st->size; ++i) {
		// printf(", ");
		putchar(','); putchar(' ');
		pf(st->data + i * st->elem_size);
	}

	// printf("]\n");
	putchar(']'); putchar('\n');
}

