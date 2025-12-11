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

#ifndef SAMPLER_H
#define SAMPLER_H

#include <stdint.h>
#include <stddef.h>

#define STAT_FILE "/proc/stat"
#define MAX_SAMPLES 128
#define SAMPLE_PERIOD 9000      /* usec */

/*
 * Pool of stats
 *
 * @pool: Each entry is the number of forks at a given time
 * @n_samples: Number of samples taken
 * @fd: File descriptor to stat file
 */
struct stat_pool {
    size_t pool[MAX_SAMPLES];
    uint32_t n_samples;
    int fd;
};

/*
 * Begin making samples
 *
 * @pool: Pool to sample within
 * @count: Number of samples to make
 *
 * Returns zero on success
 */
int stat_pool_sample(struct stat_pool *pool, uint32_t count);

/*
 * Open a stat pool
 *
 * @pool: Pool descriptor
 *
 * Returns zero on success
 */
int stat_pool_open(struct stat_pool *pool);

/*
 * Close a stat pool
 *
 * @pool: Pool descriptor to close
 *
 */
void stat_pool_close(struct stat_pool *pool);

#endif  /* SAMPLER_H */
