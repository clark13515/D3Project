/*
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\middleware\sys_manager\Event.c
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-25 11:31:35
 * @LastEditTime: 2023-01-25 11:38:40
 */

//-------------------------------------------------------------------------------------------------
// External component include
//-------------------------------------------------------------------------------------------------
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"
#include "sys_manager.h"

//-------------------------------------------------------------------------------------------------
// Internal include
//-------------------------------------------------------------------------------------------------
#include "Event.h"

//-------------------------------------------------------------------------------------------------
// Public Defintions
//-------------------------------------------------------------------------------------------------
#define MAX_EVENTS                   24
#define EVENT_THREAD_STACK_SIZE      1024
#define EVNET_THREAD_PRIORITY_OFFSET (2)

void Event_Init(void);
//COMP_REGISTER(EVENT, Event_Init, START_PRIO_CORE);

//-------------------------------------------------------------------------------------------------
// Private Members
//-------------------------------------------------------------------------------------------------
typedef struct EventSubCtl_t
{
    EventCB               EventCB;
    struct EventSubCtl_t *pNextEventSubCtl;
} EventSubCtl_t;

static xSemaphoreHandle EventLock = NULL;

static EventGroupHandle_t xAnyEventGroup;
static EventGroupHandle_t xAllEventGroup[MAX_EVENTS];

static EventSubCtl_t *pAnyEventSubCtl[MAX_EVENTS];
static EventSubCtl_t *pAllEventSubCtl[MAX_EVENTS];

static unsigned long AllEventMask[MAX_EVENTS] = { 0 };
static unsigned long AnyEventMask             = 0;

//-------------------------------------------------------------------------------------------------
// Private Functions
//-------------------------------------------------------------------------------------------------
static unsigned long GetEventIdx(Event_t event);
static void          WaitEvents(void);
static void          vEventThread(void *pvParameters);
static void          EventInit(void);
static void          RunEventCB(unsigned long EventBit);

/**
 * @brief  Get the Event Index
 * @param  event 
 * @return unsigned long 
 */
static unsigned long GetEventIdx(Event_t event)
{
    unsigned long i = 0;

    for (i = 0; i < MAX_EVENTS; i++)
    {
        if ((1U << i) & event)
        {
            break;
        }
    }

    return i;
}

/**
 * @brief  Run event callback functions
 * @param  EventBit: index bit to the event group
 * @return unsigned long
 */
static void RunEventCB(unsigned long EventBit)
{
    EventSubCtl_t *pEventSubCtl = NULL;
    EventBits_t    uxAllBits;

    pEventSubCtl = pAnyEventSubCtl[GetEventIdx(1U << EventBit)];

    while (pEventSubCtl != NULL)
    {
        if (pEventSubCtl->EventCB != NULL)
        {
            pEventSubCtl->EventCB(EVENT_SUB_ANY, (1U << EventBit));
        }

        pEventSubCtl = pEventSubCtl->pNextEventSubCtl;
    }

    uxAllBits = xEventGroupGetBits(xAllEventGroup[GetEventIdx(1U << EventBit)]);

    unsigned long mask = AllEventMask[GetEventIdx(1U << EventBit)];
    unsigned long j    = 0;
    while (mask != 0)
    {
        if (((uxAllBits >> j) & 1U) == 0)
        {
            break;
        }

        j++;
        mask >>= 1;
    }

    if (mask == 0)
    {
        pEventSubCtl = pAllEventSubCtl[GetEventIdx(1U << EventBit)];

        while (pEventSubCtl != NULL)
        {
            if (pEventSubCtl->EventCB != NULL)
            {
                pEventSubCtl->EventCB(EVENT_SUB_ALL, (1U << EventBit));
            }

            pEventSubCtl = pEventSubCtl->pNextEventSubCtl;
        }

        // clear all events
        xEventGroupClearBits(xAllEventGroup[GetEventIdx(1U << EventBit)], AllEventMask[GetEventIdx(1U << EventBit)]);
    }
}

/**
 * @brief  Wait for events to be set and call user callback functions based on each event status
 */
static void WaitEvents(void)
{
    EventBits_t      uxAnyBits;
    const TickType_t xTicksToWait = 100 / portTICK_PERIOD_MS;
    unsigned long    i            = 0;

    // xEventGroupWaitBits can't wait on zero bits
    if (AnyEventMask != 0)
    {
        /* Wait a maximum of 100ms for either bit to be set within
        the event group.  Clear the bits before exiting. */
        uxAnyBits = xEventGroupWaitBits(xAnyEventGroup,  // The any event group being tested.
                                        AnyEventMask,    // The bits within the event group to wait for.
                                        pdTRUE,          // Auto clear bits on exit of this function.
                                        pdFALSE,         // Wait on any bit.
                                        xTicksToWait);   // Wait a maximum of 100ms for either bit to be set.
        // The bits within the event group to wait for is refreshed after the timeout.
        // Another solution is to reserve a bit for xAnyEventGroup refreshing.

        for (i = 0; i < MAX_EVENTS; i++)
        {
            if (uxAnyBits & (1U << i))
            {
                RunEventCB(i);
            }
        }
    }
    else
    {
        vTaskDelay(10);
    }
}

/**
 * @brief  System event initialization
 */
static void EventInit(void)
{
    unsigned long i = 0;

    // Attempt to create a semaphore.
    EventLock = xSemaphoreCreateBinary();

    configASSERT(EventLock != NULL)

        // So give it at the first time
        xSemaphoreGive(EventLock);

    // Attempt to create the Any event group.
    xAnyEventGroup = xEventGroupCreate();

    // Was the event group created successfully?
    configASSERT(xAnyEventGroup != NULL);

    for (i = 0; i < MAX_EVENTS; i++)
    {
        // Attempt to create the All event group.
        xAllEventGroup[i] = xEventGroupCreate();

        // Was the event group created successfully?
        configASSERT(xAllEventGroup[i] != NULL);
    }

    for (i = 0; i < MAX_EVENTS; i++)
    {
        pAnyEventSubCtl[i] = NULL;
        pAllEventSubCtl[i] = NULL;
    }
}

/**
 * @brief  System event processing task
 * @param  pvParameters: not used
 */
static void vEventThread(void *pvParameters)
{
    (void)pvParameters;

#ifndef UNIT_TEST
    for (;;)
#endif
    {
        WaitEvents();
    }
}

//-------------------------------------------------------------------------------------------------
// Public Functions
//-------------------------------------------------------------------------------------------------

/**
 * @brief  Declare an Event that will be set by a module/task
 * @param  event: event to be set later in the SetEvent() function
 * @return Event owner ID to be used in the SetEvent() function
 */
unsigned long DeclareEvent(Event_t event)
{
    unsigned long i = 0;
    unsigned long j = 0;

    AnyEventMask |= event;

    i = GetEventIdx(event);

    for (j = 0; j < MAX_EVENTS; j++)
    {
        if ((AllEventMask[i] & (1U << j)) == 0)
        {
            AllEventMask[i] |= (1U << j);
            break;
        }
    }

    // return event owner ID
    return (1U << j);
}

/**
 * @brief  Set an Event
 * @param  OwnerID: Owner ID return from DeclareEvent() function
 * @param  event: event to be set
 * @return EventError_t
 */
EventError_t SetEvent(unsigned long OwnerID, Event_t event)
{
    BaseType_t xHigherPriorityTaskWoken;
    BaseType_t xResult;

    if ((OwnerID >= (1U << MAX_EVENTS)) || (OwnerID == 0))
    {
        return EVENT_ERROR_FAILED;
    }

    if (xPortIsInsideInterrupt() == pdFALSE)
    {
        xEventGroupSetBits(xAllEventGroup[GetEventIdx(event)], OwnerID);
        xEventGroupSetBits(xAnyEventGroup, event);
    }
    else
    {
        // xHigherPriorityTaskWoken must be initialised to pdFALSE.
        xHigherPriorityTaskWoken = pdFALSE;

        xResult = xEventGroupSetBitsFromISR(xAllEventGroup[GetEventIdx(event)], OwnerID, &xHigherPriorityTaskWoken);

        // Was the message posted successfully?
        if (xResult != pdFAIL)
        {
            // If xHigherPriorityTaskWoken is now set to pdTRUE then a context
            // switch should be requested.  The macro used is port specific and will
            // be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
            // the documentation page for the port being used.
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }

        // xHigherPriorityTaskWoken must be initialised to pdFALSE.
        xHigherPriorityTaskWoken = pdFALSE;

        xResult = xEventGroupSetBitsFromISR(xAnyEventGroup, event, &xHigherPriorityTaskWoken);

        // Was the message posted successfully?
        if (xResult != pdFAIL)
        {
            // If xHigherPriorityTaskWoken is now set to pdTRUE then a context
            // switch should be requested.  The macro used is port specific and will
            // be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
            // the documentation page for the port being used.
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }

    return EVENT_ERROR_NONE;
}

/**
 * @brief  Subscribe to a event. when the event is set, the callback function will be called
 * @param  event: event to be subscribed
 * @param  EventSub: event subscription type, Any or All
 * @param  eventCB: callback function to be called when the subscribed event is set.
 * @return Subscription status
 */
int SubscribeEvent(Event_t event, EventSub_t EventSub, EventCB eventCB)
{
    EventSubCtl_t **ppEventSubCtl = NULL;

    if (eventCB == NULL)
    {
        return EVENT_ERROR_FAILED;
    }

    xSemaphoreTake(EventLock, portMAX_DELAY);

    if (EventSub == EVENT_SUB_ALL)
    {
        ppEventSubCtl = &pAllEventSubCtl[GetEventIdx(event)];
    }
    else if (EventSub == EVENT_SUB_ANY)
    {
        ppEventSubCtl = &pAnyEventSubCtl[GetEventIdx(event)];
    }
    else
    {
        xSemaphoreGive(EventLock);
        return EVENT_ERROR_FAILED;
    }

    while (*ppEventSubCtl != NULL)
    {
        ppEventSubCtl = &((*ppEventSubCtl)->pNextEventSubCtl);
    }

    *ppEventSubCtl = pvPortMalloc(sizeof(EventSubCtl_t));

    configASSERT(*ppEventSubCtl != NULL);

    (*ppEventSubCtl)->EventCB          = eventCB;
    (*ppEventSubCtl)->pNextEventSubCtl = NULL;

    xSemaphoreGive(EventLock);

    return EVENT_ERROR_NONE;
}

/**
 * @brief  System Event initialization
 *
 */
void Event_Init(void)
{
    BaseType_t xReturn;
    EventInit();

    xReturn = xTaskCreate(&vEventThread,
                          "Event",
                          EVENT_THREAD_STACK_SIZE,
                          NULL,
                          ((UBaseType_t)(configMAX_PRIORITIES - EVNET_THREAD_PRIORITY_OFFSET)) | portPRIVILEGE_BIT,
                          NULL);

    configASSERT(xReturn == pdPASS);
}