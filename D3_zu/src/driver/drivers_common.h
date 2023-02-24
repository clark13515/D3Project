/*** 
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\driver\drivers_common.h
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-29 15:45:59
 * @LastEditTime: 2023-01-29 15:50:42
 */
#ifndef DRIVERS_COMMON_H
#define DRIVERS_COMMON_H

#include "dma_intr.h"
#include "gpiopl_intr.h"
#include "sdcard.h"
#include "sys_intr.h"

extern XAxiDma AxiDma;
extern XScuGic xInterruptController; //GIC


#endif