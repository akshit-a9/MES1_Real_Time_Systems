#include "xparameters.h"
#include "sleep.h"
#include "scheduler.h"

//Scheduler instance
scheduler myscheduler;

//Structure for representing a critical section
typedef struct critSection{
	int start;
	int duration;
	resourceType resource;
	int numResources;
	int usingTaskNumber;
}critSection;

//Table to store TCBs. This can be moved to scheduler. Now statically declared and dynamically allocated
TaskInfo taskTable[NUMTasks];


//Task 1 function. Critical section info is directly created by tasks
//When the task is scheduled, elapsed execution time is passed to the task by scheduler
//By comparing the elapsed time and timing parameters specified in the critical section,
//task make the resource request to the scheduler.
//In real-world this is not needed since requesting for a resource happens naturally when the
//task requires the resource.
int task1(int executedTime){
	int ret;
	static critSection tsk0Critic[] = {{1,2,LED}};
	if(executedTime ==  tsk0Critic[0].start){  //Compare elepsed time with start time of critical section
		print("Task-1: Requesting LED..");
		ret = getResource(&myscheduler,LED); //request for LED to the scheduler
		if(ret == 0)                         //Check whether resource was allotted
			print("Got access to LED");
		else{                                //If resource was not allotted, immediately pre-empt task and return
			print("Resource busy..");
			return -1;
		}
	}
	else if((executedTime >  tsk0Critic[0].start) && (executedTime <  tsk0Critic[0].start + tsk0Critic[0].duration - 1))
		print("Task-1: Using LED");
	else if(executedTime ==  tsk0Critic[0].start + tsk0Critic[0].duration - 1){ //Release resource at the end of this tick
		print("Task-1: Using LED then Releasing");
		freeResource(&myscheduler,LED);
	}
	else  //If no resource is used, assume task is using the processor
		print("Task-1: Using processor");

	return 0; //Return 0 to indicate the task successfully ran during the tick. -1 in case resource was denied
}

int task2(int executedTime){
	print("Task-2: Using processor");
	return 0;
}

int task3(int executedTime){
	int ret;
	static critSection tsk1Critic[] = {{1,5,LED}};
	if((executedTime ==  tsk1Critic[0].start)){
		print("Task-3: Requesting LED..");
		ret = getResource(&myscheduler,LED);
		if(ret == 0){
			print("Got access to LED");
		}
		else{
			print("Resource busy..");
			return -1;
		}
	}
	else if((executedTime >  tsk1Critic[0].start) && (executedTime <  tsk1Critic[0].start + tsk1Critic[0].duration - 1))
		print("Task-3: Using LED");
	else if(executedTime ==  tsk1Critic[0].start + tsk1Critic[0].duration - 1){
		print("Task-3: Using LED then Releasing");
		freeResource(&myscheduler,LED);
	}
	else
		print("Task-3: Using processor");

	return 0;
}



int main(){
	print("\n\rHello from Main...");
	taskTable[0] = (TaskInfo){6, 4, task1, 0, 0, WAITING, 0}; //Create TCBs and enter in the TCB table
	taskTable[1] = (TaskInfo){5, 5, task2, 1, 1, WAITING, 0}; //release time, execution time, pointer to task function
	taskTable[2] = (TaskInfo){0, 7, task3, 2, 2, WAITING, 0}; //task initial state, elapsed execution time (0)
	initScheduler(&myscheduler,BPI);
	while(1);
}
