#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

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

#endif /* STACK_H */
