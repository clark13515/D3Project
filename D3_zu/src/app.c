/*
 * @Copyright: Copyright (C) by Xx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\app.c
 * @PROJECT: P4
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2022-06-04 18:45:53
 * @LastEditTime: 2023-01-29 16:04:43
 */
#include "osif.h"
#include "task_test.h"
#include "task_adcps_send.h"
#include "task_init.h"
#include "task_DMAtrans.h"
#include "task_sdcard.h"
#include "service_ethernet.h"
#include "utility.h"
#include "utilities_plat_common.h"



OSIF_TASKS_DECLARE_BEGIN(CurrentTaskTable)

OSIF_TASKS_ITEMS (&g_service_ethernet[0])
OSIF_TASKS_ITEMS (&g_task_init)
OSIF_TASKS_ITEMS( &g_task_test )
OSIF_TASKS_ITEMS (&g_task_dmatrans)
OSIF_TASKS_ITEMS( &g_task_adcps_send )
OSIF_TASKS_ITEMS( &g_task_sdcard )

OSIF_TASKS_DECLARE_END(CurrentTaskTable)



void APP_Init(void)
{
    ModuleInitcall(MODULE_INIT_APP);
    ModuleInitcall(MODULE_INIT_AAPP);
    //-------------------------------------------//
    OSIF_TaskTableCreate(CurrentTaskTable);
}
