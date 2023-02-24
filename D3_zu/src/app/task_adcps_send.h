/*** 
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\app\task_adcps_send.h
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-27 17:41:33
 * @LastEditTime: 2023-01-27 17:42:36
 */

#ifndef TASK_ADCPS_SEND_H
#define TASK_ADCPS_SEND_H
#include "osif.h"
#include "dma_intr.h"

extern OSIF_Task_t g_task_adcps_send;
extern int task_adcps_send_give_rx_buffer_info(dma_rx_buffer_info_t **rx_buffer);
#endif
