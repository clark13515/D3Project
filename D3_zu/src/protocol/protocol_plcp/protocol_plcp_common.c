/*
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\protocol\protocol_plcp\protocol_plcp_common.c
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-02-17 16:07:17
 * @LastEditTime: 2023-02-17 16:07:21
 */
/*
 * @Copyright: Copyright (C) by Benewake Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \p4_worksapce_zu5\sources\protocol\protocol_common.c
 * @PROJECT: P4
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2022-12-28 09:17:07
 * @LastEditTime: 2023-01-03 11:06:36
 */


#include "utilities.h"


uint32_t protocol_common_calculation_check_with_protocol_version(uint16_t protocol_version, const uint8_t *p_data, uint32_t data_len)
{
    uint32_t check = 0;

    for(uint32_t i = 0; i < data_len; i++)
    {
        check += p_data[i];
    }
    

    return check;
}
