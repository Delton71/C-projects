#include <stdio.h>
#include <stdlib.h>

// -----/ DEFINITION /-----

typedef int T;
const int MOD = 1000000000;         // 10^9


// -----/ DECLARATION /-----

typedef struct _Node {
    T              key;
    long long      sum;
    struct _Node * child[2];
    struct _Node * parent;
} Node;

Node *create(T key);
long long update_sum(Node *node);
void rotate_left(Node *v);
void rotate_right(Node *v);
Node **p(Node *v);
Node **g(Node *v);
Node *search(Node *node, T key);
void splay(Node *v);
void split(Node *node, Node **res_left, Node **res_right, T key);
Node *max(Node *node);
Node *merge(Node *first, Node *second);
long long sum_range(Node **node, T min, T max);
Node *insert(Node *node, T key);
Node *delete_splay_tree(Node *node);

void print_splay_tree(Node *node, size_t level);

void splay_commands();


// -----/ MAIN FUNCTIONS /-----

int main() {
    splay_commands();
    return 0;
}

void splay_commands() {
    Node *node = NULL;

    size_t N = 0;
    scanf("%zd", &N);
    char comm[2] = {0};
    char prev = '\0';
    T key = 0;
    T min = 0, max = 0;
    long long sum = 0;

    while (N--) {
        scanf("%2s", comm);
        switch(comm[0]) {
            case '+':
                scanf("%d", &key);
                if (prev != '?') {
                    node = insert(node, key);
                } else {
                    sum += key;
                    node = insert(node, (T) (sum % MOD));
                }
                break;
            case '?':
                scanf("%d %d", &min, &max);
                sum = sum_range(&node, min, max);
                printf("%lld\n", sum);
                break;
            default:
                puts("Error in getting command (+/?)!!!");
                node = delete_splay_tree(node);
                return;
        }
        prev = comm[0];
    }

    node = delete_splay_tree(node);
}


// -----/ SPLAY TREE /-----

Node *create(T key) {
    Node *node = (Node *) calloc(1, sizeof(Node));
    if (!node) {
        return NULL;
    }

    node->key = key;
    node->sum = key;
    return node;
}

long long update_sum(Node *node) {
    long long sum = node->key;
    if (node->child[0]) {
        sum += node->child[0]->sum;
    }
    if (node->child[1]) {
        sum += node->child[1]->sum;
    }
    return sum;
}

void rotate_left(Node *v) {
    Node *par = *p(v);
    Node *right = v->child[1];
    if (par) {
        par->child[par->child[1] == v] = right;
    }

    Node *temp = right->child[0];
    right->child[0] = v;
    v->child[1] = temp;
    *p(v) = right;
    *p(right) = par;
    if (v->child[1]) {
        *p(v->child[1]) = v;
    }
    v->sum = update_sum(v);
    right->sum = update_sum(right);
}

void rotate_right(Node *v) {
    Node *par = *p(v);
    Node *left = v->child[0];
    if (par) {
        par->child[par->child[1] == v] = left;
    }

    Node *temp = left->child[1];
    left->child[1] = v;
    v->child[0] = temp;
    *p(v) = left;
    *p(left) = par;
    if (v->child[0]) {
        *p(v->child[0]) = v;
    }
    v->sum = update_sum(v);
    left->sum = update_sum(left);
}

Node **p(Node *v) {
    if (!v) {
        return NULL;
    }
    return &v->parent;
}

Node **g(Node *v) {
    return p(*p(v));
}

Node *search(Node *node, T key) {
    if (!node) {
        return NULL;
    }
    Node *curr = node;
    Node *prev = curr;
    while (curr && key != curr->key) {
        prev = curr;
        curr = curr->child[key > curr->key];
    }

    if (curr) {
        return curr;
    }
    return prev;
}

void splay(Node *v) {
    if (!v) {
        return;
    }

    while (*p(v) != NULL) {
        if (v == (*p(v))->child[0]) {
            if (*g(v) == NULL) {                      // zig
                rotate_right(*p(v));
            } else if (*p(v) == (*g(v))->child[0]) {  // zig-zig
                rotate_right(*g(v));
                rotate_right(*p(v));
            } else {                                  // zig-zag
                rotate_right(*p(v));
                rotate_left(*p(v));
            }
        } else {
            if (*g(v) == NULL) {                      // zag
                rotate_left(*p(v));
            } else if (*p(v) == (*g(v))->child[1]) {  // zag-zag
                rotate_left(*g(v));
                rotate_left(*p(v));
            } else {                                  // zag-zig
                rotate_left(*p(v));
                rotate_right(*p(v));
            }
        }
    }
}

void split(Node *node, Node **res_left, Node **res_right, T key) {
    if (!node) {
        *res_left = *res_right = NULL;
        return;
    }

    Node *curr = search(node, key);
    splay(curr);

    if (curr->key >= key) {
        *res_left = curr->child[0];
        *res_right = curr;
        (*res_right)->child[0] = NULL;

        if (*res_left) {
            (*res_left)->parent = NULL;
            (*res_right)->sum -= (*res_left)->sum;
        }
    } else {
        *res_right = curr->child[1];
        *res_left = curr;
        (*res_left)->child[1] = NULL;

        if (*res_right) {
            (*res_right)->parent = NULL;
            (*res_left)->sum -= (*res_right)->sum;
        }
    }
}

Node *max(Node *node) {
    Node *curr = node;
    while (curr->child[1]) {
        curr = curr->child[1];
    }
    return curr;
}

Node *merge(Node *first, Node *second) {
    if (!first) {
        return second;
    }
    if (!second) {
        return first;
    }
    
    Node *temp = max(first);
    splay(temp);

    temp->child[1] = second;
    second->parent = temp;
    temp->sum = update_sum(temp);

    return temp;
}

Node *insert(Node *node, T key) {
    if (!node) {
        return create(key);
    }
    
    Node *left = NULL;
    Node *right = NULL;
    split(node, &left, &right, key);

    if (right && right->key == key) {
        return merge(left, right);
    }

    Node *new_node = create(key);
    if (!new_node) {
        return node;
    }

    new_node->child[0] = left;
    new_node->child[1] = right;
    
    if (left) {
        new_node->child[0]->parent = new_node;
        new_node->sum += left->sum;
    }
    if (right) {
        new_node->child[1]->parent = new_node;
        new_node->sum += right->sum;
    }

    return new_node;
}

long long sum_range(Node **node, T min, T max) {
    if (!(*node)) {
        return 0;
    }

    Node *left  = NULL;
    Node *mid   = NULL;
    Node *right = NULL;

    split(*node, &left, &mid, min);
    split(mid, &mid, &right, max + 1);
    
    long long ans = 0;
    if (mid) {
        ans = mid->sum;
    }

    *node = merge(left, merge(mid, right));

    return ans;
}

Node *delete_splay_tree(Node *node) {
    if (node) {
        if (node->child[0]) {
            node->child[0] = delete_splay_tree(node->child[0]);
        }
        if (node->child[1]) {
            node->child[1] = delete_splay_tree(node->child[1]);
        }
        free(node);
    }
    return NULL;
}

void print_splay_tree(Node *node, size_t level) {
    if (node != NULL) {
        print_splay_tree(node->child[1], level + 1);

        for (size_t i = 0; i < level; ++i) {
            printf("  ");
        }
        printf("%d\n", node->key);
        
        print_splay_tree(node->child[0], level + 1);
    }
}