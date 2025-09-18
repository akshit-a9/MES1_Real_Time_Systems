#include <stdio.h>
#include "scheduler.h"
#include "xil_printf.h"
#include "task.h"

static unsigned int global_time = 0;

void task1() { xil_printf("Hello from task 1\n\r"); }
void task2() { xil_printf("Hello from task 2\n\r"); }
void task3() { xil_printf("Hello from task 3\n\r"); }


tcb tasks[] = {
    {1000000, task1},   // every 1s
    {2000000, task2},   // every 2s
    {3000000, task3}    // every 3s
};
int num_tasks = sizeof(tasks) / sizeof(tasks[0]);



void* myFunct(void *param){
//	int val = *((int *)param);
//	xil_printf("Hello parameter is %0x\n\r",val);
//	loadTimer(1000000); //Delay in us
//	return NULL;

	global_time += 1000000; //(1 second each)

	    for (int i = 0; i < num_tasks; i++) {
	        if (global_time % (int)tasks[i].execTime == 0) {
	            tasks[i].fun(); //This causes that function to print
	        }
	    }

	    loadTimer(1000000); // reload timer (1s tick)
	    return NULL;
}


scheduler myscheduler;

int main(){
	//static int a=5;
	initScheduler(&myscheduler,myFunct,NULL);
	loadTimer(1000000);
	while(1);
}