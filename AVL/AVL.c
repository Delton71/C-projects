#include <stdio.h>
#include <stdlib.h>

// -----/ DEFINE ZONE /-----

typedef char* T;
#define T_LIMIT 21


// -----/ DECLARATION /-----

typedef struct _Node {
    T             key;
    T             value;
    struct _Node *left;
    struct _Node *right;
    unsigned char height;
} Node;

unsigned int height(Node *node);
void fix_height(Node *node);
Node *create_node(T key, T value);
Node *left_rotate(Node *p);
Node *right_rotate(Node *q);
Node *insert(Node *node, T key, T value);
Node *balance_node(Node *node);
int bfactor(Node *node);
Node *find_min(Node *node);
Node *find_max(Node *node);
Node *unlink_min(Node *node);
Node *unlink_max(Node *node);
Node *erase(Node *node, T key);
T find(Node *node, T key);
void free_node(Node *node);                 // for char*
Node *delete_nodes(Node *node);


// -----/ PRINT TREE /-----
typedef void (*walk_func)(Node *, size_t);
void walk(Node *node, walk_func wf, size_t level);
void print_elem(Node *node, size_t level);


// -----/ AVL /-----

unsigned int height(Node *node) {
    if (!node) {
        return 0;
    }
    return node->height;
}

void fix_height(Node *node) {
    unsigned int h_left  = height(node->left);
    unsigned int h_right = height(node->right);
    node->height = (h_left > h_right? h_left : h_right) + 1;
}

Node *create_node(T key, T value) {
    Node *node = (Node *) malloc(sizeof(Node *));
    if (!node) {
        return NULL;
    }
    node->key = key;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

Node *left_rotate(Node *p) {
    Node *q = p->right;
    p->right = q->left;
    q->left = p;
    fix_height(p);
    fix_height(q);
    return q;
}

Node *right_rotate(Node *q) {
    Node *p = q->left;
    q->left = p->right;
    p->right = q;
    fix_height(q);
    fix_height(p);
    return p;
}

Node *insert(Node *node, T key, T value) {
    if (!node) {
        return create_node(key, value);
    }

    int comp_keys = strcmp(node->key, key);     // for char*
    if (!comp_keys) {                   // equal
        free(node->value);
        node->value = value;
        return node;
    } else if (comp_keys > 0) {         // node->key > key
        node->left  = insert(node->left,  key, value);
    } else {                            // node->key < key
        node->right = insert(node->right, key, value);
    }

    return balance_node(node);
}

Node *balance_node(Node *node) {
    int bf_node = bfactor(node);

    if (bf_node > 1) {
        if (bfactor(node->right) == -1) {
            right_rotate(node->right);
        }
        return left_rotate(node);
    } else if (bf_node < -1) {
        if (bfactor(node->left) == 1) {
            left_rotate(node->left);
        }
        return right_rotate(node);
    }

    return node;
}

int bfactor(Node *node) {
    if (!node) {
        return 0;
    }
    return height(node->right) - height(node->left);
}

Node *find_min(Node *node) {
    Node *curr = node;
    while (curr->left != NULL) {
        curr = curr->left;
    }
    return curr;
}

Node *find_max(Node *node) {
    Node *curr = node;
    while (curr->right != NULL) {
        curr = curr->right;
    }
    return curr;
}

Node *unlink_min(Node *node) {
    if (!node->left) {
        return node->right;
    }
    node->left = unlink_min(node->left);
    return balance_node(node);
}

Node *unlink_max(Node *node) {
    if (!node->right) {
        return node->left;
    }
    node->right = unlink_max(node->right);
    return balance_node(node);
}

Node *erase(Node *node, T key) {
    if (!node) {
        return NULL;
    }

    if (node->key == key) {
        Node *temp = NULL;
        if (node->right) {
            temp = find_min(node->right);
            node->right = unlink_min(node->right);
        } else if (node->left) {
            temp = find_max(node->left);
            node->left = unlink_max(node->left);
        } else {
            free_node(node);
            return NULL;
        }

        temp->left = node->left;
        temp->right = node->right;
        free_node(node);
        return balance_node(temp);
    }

    if ((node->key) > key) {
        node->left  = erase(node->left, key);
    } else {
        node->right = erase(node->right, key);
    }

    return balance_node(node);
}

T find(Node *node, T key) {
    if (!node) {
        return NULL;
    }

    int comp_keys = strcmp(node->key, key);     // for char*
    if (comp_keys > 0) {                    // node->key > key
        return find(node->left, key);
    } else if (comp_keys < 0) {             // node->key < key
        return find(node->right, key);
    }
    return node->value;                     // equal
}

void free_node(Node *node) {        // for char*
    if (node) {
        if (node->key) {
            free(node->key);
        }
        if (node->value) {
            free(node->value);
        }
        free(node);
    }
}

Node *delete_nodes(Node *node) {
    if (!node) {
        return NULL;
    }
    node->left  = delete_nodes(node->left);
    node->right = delete_nodes(node->right);
    free_node(node);
    return NULL;
}


// -----/ PRINT TREE /-----
typedef void (*walk_func)(Node *, size_t);

void walk(Node *node, walk_func wf, size_t level) {
    if (node->right != NULL) {
        walk(node->right, wf, level + 1);
    }
    wf(node, level);
    if (node->left != NULL) {
        walk(node->left, wf, level + 1);
    }
}

void print_elem(Node *node, size_t level) {
    for (size_t i = 0; i < level; ++i) {
        printf("  ");
    }
    printf("%s\n", node->key);
}
// -----/ END PRINT TREE /-----
