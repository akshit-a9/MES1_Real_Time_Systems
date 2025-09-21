
#include <stdio.h>
#include "scheduler.h"
#include "xil_printf.h"
#include "task.h"

#define TICK_SEC 1
#define TICK_US  1000000


static tcb tasks[] = {
    { .id=1, .period=5,  .wcet=1, .next_release=0, .remaining=0, .ready=0 }, // T1(5,1)
    { .id=2, .period=10, .wcet=2, .next_release=0, .remaining=0, .ready=0 }, // T2(10,2)
    { .id=3, .period=15, .wcet=3, .next_release=0, .remaining=0, .ready=0 }, // T3(15,3)
};
static const int N = sizeof(tasks)/sizeof(tasks[0]);

static unsigned now = 0;

static int pick_task(void) {
    int chosen = -1;
    unsigned best_period = 0xFFFFFFFF;  //just something absurdly large :)

    for (int i = 0; i < N; ++i) {
        if (tasks[i].ready && tasks[i].remaining > 0) {
            if (tasks[i].period < best_period) {
                best_period = tasks[i].period;
                chosen = i;
            }
        }
    }
    return chosen; //THE CHOSEN ONEEEE
}

static void print_finish(unsigned id, unsigned finish_time_s) {
    xil_printf("Hello from Task %u at time %u s\n\r", id, finish_time_s);
}

void* rm_scheduler_cb(void *unused) {
    for (int i = 0; i < N; ++i) {
        while ((int)(now - tasks[i].next_release) >= 0) {
//			GPT GAVE THIS BACKUP CODE FOR TESTING - IN CASE THE SYSTEM IS NOT SCHEDULABLE
//            if (tasks[i].ready && tasks[i].remaining > 0) {
//                xil_printf("Deadline miss: Task %u at %u s\n\r", tasks[i].id, now);
//            }
            tasks[i].ready     = 1;
            tasks[i].remaining = (int)tasks[i].wcet;
            tasks[i].next_release += tasks[i].period;
        }
    }


    int k = pick_task();
    if (k >= 0) {
        int will_finish = (tasks[k].remaining <= TICK_SEC);
        tasks[k].remaining -= TICK_SEC;
        if (will_finish) {
            tasks[k].remaining = 0;
            tasks[k].ready = 0;
            print_finish(tasks[k].id, now + TICK_SEC);
        }
    }

    now += TICK_SEC;
    loadTimer(TICK_US);
    return NULL;
}

scheduler myscheduler;

int main(void) {

    initScheduler(&myscheduler, rm_scheduler_cb, NULL);
    loadTimer(TICK_US);
    while (1) { /* idle forever */ }
}
