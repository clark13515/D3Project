/*** 
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\middleware\sys_manager\Event.h
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-25 11:31:35
 * @LastEditTime: 2023-01-25 11:38:45
 */


#ifndef __EVENT_H__
#define __EVENT_H__

//-------------------------------------------------------------------------------------------------
// Public Defintions
//-------------------------------------------------------------------------------------------------

/**
 * @brief  System and User Event type
 */
typedef enum Event_tag
{
    EVENT_STARTED         = (1U << 0),
    EVENT_SLEEP           = (1U << 1),
    EVENT_WAKE_UP         = (1U << 2),
    EVENT_RESTART_REQUEST = (1U << 3),
    EVENT_RESTART         = (1U << 4),
    EVENT_LOW_POWER       = (1U << 5),

    EVENT_SYS_RESERVED_1 = (1U << 6),
    EVENT_SYS_RESERVED_2 = (1U << 7),
    EVENT_SYS_RESERVED_3 = (1U << 8),
    EVENT_SYS_RESERVED_4 = (1U << 9),
    EVENT_SYS_RESERVED_5 = (1U << 10),
    EVENT_SYS_RESERVED_6 = (1U << 11),
    EVENT_SYS_RESERVED_7 = (1U << 12),
    EVENT_SYS_RESERVED_8 = (1U << 13),

    EVENT_USER_1  = (1U << 14),
    EVENT_USER_2  = (1U << 15),
    EVENT_USER_3  = (1U << 16),
    EVENT_USER_4  = (1U << 17),
    EVENT_USER_5  = (1U << 18),
    EVENT_USER_6  = (1U << 19),
    EVENT_USER_7  = (1U << 20),
    EVENT_USER_8  = (1U << 21),
    EVENT_USER_9  = (1U << 22),
    EVENT_USER_10 = (1U << 23)
} Event_t;

/**
 * @brief  API return Error code
 */
typedef enum EventError_tag
{
    EVENT_ERROR_NONE   = 0,
    EVENT_ERROR_FAILED = 1
} EventError_t;

/**
 * @brief  Event subscrition type.
 */
typedef enum EventSub_tag
{
    EVENT_SUB_ANY = 1U,
    EVENT_SUB_ALL = 2U
} EventSub_t;

/**
 * @brief  User callback function prototype.
 */
typedef void (*EventCB)(EventSub_t EventSub, Event_t event);

//-------------------------------------------------------------------------------------------------
// Public Functions
//-------------------------------------------------------------------------------------------------

/**
 * @brief  Declare an Event that will be set by a module/task
 * @param  event: event to be set later in the SetEvent() function
 * @return Event owner ID to be used in the SetEvent() function
 */
unsigned long DeclareEvent(Event_t event);

/**
 * @brief  Set an Event
 * @param  OwnerID: Owner ID return from DeclareEvent() function
 * @param  event: event to be set
 * @return Set status 
 */
EventError_t SetEvent(unsigned long OwnerID, Event_t event);

/**
 * @brief  Subscribe to a event. when the subscribed event is set, the callback function will be called
 * @param  event: event to be subscribed
 * @param  EventSub: event subscription type
 * @param  eventCB: callback function to be called when the subscribed event is set.
 * @return Subscription status
 */
int SubscribeEvent(Event_t event, EventSub_t EventSub, EventCB eventCB);

#endif
