#include <stdio.h>
#include <stdbool.h>

/* Basic configuration */
#define NUMTasks          3

typedef enum { LED, SWITCH } resourceType;
typedef enum { NONE, NPCS, BPI } schedulePolicy;

#define READY    0
#define RUNNING  1
#define WAITING  2
#define FINISHED 3
#define BLOCKED  4

typedef struct {
    void (*callback)(void *);
    void *callbackParameter;
    int currTaskPriority;
    schedulePolicy policy;
} scheduler;

typedef struct {
    int releaseTime;
    int execTime;
    int (*fun)(int);   /* Pointer to task function */
    int basePriority;       /* fixed / assigned priority */
    int currentPriority;    /* may be raised under BPI */
    int state;
    int executedTime;
} TaskInfo;

typedef struct {
    resourceType resource;
    bool isused;
    int TaskNumber;    /* index of owning task in taskTable, -1 if none */
} RCB;

/* Global objects */
scheduler myscheduler;
TaskInfo taskTable[NUMTasks];

/* Resource table: we only really use LED in this example */
RCB RCBList[] = {
    { LED,    false, -1 },
    { SWITCH, false, -1 }
};

/* Simulated time and scheduler globals */
static int sim_clock = 0;
static int highestTaskIndex = -1;

/* ----------- Forward declarations ----------- */
void tickCallBack(void *schduler);
int getResource(scheduler *scheduler, resourceType resource);
int freeResource(scheduler *scheduler, resourceType resource);
int getTime(void);

/* ----------- Task implementations (same logic as FPGA version) ----------- */

typedef struct {
    int start;
    int duration;
    resourceType resource;
    int numResources;
    int usingTaskNumber;
} critSection;

int task1(int executedTime) {
    int ret;
    static critSection tsk0Critic[] = { {1, 2, LED} };

    if (executedTime == tsk0Critic[0].start) {
        printf("Task-1: Requesting LED.. ");
        ret = getResource(&myscheduler, LED);
        if (ret == 0) {
            printf("Got access to LED\n");
        } else {
            printf("Resource busy..\n");
            return -1;
        }
    } else if ((executedTime > tsk0Critic[0].start) &&
               (executedTime < tsk0Critic[0].start + tsk0Critic[0].duration - 1)) {
        printf("Task-1: Using LED\n");
    } else if (executedTime ==
               tsk0Critic[0].start + tsk0Critic[0].duration - 1) {
        printf("Task-1: Using LED then Releasing\n");
        freeResource(&myscheduler, LED);
    } else {
        printf("Task-1: Using processor\n");
    }

    return 0;
}

int task2(int executedTime) {
    (void)executedTime;
    printf("Task-2: Using processor\n");
    return 0;
}

int task3(int executedTime) {
    int ret;
    static critSection tsk1Critic[] = { {1, 5, LED} };

    if (executedTime == tsk1Critic[0].start) {
        printf("Task-3: Requesting LED.. ");
        ret = getResource(&myscheduler, LED);
        if (ret == 0) {
            printf("Got access to LED\n");
        } else {
            printf("Resource busy..\n");
            return -1;
        }
    } else if ((executedTime > tsk1Critic[0].start) &&
               (executedTime < tsk1Critic[0].start + tsk1Critic[0].duration - 1)) {
        printf("Task-3: Using LED\n");
    } else if (executedTime ==
               tsk1Critic[0].start + tsk1Critic[0].duration - 1) {
        printf("Task-3: Using LED then Releasing\n");
        freeResource(&myscheduler, LED);
    } else {
        printf("Task-3: Using processor\n");
    }

    return 0;
}

/* ----------------------- Core scheduler logic ----------------------- */

int getTime(void) {
    return sim_clock;
}

void tickCallBack(void *schduler) {
    int ret;
    int currTime;
    int currTaskPriority;
    int i;
    scheduler *s;

    currTime = getTime();
    printf("\nCurrent time %3d:\t", currTime);

    /* Update previously running task */
    if (highestTaskIndex != -1) {
        taskTable[highestTaskIndex].executedTime++;
        if (taskTable[highestTaskIndex].executedTime >=
            taskTable[highestTaskIndex].execTime) {
            taskTable[highestTaskIndex].state = FINISHED;
        }
    }

    s = (scheduler *)schduler;

schedule:

    /* NPCS behaviour: if any resource is used, keep running current task */
    if (s->policy == NPCS) {
        for (i = 0; i < (int)(sizeof(RCBList) / sizeof(RCBList[0])); i++) {
            if (RCBList[i].isused == true) {
                if (highestTaskIndex != -1) {
                    (void)taskTable[highestTaskIndex].fun(
                        taskTable[highestTaskIndex].executedTime);
                } else {
                    printf("NOP (no current task)\n");
                }
                return;
            }
        }
    }

    currTaskPriority = 1000;
    highestTaskIndex = -1;

    /* Pick highest-priority READY/WAITING task */
    for (i = 0; i < NUMTasks; i++) {
        if ((taskTable[i].releaseTime <= currTime) &&
            (taskTable[i].state != FINISHED) &&
            (taskTable[i].state != BLOCKED)) {

            if (taskTable[i].currentPriority < currTaskPriority) {
                highestTaskIndex = i;
                currTaskPriority = taskTable[i].currentPriority;
            }
        }
    }

    if (highestTaskIndex != -1) {
        ret = taskTable[highestTaskIndex].fun(
            taskTable[highestTaskIndex].executedTime);
        if (ret == -1) {
            /* resource denied → block and reschedule */
            taskTable[highestTaskIndex].state = BLOCKED;
            goto schedule;
        }
    } else {
        printf("NOP");
    }
}

int getResource(scheduler *scheduler, resourceType resource) {
    int i;
    for (i = 0; i < (int)(sizeof(RCBList) / sizeof(RCBList[0])); i++) {
        if (RCBList[i].resource == resource) {
            if (RCBList[i].isused == false) {
                RCBList[i].isused = true;
                RCBList[i].TaskNumber = highestTaskIndex;
                if (resource == LED) {
                    printf("[LED=ON] ");
                    return 0;
                } else if (resource == SWITCH) {
                    int data = 0; /* fake switch */
                    return data;
                }
            } else {
                /* Base Priority Inheritance */
                if (scheduler->policy == BPI) {
                    int owner = RCBList[i].TaskNumber;
                    if (owner >= 0) {
                        if (taskTable[highestTaskIndex].currentPriority <
                            taskTable[owner].currentPriority) {
                            taskTable[owner].currentPriority =
                                taskTable[highestTaskIndex].currentPriority;
                            printf("[BPI: task %d -> owner %d priority=%d] ",
                                   highestTaskIndex, owner,
                                   taskTable[owner].currentPriority);
                        }
                    }
                }
                return -1;
            }
        }
    }
    return -1;
}

int freeResource(scheduler *scheduler, resourceType resource) {
    int i, j;
    for (i = 0; i < (int)(sizeof(RCBList) / sizeof(RCBList[0])); i++) {
        if (RCBList[i].resource == resource) {
            if (RCBList[i].TaskNumber == highestTaskIndex) {
                if (resource == LED) {
                    printf("[LED=OFF] ");
                    RCBList[i].isused = false;
                    RCBList[i].TaskNumber = -1;

                    if (scheduler->policy == BPI) {
                        /* Restore owner priority and unblock waiters */
                        taskTable[highestTaskIndex].currentPriority =
                            taskTable[highestTaskIndex].basePriority;

                        for (j = 0; j < NUMTasks; j++) {
                            if (taskTable[j].state == BLOCKED) {
                                taskTable[j].state = WAITING;
                            }
                        }
                    }
                    return 0;
                }
            } else {
                return -1;
            }
        }
    }
    return -1;
}

/* ---------------- Simulation entry and config ---------------- */

void initScheduler(scheduler *s, schedulePolicy policy) {
    s->callback = tickCallBack;
    s->callbackParameter = s;
    s->policy = policy;
    s->currTaskPriority = 0;
}

void simulate(int ticks) {
    int t;
    for (t = 0; t < ticks; t++) {
        sim_clock = t;  /* clock IP equivalent */
        myscheduler.callback(myscheduler.callbackParameter);  /* timer ISR equivalent */
    }
}

int main(void) {
    printf("Starting RT scheduler simulation with BPI policy.\n");

    /* Same task parameters as your FPGA version */
    taskTable[0] = (TaskInfo){6, 4, task1, 0, 0, WAITING, 0};
    taskTable[1] = (TaskInfo){5, 5, task2, 1, 1, WAITING, 0};
    taskTable[2] = (TaskInfo){0, 7, task3, 2, 2, WAITING, 0};

    initScheduler(&myscheduler, BPI);

    /* Run for 15 ticks – adjust if you want */
    simulate(15);

    return 0;
}
