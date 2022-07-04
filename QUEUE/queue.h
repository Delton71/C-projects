#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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


#endif /* QUEUE_H */
