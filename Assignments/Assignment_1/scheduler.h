#include "xil_types.h"

typedef struct scheduler{
	void (*callback)(void *);
	void *callbackParameter;
}scheduler;


int initScheduler(scheduler *scheduler,void *callback(void *),void *callBackParameter);
void loadTimer(u32 timerVal);//Specify in us
