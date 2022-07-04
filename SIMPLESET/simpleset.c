#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct set {
    char *data;
    size_t size;
    size_t capacity;
};

// Make new set with uninitialized data
struct set *set_new(size_t capacity) {
    assert(capacity > 0);
    struct set *s = (struct set *) malloc(sizeof(struct set));
    if (!s) {
        return NULL;            // fail
    }

    s->data = (char *) calloc(((capacity >> 3) + 1), sizeof(char));
    if (!s->data) {
        free(s);
        return NULL;            // fail
    }

    s->size = 0;
    s->capacity = capacity;
    return s;                   // success
}

// Add to s elem; if the elem is exists -> no change
int set_insert(struct set *s, size_t elem) {
    assert(s);
    if (elem >= s->capacity) {
        return 1;               // fail
    }

    s->size += !(1 & (s->data[elem >> 3] >> (elem % 8)));
    s->data[elem >> 3] |= (1 << (elem % 8));
    return 0;                   // success
}

// Delete elem from s; if the elem isn't exists -> no change
int set_erase(struct set *s, size_t elem) {
    assert(s);
    if (elem >= s->capacity) {
        return 1;               // fail
    }

    s->size -= 1 & (s->data[elem >> 3] >> (elem % 8));
    s->data[elem >> 3] &= ~(1 << (elem % 8));
    return 0;                   // success
}

// If elem is exists -> return 1; else -> return 0;
int set_find(struct set const *s, size_t elem) {
    assert(s);
    if (elem >= s->capacity) {
        return 0;               // fail
    }

    return (1 & (s->data[elem >> 3] >> (elem % 8)));
}

// Delete struct set s; return NULL;
struct set *set_delete(struct set *s) {
    assert(s);

    free(s->data);
    free(s);
    return NULL;
}

// If s is empty -> return 0; else -> return 1;
// ?????????????? (0, 1)
int set_empty(struct set const *s) {
    assert(s);
    return s->size != 0;
}

// Return the lowest of the elements, not less then start
// If no such elements -> return -1;
ssize_t set_findfirst(struct set const *s, size_t start) {
    assert(s);

    while (start < s->capacity) {
        if (set_find(s, start)) {
            return start;       // success
        }
        ++start;
    }

    return -1;                  // fail
}

// Return s->size
size_t set_size(struct set const *s) {
    assert(s);
    return s->size;
}

// Print elements in format: [] or [s1, s2, ...];
void set_print(struct set const *s) {
    assert(s);
    putchar('[');

    if (set_empty(s)) {
        size_t first_elem = set_findfirst(s, 0);
        if (first_elem != -1)
            printf("%zd", first_elem);

        for (size_t i = first_elem + 1; i < s->capacity; ++i) {
            if (set_find(s, i)) {
                printf(", %zd", i);
            }
        }
    }

    putchar(']');
    putchar('\n');
}
