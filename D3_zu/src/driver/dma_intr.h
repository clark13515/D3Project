/*** 
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\driver\dma_intr.h
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-27 20:53:00
 * @LastEditTime: 2023-02-16 17:45:33
 */
/********************MILIANKE**************************

 *Company:Liyang Milian Electronic Technology Co., Ltd
 *Technical forum:www.uisrc.com
 *Taobao: https://milianke.taobao.com
 *Create Date: 2020/12/01
 *Module Name:dma_intr
 *Copyright: Copyright (c) milianke
 *Revision: 1.1
 *Description: dma Interrupt

/****************************************************/
#ifndef DMA_INTR_H
#define DMA_INTR_H
#include "xaxidma.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "xdebug.h"
#include "xscugic.h"

/************************** Constant Definitions *****************************/
/*
 * Device hardware build related constants.
 */
#define DMA_DEV_ID		XPAR_AXIDMA_0_DEVICE_ID

#define MEM_BASE_ADDR		0x10000000


#define RX_INTR_ID		121
//#define TX_INTR_ID		XPAR_FABRIC_AXI_DMA_0_MM2S_INTROUT_INTR


#define TX_BUFFER_BASE		(MEM_BASE_ADDR + 0x00100000)
#define RX_BUFFER_BASE		(MEM_BASE_ADDR + 0x00300000)
#define RX_BUFFER0_BASE     RX_BUFFER_BASE
#define RX_BUFFER1_BASE     (RX_BUFFER_BASE + 0x00020000)
#define RX_BUFFER_HIGH		(MEM_BASE_ADDR + 0x004FFFFF)


/* Timeout loop counter for reset
 */
#define RESET_TIMEOUT_COUNTER	10000
/* test start value
 */
#define TEST_START_VALUE	0xC
/*
 * Buffer and Buffer Descriptor related constant definition
 */
#define MAX_PKT_LEN		1056//1k

#define DMA_PACKGE_SIZE    (272)
/*
 * transfer times
 */
#define NUMBER_OF_TRANSFERS	100000
//XAxiDma AxiDma;
extern volatile int TxDone;
extern volatile int RxDone;
extern volatile int Error;

typedef enum
{
    RX_BUFFER_EMPTY   = 0,
    RX_BUFFER_IN_RECV = 1,
    RX_BUFFER_FULL    = 2,
} dma_rx_buffer_status_t;

#pragma pack(push)
#pragma pack(1)
typedef struct {
    UINTPTR addr;
    UINTPTR src;
    int status;
} dma_rx_buffer_info_t;
#pragma pack(pop)


void DMA_DisableIntrSystem(XScuGic * IntcInstancePtr, u16 RxIntrId);
int  DMA_CheckData(int Length, u8 StartValue);
int  DMA_Setup_Intr_System(XScuGic * IntcInstancePtr,XAxiDma * AxiDmaPtr, u16 RxIntrId);
int  DMA_Intr_Enable(XScuGic * IntcInstancePtr,XAxiDma *DMAPtr);
int  DMA_Intr_Init(XAxiDma *DMAPtr,u32 DeviceId);
#endif
