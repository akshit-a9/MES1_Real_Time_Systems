// main.c — RM scheduler, 1s tick, print at COMPLETION, all tasks released at t=0
#include <stdio.h>
#include "scheduler.h"     // initScheduler, loadTimer (HW timer/ISR):contentReference[oaicite:1]{index=1}
#include "xil_printf.h"
#include "task.h"          // tcb with: id, period, wcet, next_release, remaining, ready

#define TICK_SEC 1
#define TICK_US  1000000   // hardware API expects microseconds

// RM priority = shorter period first (array order = priority)
static tcb tasks[] = {
    { .id=1, .period=5,  .wcet=1, .next_release=0, .remaining=0, .ready=0 }, // T1(5,1)
    { .id=2, .period=10, .wcet=2, .next_release=0, .remaining=0, .ready=0 }, // T2(10,2)
    { .id=3, .period=15, .wcet=3, .next_release=0, .remaining=0, .ready=0 }, // T3(15,3)
};
static const int N = sizeof(tasks)/sizeof(tasks[0]);

static unsigned now = 0;  // seconds since start (time at BEGIN of current tick)

// Highest-priority ready task (RM order = array order)
static int pick_task(void) {
    for (int i = 0; i < N; ++i)
        if (tasks[i].ready && tasks[i].remaining > 0) return i;
    return -1;
}

static void print_finish(unsigned id, unsigned finish_time_s) {
    xil_printf("Hello from Task %u at time %u s\n\r", id, finish_time_s);
}

// Timer ISR callback (registered via initScheduler)
void* rm_scheduler_cb(void *unused) {
    // 1) Release jobs at current time 'now' (ALL release at t=0 initially)
    for (int i = 0; i < N; ++i) {
        while ((int)(now - tasks[i].next_release) >= 0) {
            // Optional: deadline miss if previous job not finished
            if (tasks[i].ready && tasks[i].remaining > 0) {
                xil_printf("Deadline miss: Task %u at %u s\n\r", tasks[i].id, now);
            }
            tasks[i].ready     = 1;
            tasks[i].remaining = (int)tasks[i].wcet;
            tasks[i].next_release += tasks[i].period;
        }
    }

    // 2) Pick RM-highest ready task
    int k = pick_task();
    if (k >= 0) {
        // 3) Run for one tick (1 s). Completion occurs at END of this tick → time = now + 1
        int will_finish = (tasks[k].remaining <= TICK_SEC);
        tasks[k].remaining -= TICK_SEC;
        if (will_finish) {
            tasks[k].remaining = 0;
            tasks[k].ready = 0;
            print_finish(tasks[k].id, now + TICK_SEC);
        }
    }
    // else idle this tick

    // 4) Advance time and arm next tick
    now += TICK_SEC;              // we advance time AFTER executing this 1s slice
    loadTimer(TICK_US);           // program next 1 s tick (API uses microseconds):contentReference[oaicite:2]{index=2}
    return NULL;
}

scheduler myscheduler;

int main(void) {
    // All tasks are set to release at t=0 via .next_release = 0 above
    initScheduler(&myscheduler, rm_scheduler_cb, NULL);  // library call:contentReference[oaicite:3]{index=3}
    loadTimer(TICK_US);                                  // start periodic 1 s ticks (HW):contentReference[oaicite:4]{index=4}
    while (1) { /* idle forever */ }
}
