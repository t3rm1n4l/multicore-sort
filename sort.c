#include "sort.h"
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <math.h>

#define MIN_ITEMS_COUNT 50000
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

static void *sort_worker(void *arg) {
    sort_job_t *job = (sort_job_t *) arg;

    split(job->base, job->n, job->width, job->thunk, job->cmp);
    return NULL;
}

static void *split_worker(void *arg) {
    sort_job_t *job = (sort_job_t *) arg;

    qsort_r(job->base, job->n, job->width, job->thunk, job->cmp);
    return NULL;
}

int mc_sort(size_t cores, void *base, size_t n, size_t width,
                                                 void *thunk,
                                                 comparator_fn cmp) {
    int ret = 0;
    size_t nthreads;
    size_t i, j;
    size_t nsplits;
    size_t items_per_thread;
    pthread_t *threads = NULL;
    sort_job_t *jobs = NULL;

    assert(cores == 1 || cores % 2 == 0);

    if (n <= 0) {
        return 0;
    }

    nthreads = MIN(cores, MAX(1, n / MIN_ITEMS_COUNT));
    if (nthreads > 1) {
        threads = calloc(nthreads - 1, sizeof(pthread_t));
        jobs = calloc(nthreads - 1, sizeof(sort_job_t));
    }

    long unsigned t = time(NULL);
    for (i = 0; i < log2(cores); i++) {
        items_per_thread = n;
        nsplits = n / items_per_thread;
        for (j = 0; j < nsplits - 1; j++) {
            jobs[j].base = base + width * j * items_per_thread;
            jobs[j].n = items_per_thread;
            jobs[j].width = width;
            jobs[j].thunk = thunk;
            jobs[j].cmp = cmp;
            //split(base + j * chunksz * width, chunksz, width, thunk, cmp);
            if ((ret = pthread_create(&threads[j], NULL, &split_worker, (void *) &jobs[j])) < 0) {
                goto fail;
            }
        }

        split(base + width * j * items_per_thread, n - items_per_thread * (nsplits - 1), width, thunk, cmp);

        for (j = 0; j < nsplits - 1; j++) {
            pthread_join(threads[i], NULL);
        }

        items_per_thread /= 2;
    }

    printf("Time taken for split : %lus\n", time(NULL) - t);

    items_per_thread = n / nthreads;

    for (i = 0; i < nthreads - 1; i++) {
        jobs[i].base = base + width * i * items_per_thread;
        jobs[i].n = items_per_thread;
        jobs[i].width = width;
        jobs[i].thunk = thunk;
        jobs[i].cmp = cmp;

        if ((ret = pthread_create(&threads[i], NULL, &sort_worker, (void *) &jobs[i])) < 0) {
            goto fail;
        }
    }

    qsort_r(base + width * i * items_per_thread, n - items_per_thread * (nthreads - 1), width, thunk, cmp);

    for (i = 0; i < nthreads - 1; i++) {
        pthread_join(threads[i], NULL);
    }

fail:
    free(threads);
    free(jobs);
    return ret;
}
