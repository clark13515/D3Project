/*** 
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_udp_proj\src\gpiopl_intr.h
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-27 15:04:43
 * @LastEditTime: 2023-01-27 15:18:23
 */
/*
 * gpiopl_intr.h
 *
 *  Created on: 2017锟斤拷2锟斤拷16锟斤拷
 * www.osrc.cn
 * www.milinker.com
 * copyright by nan jin mi lian dian zi www.osrc.cn
 */

#ifndef GPIOPL_INTR_H_
#define GPIOPL_INTR_H_

#include <stdio.h>

#include "xscugic.h"
#include "xil_types.h"
#include "xparameters.h"
#include "xgpio.h"

#include "sleep.h"


#define AXI_GPIO0_DEV_ID	        XPAR_AXI_GPIO_0_DEVICE_ID   //Gpio_gain
#define AXI_GPIO1_DEV_ID	        XPAR_AXI_GPIO_1_DEVICE_ID	//Gpio_gesture
#define AXI_GPIO2_DEV_ID	        XPAR_AXI_GPIO_2_DEVICE_ID	//Gpio_PLctrl
#define AXI_GPIO3_DEV_ID	        XPAR_AXI_GPIO_3_DEVICE_ID	//Gpio_PHYctl
#define AXI_GPIO4_DEV_ID	        XPAR_AXI_GPIO_4_DEVICE_ID	//Gpio_SP

#define GPIO_INTR_MASK          0x00000001

#define TOUCH_INTR_MASK         0x00000001
#define BUTTON0_INTR_MASK       0x00000002
#define BUTTON1_INTR_MASK       0x00000004


#define DATA_DIRECTION_MASK_O    0x00000000
#define DATA_DIRECTION_MASK_I    0xFFFFFFFF







int Gpiopl_init(XGpio *InstancePtr, u32 DeviceId, u32 DirectionMask);

void PL_reset(XGpio *InstancePtr);
void PL_GAIN(u32 Data) ;	//锟斤拷锟斤拷锟斤拷锟�
void PHY_CTRL(u32 Data)	;//phy ctrl
void PS_DA_DATA_READY(u32 Data);
int PL_READY(u32 Data);

#endif /* GPIOPL_INTR_H_ */
