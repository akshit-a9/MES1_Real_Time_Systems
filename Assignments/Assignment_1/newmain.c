// main.c — RM scheduler (print at RELEASE), 1s tick; periods/WCETs in seconds
#include <stdio.h>
#include "scheduler.h"     // initScheduler, loadTimer (HW timer/ISR)
#include "xil_printf.h"
#include "task.h"          // extended tcb

#define TICK_SEC 1
#define TICK_US  1000000   // hardware API expects microseconds

// RM priority = shorter period first (array order = priority)
static tcb tasks[] = {
    { .id=1, .period=5,  .wcet=1, .next_release=0, .remaining=0, .ready=0 }, // T1(5,1)
    { .id=2, .period=10, .wcet=2, .next_release=0, .remaining=0, .ready=0 }, // T2(10,2)
    { .id=3, .period=15, .wcet=3, .next_release=0, .remaining=0, .ready=0 }, // T3(15,3)
};
static const int N = sizeof(tasks)/sizeof(tasks[0]);

static unsigned now = 0;  // seconds since start

static void init_tasks(void) {
    for (int i = 0; i < N; ++i) {
        tasks[i].next_release = tasks[i].period; // first release at its period
        tasks[i].remaining    = 0;
        tasks[i].ready        = 0;
    }
}

static inline void on_release(unsigned id) {
    xil_printf("Hello from Task %u\n\r", id);
}

// Return index of highest-priority ready task (RM), or -1 if none
static int pick_task(void) {
    for (int i = 0; i < N; ++i) {
        if (tasks[i].ready && tasks[i].remaining > 0) return i;
    }
    return -1;
}

// Timer ISR callback (registered via initScheduler)
void* rm_scheduler_cb(void *unused) {
    now += TICK_SEC;

    // 1) Releases (handle multiple pending releases robustly)
    for (int i = 0; i < N; ++i) {
        while ((int)(now - tasks[i].next_release) >= 0) {
            // Deadline miss if a previous job hasn't finished by next release
            if (tasks[i].ready && tasks[i].remaining > 0) {
                xil_printf("Deadline miss: Task %u at %u s\n\r", tasks[i].id, now);
            }
            // Release new job
            tasks[i].ready     = 1;
            tasks[i].remaining = (int)tasks[i].wcet;
            on_release(tasks[i].id);  // >>> print ON RELEASE <<<
            tasks[i].next_release += tasks[i].period;
        }
    }

    // 2) Pick highest-priority ready task (shortest period first)
    int k = pick_task();
    if (k >= 0) {
        // 3) Run for one tick (1 s of CPU)
        tasks[k].remaining -= TICK_SEC;
        if (tasks[k].remaining <= 0) {
            tasks[k].remaining = 0;
            tasks[k].ready = 0;
        }
    }
    // else: idle this tick

    // 4) Program next tick (hardware wants microseconds)
    loadTimer(TICK_US);
    return NULL;
}

scheduler myscheduler;

int main(void) {
    init_tasks();
    // Register our callback with the provided library (do not modify it)
    initScheduler(&myscheduler, rm_scheduler_cb, NULL);
    loadTimer(TICK_US);  // start periodic 1s ticks
    while (1) { /* idle forever */ }
}
