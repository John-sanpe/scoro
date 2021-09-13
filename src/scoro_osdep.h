/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#ifndef _SCORO_OSDEP_H_
#define _SCORO_OSDEP_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

#define scoro_pth       pthread_t
#define scoro_pth_flag  pthread_attr_t

static inline int scoro_debug(const char *fmt, ...)
{
    va_list args;
    int length;

    va_start(args, fmt);
    length = vfprintf(stderr, fmt, args);
    va_end(args);

    return length;
}

static inline void *scoro_alloc(size_t size)
{
    return malloc(size);
}

static inline void *scoro_zalloc(size_t size)
{
    void *block;

    block = scoro_alloc(size);
    if (!block)
        return NULL;

    memset(block, 0, size);
    return block;
}

static inline void scoro_free(void *block)
{
    return free(block);
}

static inline int scoro_pthread_create(scoro_pth *pth, scoro_pth_flag* flags,
                                       void *(*fun)(void *), void *args)
{
    return pthread_create(pth, flags, fun, args);
}

#endif  /* _SCORO_OSDEP_H_ */
