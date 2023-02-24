/*
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\protocol\protocol_plcp\protocol_plcp_cmd_handle.c
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-02-17 14:49:48
 * @LastEditTime: 2023-02-17 15:16:00
 */
/*
 * protocol_plcp_cmd_handle.c
 *
 *  Created on: 2022/02/08
 *      Author: jeremy.xiang
 */

#include "protocol_plcp_cmd_handle.h"
#include "protocol_plcp.h"
#include "gpiopl_intr.h"

#include "utilities.h"

#include "osif.h"

//#include "protocol_multicore_cmd_handle.h"
#define FIRMWARE_UPGRADE_PHASE_RECV_INIT_PKG           (0x00)
#define FIRMWARE_UPGRADE_PHASE_RECV_CONTINUE_PKG       (0x01)

#define PLCP_TIMEOUT_MS                 (1000)
#define PLCP_TIMEOUT_S       (20)
#define PLCP_TIMEOUT_COUNT   (PLCP_TIMEOUT_S*1000/PLCP_TIMEOUT_MS)
static TimerHandle_t  g_plcp_timer_handle = NULL;
static StaticTimer_t  g_plcp_timer;
static int plcp_timeout = PLCP_TIMEOUT_COUNT;

static void plcp_callback(TimerHandle_t xTimer)
{
    return;
}

protocol_plcp_err_t protocol_plcp_reg_control(void *param)
{
    protocol_plcp_handle_info_t *plcp_handle_info = (protocol_plcp_handle_info_t *)param;
    protocol_plcp_t   *response = plcp_handle_info->response;
    protocol_plcp_t   *request  = plcp_handle_info->request;

    uint8_t *cmd_data = request->payload.data;
    uint32_t address = 0;
    uint32_t opt = 0;
    int ret = 0;
    
    UTILITY_FILL_DATA_FROM_ARRAY(&(address), sizeof(uint32_t), &(cmd_data[0]), PROTOCOL_PLCP_DATA_ENDIAN);
    UTILITY_FILL_DATA_FROM_ARRAY(&(opt), sizeof(uint32_t), &(cmd_data[4]), PROTOCOL_PLCP_DATA_ENDIAN);

    if(address > 4)
    {
        return PROTOCOL_PLCP_ERR_CODE_COMMON_CMD_PARAM_ERROR;  
    }


    switch(address)
    {
        case 0 :
            PL_CTRL(opt);
        break;

        case 1:
            PL_GAIN(opt);

        break;

        case 2:
            PL_GESTURE(opt);

        break;

        case 3:
            PL_SP_CTRL(opt);

        break;

        default:

            return PROTOCOL_PLCP_ERR_CODE_COMMON_CMD_PARAM_ERROR;
    }
    
        
      
    return PROTOCOL_PLCP_ERR_CODE_COMMON_SUCCESS;
}
