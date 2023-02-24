/*
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\app\task_test.c
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-25 11:58:00
 * @LastEditTime: 2023-01-29 15:32:47
 */

#include <string.h>
#include "FreeRTOS.h"
#include "osif.h"
#include "task.h"
#include "semphr.h"
#include "sys_manager.h"
#include "task.h"            /*!<  FreeRTOS related header files. */
#include "queue.h"           /*!<  FreeRTOS related header files. */
#include "timers.h"          /*!<  FreeRTOS related header files. */
#include "sdcard.h"

#define APP_TEST_HANDLER_MSG_DEPTH (4)
#define TEST_HANDLER_MEMORY_SIZE   (64)
#define TEST_HANDLER_TASK_PRIORITY (6)
#define BIT_0                       (1 << 0)
#define BIT_1                       (1 << 1)
#define BIT_2                       (1 << 2)


static TaskHandle_t       pTestHandlerTaskHandle = NULL;

static void task_test_init(void);

void AppTestHandlerThread(void *pvParameters)
{
	task_test_init();
    while (1)
    {
        /**do noting*/
        xil_printf("in test task \n\r");

        OSIF_TimeDelay(1000);
    }
}

static void task_test_init(void)
{

   return;
}


#define TASK_TEST_STACK_SIZE  4096*2
static __attribute__((aligned(4))) uint8_t task_test_stack[TASK_TEST_STACK_SIZE];
OSIF_Task_t g_task_test= {
        .name = "task_test",
        .handler = AppTestHandlerThread,
        .arg = NULL,
        .stack = (void*)task_test_stack,
        .StackSize_byte = TASK_TEST_STACK_SIZE,
};


BDEV_MODULE_INIT_DECLARE(task_test_init);
