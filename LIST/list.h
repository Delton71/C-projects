#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


struct list {
	int elem;
    struct list *next;
};

struct list *list_new(int elem);
struct list *list_insert(struct list *head, int elem);
struct list *list_find(struct list *head, int elem);
struct list *list_erase(struct list *head, int elem);
struct list *list_insert_after(struct list *head, struct list *where, struct list *what);
struct list *list_insert_before(struct list *head, struct list *where, struct list *what);
struct list *list_delete(struct list *head);
struct list *list_next(struct list *curr);
void list_print(struct list const *head);


#endif /* LIST_H */
