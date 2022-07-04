#ifndef SIMPLESET_H
#define SIMPLESET_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct set {
    char *data;
    size_t size;
    size_t capacity;
};


struct set *set_new(size_t capacity);
int set_insert(struct set *s, size_t elem);
int set_erase(struct set *s, size_t elem);
int set_find(struct set const *s, size_t elem);
struct set *set_delete(struct set *s);
int set_empty(struct set const *s);
ssize_t set_findfirst(struct set const *s, size_t start);
size_t set_size(struct set const *s);
void set_print(struct set const *s);


#endif /* SIMPLESET_H */
