#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


// -----/ DECLARATION /-----

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


// -----/ LIST FUNCTIONS /-----

// Make new list: return struct list *head; head->elem = elem; head->next = NULL;
struct list *list_new(int elem) {
    struct list *head = (struct list *) malloc(sizeof(struct list));
    if (!head) {
        return NULL;
    }

    head->elem = elem;
    head->next = NULL;

    return head;
}

// Insert *curr in the end of list; curr->elem = elem; return curr;
struct list *list_insert(struct list *head, int elem) {
    if (!head) {
        return list_new(elem);
    }

    struct list *curr = list_new(elem);

    if (!curr) {
        return head;
    }

    struct list *prev = head;
    while (prev->next) {
        prev = prev->next;
    }
    prev->next = curr;

    return head;
}

// Find in the list *curr: curr->elem = elem; return *curr;
struct list *list_find(struct list *head, int elem) {
    if (!head) {
        return NULL;
    }

    struct list *curr = head;
    while (curr && curr->elem != elem) {
        curr = curr->next;
    }

    return curr;
}

// Find in the list *curr: curr->elem = elem && remove curr from the list; return new *head;
struct list *list_erase(struct list *head, int elem) {
    if (!head) {
        return NULL;
    }

    struct list *prev = head;
    if (head->elem == elem) {
        head = head->next;
        free(prev);
        return head;
    }

    while (prev->next && prev->next->elem != elem) {
        prev = prev->next;
    }
    if (!prev->next) {
        return head;
    }

    struct list *next = prev->next;
    prev->next = prev->next->next;
    free(next);

    return head;
}

// Insert *what after *where; return new *head;
// If list is empty -> new list is not empty
// If what or where is NULL -> no operations && return *head;
struct list *list_insert_after(struct list *head, struct list *where, struct list *what) {
    if (!what || !where) {
        return head;
    }
    if (!head) {
        what->next = NULL;
        return what;
    }

    struct list *where_next = where->next;
    where->next = what;
    while (what->next) {
        what = what->next;
    }
    what->next = where_next;

    return head;
}

// Insert *what before *where; return new *head;
// If list is empty -> new list is not empty
// If what or where is NULL -> no operations && return *head;
struct list *list_insert_before(struct list *head, struct list *where, struct list *what) {
    if (!what || !where) {
        return head;
    }
    if (!head) {
        what->next = NULL;
        return what;
    }

    if (head == where) {
        head = what;    
    } else {
        struct list *curr = head;
        while (curr->next != where) {
            curr = curr->next;
        }
        curr->next = what;
    }

    while (what->next) {
        what = what->next;
    }
    what->next = where;

    return head;
}

// Delete the list and return NULL;
struct list *list_delete(struct list *head) {
    struct list *curr = head, *prev;
    while (curr) {
        prev = curr;
        curr = prev->next;
        free(prev);
    }

    return NULL;
}

// return curr->next; if curr is empty -> return NULL;
struct list *list_next(struct list *curr) {
    if (!curr) {
        return NULL;
    }
    return curr->next;
}

// Print elements in format: [1, 2, ...]; [] for empty list;
void list_print(struct list const *head) {
	putchar('[');

    struct list *curr = NULL;
	if (head) {
		printf("%d", head->elem);
        curr = head->next;
	}

	while (curr) {
        printf(", %d", curr->elem);
        curr = curr->next;
	}

	putchar(']'); putchar('\n');
}
