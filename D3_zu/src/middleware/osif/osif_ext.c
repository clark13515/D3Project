/*
 * osif_ext.c
 *
 *  Created on: 2022/01/24
 *      Author: jeremy.xiang
 */

#include "osif_ext.h"
#include "osif_common.h"

struct task_level {
    uint32_t bSettled:1;
    uint64_t Min;
    uint64_t Max;
    OSIF_TaskRef_t *task_refs;
};

static struct task_level sTaskLevels[OSIF_TASKLEVEL_MAXNUM];
static int sTaskLevelCount;
//===============================================================//
static int __OSIF_TaskTableInit(OSIF_TaskRef_t TaskRefs[])
{
    struct task_level *pTaskLevel;

    if(TaskRefs == NULL) {
        return OSIF_RET_ERROR_PARAM;
    }
    sTaskLevelCount = 0;
    for(int i=0; i<OSIF_TASKLEVEL_MAXNUM; i++) {
        sTaskLevels[i].bSettled = 0;
    }
    //checks
    for(int i=0;;i++) {
        if((uint64_t)TaskRefs[i] == OSIF_TASK_END_MARKER) {
            sTaskLevelCount++;
            if( sTaskLevelCount >= OSIF_TASKLEVEL_MAXNUM
              ||(uint64_t)TaskRefs[i+1] == OSIF_TASK_END_MARKER) {
                break;
            }
        } else {
            uint64_t CurrentTaskValue = (uint64_t)TaskRefs[i];
            pTaskLevel = &sTaskLevels[sTaskLevelCount];
            if(pTaskLevel->bSettled == 0) {
                pTaskLevel->bSettled = 1;
                pTaskLevel->task_refs = &TaskRefs[i];
                pTaskLevel->Min = CurrentTaskValue;
                pTaskLevel->Max = CurrentTaskValue;
            } else {
                if(CurrentTaskValue < pTaskLevel->Min) {
                    pTaskLevel->Min = CurrentTaskValue;
                }
                if(CurrentTaskValue > pTaskLevel->Max) {
                    pTaskLevel->Max = CurrentTaskValue;
                }
            }
        }
    }
    return OSIF_RET_OK;
}

static int __OSIF_GetTaskLevelNumber(OSIF_TaskRef_t task_ref, int *level)
{
    int Level;
    if(level == NULL) {
        return OSIF_RET_ERROR_PARAM;
    }
    for(Level=0; Level<sTaskLevelCount; Level++) {
        struct task_level *pTaskLevel = &sTaskLevels[Level];
        if( (uint64_t)task_ref >= pTaskLevel->Min
          &&(uint64_t)task_ref <= pTaskLevel->Max) {
            OSIF_TaskRef_t *p2TaskRef = pTaskLevel->task_refs;
            while((uint64_t)(*p2TaskRef) != OSIF_TASK_END_MARKER) {
                if((uint64_t)(*p2TaskRef) == (uint64_t)task_ref) {
                    *level = Level;
                    return OSIF_RET_OK;
                }
                p2TaskRef++;
            }
        }
    }
    return OSIF_RET_OK;
}

static inline OSIF_Task_t * __OSIF_GetTaskByHandle(void *handle)
{
    OSIF_Task_t *cur_task=NULL;
    for(int i=0;;i++) {
        if((uint64_t)gpTaskLevelTable[i] == OSIF_TASK_END_MARKER) {
            if((uint64_t)gpTaskLevelTable[i+1] == OSIF_TASK_END_MARKER) {
                break;
            }
        } else {
            OSIF_Task_t *task = gpTaskLevelTable[i];
            if(task->stack == handle) {
                cur_task = task;
                break;
            }
        }
    }
    return cur_task;
}

//====================================================================//
void OSIF_Init(void)
{
    g_osif_inside_isr = 0;
    __OSIF_TaskTableInit(gpTaskLevelTable);
}

void OSIF_Start(void)
{
    vTaskStartScheduler();
    while(1);
}

void OSIF_CurrentTaskSetPrivData(void *data)
{
    void *handle = xTaskGetCurrentTaskHandle();
    if(handle) {
        __OSIF_GetTaskByHandle(handle)->priv_data = data;
    }
}

void *OSIF_CurrentTaskGetPrivData(void)
{
    void *handle = xTaskGetCurrentTaskHandle();
    if(handle) {
        return __OSIF_GetTaskByHandle(handle)->priv_data;
    } else {
        return NULL;
    }
}

OSIF_Task_t* OSIF_GetCurrentTask(void)
{
    void *handle = xTaskGetCurrentTaskHandle();
    return __OSIF_GetTaskByHandle(handle);
}

void*  OSIF_TaskCreate(OSIF_Task_t *task, int *err)
{
    int ret, TaskLevel;
    UBaseType_t uxStackSizeBytes;
    UBaseType_t uxPriority;
    StackType_t *pxStack;
    StaticTask_t *pxTCB;

    if(task == NULL || task->handler==NULL || task->stack==NULL) {
        if(err) {
            *err = OSIF_RET_ERROR_PARAM;
        }
        goto __error_exit;
    }
    TaskLevel = 0;
    ret = __OSIF_GetTaskLevelNumber(task, &TaskLevel);
    if(ret != OSIF_RET_OK) {
        if(err) {
            *err = ret;
        }
        goto __error_exit;
    }
    task->priv_data = NULL;
    pxTCB = (StaticTask_t*)task->stack;
    uxStackSizeBytes = (UBaseType_t)(task->StackSize_byte - ((uint64_t)pxTCB - (uint64_t)task->stack));
    pxStack = (StackType_t*)ALIGNMENT_UP(((uint64_t)pxTCB+sizeof(StaticTask_t)), sizeof(StackType_t));
    uxStackSizeBytes -= (uint64_t)pxStack - (uint64_t)pxTCB;
    uxPriority = OSIF_TASKLEVEL_MAXNUM - TaskLevel;
#if OSIF_TASK_VIEW_EN
    task->Priv.TaskLevel = TaskLevel;
    task->Priv.NumOfStkCellTotal = uxStackSizeBytes/sizeof(StackType_t);
    task->Priv.stack_cell        = (uint64_t*)pxStack;
    for(uint32_t i=0; i<task->Priv.NumOfStkCellTotal; i++) {
        task->Priv.stack_cell[i] = OSIF_STK_FREECELL_FLAG;
    }
#endif
    xTaskCreateStatic((TaskFunction_t)task->handler, task->name,
                         uxStackSizeBytes/sizeof(StackType_t),  //Task's Stack Depth
                         task->arg,
                         uxPriority,
                         pxStack,
                         pxTCB);
    return (void*)task;
__error_exit:
    return NULL;
}

void OSIF_TaskTableCreate(OSIF_TaskRef_t TaskTable[])
{
    int TaskLevel;
    UBaseType_t uxStackSizeBytes;
    UBaseType_t uxPriority;
    StackType_t *pxStack;
    StaticTask_t *pxTCB;

    if((uint64_t)TaskTable[0] == OSIF_TASK_END_MARKER) {
        return;
    }
    TaskLevel =0;
    for(int i=0;;i++) {
        if((uint64_t)TaskTable[i] == OSIF_TASK_END_MARKER) {
            if((uint64_t)TaskTable[i+1] == OSIF_TASK_END_MARKER) {
                break;
            }
            TaskLevel++;
        } else {
            OSIF_Task_t *task = TaskTable[i];
            pxTCB = (StaticTask_t*)task->stack;
            uxStackSizeBytes = (UBaseType_t)(task->StackSize_byte - ((uint64_t)pxTCB - (uint64_t)task->stack));
            pxStack = (StackType_t*)ALIGNMENT_UP(((uint64_t)pxTCB+sizeof(StaticTask_t)), sizeof(StackType_t));
            uxStackSizeBytes -= (uint64_t)pxStack - (uint64_t)pxTCB;
            uxPriority = OSIF_TASKLEVEL_MAXNUM - TaskLevel;
        #if OSIF_TASK_VIEW_EN
            task->Priv.TaskLevel = TaskLevel;
            task->Priv.NumOfStkCellTotal = uxStackSizeBytes/sizeof(StackType_t);
            task->Priv.stack_cell        = (uint64_t*)pxStack;
            for(uint32_t i=0; i<task->Priv.NumOfStkCellTotal; i++) {
                task->Priv.stack_cell[i] = OSIF_STK_FREECELL_FLAG;
            }
        #endif
            xTaskCreateStatic((TaskFunction_t)task->handler, task->name,
                             uxStackSizeBytes/sizeof(StackType_t),  //Task's Stack Depth
                             task->arg,
                             uxPriority,
                             pxStack,
                             pxTCB);
        }
    }
}

void OSIF_TaskTableStackView(OSIF_TaskRef_t TaskTable[])
{
    if((uint64_t)TaskTable[0] == OSIF_TASK_END_MARKER) {
        return;
    }
    for(int i=0;;i++) {
        if((uint64_t)TaskTable[i] == OSIF_TASK_END_MARKER) {
            if((uint64_t)TaskTable[i+1] == OSIF_TASK_END_MARKER) {
                break;
            }
        } else {
            OSIF_Task_t *task = TaskTable[i];
            task->Priv.NumOfStkCellUsed = 0;
            uint32_t freeCnt=0;
            for(freeCnt=0; freeCnt < task->Priv.NumOfStkCellTotal; freeCnt++) {
                if(task->Priv.stack_cell[freeCnt] != OSIF_STK_FREECELL_FLAG) {
                    break;
                }
            }
            task->Priv.NumOfStkCellUsed = task->Priv.NumOfStkCellTotal - freeCnt ;
            task->Priv.NumOfStkCellUsed += sizeof(StaticTask_t)/sizeof(StackType_t);
        }
    }
}

void OSIF_TaskDelete(OSIF_TaskHandle xTaskToDelete)
{
    vTaskDelete(xTaskToDelete);
}
//--------------------------//
void *OSIF_MessagePoolCreate(OSIF_MessagePoolConstructor_t *constructor, void *buffer, uint32_t BufferSize)
{
    return (void*)xMessageBufferCreateStatic(BufferSize , buffer, (StaticMessageBuffer_t*)constructor);
}

int OSIF_MessagePoolTake(void *handle, void *msg_data, uint32_t *msg_length, uint32_t MaxLength, uint32_t TicksToWait)
{
    size_t length;

    length = xMessageBufferReceive((MessageBufferHandle_t*)handle, msg_data, MaxLength, TicksToWait);
    if(length == 0) {
        return OSIF_RET_TIMEOUT;
    } else {
        if(msg_length) {
            *msg_length = length;
        }
        return OSIF_RET_OK;
    }
}

int OSIF_MessagePoolGive(void *handle, void *msg_data, uint32_t MsgLength, uint32_t TicksToWait)
{
    size_t length;

    if(__OSIF_IsIsrContext()) {
        BaseType_t HigherPriorityTaskWoken = pdFALSE;
        length = xMessageBufferSendFromISR( (MessageBufferHandle_t*)handle, msg_data, MsgLength, &HigherPriorityTaskWoken);
        portYIELD_FROM_ISR(HigherPriorityTaskWoken);
    } else {
        length = xMessageBufferSend((MessageBufferHandle_t*)handle, msg_data, MsgLength, TicksToWait);
    }
    if(length == 0) {
        return OSIF_RET_MESSAGEPOOL_FULL;
    } else {
        return OSIF_RET_OK;
    }
}

//--------------------------//
void *OSIF_MessageQCreate(OSIF_MessageQConstructor_t *constructor, OSIF_MessageQContainer_t *container)
{
    return (void*)xQueueCreateStatic(container->NumberOfMessage, container->MessageSize_byte, (uint8_t*)container->MessageArray, (StaticQueue_t*)constructor);
}

int OSIF_MessageQTake(void *handle, void *msg, uint32_t TicksToWait)
{
    if(xQueueReceive((QueueHandle_t)handle, msg, TicksToWait ) == pdPASS) {
        return OSIF_RET_OK;
    } else {
        return OSIF_RET_TIMEOUT;
    }
}

int OSIF_MessageQGive(void *handle, void *msg, uint32_t TicksToWait, bool IsTakenAsFirst)
{
    int ret;
    if(__OSIF_IsIsrContext()) {
        BaseType_t HigherPriorityTaskWoken = pdFALSE;
        if(IsTakenAsFirst == true) {
            ret = xQueueSendToFrontFromISR((QueueHandle_t)handle, msg, &HigherPriorityTaskWoken);
        } else {
            ret = xQueueSendToBackFromISR((QueueHandle_t)handle, msg, &HigherPriorityTaskWoken);
        }
        portYIELD_FROM_ISR(HigherPriorityTaskWoken);
    } else {
        if(IsTakenAsFirst == true) {
            ret = xQueueSendToFront((QueueHandle_t)handle, msg, TicksToWait);
        } else {
            ret = xQueueSendToBack((QueueHandle_t)handle, msg, TicksToWait);
        }
    }
    return ret == pdPASS ? OSIF_RET_OK : OSIF_RET_MESSAGEQ_FULL;
}

void *OSIF_TimerCreate(OSIF_TimerConstructor_t *constructor, OSIF_TimerContainer_t *container)
{
    return (void*)xTimerCreateStatic(container->ptimer_name, container->period_in_ticks, container->auto_load,
            container->timer_id, (TimerCallbackFunction_t)container->pfunc_call_back, (StaticTimer_t*)constructor);
}

int OSIF_TimerStart(void *xTimer)
{
    int ret;
    if(__OSIF_IsIsrContext()) {
        BaseType_t HigherPriorityTaskWoken;
        ret = xTimerStartFromISR((TimerHandle_t*)xTimer, &HigherPriorityTaskWoken);
        portYIELD_FROM_ISR(HigherPriorityTaskWoken);
    } else {
        ret = xTimerStart((TimerHandle_t*)xTimer, 0);
    }
    return ret == pdPASS ? OSIF_RET_OK : OSIF_RET_TIMER_FAILED;
}

int OSIF_TimerStop(void *xTimer)
{
    int ret;
    if(__OSIF_IsIsrContext()) {
        BaseType_t HigherPriorityTaskWoken;
        ret = xTimerStopFromISR((TimerHandle_t*)xTimer, &HigherPriorityTaskWoken);
        portYIELD_FROM_ISR(HigherPriorityTaskWoken);
    } else {
        ret = xTimerStop((TimerHandle_t*)xTimer, 0);
    }
    return ret == pdPASS ? OSIF_RET_OK : OSIF_RET_TIMER_FAILED;
}

int OSIF_MessageQAddToSet(void *handle, void *Qset)
{
    return xQueueAddToSet((QueueSetMemberHandle_t)handle, (QueueSetHandle_t)Qset);
}

void *OSIF_MessageQSeletFromSet(void *Qset, uint32_t TicksToWait)
{
    return (void*)xQueueSelectFromSet((QueueSetHandle_t)Qset,TicksToWait);
}

//--------------------------//
