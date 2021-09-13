/* SPDX-License-Identifier: GPL-2.0-or-later */
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "scoro.h"

static bool scoro_work1_flag, scoro_work2_flag;

scoro_state_t scoro_work(struct scoro_work *work, void *data)
{
    int task_nr = (long)data;

    for (;;) {
        if (task_nr == 1) {
            scoro_work2_flag = true;
            scoro_wait(work, scoro_work1_flag);
            scoro_work1_flag = false;
        } else {
            scoro_work1_flag = true;
            scoro_wait(work, scoro_work2_flag);
            scoro_work2_flag = false;
        }

        printf("scoro work %d running\n", task_nr);
        usleep(200000);
    }

    return SCORO_BLOCK;
} 

int main(int argc, char *argv[])
{
    struct scoro_worker *worker;

    worker = scoro_worker_create(NULL, "worker");
    if (!worker)
        return 0;

    scoro_work_run(worker, scoro_work, (void *)1, "work1");
    scoro_work_run(worker, scoro_work, (void *)2, "work2");

    for (;; scoro_worker_loop(worker));
    return 0;
}
