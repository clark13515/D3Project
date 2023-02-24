/*** 
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\app\task_sdcard.h
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-29 15:14:12
 * @LastEditTime: 2023-01-29 15:15:55
 */
#ifndef TASK_SDCARD_H
#define TASK_SDCARD_H

#include "osif.h"

extern OSIF_Task_t g_task_sdcard;
extern task_sdcard_send_give_rx_buffer_info(dma_rx_buffer_info_t **rx_buffer);
#endif