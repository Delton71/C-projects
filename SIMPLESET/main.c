#include "simpleset.h"
            
int main() {
    struct set *s = set_new(10);
    assert(set_size(s) == 0);
    assert(set_insert(s, 1) == 0);
    assert(set_insert(s, 2) == 0);
    assert(set_insert(s, 100) != 0);
    assert(set_size(s) == 2);
    set_print(s);
    printf("Size: %ld\n", s->size);
    assert(set_find(s,1) != 0);
    assert(set_find(s,100) == 0);
    assert(set_insert(s, 5) == 0);
    set_print(s);
    printf("Size: %ld\n", s->size);
    assert(set_size(s) == 3);
    assert(set_erase(s,2) == 0);
    set_print(s);
    printf("Size: %ld\n", s->size);
    assert(set_size(s) == 2);
    assert(set_findfirst(s, 2) == 5);
    assert(set_findfirst(s, 10) == -1);
    assert(set_delete(s) == NULL);
}