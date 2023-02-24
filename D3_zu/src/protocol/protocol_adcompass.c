/*
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\protocol\protocol_adcompass.c
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-25 14:11:37
 * @LastEditTime: 2023-02-16 20:27:09
 */

#include "protocol_adcompass.h"
#include "stdbool.h"
#include <stdint.h>
#include "if_protocol.h"
#include "utilities_data_coversion.h"

#define PROTOCOL_ADCPS_SEND_BUF_SIZE                    (1024)
#define PROTOCOL_ADCPS_SEND_COUNT_MAX                   (0xFFFFFFFFUL)
#define PROTOCOL_ADCPS_SEND_DATA_BUFFER_NUMBER          (2)
#define PROTOCOL_ADCPS_SEND_FRAME_LENTH                 (284)

static if_protocol_t*  g_if_protocol_adcps_ptr = NULL;


static uint8_t                      g_protocol_adcps_send_data_buffer0[PROTOCOL_ADCPS_SEND_BUF_SIZE];
static uint8_t                      g_protocol_adcps_send_data_buffer1[PROTOCOL_ADCPS_SEND_BUF_SIZE];
static uint8_t*                     g_protocol_adcps_send_data_buffer_list[PROTOCOL_ADCPS_SEND_DATA_BUFFER_NUMBER] = {&g_protocol_adcps_send_data_buffer0[0], &g_protocol_adcps_send_data_buffer1[1]};
static uint16_t                g_protocol_adcps_send_data_length  = 0;
static volatile uint32_t       g_protocol_adcps_send_count        = 0;
static uint16_t                g_protocol_adcps_ps_pl_count_pre   = 0;
static bool                    g_protocol_adcps_send_enable_flag  = true;
static bool                    g_protocol_adcps_send_disable_flag = false;


int protocol_adcps_send_raw(uint8_t *data, size_t length)
{
    return if_protocol_send(g_if_protocol_adcps_ptr, data, length);
}

int protocol_adcps_package(uint8_t *data)
{
    if(NULL == data )
    {
        return -1;
    }
    data[0] = 0x55;
    data[1] = 0xAA;

    data[2] = 0x55;

    data[3] = 0xAA;

    UTILITY_FILL_ARRAY_FROM_DATA(&data[4], sizeof(uint32_t),  g_protocol_adcps_send_count++, PROTOCOL_ADCPS_DATA_ENDIAN);


    data[280] = 0xEE;
    data[281] = 0xAA;
    data[282] = 0xDD;
    data[283] = 0xDD;

    return 0;
}


int protocol_adcps_ps_pl_parse(uint8_t *data, protocol_adcps_ps_pl_t *adcps_ps_pl, protocol_adcps_ps_pl_send_t *adcps_ps_pl_send)
{
    uint32_t index = 0;

    return 0;
}

void protocol_adcps_send_init(void)
{
    uint8_t *p_send_data = NULL;
    g_protocol_adcps_send_data_buffer_list[0] = &g_protocol_adcps_send_data_buffer0;
    g_protocol_adcps_send_data_buffer_list[1] = &g_protocol_adcps_send_data_buffer1;
    for(uint8_t i=0; i<PROTOCOL_ADCPS_SEND_DATA_BUFFER_NUMBER; i++)
    {
        p_send_data = g_protocol_adcps_send_data_buffer_list[i];
        //start flag
        p_send_data[0] = 0x55;
        p_send_data[1] = 0xAA;
        // product id
        p_send_data[2] = 0x55;
        // protocol id
        p_send_data[3] = 0xAA;
  
        memset(&p_send_data[4], 0, 280);

    }

    g_protocol_adcps_send_count = 0;
}

int protocol_adcps_send_enable(void)
{
    g_protocol_adcps_send_enable_flag = true;
    /* Note: don't set g_protocol_adcps_send_disable_flag to false to make sure current send quited*/
    g_protocol_adcps_send_count = 0;
    return 0;
}

int protocol_adcps_send_disable(void)
{
    g_protocol_adcps_send_disable_flag = true;
    g_protocol_adcps_send_enable_flag  = false;
    return 0;
}

int protocol_adcps_send( protocol_adcps_ps_pl_send_t *adcps_ps_pl_send)
{
    int ret = 0;
    g_protocol_adcps_send_data_length = 0;
    uint16_t n_emit = 0;
    uint8_t p_send_data[PROTOCOL_ADCPS_SEND_FRAME_LENTH] = {0};
    uint8_t send_index = 0;
    if(NULL == g_if_protocol_adcps_ptr)
    {
        return -1;
    }

    if(g_protocol_adcps_send_enable_flag)
    {
        g_protocol_adcps_send_disable_flag = false;
    }

    protocol_adcps_package(p_send_data);

    memcpy((void *)&p_send_data[PROTOCOL_ADCPS_HEADER_LENGTH], (void *)adcps_ps_pl_send->send_data, DMA_PACKGE_SIZE);

    ret = if_protocol_send(g_if_protocol_adcps_ptr, p_send_data, PROTOCOL_ADCPS_SEND_FRAME_LENTH);

    adcps_ps_pl_send->send_offset = 0;
    return 0;
}

int protocol_adcps_handle(void *param)
{
    uint8_t recv_buf[10];
    int n;

    if_protocol_t *protocol = NULL;
    protocol = (if_protocol_t *)param;

    if(NULL == protocol)
    {
        return -1;
    }

    n = if_protocol_recv(protocol, recv_buf, 10);
    if(n <= 0)
    {
        return -1;
    }
    
    if(!strncmp((char *)recv_buf, "ADCPS", 5))
    {
        g_if_protocol_adcps_ptr = protocol;
        if_protocol_send(protocol, (uint8_t *)"adcps\n", 5);
    }

    return 0;
}
