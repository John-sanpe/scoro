/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include "scoro.h"

static LIST_HEAD(worker_list);

void scoro_work_relax(struct scoro_work *work)
{
    struct scoro_worker *worker = work->worker;
    list_move_tail(&worker->work_list, &work->list);
}

void scoro_work_enqueue(struct scoro_worker *worker, struct scoro_work *work)
{
    work->worker = worker;
    worker->work_num++;
    list_add_prev(&worker->work_list, &work->list);
}

void scoro_work_dequeue(struct scoro_work *work)
{
    struct scoro_worker *worker = work->worker;

    work->worker = NULL;
    worker->work_num--;
    list_del(&work->list);
}

static void scoro_do_work(struct scoro_worker *worker, struct scoro_work *work)
{
    scoro_state_t retval;
    void *priv = work->data;

    retval = work->entry(work, priv);

    switch (retval) {
        case SCORO_EXITED:
            scoro_work_dequeue(work);
            break;
        case SCORO_BLOCK:
            break;
        default:
            scoro_debug("scoro work return unknow value\n");
    }
}

void *scoro_worker_loop(void *data)
{
    struct scoro_worker *worker = data;
    struct scoro_work *work;

    for (;;)
    list_for_each_entry(work, &worker->work_list, list)
        scoro_do_work(worker, work);
}

struct scoro_worker *scoro_worker_best(void)
{
    struct scoro_worker *walk, *best = NULL;

    list_for_each_entry(walk, &worker_list, list)
        if (!best || best->work_num > walk->work_num)
            best = walk;

    return best;
}

int scoro_worker_wakeup(struct scoro_worker *worker, scoro_pth_flag *flags)
{
    return scoro_pthread_create(&worker->pth, flags, scoro_worker_loop, worker);
}

void scoro_worker_suspend(struct scoro_worker *worker)
{
    scoro_pthread_cancel(worker->pth);
    worker->pth = 0;
}

struct scoro_work *scoro_work_create(scoro_fun_t fun, void *data, const char *name, ...)
{
    struct scoro_work *work;

    work = scoro_zalloc(sizeof(*work));
    if (!work)
        return NULL;

    work->name = name;
    work->entry = fun;
    work->data = data;
    return work;
}

void scoro_work_remove(struct scoro_work *work)
{
    if (work->worker)
        scoro_work_dequeue(work);
    scoro_free(work);
}

struct scoro_worker *scoro_worker_create(const char *name, ...)
{
    struct scoro_worker *worker;

    worker = scoro_zalloc(sizeof(*worker));
    if (!worker)
        return NULL;

    worker->name = name;

    list_add_prev(&worker_list, &worker->list);
    list_head_init(&worker->work_list);
    return worker;
}

void scoro_worker_remove(struct scoro_worker *worker)
{
    if (worker->pth)
        scoro_worker_suspend(worker);
    scoro_free(worker);
    list_del(&worker->list);
}
