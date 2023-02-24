/*
 * port_freertos_hook.c
 *
 *  Created on: 2022/01/24
 *      Author: jeremy.xiang
 */
#include "FreeRTOS.h"
#include "task.h"

#if (configSUPPORT_STATIC_ALLOCATION == 1)
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
    static StaticTask_t xIdleTaskTCB;
    static StackType_t  xIdleTaskStack[configMINIMAL_STACK_SIZE];

    *ppxIdleTaskTCBBuffer   = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = &xIdleTaskStack[0];

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize   = configMINIMAL_STACK_SIZE;
}
#endif


#if (configSUPPORT_STATIC_ALLOCATION==1 && configUSE_TIMERS==1)
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t  **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
    static StaticTask_t xTimerTaskTCB;
    static StackType_t xTimerTaskStack[ configTIMER_TASK_STACK_DEPTH];

    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = &xTimerTaskStack[0];
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
#endif

__attribute__((weak)) void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    for (;;);
}

__attribute__((weak)) void vApplicationMallocFailedHook( void )
{
    for (;;);
}

