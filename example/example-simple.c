/* SPDX-License-Identifier: GPL-2.0-or-later */
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "scoro.h"

static bool scoro_work1_flag, scoro_work2_flag;
static bool scoro_work3_flag, scoro_work4_flag;

scoro_state_t scoro_work1(struct scoro_work *work, void *data)
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

    return SCORO_EXITED;
}

scoro_state_t scoro_work2(struct scoro_work *work, void *data)
{
    int task_nr = (long)data;

    for (;;) {
        if (task_nr == 3) {
            scoro_work4_flag = true;
            scoro_wait(work, scoro_work3_flag);
            scoro_work3_flag = false;
        } else {
            scoro_work3_flag = true;
            scoro_wait(work, scoro_work4_flag);
            scoro_work4_flag = false;
        }

        printf("scoro work %d running\n", task_nr);
        usleep(200000);
    }

    return SCORO_EXITED;
}

int main(int argc, char *argv[])
{
    struct scoro_worker *worker1, *worker2;

    worker1 = scoro_worker_run(NULL, "worker1");
    if (!worker1)
        return 0;

    worker2 = scoro_worker_run(NULL, "worker2");
    if (!worker2)
        return 0;

    scoro_work_run(worker1, scoro_work1, (void *)1, "work1");
    scoro_work_run(worker1, scoro_work1, (void *)2, "work2");
    scoro_work_run(worker2, scoro_work2, (void *)3, "work3");
    scoro_work_run(worker2, scoro_work2, (void *)4, "work4");

    printf("worker1 works nr: %d\n", worker1->work_num);
    printf("worker2 works nr: %d\n", worker2->work_num);

    for (;; sleep(1));
    return 0;
}
