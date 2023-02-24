/*** 
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\protocol\protocol_adcompass.h
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-25 14:11:51
 * @LastEditTime: 2023-01-28 19:58:02
 */


#ifndef __PROTOCOL_PROTOCOL_ADCPS_H_
#define __PROTOCOL_PROTOCOL_ADCPS_H_

/* Xilinx includes. */
#include "xparameters.h"
#include "xil_types.h"
#include "xstatus.h"
#include "xil_io.h"

typedef enum
{
    ADCPS_CLIENT_CONNECT,
    ADCPS_CLIENT_DISCONNECT,
} ADCPS_CLIENT_STATUS_E;

#define PROTOCOL_ADCPS_HEADER_N_RESERVE       (13)
#define PROTOCOL_ADCPS_HEADER_LENGTH          (8)
#define PROTOCOL_ADCPS_PS_PL_HEADER_LENGTH    (28)
#define PROTOCOL_ADCPS_PS_PL_DATA_ENDIAN      0
#define PROTOCOL_ADCPS_DATA_ENDIAN            0
#define DMA_PACKGE_SIZE                   (272)

#define PROTOCOL_ADCPS_PRODUCT_ID             (0x01)
#define PROTOCOL_ADCPS_PROTOCOL_ID            (0x00)
#define PROTOCOL_ADCPS_PROTOCOL_VERSION       (0x02)

typedef enum
{
    PROTOCOL_ADCPS_COORDINATE_TYPE_CARTESIAN = 0x00,
    PROTOCOL_ADCPS_COORDINATE_TYPE_SPHERICAL = 0x01,
    PROTOCOL_ADCPS_COORDINATE_TYPE_ORIGINAL  = 0x02,
} protocol_adcps_coordinate_type_t;

typedef enum
{
    PROTOCOL_ADCPS_ECHO_MODE_SINGLE = 0x00,
    PROTOCOL_ADCPS_ECHO_MODE_DOUBLE = 0x03,
} protocol_adcps_return_mode_t;

#pragma pack(push)
#pragma pack(1)

typedef struct
{
    u8  start_flag[2];
    u8  product_id;
    u8  protocol_id;
    u16 protocol_version;
    u32 count;
    u16 n_frame;
    u16 n_line;
    u16 n_points;
    u8  reserve[PROTOCOL_ADCPS_HEADER_N_RESERVE];
} protocol_adcps_header_t;

typedef struct
{
    u32 checksum;
    u16 end_flag;
} protocol_adcps_tail_t;

typedef struct protocol_adcps_payload
{
    uint32_t data_length;
    u8 *data;
} protocol_adcps_payload_t;

typedef struct protocol_adcps
{
    protocol_adcps_header_t  adcps_header;
    protocol_adcps_payload_t adcps_payload;
    protocol_adcps_tail_t    adcps_tail;

    u8 *raw_data;
    uint32_t raw_data_length;
} protocol_adcps_t;



typedef struct protocol_adcps_ps_pl_header
{
    uint32_t protocol_version;

    uint16_t count;
    uint16_t n_frame;

} protocol_adcps_ps_pl_header_t;

typedef struct protocol_adcps_ps_pl_payload
{
    uint8_t *data;
} protocol_adcps_ps_pl_payload_t;

typedef struct protocol_adcps_ps_pl
{
    protocol_adcps_ps_pl_header_t header;
    protocol_adcps_ps_pl_payload_t payload;

    u8 *raw_data;
    uint32_t raw_data_length;
} protocol_adcps_ps_pl_t;

typedef struct protocol_adcps_ps_pl_send
{
    uint32_t send_length;
    uint8_t *send_data;
    uint32_t send_offset;

} protocol_adcps_ps_pl_send_t;

#pragma pack(pop)


extern int  protocol_adcps_send_raw(u8 *data, size_t length);
extern int  protocol_adcps_handle(void *param);
extern int  protocol_adcps_ps_pl_parse(u8 *data, protocol_adcps_ps_pl_t *adcps_ps_pl, protocol_adcps_ps_pl_send_t *adcps_ps_pl_send);
extern int  protocol_adcps_ps_pl_package(protocol_adcps_ps_pl_t *adcps_ps_pl, u8 *data);
extern void protocol_adcps_send_init(void);
int protocol_adcps_send( protocol_adcps_ps_pl_send_t *adcps_ps_pl_send);
extern int  protocol_adcps_send_enable(void);
extern int  protocol_adcps_send_disable(void);

#endif /* __PROTOCOL_PROTOCOL_ADCPS_H_ */
