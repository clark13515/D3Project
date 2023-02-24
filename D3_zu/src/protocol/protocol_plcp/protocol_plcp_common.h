/*** 
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\protocol\protocol_plcp\protocol_plcp_common.h
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-02-17 15:48:58
 * @LastEditTime: 2023-02-17 15:49:03
 */
/*
 * protocol_plcp_common.h
 *
 *  Created on: 2022/03/18
 *      Author: jeremy.xiang
 */

#ifndef __PROTOCOL_PROTOCOL_PLCP_COMMON_H_
#define __PROTOCOL_PROTOCOL_PLCP_COMMON_H_

#include <stdio.h>

#define PROTOCOL_PLCP_HEADER_LENGTH_MIN              (6)
#define PROTOCOL_PLCP_PAYLOAD_LENGTH_MIN             (3)
#define PROTOCOL_PLCP_TAIL_LENGTH                    (6)
#define PROTOCOL_PLCP_LENGTH_MIN                     ((PROTOCOL_PLCP_HEADER_LENGTH_MIN) + (PROTOCOL_PLCP_PAYLOAD_LENGTH_MIN) + (PROTOCOL_PLCP_TAIL_LENGTH))
#define PROTOCOL_PLCP_RESPONSE_PAYLOAD_MAX_LENGTH    (1000)

/**
 * The locations of the following fields are fixed in different protocol versions.  
 * Note that the location of some fields may change depending on the protocol version 
 * 
 */
#define PROTOCOL_PLCP_PROTOCOL_VERSION_OFFSET        (4)
#define PROTOCOL_PLCP_PROTOCOL_VERSION_LENGTH        (2)

#define PROTOCOL_PLCP_DATA_ENDIAN                    UTILITY_LITTLE_ENDIAN



//Construct a hal err code value from a group and err number
#define MAKE_PROTOCOL_PLCP_ERR_CODE(err_code_group, err_code) (((err_code_group)*100) + (err_code))


typedef enum
{
    /* common errcode */
    PROTOCOL_PLCP_ERR_CODE_COMMON_SUCCESS                 = MAKE_PROTOCOL_PLCP_ERR_CODE(0, 0),     // no err return 0
    PROTOCOL_PLCP_ERR_CODE_COMMON_ERROR                   = MAKE_PROTOCOL_PLCP_ERR_CODE(0, 1),     // common error
    PROTOCOL_PLCP_ERR_CODE_COMMON_PKG_LEN_SHORT           = MAKE_PROTOCOL_PLCP_ERR_CODE(0, 2),
    PROTOCOL_PLCP_ERR_CODE_COMMON_PKG_LEN_LONG            = MAKE_PROTOCOL_PLCP_ERR_CODE(0, 3),
    PROTOCOL_PLCP_ERR_CODE_COMMON_START_FLAG_ERROR        = MAKE_PROTOCOL_PLCP_ERR_CODE(0, 4),
    PROTOCOL_PLCP_ERR_CODE_COMMON_END_FLAG_ERROR          = MAKE_PROTOCOL_PLCP_ERR_CODE(0, 5),
    PROTOCOL_PLCP_ERR_CODE_COMMON_PKG_LEN_MISMATCH        = MAKE_PROTOCOL_PLCP_ERR_CODE(0, 6),
    PROTOCOL_PLCP_ERR_CODE_COMMON_CHECK_ERROR             = MAKE_PROTOCOL_PLCP_ERR_CODE(0, 7),
    PROTOCOL_PLCP_ERR_CODE_COMMON_PRODUCT_VERSION_ERROR   = MAKE_PROTOCOL_PLCP_ERR_CODE(0, 8),
    PROTOCOL_PLCP_ERR_CODE_COMMON_PROTOCOL_TYPE_ERROR     = MAKE_PROTOCOL_PLCP_ERR_CODE(0, 9),
    PROTOCOL_PLCP_ERR_CODE_COMMON_PROTOCOL_VERSION_ERROR  = MAKE_PROTOCOL_PLCP_ERR_CODE(0, 10),
    PROTOCOL_PLCP_ERR_CODE_COMMON_CMD_UNKNOWN             = MAKE_PROTOCOL_PLCP_ERR_CODE(0, 11),
    PROTOCOL_PLCP_ERR_CODE_COMMON_CMD_NO_HANDLE           = MAKE_PROTOCOL_PLCP_ERR_CODE(0, 12),
    PROTOCOL_PLCP_ERR_CODE_COMMON_CMD_STATUS_ERR          = MAKE_PROTOCOL_PLCP_ERR_CODE(0, 13),
    PROTOCOL_PLCP_ERR_CODE_COMMON_CMD_LEN_ERROR           = MAKE_PROTOCOL_PLCP_ERR_CODE(0, 14),
    PROTOCOL_PLCP_ERR_CODE_COMMON_CMD_PARAM_ERROR         = MAKE_PROTOCOL_PLCP_ERR_CODE(0, 15),
    PROTOCOL_PLCP_ERR_CODE_COMMON_CMD_CAN_NOT_EXECUTE     = MAKE_PROTOCOL_PLCP_ERR_CODE(0, 16),
    PROTOCOL_PLCP_ERR_CODE_COMMON_CMD_RESPONSE_LEN_LONG   = MAKE_PROTOCOL_PLCP_ERR_CODE(0, 17),
    PROTOCOL_PLCP_ERR_CODE_COMMON_CMD_EXECUTE_TIMEOUT     = MAKE_PROTOCOL_PLCP_ERR_CODE(0, 18),
    PROTOCOL_PLCP_ERR_CODE_COMMON_CMD_EXECUTE_FAILED      = MAKE_PROTOCOL_PLCP_ERR_CODE(0, 19),


    /* download firmware (0x20) */
    PROTOCOL_PLCP_ERR_CODE_DOWNLOAD_FIRMWARE_LEN_LONG           = MAKE_PROTOCOL_PLCP_ERR_CODE(0x20, 0),
    PROTOCOL_PLCP_ERR_CODE_DOWNLOAD_FIRMWARE_OUT_RANGE          = MAKE_PROTOCOL_PLCP_ERR_CODE(0x20, 1),
    PROTOCOL_PLCP_ERR_CODE_DOWNLOAD_FIRMWARE_PKG_COUNT_ERROR    = MAKE_PROTOCOL_PLCP_ERR_CODE(0x20, 2),
    PROTOCOL_PLCP_ERR_CODE_DOWNLOAD_FIRMWARE_PKG_TYPE_ERROR     = MAKE_PROTOCOL_PLCP_ERR_CODE(0x20, 3),
    PROTOCOL_PLCP_ERR_CODE_DOWNLOAD_FIRMWARE_PKG_SIZE_ERROR     = MAKE_PROTOCOL_PLCP_ERR_CODE(0x20, 4),
    PROTOCOL_PLCP_ERR_CODE_DOWNLOAD_FIRMWARE_UPDATE_FAILED      = MAKE_PROTOCOL_PLCP_ERR_CODE(0x20, 5),


    /* download lut (0x22) */
    PROTOCOL_PLCP_ERR_CODE_DOWNLOAD_LUT_LEN_ERR            = MAKE_PROTOCOL_PLCP_ERR_CODE(0x22, 0),
    PROTOCOL_PLCP_ERR_CODE_DOWNLOAD_LUT_OUT_RANGE          = MAKE_PROTOCOL_PLCP_ERR_CODE(0x22, 1),
    PROTOCOL_PLCP_ERR_CODE_DOWNLOAD_LUT_PKG_COUNT_ERROR    = MAKE_PROTOCOL_PLCP_ERR_CODE(0x22, 2),
    PROTOCOL_PLCP_ERR_CODE_DOWNLOAD_LUT_PKG_TYPE_ERROR     = MAKE_PROTOCOL_PLCP_ERR_CODE(0x22, 3),
    PROTOCOL_PLCP_ERR_CODE_DOWNLOAD_LUT_PKG_SIZE_ERROR     = MAKE_PROTOCOL_PLCP_ERR_CODE(0x22, 4),
    PROTOCOL_PLCP_ERR_CODE_DOWNLOAD_LUT_ID_ERROR           = MAKE_PROTOCOL_PLCP_ERR_CODE(0x22, 5),
    PROTOCOL_PLCP_ERR_CODE_DOWNLOAD_LUT_UPDATE_FAILED      = MAKE_PROTOCOL_PLCP_ERR_CODE(0x22, 6),

} protocol_plcp_err_t;

typedef enum
{
    PLCP_BUFFER_EMPTY    = 0,
    PLCP_BUFFER_RECV_IN  = 1,
    PLCP_BUFFER_FULL     = 2,
} protocol_plcp_process_msg_status_t;

typedef int(*protocol_plcp_process_send_func_t)(uint8_t *data, uint16_t length);

typedef struct protocol_plcp_process_msg_info {
    uint8_t *data;
    uint16_t  length;
    protocol_plcp_process_msg_status_t  status;
    protocol_plcp_process_send_func_t     send;
} protocol_plcp_process_msg_info_t;

typedef struct {
    protocol_plcp_process_msg_info_t *info;
} protocol_plcp_process_msg_t;

uint32_t protocol_common_calculation_check_with_protocol_version(uint16_t protocol_version, const uint8_t *p_data, uint32_t data_len);

#endif /*__PROTOCOL_PROTOCOL_PLCP_COMMON_H_*/
