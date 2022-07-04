#include "list.h"

int main() {
    struct list *head = list_new(0);
    printf("head elem: %d\n", head->elem);
    list_print(head);
    head = list_insert(head, 1);
    list_print(head);
    head = list_delete(head);
    list_print(head);

    return 0;
}