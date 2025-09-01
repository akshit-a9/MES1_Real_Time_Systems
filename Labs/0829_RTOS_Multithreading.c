#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;

void myTask(void *p)
{
	while(1)
	{
		printf("Hello World!\n\r");
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}

void myTask2(void *p)
{
	while(1)
	{
		printf("Hello 2nd World!\n\r");
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}

int main()
{
	xTaskCreate(myTask,"T1",200,(void *)NULL,tskIDLE_PRIORITY, &task1Handle);
	xTaskCreate(myTask2,"T2",200,(void *)NULL,tskIDLE_PRIORITY+1, &task2Handle);
	vTaskStartScheduler();
	return 0;
}
