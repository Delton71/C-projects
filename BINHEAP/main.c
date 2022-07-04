#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>


// -----/ DECLARATION /-----

typedef int T;

typedef struct binary_heap {
    T      *body;
    size_t capacity;
    size_t size;
    T      type;
} bh;

bh    *bh_create(size_t limit, size_t type);
bh    *bh_destroy(bh *heap);
void   bh_swap(bh *heap, T a, T b);
T      bh_get_top(bh *heap);
void   bh_shift_up(bh *heap, size_t ind);
void   bh_insert(bh *heap, T node);
T      bh_extract_top(bh *heap);
void   bh_shift_down(bh *heap, size_t ind);
void   bh_decrease_key(bh *heap, size_t ind, T delta);
size_t bh_size(bh *heap);
void   merge_bh(bh *heap1, bh *heap2);
void bh_print_body(bh *heap);

void bh_operations();
size_t find_ind(int arr[], int elem, size_t N);


// -----/ MAIN /-----

int main() {
    bh_operations();
    return 0;
}

void bh_operations() {
    size_t N = 0;
    scanf("%zd", &N);
    int *num_of_bh = (int *) calloc(N, sizeof(int));
    if (!num_of_bh) {
        return;
    }
    bh **bh_arr = (bh **) calloc(N, sizeof(bh *));
    if (!bh_arr) {
        free(num_of_bh);
        return;
    }


    for (size_t i = 0; i < N; ++i) {
        num_of_bh[i] = 2147483647;
    }
    size_t bh_count = 0;

    size_t limit = 0, type = 0; // type = 0 (min) / type = 1 (max)

    size_t c = 0, ind = 0, ind2 = 0;
    int elem = 0;
    while ((scanf("%zd", &c)) == 1) {
        switch (c) {
            case 0:
                scanf("%d %zd %zd", &num_of_bh[bh_count],
                            &limit, &type);
                bh_arr[bh_count++] = bh_create(limit, type);
                break;
            case 1:
                scanf("%zd %d", &ind, &elem);
                bh_insert(bh_arr[find_ind(num_of_bh, ind, N)], elem);
                break;
            case 2:
                scanf("%zd", &ind);
                printf("%d\n", bh_get_top(bh_arr[find_ind(num_of_bh, ind, N)]));
                break;
            case 3:
                scanf("%zd", &ind);
                bh_extract_top(bh_arr[find_ind(num_of_bh, ind, N)]);
                break;
            case 4:
                scanf("%zd", &ind);
                printf("%zd\n", bh_size(bh_arr[find_ind(num_of_bh, ind, N)]));
                break;
            case 5:
                scanf("%zd %zd", &ind, &ind2);
                merge_bh(bh_arr[find_ind(num_of_bh, ind, N)], bh_arr[find_ind(num_of_bh, ind2, N)]);
                break;
            case 6:
                scanf("%zd", &ind);
                ind = find_ind(num_of_bh, ind, N);
                bh_arr[ind] = bh_destroy(bh_arr[ind]);
                break;
            default:
                puts("No command(((");
                break;
        }
    }

    free(num_of_bh);
    for (size_t i = 0; i < bh_count; ++i) {
        bh_arr[i] = bh_destroy(bh_arr[i]);
    }
    free(bh_arr);
}

size_t find_ind(int arr[], int elem, size_t N) {
    for (size_t i = 0; i < N; ++i) {
        if (arr[i] == elem) {
            return i;
        }
    }
    return 10;
}


// -----/ BINHEAP FUNCTIONS /-----

bh *bh_create(size_t limit, size_t type) {
    bh *heap = (bh *) malloc(sizeof(bh));
    if (!heap) {
        return NULL;
    }
    heap->body = (T *) malloc((limit + 1) * sizeof(T));
    if (!heap->body) {
        free(heap);
        return NULL;
    }
    heap->capacity = limit;
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

void bh_insert(bh *heap, T node) {
    assert(heap);
    assert(heap->size < heap->capacity);
    heap->body[++heap->size] = node;
    bh_shift_up(heap, heap->size);
}

T bh_extract_top(bh *heap) {
    assert(heap);
    assert(heap->size > 0);

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
