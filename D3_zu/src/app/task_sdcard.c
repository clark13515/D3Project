/*
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\app\task_sdcard.c
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-29 15:14:02
 * @LastEditTime: 2023-02-17 13:50:56
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
#include "sdcard.h"
#include "dma_intr.h"
#include "utilities_data_coversion.h"

#define APP_SDCARD_HANDLER_MSG_DEPTH (4)
#define SDCARD_HANDLER_MEMORY_SIZE   (64)
#define SDCARD_HANDLER_TASK_PRIORITY (6)
#define BIT_0                       (1 << 0)
#define BIT_1                       (1 << 1)
#define BIT_2                       (1 << 2)

#define    FILE_NAME    "testdata.txt"          //name

#define TASK_SDCARD_QUEUE_DEPTH  (256)
#define SDCARD_PACKGE_SIZE                   (272)
#define SDCARD_PACKGE_HEAD_LEN                  (8)
#define SDCARD_PACKGE_TAIL_LEN                  (4)
#define SDCARD_PACKGENUM_PER_WRITE (100)
#define SDCARD_FILE_NUM            (120)
#define SDCARD_MAX_FILE_NUM        (20)
#define SDCARD_MAX_FILE_SIZE       (1*1024*10244*1024)
#define SDCARD_TXT_BUF_SIZE        (300)        

static dma_rx_buffer_info_t*      g_task_sdcard_queue_buffer[TASK_SDCARD_QUEUE_DEPTH];
static OSIF_MessageQConstructor_t g_task_sdcard_queue_constructor;
static OSIF_MessageQContainer_t   g_task_sdcard_queue_container =
{
    .NumberOfMessage = sizeof(g_task_sdcard_queue_buffer)/sizeof(dma_rx_buffer_info_t*),
    .MessageSize_byte = sizeof(dma_rx_buffer_info_t *),
    .MessageArray = g_task_sdcard_queue_buffer,
};
static void *g_task_sdcard_handle = NULL;

uint32_t g_sdcard_send_count =0;

static TaskHandle_t       pSDcardHandlerTaskHandle = NULL;

static void task_sdcard_init(void);
static char testbuf[200] = {0x55,0xAA,0x55,0xAA};

static char g_file_names[SDCARD_FILE_NUM][20] ;

void hex_to_str(char *hex,  int hex_len, char *str)
{
    int i, pos=0;

    for(i=0; i<hex_len; i++)

    {

        sprintf(str+pos,  "%02x", hex[i]);

        pos +=2;

    }
}

int init_file_names(int n )
{
    if(n > SDCARD_FILE_NUM)
    {
        return -1;
    }
    for(int i =0; i<n ;i++)
    {
         sprintf(g_file_names[i],  "datafile_%d",i);
    }
}
int task_sdcard_send_give_rx_buffer_info(dma_rx_buffer_info_t **rx_buffer)
{
   int ret = 0;
   if ((g_task_sdcard_handle != NULL) && (rx_buffer != NULL))
   {
       ret = OSIF_MessageQGive(g_task_sdcard_handle, rx_buffer, 0, false);
   }
   else
   {
       ret = -1;
   }
   return ret;
}


int protocol_sdcard_package(uint8_t *data)
{
    if(NULL == data )
    {
        return -1;
    }
    data[0] = 0x55;
    data[1] = 0xAA;

    data[2] = 0x55;

    data[3] = 0xAA;

    UTILITY_FILL_ARRAY_FROM_DATA(&data[4], sizeof(uint32_t),  g_sdcard_send_count++, 0);


    data[280] = 0xEE;
    data[281] = 0xAA;
    data[282] = 0xDD;
    data[283] = 0xDD;
    data[284] = '\n';
    data[285] = '\r';

    return 0;
}
static char recdatabuf[300];
static char txtbuf[30000] ={0};

void AppSDcardHandlerThread(void *pvParameters)
{
	task_sdcard_init();
	int32_t cnt =0;
	uint32_t offset =0;

	uint32_t txt_cur = 0;
    uint32_t file_index =0;
    uint32_t file_write_cnt =0 ;
    dma_rx_buffer_info_t *p_rx_buffer_info = NULL;
//     while (1)
//     {
//         /**do noting*/
//         xil_printf("in sdcard task \n\r");
////         testbuf[4] = cnt++;
//         hex_to_str(testbuf , 100 ,txtbuf);
////         txtbuf[198] = '\n';
////         txtbuf[199] = '\r';
//         protocol_sdcard_package(txtbuf);
//
//         sd_write_data_withoffset(FILE_NAME,(u32)txtbuf,285,offset);
//
//         offset += 285 ;
//
//         OSIF_TimeDelay(1000);
//     }
    while(1)
    {
        OSIF_MessageQTake(g_task_sdcard_handle, &p_rx_buffer_info, OSIF_WAIT_FOREVER);
        if(p_rx_buffer_info)
        {
            //wwrite sd card

            protocol_sdcard_package(recdatabuf);
            memcpy((void *)&recdatabuf[8], (void *)p_rx_buffer_info->addr, SDCARD_PACKGE_SIZE);

            hex_to_str(recdatabuf , SDCARD_PACKGE_SIZE+SDCARD_PACKGE_HEAD_LEN+SDCARD_PACKGE_TAIL_LEN ,(char*)(txtbuf+txt_cur));

            txt_cur += SDCARD_PACKGE_SIZE+SDCARD_PACKGE_HEAD_LEN+SDCARD_PACKGE_TAIL_LEN ;

            txtbuf[txt_cur++] = '\n';
            txtbuf[txt_cur++] = '\r';

            if(txt_cur > ((SDCARD_PACKGE_SIZE+SDCARD_PACKGE_HEAD_LEN+SDCARD_PACKGE_TAIL_LEN +2)*SDCARD_PACKGENUM_PER_WRITE) )
            {
            	sd_write_data_withoffset(g_file_names[file_index],(u32)txtbuf,txt_cur,offset);
            	offset += txt_cur;
            	txt_cur =0;
                file_write_cnt++;
                if(file_write_cnt > (SDCARD_MAX_FILE_SIZE/(SDCARD_TXT_BUF_SIZE*SDCARD_PACKGENUM_PER_WRITE)))
                {
                    file_index++;
                    offset =0;
                }

                if(file_index > SDCARD_FILE_NUM)
                {
                    file_index =0;
                }
                
            }
            OSIF_TimeDelay(10);

        }
    }
}

static void task_sdcard_init(void)
{
	 g_task_sdcard_handle = OSIF_MessageQCreate(&g_task_sdcard_queue_constructor, &g_task_sdcard_queue_container);
     init_file_names(SDCARD_FILE_NUM);
	 int status = sd_mount();
	 if(status == XST_FAILURE)
	 {
	 	xil_printf("Failed to open SD card!\n");

	 }
	 else
	 	xil_printf("Success to open SD card!\n");
    
    return;
}


#define TASK_SDCARD_STACK_SIZE  4096*20
static __attribute__((aligned(4))) uint8_t task_sdcard_stack[TASK_SDCARD_STACK_SIZE];
OSIF_Task_t g_task_sdcard= {
        .name = "task_sdcard",
        .handler = AppSDcardHandlerThread,
        .arg = NULL,
        .stack = (void*)task_sdcard_stack,
        .StackSize_byte = TASK_SDCARD_STACK_SIZE,
};


BDEV_MODULE_INIT_DECLARE(task_sdcard_init);