/*
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\app\task_ADCPS_send.c
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-26 18:37:13
 * @LastEditTime: 2023-02-16 20:45:38
 */


#include "task_adcps_send.h"
#include "osif.h"
#include "netif/xadapter.h"
#include "lwip/init.h"
#include "lwip/inet.h"
#include "lwip/dhcp.h"
#include "stdbool.h"
#include "protocol_adcompass.h"
#include "dma_intr.h"
#include "xaxidma.h"

#define TASK_ADCPS_SEND_INFO_QUEUE_DEPTH  (256)

static dma_rx_buffer_info_t*      g_task_adcps_send_info_queue_buffer[TASK_ADCPS_SEND_INFO_QUEUE_DEPTH];
static OSIF_MessageQConstructor_t g_task_adcps_send_info_queue_constructor;
static OSIF_MessageQContainer_t   g_task_adcps_send_info_queue_container =
{
    .NumberOfMessage = sizeof(g_task_adcps_send_info_queue_buffer)/sizeof(dma_rx_buffer_info_t*),
    .MessageSize_byte = sizeof(dma_rx_buffer_info_t *),
    .MessageArray = g_task_adcps_send_info_queue_buffer,
};
static void *g_task_adcps_send_info_handle = NULL;



static protocol_adcps_ps_pl_t      g_protocol_adcps_ps_pl;
static protocol_adcps_ps_pl_send_t g_protocol_adcps_ps_pl_send;

uint8_t tmp_send_buff[800] = {0, 0, 0, 0, 100, 0,0,0,0, 0,0,0,0, 1, 0, 00, 01, 00, 00, 00};

static void task_adcps_send_init(void)
{
    protocol_adcps_send_init();

    g_task_adcps_send_info_handle = OSIF_MessageQCreate(&g_task_adcps_send_info_queue_constructor, &g_task_adcps_send_info_queue_container);

}



void task_adcps_send_disable(void)
{
    // clear messageQ
    if(g_task_adcps_send_info_handle)
    {
        xQueueReset(g_task_adcps_send_info_handle);
    }
    protocol_adcps_send_disable();

}

void task_adcps_send_enable(void)
{
    // clear messageQ 
    if(g_task_adcps_send_info_handle)
    {
        xQueueReset(g_task_adcps_send_info_handle);
    }
    protocol_adcps_send_enable();
}


int task_adcps_send_give_rx_buffer_info(dma_rx_buffer_info_t **rx_buffer)
{
   int ret = 0;
   if ((g_task_adcps_send_info_handle != NULL) && (rx_buffer != NULL))
   {
       ret = OSIF_MessageQGive(g_task_adcps_send_info_handle, rx_buffer, 0, false);
   }
   else
   {
       ret = -1;
   }
   return ret;
}

static void task_adcps_send_handler(void* arg)
{
    (void)arg;
    task_adcps_send_init();
    dma_rx_buffer_info_t *p_rx_buffer_info = NULL;
    protocol_adcps_ps_pl_send_t adcps_ps_pl_send;

    for(;;)

    {

         OSIF_MessageQTake(g_task_adcps_send_info_handle, &p_rx_buffer_info, OSIF_WAIT_FOREVER);
         
        if(p_rx_buffer_info)
        {
            if(p_rx_buffer_info ->status == RX_BUFFER_FULL)
            {
                adcps_ps_pl_send.send_data = p_rx_buffer_info->addr;
                adcps_ps_pl_send.send_length = DMA_PACKGE_SIZE;

                protocol_adcps_send( &adcps_ps_pl_send);
                // xil_printf("********************data to send : **************************\n\r");
                // for(int i =  0 ; i < 256 ;i++ )
                // {
                //     xil_printf("[%d]", *(uint8_t*)(p_rx_buffer_info->addr+i));
                // }
                // xil_printf("********************************************\n\r");
                
                p_rx_buffer_info ->status = RX_BUFFER_EMPTY;
            }

        }
         

    	vTaskDelay(5);
    }
}

#define TASK_ADCPS_SEND_STACK_SIZE  8192
static FORCE_ALIGNED_INBYTE(4) uint8_t task_adcps_send_stack[TASK_ADCPS_SEND_STACK_SIZE];
OSIF_Task_t g_task_adcps_send = {
        .name = "task_adcps",
        .handler = task_adcps_send_handler,
        .arg = NULL,
        .stack = (void*)task_adcps_send_stack,
        .StackSize_byte = TASK_ADCPS_SEND_STACK_SIZE,
};

