/*
 * os_if_freertos.c
 *
 *  Created on: 2022/01/24
 *      Author: jeremy.xiang
 */

#include "osif_freertos.h"
#include <stdbool.h>
#include "osif_common.h"
#include "task.h"
#include "semphr.h"

#if !defined (USING_OS_FREERTOS)
#error "Wrong OSIF selected. Please define symbol USING_OS_FREERTOS in project settings or change the OSIF variant"
#endif

/* Assert macro does nothing */
#define DEV_ASSERT(x) ((void)0)
#define MSEC_TO_TICK(msec) (pdMS_TO_TICKS(msec))


#if configSUPPORT_STATIC_ALLOCATION == 1
#define SEM_HANDLE(sem)    ((sem).handle)
#else
#define SEM_HANDLE(sem)    (sem)
#endif

void OSIF_TimeDelay(uint32_t delay_ms)
{
    vTaskDelay(MSEC_TO_TICK(delay_ms));
}


uint32_t OSIF_GetMilliseconds(void)
{
    if(__OSIF_IsIsrContext()) {
        return (uint32_t)((((uint64_t)xTaskGetTickCountFromISR()) * 1000u) / configTICK_RATE_HZ);
    }
    else
    {
        return (uint32_t)((((uint64_t)xTaskGetTickCount()) * 1000u) / configTICK_RATE_HZ);
    }
}


int OSIF_MutexLock(const mutex_t * const pMutex, const uint32_t timeout)
{
    /* The (pMutex == NULL) case is a valid option, signaling that the mutex does
     * not need to be locked - do not use DEV_ASSERT in this case */

    uint32_t timeoutTicks;
    int osif_ret_code = 0;
    TaskHandle_t mutex_holder_handle;
    TaskHandle_t current_task_handle;
    BaseType_t operation_status = pdFAIL;

    if (pMutex != NULL)
    {
        SemaphoreHandle_t mutex_handle = SEM_HANDLE(*pMutex);
        /* Two dependencies for FreeRTOS config file */
        /* INCLUDE_xQueueGetMutexHolder */
        /* INCLUDE_xTaskGetCurrentTaskHandle */
        mutex_holder_handle = xSemaphoreGetMutexHolder(mutex_handle);
        current_task_handle = xTaskGetCurrentTaskHandle();

        /* If pMutex has been locked by current task, return error. */
        if (mutex_holder_handle == current_task_handle)
        {
            osif_ret_code = 1;
        }
        else
        {
            /* Convert timeout from millisecond to tick. */
            if (timeout == OSIF_WAIT_FOREVER)
            {
                timeoutTicks = portMAX_DELAY;
            }
            else
            {
                timeoutTicks = MSEC_TO_TICK(timeout);
            }

            /* Try to take the semaphore */
            operation_status = xSemaphoreTake(mutex_handle, timeoutTicks);

            osif_ret_code = (operation_status == pdPASS) ? 0 : 2;
        }
    }

    return osif_ret_code;
}


int OSIF_MutexUnlock(const mutex_t * const pMutex)
{
    /* The (pMutex == NULL) case is a valid option, signaling that the mutex does
     * not need to be unlocked - do not use DEV_ASSERT in this case */

    int osif_ret_code = 0;
    TaskHandle_t mutex_holder_handle;
    TaskHandle_t current_task_handle;
    BaseType_t operation_status = pdFAIL;

    if (pMutex != NULL)
    {
        SemaphoreHandle_t mutex_handle = SEM_HANDLE(*pMutex);
        /* Two dependencies for FreeRTOS config file */
        /* INCLUDE_xQueueGetMutexHolder */
        /* INCLUDE_xTaskGetCurrentTaskHandle */
        mutex_holder_handle = xSemaphoreGetMutexHolder(mutex_handle);
        current_task_handle = xTaskGetCurrentTaskHandle();

        /* If pMutex is not locked by current task, return error. */
        if (mutex_holder_handle != current_task_handle)
        {
            osif_ret_code = 1;
        }
        else
        {
            operation_status = xSemaphoreGive(mutex_handle);
            osif_ret_code = (operation_status == pdPASS) ? 0 : 1;
        }
    }

    return osif_ret_code;
}


int OSIF_MutexCreate(mutex_t * const pMutex)
{
    /* The (pMutex == NULL) case is a valid option, signaling that the mutex does
     * not need to be created - do not use DEV_ASSERT in this case */

    int osif_ret_code = 0;

    if (pMutex != NULL)
    {
#if configSUPPORT_STATIC_ALLOCATION == 1
        pMutex->handle = xSemaphoreCreateMutexStatic(&(pMutex->buffer));
        if (pMutex->handle == NULL)
        {
            osif_ret_code = 1; /* mutex not created successfully */
        }

#else /* configSUPPORT_STATIC_ALLOCATION == 0, it's dynamic allocation */
        *pMutex = xSemaphoreCreateMutex();
        if (*pMutex == NULL)
        {
            osif_ret_code = 1; /* mutex not created successfully */
        }

#endif /* if configSUPPORT_STATIC_ALLOCATION == 1 */
    }

    return osif_ret_code;
}


int OSIF_MutexDestroy(const mutex_t * const pMutex)
{
    /* The (pMutex == NULL) case is a valid option, signaling that the mutex does
     * not need to be destroyed - do not use DEV_ASSERT in this case */

    if (pMutex != NULL)
    {
        SemaphoreHandle_t mutex_handle = SEM_HANDLE(*pMutex);
        DEV_ASSERT(mutex_handle);
        vSemaphoreDelete(mutex_handle);
    }

    return 0;
}


int OSIF_SemaWait(semaphore_t * const pSem, const uint32_t timeout)
{
    DEV_ASSERT(pSem);

    SemaphoreHandle_t sem_handle = SEM_HANDLE(*pSem);
    uint32_t timeoutTicks;
    BaseType_t operation_status;
    int osif_ret_code;

    /* Convert timeout from millisecond to ticks. */
    if (timeout == OSIF_WAIT_FOREVER)
    {
        timeoutTicks = portMAX_DELAY;
    }
    else
    {
        timeoutTicks = MSEC_TO_TICK(timeout);
    }

    /* Try to take the semaphore */
    operation_status = xSemaphoreTake(sem_handle, timeoutTicks);

    osif_ret_code = (operation_status == pdPASS) ? 0 : 2;

    return osif_ret_code;
}


int OSIF_SemaPost(semaphore_t * const pSem)
{
    DEV_ASSERT(pSem);

    BaseType_t operation_status = pdFAIL;
    int osif_ret_code;

    SemaphoreHandle_t sem_handle = SEM_HANDLE(*pSem);
    /* Check if the post operation is executed from ISR context */
    bool is_isr = __OSIF_IsIsrContext();
    if (is_isr)
    {
        /* Execution from exception handler (ISR) */
        BaseType_t taskWoken = pdFALSE;
        operation_status = xSemaphoreGiveFromISR(sem_handle, &taskWoken);

        if (operation_status == pdPASS)
        {
            /* Perform a context switch if necessary */
            portYIELD_FROM_ISR(taskWoken);
        }
    }
    else
    {
        /* Execution from task */
        operation_status = xSemaphoreGive(sem_handle);
    }

    /* pdFAIL in case that the semaphore is full */
    osif_ret_code = (operation_status == pdPASS) ? 0 : 1;

    return osif_ret_code;
}


int OSIF_SemaCreate(semaphore_t * const pSem, const uint8_t initValue)
{
    DEV_ASSERT(pSem);

    int osif_ret_code = 0;

#if configSUPPORT_STATIC_ALLOCATION == 1
    pSem->handle = xSemaphoreCreateCountingStatic(0xFFu, initValue, &(pSem->buffer));
    if (pSem->handle == NULL)
    {
        osif_ret_code = 1; /* semaphore not created successfully */
    }

#else /* configSUPPORT_STATIC_ALLOCATION == 0, it's dynamic allocation */
    *pSem = xSemaphoreCreateCounting(0xFFu, initValue);
    if (*pSem == NULL)
    {
        osif_ret_code = 1; /* semaphore not created successfully */
    }

#endif /* if configSUPPORT_STATIC_ALLOCATION == 1 */

    return osif_ret_code;
}


int OSIF_SemaCreateMaxValue(semaphore_t * const pSem, const uint8_t maxValue, const uint8_t initValue)
{
    DEV_ASSERT(pSem);

    int osif_ret_code = 0;

#if configSUPPORT_STATIC_ALLOCATION == 1
    pSem->handle = xSemaphoreCreateCountingStatic(maxValue, initValue, &(pSem->buffer));
    if (pSem->handle == NULL)
    {
        osif_ret_code = 1; /* semaphore not created successfully */
    }

#else /* configSUPPORT_STATIC_ALLOCATION == 0, it's dynamic allocation */
    *pSem = xSemaphoreCreateCounting(maxValue, initValue);
    if (*pSem == NULL)
    {
        osif_ret_code = 1; /* semaphore not created successfully */
    }

#endif /* if configSUPPORT_STATIC_ALLOCATION == 1 */

    return osif_ret_code;
}


int OSIF_SemaDestroy(const semaphore_t * const pSem)
{
    DEV_ASSERT(pSem);
    SemaphoreHandle_t sem_handle = SEM_HANDLE(*pSem);

    DEV_ASSERT(sem_handle);
    vSemaphoreDelete(sem_handle);

    return 0;
}


int OSIF_SemaCreateBinary(semaphore_t * const pSem, const uint8_t initValue)
{
    DEV_ASSERT(pSem);

    int osif_ret_code = 0;

#if configSUPPORT_STATIC_ALLOCATION == 1
    pSem->handle = xSemaphoreCreateCountingStatic(0x01u, initValue, &(pSem->buffer));
    if (pSem->handle == NULL)
    {
        osif_ret_code = 1; /* semaphore not created successfully */
    }

#else /* configSUPPORT_STATIC_ALLOCATION == 0, it's dynamic allocation */
    *pSem = xSemaphoreCreateCounting(0xFFu, initValue);
    if (*pSem == NULL)
    {
        osif_ret_code = 1; /* semaphore not created successfully */
    }

#endif /* if configSUPPORT_STATIC_ALLOCATION == 1 */

    return osif_ret_code;
}

