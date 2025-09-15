#include <stdio.h>
#include "scheduler.h"
#include "xil_printf.h"


void* myFunct(void *param){
	int val = *((int *)param);
	xil_printf("Hello parameter is %0x\n\r",val);
	loadTimer(1000000); //Delay in us
	return NULL;
}

scheduler myscheduler;

int main(){
	static int a=5;
	initScheduler(&myscheduler,myFunct,(void *)&a);
	loadTimer(1000000);
}
