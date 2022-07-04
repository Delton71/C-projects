#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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
void stack_print(struct stack const *, void (*)(void const *));

#endif /* STACK_H */
