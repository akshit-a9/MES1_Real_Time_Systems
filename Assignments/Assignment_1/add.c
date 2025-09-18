#include <stdio.h>
#include "scheduler.h"
#include "task.h"
#include "xil_printf.h"

// Tick counter
static unsigned int global_time = 0;

// Define simple task functions
void task1() { xil_printf("Hello from task-1\n\r"); }
void task2() { xil_printf("Hello from task-2\n\r"); }
void task3() { xil_printf("Hello from task-3\n\r"); }

// Task table
tcb tasks[] = {
    {1000000, task1},   // every 1s
    {2000000, task2},   // every 2s
    {3000000, task3}    // every 3s
};
int num_tasks = sizeof(tasks) / sizeof(tasks[0]);

// Scheduler callback (called by timer ISR)
void* schedulerCallback(void *param) {
    global_time += 1000000; // assuming timer set for 1s = 1,000,000 us

    for (int i = 0; i < num_tasks; i++) {
        if (global_time % (int)tasks[i].execTime == 0) {
            tasks[i].fun();
        }
    }

    loadTimer(1000000); // reload timer (1s tick)
    return NULL;
}

scheduler myscheduler;

int main() {
    initScheduler(&myscheduler, schedulerCallback, NULL);
    loadTimer(1000000); // start periodic interrupts (1s tick)
    while(1); // keep main alive
}
