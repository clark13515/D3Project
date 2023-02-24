/*** 
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\utilities\fifo.h
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-25 10:56:10
 * @LastEditTime: 2023-01-25 11:26:13
 */


#ifndef _FIFO_H
#define _FIFO_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>

typedef struct __FifoStruct *FifoPtr;

extern FifoPtr Fifo_New(uint32_t size);
extern uint32_t Fifo_GetFifoSize(FifoPtr fifo);
extern uint32_t Fifo_GetDataSize(FifoPtr fifo);
extern void Fifo_Write(FifoPtr fifo, uint8_t *buf, uint32_t size);
extern void Fifo_Read(FifoPtr fifo, uint8_t *buf, uint32_t size);
extern void Fifo_UpdateReadPos(FifoPtr fifo, uint32_t inc);
extern void Fifo_Delete(FifoPtr fifo);

#ifdef __cplusplus
}
#endif
#endif

