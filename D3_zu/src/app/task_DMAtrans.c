/*
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\app\task_DMAtrans.c
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-27 20:54:27
 * @LastEditTime: 2023-02-16 18:30:01
 */

/*
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\app\task_dmatrans.c
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-25 11:58:00
 * @LastEditTime: 2023-01-27 18:48:48
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
#include "dma_intr.h"
#include "sys_intr.h"
#include "gpiopl_intr.h"
#include "protocol_adcompass.h"
#include "task_adcps_send.h"
#include "task_sdcard.h"


#define APP_DMATRANS_HANDLER_MSG_DEPTH (4)
#define DMATRANS_HANDLER_MEMORY_SIZE   (64)
#define DMATRANS_HANDLER_TASK_PRIORITY (6)

volatile u32 RX_success;
volatile u32 TX_success;

volatile u32 RX_ready=1;
volatile u32 TX_ready=1;

int Tries = NUMBER_OF_TRANSFERS;
int i;
int Index;
u8 *TxBufferPtr= (u8 *)TX_BUFFER_BASE;
u8 *RxBufferPtr[2];
u8 Value=0;
float speed_tx;
float speed_rx;

XAxiDma AxiDma;

static TaskHandle_t       pDMATRANSHandlerTaskHandle = NULL;
static void task_dmatrans_init(void);
extern volatile int packet_trans_done;

static volatile dma_rx_buffer_info_t g_dma_rx_buffer_info[2];

void AppDMATRANSHandlerThread(void *pvParameters)
{
	int Status = 0;
	task_dmatrans_init();
	uint32_t TickCount =0;
    uint8_t  rxCnt =0;
    dma_rx_buffer_info_t *rx_buffer_info = NULL;

    while (1)
    {
        /**do noting*/

        TickCount++;

        if(RX_ready)
		{
		   RX_ready=0;
		   Status = XAxiDma_SimpleTransfer(&AxiDma,(u32)RxBufferPtr[rxCnt&1],(u32)(MAX_PKT_LEN), XAXIDMA_DEVICE_TO_DMA);

           rx_buffer_info->status = RX_BUFFER_FULL;

           rxCnt++;

           task_adcps_send_give_rx_buffer_info(&rx_buffer_info);
           task_sdcard_send_give_rx_buffer_info(&rx_buffer_info);

		   if (Status != XST_SUCCESS) {return XST_FAILURE;}
		}

		if(RxDone)
		{
            rx_buffer_info = &g_dma_rx_buffer_info[rxCnt&1];
			Xil_DCacheInvalidateRange((u32)RxBufferPtr[rxCnt&1], MAX_PKT_LEN);
            rx_buffer_info->status = RX_BUFFER_IN_RECV;
			RxDone=0;
			RX_ready=1;
			RX_success++;
		}
		if(TickCount > 100)
		{
            xil_printf("in dmatrans task \n\r");
			TickCount = 0;
	    	printf("RX=%d\r\n",RX_success);
			speed_rx = (float)MAX_PKT_LEN*RX_success/1024/1024;
			printf("RX_sp=%.2fMB/S",speed_rx);
			RX_success=0;
        }

		if (Error) {
			xil_printf("Failed test transmit%s done, "
			"receive%s done\r\n", TxDone? "":" not",
							RxDone? "":" not");
			//goto Done;
		}

		

        // Xil_DCacheInvalidateRange((INTPTR)(RxBufferPtr), DMA_PACKGE_SIZE);

        // Status = XAxiDma_SimpleTransfer(&AxiDma, (u32)RxBufferPtr[(rxCnt++)&1],
        // (u32)(DMA_PACKGE_SIZE), XAXIDMA_DEVICE_TO_DMA);

        // packet_trans_done = 0;
        
        
        OSIF_TimeDelay(10);
    }
}

static void task_dmatrans_init(void)
{
    RxBufferPtr[0] = (u32 *)RX_BUFFER0_BASE;
    RxBufferPtr[1] = (u32 *)RX_BUFFER1_BASE;

    g_dma_rx_buffer_info[0].addr = RxBufferPtr[0];
    g_dma_rx_buffer_info[1].addr = RxBufferPtr[1];
    return;
}


#define TASK_DMATRANS_STACK_SIZE  4096*2
static __attribute__((aligned(4))) uint8_t task_dmatrans_stack[TASK_DMATRANS_STACK_SIZE];
OSIF_Task_t g_task_dmatrans= {
        .name = "task_dmatrans",
        .handler = AppDMATRANSHandlerThread,
        .arg = NULL,
        .stack = (void*)task_dmatrans_stack,
        .StackSize_byte = TASK_DMATRANS_STACK_SIZE,
};


BDEV_MODULE_INIT_DECLARE(task_dmatrans_init);
