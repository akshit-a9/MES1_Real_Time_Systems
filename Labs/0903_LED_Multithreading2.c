#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

//CONFIRM THIS
#include "xil_mmu.h"

#define LED          0x41200008U

// CHECK THIS
#define SHARED_BASE  0x30000000U  

static volatile unsigned int *shared_value = (volatile unsigned int *)SHARED_BASE;

TaskHandle_t taskHandle = NULL;

static void writeLED(void *p)
{
    (void)p;
    volatile int *ledAddr = (volatile int *)LED;

    *ledAddr = 0; 

    while (1)
    {
        unsigned int v = *shared_value; 
        *ledAddr = (int)v;              
    }
}

int main(void)
{
    Xil_SetTlbAttributes(SHARED_BASE, NORM_NONCACHE | SHAREABLE);

    xTaskCreate(writeLED, "T_LED", 200, NULL, tskIDLE_PRIORITY, &taskHandle);
    vTaskStartScheduler();
    return 0;
}
