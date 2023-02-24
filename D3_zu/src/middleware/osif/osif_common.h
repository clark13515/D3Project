/*
 * osif_common.h
 *
 *  Created on: 2022/01/24
 *      Author: jeremy.xiang
 */
#ifndef OSI_COMMON_H
#define OSI_COMMON_H

#include <stdbool.h>
#include "stdint.h"

extern volatile uint32_t g_osif_inside_isr;

//Note: Called in pairs in interrupts, one when entering the interrupt and one before exiting the interrupt
#define OSIF_ENTER_ISR()        (g_osif_inside_isr++)
#define OSIF_EXIT_ISR()         (g_osif_inside_isr--)
#define __OSIF_IsIsrContext()   ((g_osif_inside_isr != 0) ? true : false)  

#endif
