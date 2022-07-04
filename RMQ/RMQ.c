#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// -----/ DEFINITION /-----

typedef int T;

typedef struct _Table {
    T **data;
    size_t N;
    size_t log_N;
    T *pows;
} Table;


// -----/ DECLARATION /-----

Table *create_table(T *arr, size_t N);
Table *free_table(Table *table, size_t N);
T max(T a, T b);
T RMQ(Table *table, size_t left, size_t right);

void print_table(Table *table);

T *scanf_arr(size_t N);
void do_commands(Table *table, size_t M);


// -----/ MAIN FUNCTIONS /-----

int main() {
    size_t N = 0, M = 0;
    scanf("%zd %zd", &N, &M);

    // -----/ MAKE ARRAY & TABLE /-----
    T *arr = scanf_arr(N);
    if (!arr) {
        return 1;
    }

    Table *table = create_table(arr, N);
    if (!table) {
        free(arr);
        return 1;
    }
    // ----------

    do_commands(table, M);

    // -----/ FREE MEMORY /-----
    free(arr);
    table = free_table(table, table->log_N);
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

void do_commands(Table *table, size_t M) {
    size_t L = 0, R = 0;

    while (M--) {
        scanf("%zd %zd", &L, &R);
        printf("%d\n", RMQ(table, L, R));
    }
}


// -----/ RMQ FUNCTIONS /-----

Table *create_table(T *arr, size_t N) {
    if (!arr) {
        return NULL;
    }
    
    Table *table = (Table *) malloc(sizeof(Table));
    if (!table) {
        return NULL;
    }

    table->pows = (T *) malloc((N + 1) * sizeof(T));
    if (!table->pows) {
        free(table);
        return NULL;
    }

    table->pows[1] = 0;
    for (size_t i = 2; i <= N; ++i) {
        table->pows[i] = table->pows[i >> 1] + 1;
    }

    table->N = N;
    table->log_N = table->pows[N] + 1;

    table->data = (T **) malloc(table->log_N * sizeof(T *));
    if (!table->data) {
        free(table->pows);
        free(table);
        return NULL;
    }

    for (size_t i = 0; i < table->log_N; ++i) {
        table->data[i] = (T *) calloc(table->N, sizeof(T));
        if (!table->data[i]) {
            table = free_table(table, i);
            return NULL;
        }
    }

    for (size_t i = 0; i < table->N; ++i) {
        table->data[0][i] = arr[i];
    }

    for (size_t level = 1; level < table->log_N; ++level) {
        for (size_t i = 0; i < table->N; ++i) {
            if (i + (1 << level) - 1 >= table->N) {
                break;
            }
            table->data[level][i] = max(table->data[level - 1][i],
                            table->data[level - 1][i + (1 << (level - 1))]);
        }
    }

    // print_table(table);

    return table;
}

Table *free_table(Table *table, size_t N) {
    if (!table) {
        return NULL;
    }

    for (size_t i = 0; i < N; ++i) {
        free(table->data[i]);
    }
    free(table->data);
    free(table->pows);
    free(table);
    return NULL;
}

T max(T a, T b) {
    return (a > b)? a : b;
}

T RMQ(Table *table, size_t left, size_t right) {
    if (!table) {
        return 1 << 31;
    }

    T curr_pow = table->pows[right - left + 1];
    return max(table->data[curr_pow][left],
               table->data[curr_pow][right - (1 << curr_pow) + 1]);
}

void print_table(Table *table) {
    if (!table) {
        return;
    }
    printf("N, log_N: %zd %zd\n", table->N, table->log_N);
    for (size_t level = 0; level < table->log_N; ++level) {
        for (size_t i = 0; i < table->N; ++i) {
            printf("%4d ", table->data[i][level]);
        }
        putchar('\n');
    }

    puts("Pows:");
    for (size_t i = 0; i < table->N; ++i) {
        printf("%d ", table->pows[i]);
    }
    putchar('\n');
}