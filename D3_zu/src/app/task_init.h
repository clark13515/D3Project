/*** 
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\app\task_init.h
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-27 15:47:50
 * @LastEditTime: 2023-01-27 17:42:13
 */

#ifndef TASKS_TASK_INIT_H_
#define TASKS_TASK_INIT_H_

#include "osif.h"
#include "netif/xadapter.h"
#include "lwip/init.h"
#include "lwip/inet.h"
#include "stdint.h"

extern OSIF_Task_t g_task_init;

typedef struct 
{
    struct netif *netif_handle;
    uint8_t       netif_init_flag;
} eth_netif_info_t;

extern eth_netif_info_t* task_init_eth_get_netif_info(void);

#endif /* P4_APP_A53_0_TASKS_TASK_INIT_H_ */
