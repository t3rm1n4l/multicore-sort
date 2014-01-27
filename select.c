#include <stdlib.h>
#include <string.h>
#include "sort.h"

static void swap(void *x, void *y, size_t width) {
    int *a = x, *b = y;
    if (x != y) {
        *a = *a ^ *b;
        *b = *a ^ *b;
        *a = *a ^ *b;
    }
    /*void *tmp = malloc(width);*/
    /*memcpy(tmp, x, width);*/
    /*memcpy(x, y, width);*/
    /*memcpy(y, tmp, width);*/
    /*free(tmp);*/
}

static int paritition(void *items, int lo, int hi, size_t width,
                                                   void *thunk,
                                                   comparator_fn cmp) {
    int i = lo, j = hi + 1;
    int pivot = rand() % (hi - lo + 1);

    while (1) {
        while (cmp(thunk, items + i++ * width, items + pivot * width) < 0) {
            if (i == hi) break;
        }

        while (cmp(thunk, items + --j * width, items + pivot * width) > 0) {
            if (j == lo) break;
        }

        if (i >= j) break;

        swap(items + i * width, items + j * width, width);
    }

    swap(items + lo * width, items + j * width, width);

    return j;
}

static void select(void *items, int i, int j, size_t width,
                                              void *thunk,
                                              comparator_fn cmp, int k) {
    int pivot;

    if (i >= j) {
        return;
    }

    pivot = paritition(items, i, j, width, thunk, cmp);

    if (k < pivot) {
        select(items, i, pivot - 1, width, thunk, cmp, k);
    } else {
        select(items, pivot + 1, j, width, thunk, cmp, k - pivot - 1);
    }
}

void split(void *items, size_t n, size_t width, void *thunk, comparator_fn cmp) {
    select(items, 0, n - 1, width, thunk, cmp, n / 2);

    /*int i,j = n-1;*/
    /*for (i=0; i <n; i++,j--) {*/
        /*swap(items+i*width, items+j*width,width);*/
    /*}*/
}
