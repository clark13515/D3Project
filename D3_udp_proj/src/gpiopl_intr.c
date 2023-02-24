/*
 * gpiopl_intr.c
 *
 *  Created on: 2017锟斤拷2锟斤拷16锟斤拷
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

void PL_reset(XGpio *InstancePtr)   //锟斤拷位锟脚猴拷
{
	XGpio_DiscreteWrite(InstancePtr, 1, 0);
	usleep(5);
	XGpio_DiscreteWrite(InstancePtr, 1, 1);
}

void PL_GAIN(u32 Data) //锟斤拷锟斤拷锟斤拷锟�
{
	XGpio_DiscreteWrite(&Gpio_gain, 1, Data);

}

void PHY_CTRL(u32 Data)	//phy ctrl
{
	XGpio_DiscreteWrite(&Gpio_PHYctl, 1, Data);
}

//void PS_DA_DATA_READY(u32 Data)//PS锟斤拷锟斤拷锟斤拷准锟斤拷锟斤拷
//{
//	XGpio_DiscreteWrite(&Gpio_PS2PL, 1, Data);
//}
//
//int PL_READY(u32 Data)//PL锟斤拷锟叫空硷拷锟斤拷锟斤拷锟斤拷锟�
//{
//	Data = XGpio_DiscreteRead(&Gpio_PL2PS, 1);
//	return Data;
//}

