#ifndef _MULTICORE_SORT_H
#define _MULTICORE_SORT_H

#include <stddef.h>

typedef int (*comparator_fn)(void *, const void *, const void *);

typedef struct {
    void *base;
    size_t n;
    size_t width;
    void *thunk;
    comparator_fn cmp;
} sort_job_t;

int mc_sort(size_t cores, void *base, size_t n, size_t width,
                                                 void *thunk,
                                                 comparator_fn cmp);

#endif
