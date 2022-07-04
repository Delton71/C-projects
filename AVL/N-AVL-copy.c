#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -----/ DEFINE ZONE /-----

#define T_LIMIT 21
typedef char T[T_LIMIT];


// -----/ DECLARATION /-----

typedef struct _Node {
    T              key;
    T              value;
    struct _Node * child[2];    // 0 is left, 1 is right childs
    int            height;
} Node;

int    height(Node *node);
void   fix_height(Node *node);
Node  *create_node(T key, T value);
Node  *left_rotate(Node *p);
Node  *right_rotate(Node *q);
Node  *insert(Node *node, T key, T value);
Node  *balance_node(Node *node);
int    bfactor(Node *node);
Node  *unlink_min(Node *node, Node **min);
Node  *erase(Node *node, T key);
char  *find(Node *node, T key);
Node  *delete_nodes(Node *node);

void AVL_operations();


// -----/ PRINT TREE /-----

typedef void (*walk_func)(Node *, size_t);
void walk(Node *node, walk_func wf, size_t level);
void print_elem(Node *node, size_t level);


// -----/ MAIN FUNCTION /-----

int main() {
    AVL_operations();
    return 0;
}

void AVL_operations() {
    Node *node = NULL;
    char command[8] = {0};
    T key = {0};
    T ins_key = {0};
    T value = {0};
    char *found = NULL;

    while ((scanf("%8s", command)) == 1) {
        if (!strcmp(command, "insert")) {
            scanf("%20s %20s", ins_key, value);
            node = insert(node, ins_key, value);
        } else if (!strcmp(command, "erase")) {
            scanf("%20s", key);
            node = erase(node, key);;
        } else if (!strcmp(command, "find")) {
            scanf("%20s", key);
            found = find(node, key);
            if (!found) {
                puts("MISSING");
            } else{
                printf("%s\n", find(node, key));
            }
        }
    }

    node = delete_nodes(node);
}


// -----/ AVL /-----

int height(Node *node) {
    if (!node) {
        return 0;
    }
    return node->height;
}

void fix_height(Node *node) {
    int h_left  = height(node->child[0]);
    int h_right = height(node->child[1]);
    node->height = (h_left > h_right? h_left : h_right) + 1;
}

Node *create_node(T key, T value) {
    Node *node = (Node *) calloc(1, sizeof(Node));
    if (!node) {
        return NULL;
    }

    node->child[0] = NULL;
    node->child[1] = NULL;
    node->height = 1;

    memcpy(node->key, key, strlen(key));
    memcpy(node->value, value, strlen(value));
    
    return node;
}

Node *left_rotate(Node *p) {
    Node *q = p->child[1];
    p->child[1] = q->child[0];
    q->child[0] = p;
    fix_height(p);
    fix_height(q);
    return q;
}

Node *right_rotate(Node *q) {
    Node *p = q->child[0];
    q->child[0] = p->child[1];
    p->child[1] = q;
    fix_height(q);
    fix_height(p);
    return p;
}

Node *insert(Node *node, T key, T value) {
    if (!node) {
        return create_node(key, value);
    }

    int comp_keys = strcmp(key, node->key);     // for char*

    if (comp_keys) {
        node->child[comp_keys > 0] = insert(node->child[comp_keys > 0],  key, value);
        return balance_node(node);
    }

    size_t len_value = strlen(value);
    memcpy(node->value, value, len_value);      // equal
    memset(node->value + len_value, 0, T_LIMIT - len_value);
    return node;
}

Node *balance_node(Node *node) {
    if (!node) {
        return NULL;
    }
    fix_height(node);
    int bf_node = bfactor(node);

    if (bf_node > 1) {
        if (bfactor(node->child[1]) < 0) {
            node->child[1] = right_rotate(node->child[1]);
        }
        return left_rotate(node);
    } else if (bf_node < -1) {
        if (bfactor(node->child[0]) > 0) {
            node->child[0] = left_rotate(node->child[0]);
        }
        return right_rotate(node);
    }

    return node;
}

int bfactor(Node *node) {
    if (!node) {
        return 0;
    }
    return height(node->child[1]) - height(node->child[0]);
}

Node *unlink_min(Node *node, Node **min) {
    if (!node->child[0]) {
        *min = node;
        return node->child[1];
    }
    node->child[0] = unlink_min(node->child[0], min);
    return balance_node(node);
}

Node *erase(Node *node, T key) {
    if (!node) {
        return NULL;
    }

    int comp_keys = strcmp(key, node->key);     // for char*

    if (comp_keys) {
        node->child[comp_keys > 0] = erase(node->child[comp_keys > 0], key);
        return balance_node(node);
    }
    
    Node *left  = node->child[0];               // equal
    Node *right = node->child[1];
    free(node);
    if (!right) {
        return left;
    }

    Node *min = NULL;
    Node *temp = NULL;
    temp = unlink_min(right, &min);
    min->child[1] = temp;
    min->child[0] = left;

    return balance_node(min);
}

char *find(Node *node, T key) {
    if (!node) {
        return NULL;
    }

    int comp_keys = strcmp(key, node->key);     // for char*
    
    if (comp_keys) {
        return find(node->child[comp_keys > 0], key);
    }

    return node->value;                     // equal
}

Node *delete_nodes(Node *node) {
    if (!node) {
        return NULL;
    }
    node->child[0] = delete_nodes(node->child[0]);
    node->child[1] = delete_nodes(node->child[1]);
    free(node);
    return NULL;
}


// -----/ PRINT TREE /-----

typedef void (*walk_func)(Node *, size_t);

void walk(Node *node, walk_func wf, size_t level) {
    if (node->child[1] != NULL) {
        walk(node->child[1], wf, level + 1);
    }
    if (node) {
        wf(node, level);
    }
    if (node->child[0] != NULL) {
        walk(node->child[0], wf, level + 1);
    }
}

void print_elem(Node *node, size_t level) {
    for (size_t i = 0; i < level; ++i) {
        printf("  ");
    }
    printf("%s\n", node->value);
}
