/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#ifndef _SCORO_H_
#define _SCORO_H_

#include "scoro_osdep.h"
#include "list.h"

struct scoro_work;

typedef enum scoro_state {
    SCORO_EXITED,
    SCORO_BLOCK,
} scoro_state_t;

typedef scoro_state_t (*scoro_fun_t)(struct scoro_work *, void *);

struct scoro_work {
    const char *name;
    struct scoro_worker *worker;
    struct list_head list;
    scoro_fun_t entry;
    void *data;
};

struct scoro_worker {
    const char *name;
    scoro_pth   pth;
    unsigned int work_num;
    struct list_head work_list;
    struct list_head list;
};

void scoro_work_relax(struct scoro_work *worker);
void scoro_work_enqueue(struct scoro_worker *worker, struct scoro_work *work);
void scoro_work_dequeue(struct scoro_work *work);
struct scoro_work *scoro_work_create(scoro_fun_t fun, void *data, const char *name, ...);
void scoro_work_remove(struct scoro_work *work);

void *scoro_worker_loop(void *data);
struct scoro_worker *scoro_worker_best(void);
int scoro_worker_wakeup(struct scoro_worker *worker, scoro_pth_flag *flags);
void scoro_worker_suspend(struct scoro_worker *worker);
struct scoro_worker *scoro_worker_create(const char *name, ...);
void scoro_worker_remove(struct scoro_worker *worker);

#define scoro_wait(work, condition) ({                          \
    bool val = !!(condition);                                   \
    if (!val)                                                   \
        return SCORO_BLOCK;                                     \
    val;                                                        \
})

#define scoro_work_auto_enqueue(work) ({                        \
    struct scoro_worker *best_worker;                           \
    best_worker = scoro_worker_best();                          \
    if (best_worker)                                            \
        scoro_work_enqueue(best_worker, work);                  \
    best_worker;                                                \
})

#define scoro_work_run(worker, fun, data, name, ...) ({         \
    struct scoro_work *work;                                    \
    work = scoro_work_create(fun, data, name, ##__VA_ARGS__);   \
    if (work)                                                   \
        scoro_work_enqueue(worker, work);                       \
    work;                                                       \
})

#define scoro_work_auto_run(fun, data, name, ...) ({            \
    struct scoro_work *work;                                    \
    work = scoro_work_create(fun, data, name, ##__VA_ARGS__);   \
    if (!work)                                                  \
        NULL;                                                   \
    if (!scoro_work_auto_enqueue(work)) {                       \
        scoro_work_remove(work);                                \
        NULL;                                                   \
    }                                                           \
    work;                                                       \
})

#define scoro_worker_run(flags, name, ...) ({                   \
    struct scoro_worker *worker;                                \
    worker = scoro_worker_create(name, ##__VA_ARGS__);          \
    if (!worker || scoro_worker_wakeup(worker, flags))          \
        NULL;                                                   \
    worker;                                                     \
})

#endif  /* _SCORO_H_ */
