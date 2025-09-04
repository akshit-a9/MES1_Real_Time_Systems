#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

//NEED TO CONFIRM THIS WITH SIR
#include "xil_mmu.h"



#define SWITCH 0x41200000
#define SHARED_BASE  0x0AB00000


//HAVE TO CHECK MEMORY IN LAB
static volatile unsigned int *shared_value = (unsigned int *)SHARED_BASE;


TaskHandle_t task1Handle = NULL;

static void switchRead(void *p)
{
    (void)p;
    volatile int *swAddr = (volatile int *)SWITCH;

    *shared_value = 0;

    while (1)
    {
        *shared_value = (unsigned int)(*swAddr);
    }
}

int main(void)
{
    Xil_SetTlbAttributes(SHARED_BASE, NORM_NONCACHE | SHAREABLE);
    xTaskCreate(switchRead, "T1", 200, NULL, tskIDLE_PRIORITY, &task1Handle);
    vTaskStartScheduler();
    return 0;
}
