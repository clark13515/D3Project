/*
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\service\ethernet\if_protocol.c
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-26 14:56:28
 * @LastEditTime: 2023-01-27 17:38:47
 */

#include "if_protocol.h"

#include "stdint.h"

#include "xparameters.h"
#include "xemacps.h"

#include "lwipopts.h"
#include "lwip/ip_addr.h"
#include "lwip/err.h"
#include "lwip/udp.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netif.h"
#include "errno.h"
#include "xil_printf.h"
#include "osif.h"

static int if_protocol_udp_server_init(if_protocol_t *if_protocol_ptr)
{
    int sock = -1;
    err_t err = 0;
    struct sockaddr_in address;
    int ret = 0;
    do
    {
        if_protocol_ptr->init_flag = 0;

        /* create mutex */
        if_protocol_ptr->mutex_handle = NULL;
        ret = OSIF_MutexCreate(&(if_protocol_ptr->mutex));
        if (0 != ret)
        {
            ret = -1;
            break;
        }
        if_protocol_ptr->mutex_handle = &(if_protocol_ptr->mutex);


        /**
         * param1: domain
         *    AF_INET: tcp/ip
         * param2: type
         *    SOCK_DGRAM: UDP
         *    SOCK_STREAM: TCP
         * param3: protocol alway 0 for IPv4
         *    0
         */
        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            ret = -1;
            break;
        }

        memset((char *)&address, 0, sizeof(struct sockaddr_in));
        address.sin_family = AF_INET;
        address.sin_port = htons(if_protocol_ptr->port);

        if(if_protocol_ptr->multicast_enable)
        {
            address.sin_addr.s_addr = inet_addr(if_protocol_ptr->multicast_ip);
        }
        else
        {
            /**
             * The system assigns the ip number to the socket
             * if there is only one local NIC, the ip is the NIC ip
             * if there are multiple NICs, the system assigns
             */
            address.sin_addr.s_addr = htonl(INADDR_ANY);
        }

        /*bind ip to socket*/
        err = bind(sock, (struct sockaddr *)&address, sizeof(address));
        if ( err < ERR_OK)
        {
            ret = -1;
            break;
        }

        if(if_protocol_ptr->multicast_enable)
        {
            struct ip_mreq mreq;
            memset(&mreq, 0, sizeof(struct ip_mreq));
            /* multicast ip */
            mreq.imr_multiaddr.s_addr = inet_addr(if_protocol_ptr->multicast_ip);
            //TODO:
            mreq.imr_interface.s_addr = htonl(INADDR_ANY);
            /**
             * param1: socket
             * param2: level
             *    SOL_SOCKET: socket layer
             *    IPPROTO_TCP: tcp layer
             *    IPPPROTO_IP: IP layer
             * param3: optname
             *     if SOL_SOCKET:
             *
             */
            if(setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(struct ip_mreq)) < 0)
            {
                ret = -1;
                break;
            }


            // u32 temp = XEmacPs_ReadReg(0xE000B000, XEMACPS_NWCFG_OFFSET);
            // temp |= BIT(4);
            // XEmacPs_WriteReg(0xE000B000, XEMACPS_NWCFG_OFFSET, temp);
            // address.sin_addr.s_addr = inet_addr(if_protocol_ptr->multicast_ip);
        }


        if_protocol_ptr->sock = sock;
        memset((char *)&(if_protocol_ptr->from_addr_current), 0, sizeof(struct sockaddr_in));
        memset((char *)&(if_protocol_ptr->from_addr_pre), 0, sizeof(struct sockaddr_in));
        memset((char *)&(if_protocol_ptr->to_addr), 0, sizeof(struct sockaddr_in));

        if_protocol_ptr->init_flag = 1;

    } while(0);

    if(ret != 0)
    {
        if(sock >= 0)
        {
            close(sock);
        }
        if(NULL != (if_protocol_ptr->mutex_handle))
        {
            OSIF_MutexDestroy(if_protocol_ptr->mutex_handle);
        }

        if_protocol_ptr->mutex_handle = NULL;
        if_protocol_ptr->init_flag = 0;

    }
    return ret;
}

static int if_protocol_udp_client_init(if_protocol_t *if_protocol_ptr)
{
    int sock;
    struct sockaddr_in address;
    int ret = 0;

    do
    {
        if_protocol_ptr->init_flag = 0;

        /* create mutex */
        if_protocol_ptr->mutex_handle = NULL;
        ret = OSIF_MutexCreate(&(if_protocol_ptr->mutex));
        if (0 != ret)
        {
            ret = -1;
            break;
        }
        if_protocol_ptr->mutex_handle = &(if_protocol_ptr->mutex);

        /**
         * param1: domain
         *    AF_INET: tcp/ip
         * param2: type
         *    SOCK_DGRAM: UDP
         *    SOCK_STREAM: TCP
         * param3: protocol alway 0 for IPv4
         *
         */
        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            ret = -1;
            break;
        }

        bzero((char *) &address, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_port = htons(if_protocol_ptr->port);

        if(if_protocol_ptr->broadcast_enable)
        {
            address.sin_addr.s_addr = inet_addr(if_protocol_ptr->broadcast_ip);
        }
        else if(if_protocol_ptr->multicast_enable)
        {
            address.sin_addr.s_addr = inet_addr(if_protocol_ptr->multicast_ip);

            struct ip_mreq mreq;
            mreq.imr_multiaddr.s_addr = inet_addr(if_protocol_ptr->multicast_ip);
            mreq.imr_interface.s_addr = INADDR_ANY;
            if(setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(struct ip_mreq)) < 0)
            {
                ret = -1;
                break;
            }
        }
        else
        {
            address.sin_addr.s_addr = inet_addr(if_protocol_ptr->unicast_ip);
        }


        if_protocol_ptr->sock = sock;
        memcpy((char *)&(if_protocol_ptr->to_addr), (char *)&(address), sizeof(struct sockaddr_in));

        if_protocol_ptr->init_flag = 1;
    } while(0);

    if(ret != 0)
    {
        if(sock >= 0)
        {
            close(sock);
        }
        if(NULL != (if_protocol_ptr->mutex_handle))
        {
            OSIF_MutexDestroy(if_protocol_ptr->mutex_handle);
        }

        if_protocol_ptr->mutex_handle = NULL;
        if_protocol_ptr->init_flag = 0;
    }
    return ret;
}


static int if_protocol_udp_server_recv(if_protocol_t *if_protocol_ptr, uint8_t *data, uint32_t length)
{
    int count = 0;
    /*Note: addrlen must be inited*/
    socklen_t  addrlen = sizeof(struct sockaddr_in);
    count = recvfrom(if_protocol_ptr->sock, data, length, 0, (struct sockaddr *)&(if_protocol_ptr->from_addr_current), &addrlen);
    if(count <= 0)
    {
        return count;
    }

    /* Lock resource mutex */
    int status = OSIF_MutexLock(&(if_protocol_ptr->mutex), OSIF_WAIT_FOREVER);
    if( 0 != status)
    {
        return -1;
    }
    if(memcmp((char *)&(if_protocol_ptr->from_addr_pre), (char *)&(if_protocol_ptr->from_addr_current), sizeof(struct sockaddr_in)) != 0)
    {
        memcpy((char *)&(if_protocol_ptr->from_addr_pre), (char *)&(if_protocol_ptr->from_addr_current), sizeof(struct sockaddr_in));
        if_protocol_ptr->is_from_addr_changed = true;
    }
    /* Unlock resource mutex */
    OSIF_MutexUnlock(&(if_protocol_ptr->mutex));

    return count;
}

static int if_protocol_udp_server_send(if_protocol_t *if_protocol_ptr, const uint8_t *data, size_t length)
{
    int count = 0;
    int status = 0;

    /* send addr is from recv addr */
    /* Lock resource mutex */
    status = OSIF_MutexLock(&(if_protocol_ptr->mutex), OSIF_WAIT_FOREVER);
    if( 0 != status)
    {
        return IF_PROTOCOL_ERR_SEND_LOCK;
    }
    if(if_protocol_ptr->is_from_addr_changed)
    {
        memcpy((char *)&(if_protocol_ptr->to_addr), (char *)&(if_protocol_ptr->from_addr_pre), sizeof(struct sockaddr_in));
        if_protocol_ptr->is_from_addr_changed = false;
    }
    /* Unlock resource mutex */
    OSIF_MutexUnlock(&(if_protocol_ptr->mutex));

    count = sendto(if_protocol_ptr->sock, data, length, 0, (struct sockaddr *)&if_protocol_ptr->to_addr, sizeof(struct sockaddr_in));
    if(count < 0)
    {
        return IF_PROTOCOL_ERR_SEND_RETURN_FAILED;
    }
    return count;
}

static int if_protocol_udp_client_recv(if_protocol_t *if_protocol_ptr, uint8_t *data, uint32_t length)
{
    int count = 0;
    /*Note: addrlen must be inited*/
    socklen_t  addrlen = sizeof(struct sockaddr_in);
    count = recvfrom(if_protocol_ptr->sock, data, length, 0, (struct sockaddr *)&(if_protocol_ptr->from_addr_current), &addrlen);
    //TODO: check from_addr_current is same as to_addr
    if(count <= 0)
    {
        return -1;
    }
    return count;
}

static int if_protocol_udp_client_send(if_protocol_t *if_protocol_ptr, const uint8_t *data, size_t length)
{
    int count;
    /* udp client send addr(to_addr) is asigned when init*/
    count = sendto(if_protocol_ptr->sock, data, length, 0, (struct sockaddr *)&if_protocol_ptr->to_addr, sizeof(struct sockaddr_in));
    if(count < 0)
    {
        return IF_PROTOCOL_ERR_SEND_RETURN_FAILED;
    }
    return count;
}


int if_protocol_init(if_protocol_t *if_protocol_ptr)
{
    int result = 0;
    if(NULL == if_protocol_ptr)
    {
        return -1;
    }
    if(1 == if_protocol_ptr->init_flag)
    {
        return 0;
    }
    if(IF_PROTOCOL_TYPE_UDP_SERVER == if_protocol_ptr->type)
    {
        result = if_protocol_udp_server_init(if_protocol_ptr);
    } 
    else if(IF_PROTOCOL_TYPE_UDP_CLIENT == if_protocol_ptr->type) 
    {
        result = if_protocol_udp_client_init(if_protocol_ptr);
    } else 
    {
        //TODO
    }
    return result;
}

int if_protocol_recv(if_protocol_t *if_protocol_ptr, uint8_t *data, uint32_t length)
{
    int result = 0;

    if(NULL == if_protocol_ptr)
    {
        return -1;
    }

    if(1 != if_protocol_ptr->init_flag)
    {
        return -1;
    }

    /* check if phy linkup */
    if( !((if_protocol_ptr->netif_handle->flags) & NETIF_FLAG_LINK_UP) )
    {
        return -1;
    }

    if(IF_PROTOCOL_TYPE_UDP_SERVER == if_protocol_ptr->type)
    {
        result = if_protocol_udp_server_recv(if_protocol_ptr, data, length);
    }
    else if(IF_PROTOCOL_TYPE_UDP_CLIENT == if_protocol_ptr->type)
    {
        result = if_protocol_udp_client_recv(if_protocol_ptr, data, length);
    }
    else 
    {
        //TODO
    }
    return result;
}

#if HW_PTP
static int if_protocol_udp_server_recv_with_timestamp(if_protocol_t *if_protocol_ptr, uint8_t *data, uint32_t length, mac_timestamp_t *time)
{
    int count;
    /*Note: addrlen must be inited*/
    socklen_t  addrlen = sizeof(struct sockaddr_in);
    count = recvfrom_with_timestamp(if_protocol_ptr->sock, data, length, 0, (struct sockaddr *)&(if_protocol_ptr->from_addr_current), &addrlen, time);
    if(count <= 0)
    {
        return -1;
    }

    /* Lock resource mutex */
    int status = OSIF_MutexLock(&(if_protocol_ptr->mutex), OSIF_WAIT_FOREVER);
    if( 0 != status)
    {
        return -1;
    }

    if(memcmp((char *)&(if_protocol_ptr->from_addr_pre), (char *)&(if_protocol_ptr->from_addr_current), sizeof(struct sockaddr_in)) != 0)
    {
        memcpy((char *)&(if_protocol_ptr->from_addr_pre), (char *)&(if_protocol_ptr->from_addr_current), sizeof(struct sockaddr_in));
        if_protocol_ptr->is_from_addr_changed = true;
    }

    /* Unlock resource mutex */
    OSIF_MutexUnlock(&(if_protocol_ptr->mutex));

    return count;
}

static int if_protocol_udp_client_recv_with_timestamp(if_protocol_t *if_protocol_ptr, uint8_t *data, uint32_t length, mac_timestamp_t *time)
{
    int count;

    /*Note: addrlen must be inited*/
    socklen_t  addrlen = sizeof(struct sockaddr_in);
    count = recvfrom_with_timestamp(if_protocol_ptr->sock, data, length, 0, (struct sockaddr *)&(if_protocol_ptr->from_addr_current), &addrlen, time);
    if(count <= 0)
    {
        return -1;
    }
    return count;
}

int if_protocol_recv_with_timestamp(if_protocol_t *if_protocol_ptr, uint8_t *data, uint32_t length, mac_timestamp_t *time)
{
    int result = 0;

    if(NULL == if_protocol_ptr)
    {
        return -1;
    }

    if(1 != if_protocol_ptr->init_flag)
    {
        return -1;
    }

    /* check if phy linkup */
    if( !((if_protocol_ptr->netif_handle->flags) & NETIF_FLAG_LINK_UP) )
    {
        return -1;
    }

    if(IF_PROTOCOL_TYPE_UDP_SERVER == if_protocol_ptr->type)
    {
        result = if_protocol_udp_server_recv_with_timestamp(if_protocol_ptr, data, length, time);
    }
    else if(IF_PROTOCOL_TYPE_UDP_CLIENT == if_protocol_ptr->type)
    {
        result = if_protocol_udp_client_recv_with_timestamp(if_protocol_ptr, data, length, time);
    }
    else
    {
        //TODO
    }
    return result;
}

#endif

int if_protocol_send(if_protocol_t *if_protocol_ptr, const uint8_t *data, size_t length)
{
    int result = IF_PROTOCOL_ERR_OK;

    if(NULL == if_protocol_ptr)
    {
        return IF_PROTOCOL_ERR_PARAM;
    }

    if(1 != if_protocol_ptr->init_flag)
    {
        return IF_PROTOCOL_ERR_NOT_INIT;
    }

    /* check if phy linkup */
    if( !((if_protocol_ptr->netif_handle->flags) & NETIF_FLAG_LINK_UP) )
    {
        return IF_PROTOCOL_ERR_NOT_LINK_UP;
    }

    if(IF_PROTOCOL_TYPE_UDP_SERVER == if_protocol_ptr->type)
    {
        result = if_protocol_udp_server_send(if_protocol_ptr, data, length);
    }
    else if(IF_PROTOCOL_TYPE_UDP_CLIENT == if_protocol_ptr->type)
    {
        result = if_protocol_udp_client_send(if_protocol_ptr, data, length);
    }
    else 
    {
        //TODO
    }
    return result;
}

void if_protocol_register_handle(if_protocol_t *if_protocol_ptr, int  (*handle)(void *))
{

    if(NULL == if_protocol_ptr)
    {
        if_protocol_ptr->handle = handle;
    }
}
