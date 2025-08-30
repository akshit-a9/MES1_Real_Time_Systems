#define SWITCH 0x41200000 //Switch address
#define LED 0x41200008 //Switch address


//  PROGRAM 1 : TO USE LED'S TO SHOW BINARY VERSIONS OF HEX CODES

//	int main()
//	{
//	int* ledAddr;
//	ledAddr = (int *)LED;
//	*ledAddr = 0xA5;
//	return 0;
//  }
//  --------------------------------------------------------------------


// PROGRAM 2 : TO READ SWITCH POSITIONS AND CHANGE CORRESPONDING LED
//	int main()
//	{
//	int* ledAddr;
//	int* swAddr;
//	int temp;
//	ledAddr = (int *)LED;
//	swAddr = (int *)SWITCH;
//	while(1)
//	{
//		temp = (int)*swAddr;
//		*ledAddr = temp;
//	}
//	return 0;
//  }
//  ----------------------------------------------------------------------


//PROGRAM 3 : TO USE MULTITHREADING TO DO THE SAME
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

int temp;

TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;

void switchRead(void *p)
{
	int* swAddr;
	swAddr = (int *)SWITCH;
	while(1)
	{
		temp = (int)*swAddr;
	}
}

void writeLED(void *p)
{
	int* ledAddr;
	ledAddr = (int *)LED;
	while(1)
	{
		*ledAddr = temp;
	}
}

int main()
{
	xTaskCreate(switchRead,"T1",200,(void *)NULL,tskIDLE_PRIORITY, &task1Handle);
	xTaskCreate(writeLED,"T2",200,(void *)NULL,tskIDLE_PRIORITY, &task2Handle);
	vTaskStartScheduler();
	return 0;
}

// AFTER THIS, TRIED MULTICORE