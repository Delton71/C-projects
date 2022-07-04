#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// -----/ DEFINITION /-----

typedef int T;


// -----/ DECLARATION /-----

// tree[ind][0] - XOR; tree[ind][1] - MAX
T *make_tree(size_t tree_N);
void build(size_t root, T *arr, T *tree_xor, T *tree_max, size_t left, size_t right);
void update(size_t root, size_t need_ind, T *tree_xor, T *tree_max, T new_data, size_t left, size_t right);
T get_xor(size_t root, T *tree_xor, size_t left, size_t right, size_t need_l, size_t need_r);
T get_max(size_t root, T *tree_max, size_t left, size_t right, size_t need_l, size_t need_r);
T min(T a, T b);
T max(T a, T b);

T *scanf_arr(size_t N);
void do_commands(T *tree_xor, T *tree_max, size_t N, size_t M);

void walk(size_t root, T *tree, size_t N, size_t level);
void print_elem(T elem, size_t level);


// -----/ MAIN FUNCTIONS /-----

int main() {
    size_t N = 0, M = 0;
    scanf("%zd %zd", &N, &M);

    // -----/ MAKE ARRAY & TREES /-----
    T *arr = scanf_arr(N);
    if (!arr) {
        return 1;
    }

    size_t tree_N = N << 2; // (size_t) (ceil(N * log2(N)) + 1);
    T *tree_xor = make_tree(tree_N);
    if (!tree_xor) {
        free(arr);
        return 1;
    }
    T *tree_max = make_tree(tree_N);
    if (!tree_max) {
        free(arr);
        free(tree_xor);
        return 1;
    }
    // ----------


    build(0, arr, tree_xor, tree_max, 0, N);
    do_commands(tree_xor, tree_max, N, M);

    // -----/ FREE MEMORY /-----
    free(arr);
    free(tree_xor);
    free(tree_max);
    return 0;
}

T *scanf_arr(size_t N) {
    T *arr = (T *) malloc(N * sizeof(T));
    if (!arr) {
        return NULL;
    }

    for (size_t i = 0; i < N; ++i) {
        scanf("%d", arr + i);
    }

    return arr;
}

void do_commands(T *tree_xor, T *tree_max, size_t N, size_t M) {
    int comm = 0;
    size_t L = 0, R = 0;
    while (M--) {
        scanf("%d %zd %zd", &comm, &L, &R);

        switch(comm) {
            case 1:       // XOR
                printf("%d\n", get_xor(0, tree_xor, 0, N, L, R + 1));
                break;
            case 2:       // MAX
                printf("%d\n", get_max(0, tree_max, 0, N, L, R + 1));
                break;
            case 3:       // CHANGE ELEM
                update(0, L, tree_xor, tree_max, R, 0, N);
                /*
                puts("-----/ XOR /-----");
                walk(0, tree_xor, N, 0);
                puts("-----/ MAX /-----");
                walk(0, tree_max, N, 0);
                puts("----------");
                */
                break;
            default:
                puts("Error in do_commands!");
                return;
        }
    }
}


// -----/ REQUESTS FUNCTIONS /-----

T *make_tree(size_t tree_N) {
    T *tree = (T *) calloc(tree_N, sizeof(T));
    if (!tree) {
        return NULL;
    }

    return tree;
}

void build(size_t root, T *arr, T *tree_xor, T *tree_max, size_t left, size_t right) {
    if (right == left + 1) {
        tree_xor[root] = arr[left];
        tree_max[root] = arr[left];
    } else {
        size_t mid = left + ((right - left) >> 1);
        build((root << 1) + 1, arr, tree_xor, tree_max, left, mid);
        build((root << 1) + 2, arr, tree_xor, tree_max, mid, right);

        tree_xor[root] = tree_xor[(root << 1) + 1] ^ tree_xor[(root << 1) + 2];
        tree_max[root] = max(tree_max[(root << 1) + 1], tree_max[(root << 1) + 2]);
    }
}

void update(size_t root, size_t need_ind, T *tree_xor, T *tree_max, T new_data, size_t left, size_t right) {
    if (right == left + 1) {
        tree_xor[root] = new_data;
        tree_max[root] = new_data;
    } else {
        size_t mid = left + ((right - left) >> 1);
        if (need_ind < mid) {
            update((root << 1) + 1, need_ind, tree_xor, tree_max, new_data, left, mid);
        } else {
            update((root << 1) + 2, need_ind, tree_xor, tree_max, new_data, mid, right);
        }

        tree_xor[root] = tree_xor[(root << 1) + 1] ^ tree_xor[(root << 1) + 2];
        tree_max[root] = max(tree_max[(root << 1) + 1], tree_max[(root << 1) + 2]);
    }
}

T get_xor(size_t root, T *tree_xor, size_t left, size_t right, size_t need_l, size_t need_r) {
    if (need_l >= need_r) {
        // printf("! 1: %d %zd %zd %zd %zd %zd\n", 0, root, left, right, need_l, need_r);
        return 0;
    }
    size_t mid = left + ((right - left) >> 1);
    if (left == need_l && right == need_r) {
        // printf("! 2: %d %zd %zd %zd %zd %zd\n", tree_xor[root], root, left, right, need_l, need_r);
        return tree_xor[root];
    }
    // printf("! 3: %zd %zd\n", (root << 1) + 1, (root << 1) + 2);
    return get_xor((root << 1) + 1, tree_xor, left, mid, need_l, min(need_r, mid)) ^
           get_xor((root << 1) + 2, tree_xor, mid, right, max(mid, need_l), need_r);
}

T get_max(size_t root, T *tree_max, size_t left, size_t right, size_t need_l, size_t need_r) {
    if (need_l >= need_r) {
        // printf("! 1: %d %zd %zd %zd %zd %zd\n", 1 << 31, root, left, right, need_l, need_r);
        return 1 << 31;
    }
    size_t mid = left + ((right - left) >> 1);
    if (left >= need_l && right <= need_r) {
        // printf("! 2: %d %zd %zd %zd %zd %zd\n", tree_max[root], root, left, right, need_l, need_r);
        return tree_max[root];
    }
    // printf("! 3: %zd %zd\n", (root << 1) + 1, (root << 1) + 2);
    return max(get_max((root << 1) + 1, tree_max, left, mid, need_l, min(need_r, mid)),
           get_max((root << 1) + 2, tree_max, mid, right, max(mid, need_l), need_r));
}

T min(T a, T b) {
    return (a < b)? a : b;
}

T max(T a, T b) {
    return (a > b)? a : b;
}

// -----/ PRINT TREE /-----

void walk(size_t root, T *tree, size_t N, size_t level) {
    // printf("root, level, N, elem: %zd %zd %zd %d\n", root, level, N, tree[root]);
    // printf("%zd %zd\n", (root << 1) + 2, (root << 1) + 1);
    // getchar();
    if (tree[root]) {
        if ((root << 1) + 2 < N) {
            walk((root << 1) + 2, tree, N, level + 1);
        }
        print_elem(tree[root], level);
        if ((root << 1) + 1 < N) {
            walk((root << 1) + 1, tree, N, level + 1);
        }
    }
}

void print_elem(T elem, size_t level) {
    for (size_t i = 0; i < level; ++i) {
        printf("  ");
    }
    printf("%d\n", elem);
}
