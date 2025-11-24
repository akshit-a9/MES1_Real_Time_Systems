#include "xil_types.h"
#include <stdbool.h>

#define NUMTasks 		  3
#define TIMER_CLOCK_HZ    100000000
#define TICK_DURATION_SEC 1

typedef enum {LED,SWITCH}resourceType;

typedef enum {NONE,NPCS,BPI}schedulePolicy;

typedef struct scheduler{
	void (*callback)(void *);
	void *callbackParameter;
	int currTaskPriority;
	schedulePolicy policy;
}scheduler;


#define RUNNING 1
#define WAITING 2
#define FINISHED 3
#define READY 0
#define BLOCKED 4


typedef struct TaskInfo{
    int releaseTime;
    int execTime;
    int (*fun)(int); //Pointer to task function
    int basePriority;       // fixed / assigned priority
    int currentPriority;    // may be raised under PI
    int state;
    int executedTime;
} TaskInfo;


typedef struct RCB{
	resourceType resource;
	bool isused;
	int TaskNumber;
}RCB;


int initScheduler(scheduler *scheduler,schedulePolicy policy);
void loadTimer(u32 timerVal);//Specify in us
int getResource(scheduler *,resourceType);
int freeResource(scheduler *scheduler,resourceType resource);
int getTime();
