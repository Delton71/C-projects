#include "queue.h"

static void print_ld(void const *q) {
   printf("%Lg", *(long double *)q);
}

int main() {
    struct queue *q = queue_new(sizeof (long double));
    queue_print(q, print_ld);
    for (size_t i = 0; i < 3; i++) {
        long double tmp = i * i;
        queue_push(q, &tmp);
        queue_print(q, print_ld);
    }
    queue_print(q, print_ld);

    queue_print(q, print_ld);

    q = queue_delete(q);
}