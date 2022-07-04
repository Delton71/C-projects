#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include <time.h>

static void print_int(void const *st) {
	printf("%d", *(int *)st);
}

int main() {
	struct vector *v = vector_new(32, sizeof(int));
	vector_resize(v, 0);
	vector_print(v, print_int);
	for (int i = 0; i < 100; ++i) {
		vector_set(v, i, &i);
		vector_print(v, print_int);
		printf("Cap, size: %ld, %ld\n", v->capacity, v->size);
	}
	int elem = 5;
	printf("Cap, size: %ld, %ld\n", v->capacity, v->size);
	vector_push(v, &elem);
	vector_print(v, print_int);
	printf("Cap, size: %ld, %ld\n", v->capacity, v->size);

	for (int i = 0; i < 42; ++i) {
		vector_pop(v, &elem);
		vector_print(v, print_int);
		printf("Cap, size: %ld, %ld\n", v->capacity, v->size);
	}
	
	vector_resize(v, 0);
	vector_resize(v, 0);
	vector_resize(v, 0);
	printf("Cap, size: %ld, %ld\n", v->capacity, v->size);
	vector_pop(v, &elem);
	printf("Cap, size: %ld, %ld\n", v->capacity, v->size);
	vector_get(v, 0, &elem);
	printf("Cap, size: %ld, %ld\n", v->capacity, v->size);
	vector_set(v, 0, &elem);
	printf("Cap, size: %ld, %ld\n", v->capacity, v->size);
	vector_print(v, print_int);
	vector_push(v, &elem);
	printf("Cap, size: %ld, %ld\n", v->capacity, v->size);
	vector_print(v, print_int);
	printf("Size: %ld\n", vector_size(v));
	vector_pop(v, &elem);
	vector_pop(v, &elem);
	vector_pop(v, &elem);
	printf("Cap, size: %ld, %ld\n", v->capacity, v->size);
	vector_print(v, print_int);
	printf("Size: %ld\n", vector_size(v));
	
	v = vector_delete(v);
	return 0;
}


/*
int main() {
	clock_t start, end;
	start = clock();
	
	// int elem = 0;
	struct vector *v = vector_new(0, sizeof (int));
	vector_print(v, print_int);
	
	int p1 = 1, p2, p3 = 3, p4 = 4, p5 = 5;
	int elem = 0;
	vector_pop(v, &elem);
	printf("After pop: %d\n", elem);
	vector_push(v, &p1);
	vector_push(v, &p2);
	vector_push(v, &p3);
	vector_pop(v, &elem);
	printf("After pop: %d\n", elem);
	vector_push(v, &p4);
	vector_push(v, &p5);
	vector_print(v, print_int);
	vector_pop(v, &elem);
	for (int i = 0; i < 10; ++i) {
		printf("After pop: %d\n", elem);
		vector_pop(v, &elem);
	}

	v = vector_delete(v);
	end = clock();
	printf("Time: %.20Lf\n", (long double) (end - start) / CLOCKS_PER_SEC);
	return 0;
}
*/
