#ifndef BINHEAP_H
#define BINHEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int T;

typedef struct binary_heap {
    T      *body;
    size_t elem_size;
    size_t capacity;
    size_t size;
    size_t type;
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
// void   heapify(T v[], size_t index, size_t size);
// void   sort_heap(T v[], size_t size);

#endif /* BIN_HEAP */