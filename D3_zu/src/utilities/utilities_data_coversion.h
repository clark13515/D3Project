/*** 
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\utilities\utilities_data_coversion.h
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-25 10:56:10
 * @LastEditTime: 2023-01-25 11:25:40
 */


#ifndef UTILITIES_DATA_COVERSION_H_
#define UTILITIES_DATA_COVERSION_H_
#include "stdint.h"

#define UTILITY_LITTLE_ENDIAN 0
#define UTILITY_BIG_ENDIAN    1

#define UTILITY_FILL_DATA_FROM_ARRAY(pdata, type_len, array_u8, endian)\
do{\
    *(pdata) = 0;\
    if ((endian) == UTILITY_LITTLE_ENDIAN){\
        for(uint8_t cnt=0; cnt<(type_len); cnt++){\
            (*(pdata)) |= ((*((array_u8)+cnt))&0xff)<<(cnt*8);\
        }\
    }\
    else{\
        for(uint8_t cnt=0; cnt<(type_len); cnt++){\
            (*(pdata)) |= ((*((array_u8)+cnt))&0xff)<<(((type_len)-1-cnt)*8);\
        }\
    }\
}while(0)

#define UTILITY_FILL_ARRAY_FROM_DATA(array_u8, type_len, data, endian)\
do{\
    if ((endian) == UTILITY_LITTLE_ENDIAN){\
        for(uint8_t cnt=0; cnt<(type_len); cnt++){\
            *((array_u8)+cnt) = ((data)>>(cnt*8))&0xff;\
        }\
    }\
    else{\
        for(uint8_t cnt=0; cnt<(type_len); cnt++){\
            *((array_u8)+cnt) = ((data)>>(((type_len)-1-cnt)*8))&0xff;\
        }\
    }\
}while(0)

extern uint32_t utilities_swap_uint32(uint32_t data);
extern uint16_t utilities_swap_uint16(uint16_t data);

#endif /* UTILITIES_DATA_COVERSION_H_ */
