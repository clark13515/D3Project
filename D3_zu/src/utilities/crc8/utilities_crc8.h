/*** 
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\utilities\crc8\utilities_crc8.h
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-25 10:56:10
 * @LastEditTime: 2023-01-25 11:26:44
 */


#ifndef __UTILITIES_CRC8_H__
#define __UTILITIES_CRC8_H__

#include <stdint.h>
#include <stdbool.h>

extern uint8_t UTILITY_CRC8_Calculate(uint8_t init_value, const uint8_t data[], uint32_t data_length, bool is_init_enable);

#endif /* __UTILITIES_CRC8_H__ */

