/*** 
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\utilities\crc32\utilities_crc32.h
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-25 10:56:10
 * @LastEditTime: 2023-01-25 11:26:28
 */
/*


#ifndef __UTILITIES_CRC32_H__
#define __UTILITIES_CRC32_H__

#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
//=================================================================================//
uint32_t UTILITY_CRC32_Calculate(uint32_t init_value, const uint8_t data[], uint32_t data_length, bool is_init_enable,bool result_xor_enable);
//=================================================================================//
#ifdef __cplusplus
}
#endif
#endif /* __UTILITIES_CRC32_H__ */

