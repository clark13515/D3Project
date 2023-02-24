/*
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\driver\gpiopl_intr.c
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-28 10:31:29
 * @LastEditTime: 2023-02-17 15:17:35
 */
/*
 * gpiopl_intr.c
 *
 *  Created on: 2017閿熸枻鎷�2閿熸枻鎷�16閿熸枻鎷�
 * www.osrc.cn
 * www.milinker.com
 * copyright by nan jin mi lian dian zi www.osrc.cn
 */

#include "gpiopl_intr.h"

XGpio Gpio_gain;	//
XGpio Gpio_gesture;
XGpio Gpio_PLctrl;	//PL control
XGpio Gpio_PHYctl;	//PHY control
XGpio Gpio_SP;	//SP
volatile u32 touch_flag;


volatile u32 gpio_output_state;

int Gpiopl_init(XGpio *InstancePtr, u32 DeviceId, u32 DirectionMask)
{

	int Status;
	/* Initialize AXI GPIO */
	Status = XGpio_Initialize(InstancePtr, DeviceId);
	if (Status != XST_SUCCESS) {
		xil_printf("AXI GPIO %d config failed!\r\n", DeviceId);
		return XST_FAILURE;
	}

	XGpio_SetDataDirection(InstancePtr, 1, DirectionMask);

	XGpio_DiscreteClear(InstancePtr, 1, 0);

    return 1;
}

int Gpiopl_initall(void)
{
	Gpiopl_init(&Gpio_PLctrl, AXI_GPIO0_DEV_ID, DATA_DIRECTION_MASK_O);
	Gpiopl_init(&Gpio_gesture, AXI_GPIO3_DEV_ID, DATA_DIRECTION_MASK_O);
	Gpiopl_init(&Gpio_gain, AXI_GPIO1_DEV_ID, DATA_DIRECTION_MASK_O);
	Gpiopl_init(&Gpio_SP, AXI_GPIO2_DEV_ID, DATA_DIRECTION_MASK_O);
//	Gpiopl_init(&Gpio_SP, AXI_GPIO4_DEV_ID, DATA_DIRECTION_MASK_O);}
}

void PL_reset(XGpio *InstancePtr)   //閿熸枻鎷蜂綅閿熻剼鐚存嫹
{
	XGpio_DiscreteWrite(InstancePtr, 1, 0);
	usleep(5);
	XGpio_DiscreteWrite(InstancePtr, 1, 1);
}

void PL_CTRL(u32 data)
{
	XGpio_DiscreteWrite(&Gpio_PLctrl ,1,data);
}

void PL_GESTURE(u32 data)
{
	XGpio_DiscreteWrite(&Gpio_gesture ,1,data);
}

void PL_GAIN(u32 Data) //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓锟�
{
	XGpio_DiscreteWrite(&Gpio_gain, 1, Data);

}

void PL_SP_CTRL(u32 Data)	//phy ctrl
{
	XGpio_DiscreteWrite(&Gpio_SP, 1, Data);
}

//void PS_DA_DATA_READY(u32 Data)//PS閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷峰噯閿熸枻鎷烽敓鏂ゆ嫹
//{
//	XGpio_DiscreteWrite(&Gpio_PS2PL, 1, Data);
//}
//
//int PL_READY(u32 Data)//PL閿熸枻鎷烽敓鍙┖纭锋嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓锟�
//{
//	Data = XGpio_DiscreteRead(&Gpio_PL2PS, 1);
//	return Data;
//}

