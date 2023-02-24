/*
 * osif_freertos.h
 *
 *  Created on: 2022/01/24
 *      Author: jeremy.xiang
 */

#ifndef FREERTOS_OSIF_H
#define FREERTOS_OSIF_H

#include <stdint.h>
#include "osif_ext.h"

#define USING_OS_FREERTOS

#ifdef USING_OS_FREERTOS
#include "FreeRTOS.h"
#include "semphr.h"
#if configSUPPORT_STATIC_ALLOCATION == 1
typedef struct
{
    SemaphoreHandle_t handle;
    StaticSemaphore_t buffer;
} semaphore_t;
typedef semaphore_t mutex_t;
#else 
typedef SemaphoreHandle_t mutex_t;
typedef SemaphoreHandle_t semaphore_t;
#endif 
#else 
typedef uint8_t mutex_t;
typedef volatile uint8_t semaphore_t;
#endif 

/*! @endcond */

#define OSIF_WAIT_FOREVER 0xFFFFFFFFu




/*******************************************************************************
 * API
 ******************************************************************************/
extern void           OSIF_TimeDelay(const uint32_t delay_ms);
extern uint32_t       OSIF_GetMilliseconds(void);
extern int OSIF_MutexLock(const mutex_t * const pMutex, const uint32_t timeout);
extern int OSIF_MutexUnlock(const mutex_t * const pMutex);
extern int OSIF_MutexCreate(mutex_t * const pMutex);
extern int OSIF_MutexDestroy(const mutex_t * const pMutex);
extern int OSIF_SemaWait(semaphore_t * const pSem, const uint32_t timeout);
extern int OSIF_SemaPost(semaphore_t * const pSem);
extern int OSIF_SemaCreate(semaphore_t * const pSem, const uint8_t initValue);
extern int OSIF_SemaCreateMaxValue(semaphore_t * const pSem, const uint8_t maxValue, const uint8_t initValue);
extern int OSIF_SemaDestroy(const semaphore_t * const pSem);
extern int OSIF_SemaCreateBinary(semaphore_t * const pSem, const uint8_t initValue);

#endif /* FREERTOS_OSIF_H */
