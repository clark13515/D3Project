/*
 * @Copyright: Copyright (C) by Benewake Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\protocol\protocol_plcp\protocol_plcp_cmd_map.c
 * @PROJECT: P4
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2022-07-11 17:50:49
 * @LastEditTime: 2023-02-17 14:55:34
 */

/*
 * protocol_plcp_cmd_map.c
 *
 *  Created on: 2022/03/18
 *      Author: jeremy.xiang
 */

#include "protocol_plcp_cmd_map.h"
#include "protocol_plcp_cmd_handle.h"



const protocol_plcp_cmd_info_t     g_protocol_plcp_cmd_info[PROTOCOL_PLCP_CMD_MAX] =
{
    {PROTOCOL_PLCP_CMD_GAIN_CONTROL        ,   0x00, 0x00, 0x01, protocol_plcp_reg_control              },
    {PROTOCOL_PLCP_CMD_ATT_CONTROL          ,   0x00, 0x00, 0x01,   NULL                },
    {PROTOCOL_PLCP_CMD_PL_CONTROL             ,   0x01, 0x01, 0x01, NULL                   },
    {PROTOCOL_PLCP_CMD_GET_SRATE_CONTROL   ,   0x00, 0x00, 0x01,    NULL        },
    // 0xFF should not used, JUST FOR ENDMARKER
    {PROTOCOL_PLCP_CMD_END_MARKER             ,   0x00, 0x00, 0x00, NULL                                       },
};

int protocol_plcp_get_cmd_info_with_cmd_id(uint8_t cmd_id, protocol_plcp_cmd_info_t **p_cmd_info)
{
    uint8_t cmd_idx = 0;
    for(cmd_idx = 0; cmd_idx < PROTOCOL_PLCP_CMD_MAX; cmd_idx++)
    {
        /* check if reached end marker */
        if(PROTOCOL_PLCP_CMD_END_MARKER == cmd_id)
        {
            break;
        }

        if(g_protocol_plcp_cmd_info[cmd_idx].cmd_id == cmd_id)
        {
            break;
        }
    }

    if((PROTOCOL_PLCP_CMD_END_MARKER == cmd_idx) || (cmd_idx >= PROTOCOL_PLCP_CMD_MAX))
    {
        *p_cmd_info = NULL;
        return -1;
    }

    *p_cmd_info = &g_protocol_plcp_cmd_info[cmd_idx];
    return 0;
}
