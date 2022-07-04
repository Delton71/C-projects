#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>


// -----/ DEFINE ZONE /-----

typedef int T;

#define CAPACITY 32
#define CAPACITY_LIMIT 8

#define CAP_UP_COEF 2
#define LOW_CAP_UP_COEF 0.75
#define LIM_CAP_UP_COEF 1.1
#define EPS 0.0001

#define CAP_DOWN_COEF 4


// -----/ DECLARATION /-----

typedef struct binary_heap {
    T      *body;
    size_t elem_size;
    size_t capacity;
    size_t size;
    T      type;
} bh;

bh    *bh_create(size_t type);
bh    *bh_destroy(bh *heap);
void   bh_swap(bh *heap, T a, T b);
T      bh_get_top(bh *heap);
void   bh_shift_up(bh *heap, size_t ind);
int    bh_insert(bh *heap, T node);
T      bh_extract_top(bh *heap);
void   bh_shift_down(bh *heap, size_t ind);
void   bh_decrease_key(bh *heap, size_t ind, T delta);
size_t bh_size(bh *heap);
void   merge_bh(bh *heap1, bh *heap2);
void   bh_print_body(bh *heap);

static int  up_capacity(bh *heap, long double cap_up_coef);
static void down_capacity(bh *heap);
static int  is_equal(long double a, long double b);


// -----/ CHANGE CAPACITY /-----

static int  up_capacity(bh *heap, long double cap_up_coef) {
	T *ch_body;					// changed data point
    size_t new_capacity = (size_t) (cap_up_coef * heap->capacity);

	ch_body = (T *) realloc(heap->body, new_capacity * heap->elem_size);

    if (!ch_body && !is_equal(cap_up_coef, LIM_CAP_UP_COEF)) {
        return up_capacity(heap, cap_up_coef * LOW_CAP_UP_COEF);
    }

	if (!ch_body) {					// not enough space -> ++capacity
		ch_body = (T *) realloc(heap->body,
                    (1 + heap->capacity) * heap->elem_size);
		
		if (!ch_body)
			return 1;				// ++capacity -> fail
		
		heap->body = ch_body;
		++heap->capacity;
		return 0;					// ++capacity -> success
	}

	heap->body = ch_body;
	heap->capacity = new_capacity;
	return 0;						// capacity *= CAP_UP_COEF -> success
}

// capacity = heap->size
static void down_capacity(bh *heap) {
	heap->body = (T *) realloc(heap->body, (heap->size + 1) * heap->elem_size);
	heap->capacity = heap->size + 1;
}

static int is_equal(long double a, long double b) {
    long double diff = a - b;
    return (diff < EPS) && (diff > -EPS);
}


// -----/ BINHEAP FUNCTIONS /-----

bh *bh_create(size_t type) {
    bh *heap = (bh *) malloc(sizeof(bh));
    if (!heap) {
        return NULL;
    }
    heap->body = (T *) malloc((CAPACITY + 1) * sizeof(T));
    if (!heap->body) {
        free(heap);
        return NULL;
    }
    heap->elem_size = sizeof(T);
    heap->capacity = CAPACITY;
    heap->size = 0;
    if (type) {
        heap->type = -1;
    } else {
        heap->type = 1;
    }
    return heap;
}

bh *bh_destroy(bh *heap) {
    if (!heap) {
        return NULL;
    }
    if (heap->body) {
        free(heap->body);
    }
    free(heap);
    return NULL;
}

void bh_swap(bh *heap, T a, T b) {
    assert(heap);

    T tmp = heap->body[a];
    heap->body[a] = heap->body[b];
    heap->body[b] = tmp;
}

T bh_get_top(bh *heap) {
    assert(heap);
    assert(heap->size > 0);
    return heap->body[1];
}

void bh_shift_up(bh *heap, size_t ind) {
    assert(heap);
    for (size_t i = ind; i > 1 &&
                (heap->type) * heap->body[i] < (heap->type) * heap->body[i >> 1]; i >>= 1) {
        bh_swap(heap, i, i >> 1);
    }
}

int bh_insert(bh *heap, T node) {
    assert(heap);
    if (heap->size >= (heap->capacity - 1) && up_capacity(heap, CAP_UP_COEF)) {
        return 1;
    }
    
    heap->body[++heap->size] = node;
    bh_shift_up(heap, heap->size);
    return 0;
}

T bh_extract_top(bh *heap) {
    assert(heap);
    assert(heap->size > 0);
    if (heap->size > (CAPACITY_LIMIT + 1) && heap->capacity > CAP_DOWN_COEF * (heap->size + 1)) {
        down_capacity(heap);
    }

    bh_swap(heap, 1, heap->size);
    --heap->size;
    bh_shift_down(heap, 1);
    return heap->body[heap->size + 1];
}

void bh_shift_down(bh *heap, size_t ind) {
    assert(heap);
    if (!heap->size) {
        return;
    }

    for (;;) {
        size_t left = (ind << 1);
        size_t right = left + 1;
        size_t smallest = ind;
        if (left <= heap->size &&
                    (heap->type) * heap->body[left] < (heap->type) * heap->body[ind]) {
            smallest = left;
        }
        if (right <= heap->size &&
                    (heap->type) * heap->body[right] < (heap->type) * heap->body[smallest]) {
            smallest = right;
        }
        if (smallest == ind) {
            break;
        }
        bh_swap(heap, ind, smallest);
        ind = smallest;
    }
}

void bh_decrease_key(bh *heap, size_t ind, T delta) {
    assert(heap);
    assert(ind < heap->size);
    assert(delta > 0);
    heap->body[ind] -= delta;
    bh_shift_up(heap, ind);
}

size_t bh_size(bh *heap) {
    assert(heap);
    return heap->size;
}


void merge_bh(bh *heap1, bh *heap2) {
    assert(heap1);
    assert(heap2);

    memcpy(&heap1->body[heap1->size + 1], 
                &heap2->body[1], heap2->size * sizeof(T));

    heap1->size += heap2->size;

    for (size_t i = heap1->size >> 1; i > 0; --i) {
        bh_shift_down(heap1, i);
    }

    heap2->size = 0;
}

void bh_print_body(bh *heap) {
    for (size_t i = 1; i <= heap->size; ++i) {
        printf("%d ", heap->body[i]);
    }
    putchar('\n');
}
