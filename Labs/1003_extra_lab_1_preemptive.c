#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#define SEC_TO_TICKS(s) ((TickType_t)((s) * 1000 / portTICK_PERIOD_MS))

typedef struct {
    TaskHandle_t h;
    const char  *name;
    int          period_s;
    int          exec_s;        // execution time required per job
    int          remaining;     // remaining execution for current job
    TickType_t   deadline;      // absolute deadline
} STask;

static TaskHandle_t schedulerHandle;
static STask T[3];

static void TaskBody(void *pvParameters)
{
    STask *self = (STask *)pvParameters;

    for (;;) {
        // wait until scheduler lets me run for 1s
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // log start
        printf("Time: %lu sec -> Running %s\n",
               (unsigned long)(xTaskGetTickCount()/configTICK_RATE_HZ), self->name);
        fflush(stdout);

        // run for 1 sec quantum
        vTaskDelay(1000/portTICK_PERIOD_MS);

        // one unit of work done
        self->remaining--;

        // if job finished, set up next job
        if (self->remaining == 0) {
            self->deadline += SEC_TO_TICKS(self->period_s);
            self->remaining = self->exec_s;
        }

        // tell scheduler this 1-sec quantum is done
        xTaskNotifyGive(schedulerHandle);
    }
}

static void EDF_Scheduler(void *p)
{
    (void)p;
    schedulerHandle = xTaskGetCurrentTaskHandle();
    TickType_t now = xTaskGetTickCount();

    // initialize all jobs
    for (int i = 0; i < 3; i++) {
        T[i].deadline = now + SEC_TO_TICKS(T[i].period_s);
        T[i].remaining = T[i].exec_s;
    }

    while (1) {
        now = xTaskGetTickCount();
        int chosen = -1;
        TickType_t best_deadline = (TickType_t)-1;

        // choose among released jobs (deadline - period <= now)
        for (int i = 0; i < 3; i++) {
            TickType_t release = T[i].deadline - SEC_TO_TICKS(T[i].period_s);
            if (now >= release && T[i].remaining > 0) {
                if (T[i].deadline < best_deadline) {
                    best_deadline = T[i].deadline;
                    chosen = i;
                }
            }
        }

        if (chosen < 0) {
            // nothing ready -> idle 1s
            vTaskDelay(1000/portTICK_PERIOD_MS);
            continue;
        }

        // run the chosen task for 1s
        xTaskNotifyGive(T[chosen].h);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }
}

void main_blinky(void)
{
    // Define tasks: T1(12,5), T2(5,2), T3(6,1)
    T[0] = (STask){ .name="T1", .period_s=12, .exec_s=5 };
    T[1] = (STask){ .name="T2", .period_s=5,  .exec_s=2 };
    T[2] = (STask){ .name="T3", .period_s=6,  .exec_s=1 };

    xTaskCreate(TaskBody, "T1", 256, &T[0], 2, &T[0].h);
    xTaskCreate(TaskBody, "T2", 256, &T[1], 2, &T[1].h);
    xTaskCreate(TaskBody, "T3", 256, &T[2], 2, &T[2].h);
    xTaskCreate(EDF_Scheduler, "EDF", 512, NULL, 3, &schedulerHandle);

    vTaskStartScheduler();
    for(;;);
}
