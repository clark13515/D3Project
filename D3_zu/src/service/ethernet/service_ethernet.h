/*** 
 * @Copyright: Copyright (C) by Benewake Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \p4_worksapce_zu5\sources\service\ethernet\service_ethernet.h
 * @PROJECT: P4
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2022-07-11 17:33:00
 * @LastEditTime: 2022-10-24 09:54:59
 */


#ifndef SERVICE_ETHERNET_H_
#define SERVICE_ETHERNET_H_

#include <stdint.h>
#include "stdbool.h"
#include "osif.h"
#include "if_protocol.h"


#define SERVICE_ETHERNET_NUMBER_OF_INTS          (7)


extern OSIF_Task_t g_service_ethernet[SERVICE_ETHERNET_NUMBER_OF_INTS];

extern bool           service_ethernet_is_port_reserved(uint32_t port);


#endif /* SERVICE_ETHERNET_H_ */
