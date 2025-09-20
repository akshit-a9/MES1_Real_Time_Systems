// task.h — Extended TCB for RM (seconds-based)
#ifndef TASK_H
#define TASK_H

typedef struct tcb {
    // Configuration (all in seconds)
    unsigned id;         // 1,2,3
    unsigned period;     // period (s)
    unsigned wcet;       // worst-case execution time (s)

    // Runtime state
    unsigned next_release; // absolute release time (s)
    int      remaining;    // remaining exec for current job (s)
    int      ready;        // 0/1: job released and awaiting CPU?
} tcb;

#endif
