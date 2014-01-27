#include <stdio.h>
#include <stdlib.h>
#include "sort.h"
#include <time.h>

static int *gen_workload(size_t n) {
    int i = 0;
    int *load = calloc(n, sizeof(int));

    srand(10876430);
    for (i = 0; i < n; i++) {
        load[i] = rand();
    }

    return load;
}


static void print(int *load, size_t n) {
    size_t i;
    for (i = 0; i < n; i++) {
        printf("%d\n", load[i]);
    }
}

static int cmp(void *thunk, const void *a, const void *b) {
    return *((int *) a) - *((int *) b);
}


int main(int argc, char **argv) {

    int *load;
    long unsigned  t;
    size_t i, j, chunksz;
    size_t n, ncores;
    if (argc != 3) {
        fprintf(stderr, "Usage: %s ncores items\n", argv[0]);
        exit(1);
    }

    ncores = atoi(argv[1]);
    n = atoi(argv[2]);

    load = gen_workload(n);

    /*printf("Unsorted:\n");*/
    /*print(load, n);*/




    t = time(NULL);
    mc_sort(ncores, load, n, sizeof(int), NULL, cmp);
    printf("Time taken for sorting: %lu s\n", time(NULL) - t);

    /*printf("Sorted:\n");*/
    /*print(load, n);*/

    return 0;
}
