/*** 
 * @Copyright: Copyright (C) by Benewake Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\service\ethernet\if_protocol.h
 * @PROJECT: P4
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2022-05-30 18:30:01
 * @LastEditTime: 2023-01-26 19:19:15
 */

#ifndef SERVICE_ETHERNET_IF_PROTOCOL_H_
#define SERVICE_ETHERNET_IF_PROTOCOL_H_

#include "stdint.h"
#include "lwipopts.h"
#include "lwip/ip_addr.h"
#include "lwip/err.h"
#include "lwip/udp.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netif.h"


#include "osif.h"
#include "stdbool.h"

#define IF_PROTOCOL_LENGTH_NAME   (20)
#define IF_PROTOCOL_LENGTH_IP     (20)

typedef enum if_protocol_tag
{
    IF_PROTOCOL_TAG_ADCPS        = 0,
    IF_PROTOCOL_TAG_DSOP        = 1,
    IF_PROTOCOL_TAG_PLCP        = 2,
    IF_PROTOCOL_TAG_LOGGER      = 3,
    IF_PROTOCOL_TAG_PTP_EVENT   = 4, 
    IF_PROTOCOL_TAG_PTP_GENERAL = 5,
    IF_PROTOCOL_TAG_DOIP = 6,
    IF_PROTOCOL_TAG_UNKNOWN
} if_protocol_tag_t;

typedef enum if_protocol_type
{
    IF_PROTOCOL_TYPE_UDP_SERVER   = 0,
    IF_PROTOCOL_TYPE_UDP_CLIENT   = 1,
    IF_PROTOCOL_TYPE_TCP_SERVER   = 2,
    IF_PROTOCOL_TYPE_TCP_CLIENT   = 3,
    IF_PROTOCOL_TYPE_UART         = 4,
    IF_PROTOCOL_TYPE_UNKNOWN
} if_protocol_type_t;

typedef struct if_protocol {
    if_protocol_tag_t  tag;
    if_protocol_type_t type;
    const char  name[IF_PROTOCOL_LENGTH_NAME];

    char  unicast_ip[IF_PROTOCOL_LENGTH_IP];
    uint32_t port;
    int sock;

    bool is_from_addr_changed;
    struct sockaddr_in from_addr_current;
    struct sockaddr_in from_addr_pre;
    struct sockaddr_in to_addr;

    int  (*handle)(void *);
    void (*destroy)(void);

    uint8_t   multicast_enable;
    char multicast_ip[IF_PROTOCOL_LENGTH_IP];

    uint8_t   broadcast_enable;
    char broadcast_ip[IF_PROTOCOL_LENGTH_IP];

    int init_flag;

    mutex_t mutex;
    mutex_t *mutex_handle;

    struct netif *netif_handle;
} if_protocol_t;

typedef enum
{
    IF_PROTOCOL_ERR_OK                     = 0, /**< performed successfully */
    IF_PROTOCOL_ERR_NOK                    = -1, /**< General error */
    IF_PROTOCOL_ERR_PARAM                  = -2, /**< Param err*/
    IF_PROTOCOL_ERR_NOT_INIT               = -3, /**< Not init */
    IF_PROTOCOL_ERR_NOT_LINK_UP            = -4,
    IF_PROTOCOL_ERR_SEND_LOCK              = -5,
    IF_PROTOCOL_ERR_SEND_RETURN_FAILED     = -6,
    
} if_protocol_err_t;

extern int  if_protocol_init(if_protocol_t *if_protocol_ptr);
extern int  if_protocol_recv(if_protocol_t *if_protocol_ptr, uint8_t *data, uint32_t length);
extern int  if_protocol_send(if_protocol_t *if_protocol_ptr, const uint8_t *data, size_t length);
extern void if_protocol_register_handle(if_protocol_t *if_protocol_ptr, int  (*handle)(void *));

#if HW_PTP
extern int  if_protocol_recv_with_timestamp(if_protocol_t *if_protocol_ptr, uint8_t *data, uint32_t length, mac_timestamp_t *time);
#endif

#endif /* SERVICE_ETHERNET_IF_PROTOCOL_H_ */
