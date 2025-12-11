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
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "sampler.h"

/*
 * Get a field from the stat file
 *
 * @buf: Buffer to search within
 * @name: Name of field (e.g., "processes")
 *
 * Returns field value
 */
static size_t
stat_get_field(char *buf, const char *name)
{
    char *p, *p1;

    if (buf == NULL || name == NULL) {
        return 0;
    }

    if ((p = strstr(buf, name)) == NULL) {
        return 0;
    }

    p1 = p + strlen(name);
    return atoi(p1);
}

int
stat_pool_sample(struct stat_pool *pool, uint32_t count)
{
    static char buf[4096];

    if (pool == NULL || count == 0) {
        errno = -EINVAL;
        return -1;
    }

    if (count > MAX_SAMPLES) {
        count = MAX_SAMPLES;
    }

    for (uint32_t i = 0; i < count; ++i) {
        read(pool->fd, buf, sizeof(buf));
        lseek(pool->fd, 0, SEEK_SET);

        pool->pool[i] = stat_get_field(buf, "processes");
        usleep(SAMPLE_PERIOD);
    }

    pool->n_samples = count;
    return 0;
}

int
stat_pool_open(struct stat_pool *pool)
{
    if (pool == NULL) {
        errno = -EINVAL;
        return -1;
    }

    pool->n_samples = 0;
    pool->fd = open(STAT_FILE, O_RDONLY);
    if (pool->fd < 0) {
        perror("open");
        return errno;
    }

    return 0;
}

void
stat_pool_close(struct stat_pool *pool)
{
    if (pool == NULL) {
        return;
    }

    close(pool->fd);
}
