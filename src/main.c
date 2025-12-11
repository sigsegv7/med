/*
 * Copyright (c) 2025 Ian Marco Moffett and the Osmora Team.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Hyra nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <math.h>
#include "sampler.h"

struct fork_stat {
    float mean;
    float stddev;
    float variance;
    float sigma;
};

static void
stat_compute(struct stat_pool *pool, struct fork_stat *res)
{
    float sum = 0;
    float x;

    if (pool == NULL || res == NULL) {
        return;
    }

    /* Reset all values */
    sum = 0;
    res->mean = 0;
    res->stddev = 0;

    /* Compute the mean */
    for (size_t i = 0; i < pool->n_samples; ++i) {
        sum += pool->pool[i];
    }

    res->mean = sum / pool->n_samples;
    sum = 0;

    /* Compute the standard deviation */
    for (size_t i = 0; i < pool->n_samples; ++i) {
        sum += pow(pool->pool[i] - res->mean, 2);
    }

    res->variance = sum / pool->n_samples;
    res->stddev = sqrt(res->variance);

    for (size_t i = 0; i < pool->n_samples; ++i) {
        x = (float)(pool->pool[pool->n_samples - 1]);
        x = (x - res->mean) / res->stddev;
        if (x > res->sigma) {
            res->sigma = x;
        }
    }
}

int
main(void)
{
    struct stat_pool pool;
    struct fork_stat stat;
    int error;

    /* Open a new stat pool */
    if ((error = stat_pool_open(&pool)) < 0) {
        printf("error: failed to open stat pool\n");
        return error;
    }

    for (int i = 0; i < 30; ++i) {
        if (stat_pool_sample(&pool, MAX_SAMPLES) < 0) {
            printf("error: failed to sample\n");
            stat_pool_close(&pool);
        }

        stat_compute(&pool, &stat);
        printf("[%d/30]: rate/σ=%f\n", i, stat.sigma);
        stat.sigma = 0;
    }

    stat_pool_close(&pool);
    return 0;
}
