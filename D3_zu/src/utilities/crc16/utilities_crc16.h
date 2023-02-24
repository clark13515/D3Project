/*** 
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\utilities\crc16\utilities_crc16.h
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-25 10:56:10
 * @LastEditTime: 2023-01-25 11:26:37
 */

#ifndef __UTILITIES_CRC16_H__
#define __UTILITIES_CRC16_H__

#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
//=================================================================================//
extern uint16_t UTILITY_CRC16_Calculate(uint8_t init_value, const uint8_t data[], uint32_t data_length, bool is_init_enable);
#ifdef __cplusplus
}
#endif
#endif /* __UTILITIES_CRC16_H__ */
