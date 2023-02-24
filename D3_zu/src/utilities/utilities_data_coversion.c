/*
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\utilities\utilities_data_coversion.c
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-25 10:56:10
 * @LastEditTime: 2023-01-25 11:25:49
 */

#include "utilities_data_coversion.h"

uint32_t utilities_swap_uint32(uint32_t data)
{
    uint32_t swap = ((data & 0x000000FF) << 24) |
               ((data & 0x0000FF00) << 8) |
               ((data & 0x00FF0000) >> 8) |
               ((data & 0xFF000000) >> 24) ;
    return swap;
}


uint16_t utilities_swap_uint16(uint16_t data)
{
    uint16_t swap = ((data & 0x00FF) << 8 ) | ((data & 0xFF00) >> 8);
    return swap;
}