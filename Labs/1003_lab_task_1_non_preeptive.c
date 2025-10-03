#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#define SEC_TO_TICKS(s) ((TickType_t)((s) * 1000 / portTICK_PERIOD_MS))
//our time is given in seconds. for freeRTOs to understand us, we need to give him the time in ticks. hence, we typecast it to that datatype and then covert it back.



TaskHandle_t t1Handle, t2Handle, t3Handle, schedHandle;


#define T1_PERIOD  12
#define T1_EXEC    5
#define T2_PERIOD  5
#define T2_EXEC    2
#define T3_PERIOD  6
#define T3_EXEC    1


TickType_t d1, d2, d3;


void run_for_seconds(int sec, const char *name) {
    int now = xTaskGetTickCount() / configTICK_RATE_HZ;
    printf("Time: %d sec -> Running %s\n", now, name);
    fflush(stdout);


    for (int i = 0; i < sec; i++) {
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void Task1(void *p) {
    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);   // this is basically the task waiting for a signal from our scheduler. MAX_DELAY here means an infinite delay, basically the task waiting till its told to execute.
        run_for_seconds(T1_EXEC, "T1");
        d1 += SEC_TO_TICKS(T1_PERIOD);
        xTaskNotifyGive(schedHandle);              // opposite of TaskNotifyTake. Tells our cute scheduler that "I am done"
    }
}

void Task2(void *p) {
    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        run_for_seconds(T2_EXEC, "T2");
        d2 += SEC_TO_TICKS(T2_PERIOD);
        xTaskNotifyGive(schedHandle);
    }
}

void Task3(void *p) {
    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        run_for_seconds(T3_EXEC, "T3");
        d3 += SEC_TO_TICKS(T3_PERIOD);
        xTaskNotifyGive(schedHandle);
    }
}

void Scheduler(void *p) {
    schedHandle = xTaskGetCurrentTaskHandle();
    TickType_t now = xTaskGetTickCount();

    // initial deadlines, phase = 0, so at start, it will all be 0+period (now here is considered 0)
    d1 = now + SEC_TO_TICKS(T1_PERIOD);
    d2 = now + SEC_TO_TICKS(T2_PERIOD);
    d3 = now + SEC_TO_TICKS(T3_PERIOD);

    while (1) {
        now = xTaskGetTickCount();

        // find released jobs (release = deadline - period). for 0, it will be zero ofc, but going ahead this is used.
        TickType_t r1 = d1 - SEC_TO_TICKS(T1_PERIOD);
        TickType_t r2 = d2 - SEC_TO_TICKS(T2_PERIOD);
        TickType_t r3 = d3 - SEC_TO_TICKS(T3_PERIOD);

        int chosen = -1;
        TickType_t best = (TickType_t)-1;

        if (now >= r1 && d1 < best) { best = d1; chosen = 1; }
        if (now >= r2 && d2 < best) { best = d2; chosen = 2; }
        if (now >= r3 && d3 < best) { best = d3; chosen = 3; }

        if (chosen < 0) { vTaskDelay(1); continue; }

        // we set the priority of chosen task higher than scheduler
        if (chosen == 1) { vTaskPrioritySet(t1Handle, 3); xTaskNotifyGive(t1Handle); }
        if (chosen == 2) { vTaskPrioritySet(t2Handle, 3); xTaskNotifyGive(t2Handle); }
        if (chosen == 3) { vTaskPrioritySet(t3Handle, 3); xTaskNotifyGive(t3Handle); }

        // this is the last line of each task. basically tells us when a task finishes
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // once done, we get the priority back to 1
        if (chosen == 1) vTaskPrioritySet(t1Handle, 1);
        if (chosen == 2) vTaskPrioritySet(t2Handle, 1);
        if (chosen == 3) vTaskPrioritySet(t3Handle, 1);
    }
}


void main_blinky(void) {
    xTaskCreate(Task1, "T1", 256, NULL, 1, &t1Handle);
    xTaskCreate(Task2, "T2", 256, NULL, 1, &t2Handle);
    xTaskCreate(Task3, "T3", 256, NULL, 1, &t3Handle);
    xTaskCreate(Scheduler, "Scheduler", 512, NULL, 2, &schedHandle);

    vTaskStartScheduler();
    for(;;);
}
