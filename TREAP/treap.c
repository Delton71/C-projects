#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// -----/ DEFINITION /-----

typedef int  P;         // prior
typedef char K[31];     // name
typedef int  V;         // val


// -----/ DECLARATION /-----

typedef struct _Node {
    size_t size;
    P prior;
    K name;
    V val;
    struct _Node * child[2];
} Node;

Node  *create_node(K name, V val);
size_t get_size(Node *node);
void   set_size(Node *node);
Node  *merge(Node *first, Node *second);
void   split(Node *node, Node **res_l, Node **res_r, size_t pos);
Node  *insert(Node *node, size_t pos, K name, V val);
Node  *erase(Node *node, size_t pos);
Node  *find(Node *node, size_t pos);
void   print_treap(Node *node, size_t level);
void   delete_node(Node *node);

Node *do_insert(size_t N);
Node *do_commands(Node *root, size_t M);


// -----/ MAIN FUNCTION /-----

int main() {
    srand(time(NULL));      // to get random values

    size_t N = 0, M = 0;
    scanf("%zd %zd", &N, &M);

    Node *root = do_insert(N);
    root = do_commands(root, M);

    delete_node(root);
    return 0;
}

Node *do_insert(size_t N) {
    Node *root = NULL;
    K name = {0};
    V val = 0;
    for (size_t i = 1; i <= N; ++i) {
        scanf("%31s %d", name, &val);
        root = insert(root, i, name, val);
    }
    return root;
}

Node *do_commands(Node *root, size_t M) {
    int comm = 0;
    size_t pos = 0;
    K name = {0};
    V val = 0;
    while (M--) {
        scanf("%d", &comm);
        switch(comm) {
            case 0:
                scanf("%zd %31s %d", &pos, name, &val);
                root = insert(root, pos, name, val);
                break;
            case 1:
                scanf("%zd", &pos);
                root = erase(root, pos);
                break;
            case 2:
                scanf("%zd", &pos);
                root = find(root, pos);
                break;
            default:
                puts("Error in do_commands!!!");
                return root;
        }
    }

    return root;
}


// -----/ TREAP FUNCTIONS /-----

// Creaate node: size = 1; proir = random num; name & val are given
Node *create_node(K name, V val) {
    Node *new_node = (Node *) calloc(1, sizeof(Node));
    if (!new_node) {
        return NULL;
    }

    new_node->size = 1;
    new_node->prior = rand();
    memcpy(new_node->name, name, 31);
    new_node->val = val;

    return new_node;
}

// Return node size
size_t get_size(Node *node) {
    return (node)? node->size : 0;
}

// Set size for the node by childs size
void set_size(Node *node) {
    if (!node) {
        return;
    }
    node->size = get_size(node->child[0]) + get_size(node->child[1]) + 1;
}

// Merge nodes by the proir
Node *merge(Node *first, Node *second) {
    if (!first) {
        return second;
    }
    if (!second) {
        return first;
    }

    if (first->prior > second->prior) {
        first->child[1] = merge(first->child[1], second);
        set_size(first);
        return first;
    } // else

    second->child[0] = merge(first, second->child[0]);
    set_size(second);
    return second;
}

// Split tree: left consist all nodes <= pos; right consist all nodes > pos
void split(Node *node, Node **res_l, Node **res_r, size_t pos) {
    if (!node) {
        *res_l = *res_r = NULL;
        return;
    }

    Node *left = NULL;
    Node *right = NULL;
    if (get_size(node->child[0]) + 1 <= pos) {
        split(node->child[1], &left, &right, pos - get_size(node->child[0]) - 1);
        node->child[1] = left;
        set_size(node);
        *res_l = node;
        *res_r = right;
    } else {
        split(node->child[0], &left, &right, pos);
        node->child[0] = right;
        set_size(node);
        *res_l = left;
        *res_r = node;
    }
}

// Insert node by the pos
Node *insert(Node *node, size_t pos, K name, V val) {
    Node *new_node = create_node(name, val);
    if (!node) {
        return new_node;
    }
    if (!new_node) {
        return node;
    }

    Node *left = NULL;
    Node *right = NULL;
    split(node, &left, &right, pos - 1);
    return merge(left, merge(new_node, right));
}

// Erase node by the pos
Node *erase(Node *node, size_t pos) {
    Node *left = NULL;
    Node *mid = NULL;
    Node *right = NULL;
    
    split(node, &mid, &right, pos);
    split(mid, &left, &mid, pos - 1);

    delete_node(mid);
    return merge(left, right);
}

// Find node by the pos
Node *find(Node *node, size_t pos) {
    Node *left = NULL;
    Node *mid = NULL;
    Node *right = NULL;
    
    split(node, &mid, &right, pos);
    split(mid, &left, &mid, pos - 1);

    printf("%s %d\n", mid->name, mid->val);
    return merge(merge(left, mid), right);
}

// Print treap
void print_treap(Node *node, size_t level) {
    if (!node) {
        return;
    }

    print_treap(node->child[1], level + 1);
    
    for (size_t i = 0; i < level; ++i) {
        printf("  ");
    }
    printf("(%d; %s; %d; %zd)\n", node->prior, node->name, node->val, node->size);
    
    print_treap(node->child[0], level + 1);
}

// Free treap memory
void delete_node(Node *node) {
    if (!node) {
        return;
    }
    delete_node(node->child[0]);
    delete_node(node->child[1]);
    free(node);
}