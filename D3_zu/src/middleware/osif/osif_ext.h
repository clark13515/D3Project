/*
 * osif_ext.h
 *
 *  Created on: 2022/01/24
 *      Author: jeremy.xiang
 */

#ifndef OSIF_EXT_H
#define OSIF_EXT_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "osif.h"
#include "message_buffer.h"
#include "event_groups.h"
#include "queue.h"

#define OSIF_TASK_VIEW_EN   1

//=======================================================//
// 64-bit address
#ifdef PLATFORM_SYS_ARCH_64
#define OSIF_STK_FREECELL_FLAG      0xA5A5A5A5A5A5A5A5U //The settings must be consistent with FreeRTOS
#else
#define OSIF_STK_FREECELL_FLAG      0xA5A5A5A5U //The settings must be consistent with FreeRTOS
#endif

#define ALIGNMENT_UP(x,size)        (((unsigned int)(x)+(size)-1 )/(size)*(size))
#define ALIGNMENT_DOWN(x,size)      ((unsigned int)(x)/(size)*(size))
#define FORCE_ALIGNED_INBYTE(x)     __attribute__((aligned(x)))

//------------------------------//
typedef void (*OSIF_TaskHandler_t)(void *arg);
typedef struct osif_task {
    const char *name;
    OSIF_TaskHandler_t handler;
    void *arg;
    void *stack;
    uint32_t StackSize_byte;    //in Bytes.
    void *priv_data;
#if OSIF_TASK_VIEW_EN
    struct {
        uint64_t *stack_cell;
        uint32_t NumOfStkCellTotal:16;
        uint32_t NumOfStkCellUsed:16;
        uint32_t TaskLevel:8;
    } Priv;
#endif
} OSIF_Task_t, *OSIF_TaskRef_t;
#define OSIF_TASKLEVEL_MAXNUM   (configMAX_PRIORITIES-3)      //OSIF The maximum number of task levels is: 31-3
////////////////
/* OSIF return code definitions. */
#define OSIF_RET_OK                   0
#define OSIF_RET_ERROR               -1      //general error.
#define OSIF_RET_ERROR_PARAM         -2
#define OSIF_RET_ERROR_NO_TASK       -3      //The specified Task was not found in the task table
#define OSIF_RET_TIMEOUT             -4      //wait timeout.
#define OSIF_RET_MESSAGEQ_FULL       -5
#define OSIF_RET_MESSAGEPOOL_FULL    -6
#define OSIF_RET_TIMER_FAILED        -7
////////////////
// 64-bit address
#ifdef PLATFORM_SYS_ARCH_64
#define OSIF_TASK_END_MARKER                        (0xFFFFFFFFFFFFFFFFUL)
#else
#define OSIF_TASK_END_MARKER                        (0xFFFFFFFFUL)
#endif
#define OSIF_TASKS_DECLARE_BEGIN( TableName )       OSIF_TaskRef_t TableName[] = {
#define OSIF_TASKS_ITEMS( ... )                     __VA_ARGS__ , (OSIF_TaskRef_t)OSIF_TASK_END_MARKER,
#define OSIF_TASKS_DECLARE_END( TableName )         (OSIF_TaskRef_t)OSIF_TASK_END_MARKER };    \
                                                    OSIF_TaskRef_t *gpTaskLevelTable = TableName;

#define OSIF_GET_TICK_INTERVAL(now, last) ((now >= last) ? (now - last) : (0xFFFFFFFF - last + now))

extern OSIF_TaskRef_t *gpTaskLevelTable;
////////////////
typedef struct OSIF_MessageQContainer {
    uint32_t NumberOfMessage;
    uint32_t MessageSize_byte;
    void    *MessageArray;
} OSIF_MessageQContainer_t;

typedef struct OSIF_MessageQConstructor {
    union {
        uint64_t Alignment;
        uint8_t  Dummy[sizeof(StaticQueue_t)];
    } u;
} OSIF_MessageQConstructor_t;

typedef struct OSIF_TimeContainer {
    const char *ptimer_name;
    const uint32_t period_in_ticks;
    const uint32_t auto_load;
    void *timer_id;
    void *pfunc_call_back;
} OSIF_TimerContainer_t;

typedef struct OSIF_TimerConstructor
{
    union {
        uint64_t Alignment;
        uint8_t  Dummy[sizeof(StaticTimer_t)];
    } u;
}OSIF_TimerConstructor_t;

typedef struct OSIF_MessagePoolConstructor {
    union {
        uint64_t Alignment;
        uint8_t  Dummy[sizeof(StaticMessageBuffer_t)];
    } u;
} OSIF_MessagePoolConstructor_t;



typedef TaskHandle_t OSIF_TaskHandle;

extern void         OSIF_Init(void);
extern void         OSIF_Start(void);
extern void         OSIF_CurrentTaskSetPrivData(void *data);
extern void*        OSIF_CurrentTaskGetPrivData(void);
extern OSIF_Task_t* OSIF_GetCurrentTask(void);
extern void*        OSIF_TaskCreate(OSIF_Task_t *task, int *err);
extern void         OSIF_TaskDelete(OSIF_TaskHandle xTaskToDelete);
extern void         OSIF_TaskTableCreate(OSIF_TaskRef_t TaskTable[]);
extern void         OSIF_TaskTableStackView(OSIF_TaskRef_t TaskTable[]);
extern void*        OSIF_MessagePoolCreate(OSIF_MessagePoolConstructor_t *constructor, void *buffer, uint32_t BufferSize);
extern int          OSIF_MessagePoolTake(void *handle, void *msg_data, uint32_t *msg_length, uint32_t MaxLength, uint32_t TicksToWait);
extern int          OSIF_MessagePoolGive(void *handle, void *msg_data, uint32_t MsgLength, uint32_t TicksToWait);
extern void*        OSIF_MessageQCreate(OSIF_MessageQConstructor_t *constructor, OSIF_MessageQContainer_t *container);
extern int          OSIF_MessageQTake(void *handle, void *msg, uint32_t TicksToWait);
extern int          OSIF_MessageQGive(void *handle, void *msg, uint32_t TicksToWait, bool IsTakenAsFirst);
extern int          OSIF_MessageQAddToSet(void *handle, void *Qset);
extern void*        OSIF_MessageQSeletFromSet(void *Qset, uint32_t TicksToWait);
extern void*        OSIF_TimerCreate(OSIF_TimerConstructor_t *constructor, OSIF_TimerContainer_t *container);
extern int          OSIF_TimerStart(void *xTimer);
extern int          OSIF_TimerStop(void *xTimer);
//=======================================================//
#endif /* OSIF_EXT_H */
