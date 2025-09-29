#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"


//4.2 Task Functions

//void vATaskFunction( void * pvparameters);

// A freeRTOS task must not be allowed to return from the function that implements it in any way.

//void vATaskFunction( void * pvparameters){
//	long lStackVariable = 0; // A new instance of this is created every time function is called.
//	static long lStackVariable = 0; //All instances / tasks share this variable
//
//	for( ; ; )
//	{
//		printf("Continue...");//we implement task functionality here
//	}
//
//	vTaskDelete(NULL); //If task exits loop, delete before it reaches end of function. NULL is sort of a this pointer.
//}

//---------------------------------------------------------------------------------------------------------------------

//4.3 Top Level Task States

// For now, two types of tasks: RUNNING and NOT_RUNNING

//---------------------------------------------------------------------------------------------------------------------

//4.4 Task Creation

// Six API Functions:
//1. xTaskCreate()
//2. xTaskCreateStatic()     ---> Pre-allocated block of RAM
//3. xTaskCreateRestricted()    ---> Limited Access to system's memory
//4. xTaskCreateRestrictedStatic()
//5. xTaskCreateAffinitySet()    ---> For systems with SMP (Symmetric Multi Procesing), used to specify which core the task will run on.
//6. xTaskCreateStaticAffinitySet()

//FUNCTION PROTOTYPE FOR xTaskCreate():
//BaseType_t xTaskCreate( TaskFunction_t pvTaskCode, const char * const pcName, configSTACK_DEPTH_TYPE usStackDepth, void * pvParameters, UBaseType_t uxProximity, TaskHandle_t * pxCreatedTask);

//pvTaskCode -> pointer to function that implements the task
//pcName -> For debugging, a descriptive task name
//usStackDepth -> size of stack
//pvParameters -> task implementing functions accept a single void pointer
//uxPriority -> Defines Task's Priority. 0 Lowest.
//pxCreatedTask -
//RETURNVALUES: pdPASS - task created, pfFAIL - Not enough heap memory.



//A BASIC TASK


//TaskHandle_t task1Handle = NULL;
//TaskHandle_t task2Handle = NULL;
//
//void vTask1(void * pvParameter)
//{
//	for (;;)
//	{
//		printf("Task 1 is Running\n\r");
//		//vTaskDelay(1000/portTICK_PERIOD_MS);
//		//sleep(1);
//		for(int a =0; a<1000000; a++);
//	}
//}
//
//void vTask2(void * pvParameter)
//{
//	for (;;)
//	{
//		printf("Task 2 is Running\n\r");
//		//sleep(1);
//		//vTaskDelay(1000/portTICK_PERIOD_MS);
//		for(int a =0; a<1000000; a++);
//	}
//}
//
//int main(void)
//{
//	xTaskCreate(vTask1,"T1",1000,(void *)NULL,1, &task1Handle);
//	xTaskCreate(vTask2,"T2",1000,(void *)NULL,1, &task1Handle);
//	vTaskStartScheduler();
//
//	for (;;);
////	return 0;
//}



// NOW, LETS ONLY CREATE 1 TASK, BUT TWO INSTANCES OF IT

//TaskHandle_t task1Handle = NULL;
//
//void vTask1(void * pvParameter)
//{
//	char * pcTaskName;
//	pcTaskName = (char *) pvParameter;
//	for (;;)
//	{
//		printf("%s\n", pcTaskName);
//		//vTaskDelay(1000/portTICK_PERIOD_MS);
//		//sleep(1);
//		for(int a =0; a<10000000; a++);
//	}
//}
//
//	static const char * pcTextForTask1 = "Task 1 is Running";
//	static const char * pcTextForTask2 = "Task 2 is Running";
//
//int main(void)
//{
//
//	xTaskCreate(vTask1,"T1",1000,(void *)pcTextForTask1,1, &task1Handle);
//	xTaskCreate(vTask1,"T2",1000,(void *)pcTextForTask2,1, &task1Handle);
//	vTaskStartScheduler();
//
//	for (;;);
////	return 0;
//}

