/*
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\utilities\fifo.c
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-25 10:56:10
 * @LastEditTime: 2023-01-25 11:26:19
 */

#include "fifo.h"

struct __FifoStruct
{
	uint8_t *buf;
	uint32_t size;
	uint32_t ReadPos;
	uint32_t WritePos;
};

FifoPtr Fifo_New(uint32_t size)
{
	if(0 == size)
	{
		return NULL;
	}

	FifoPtr fifo_ptr = (FifoPtr)malloc(sizeof(struct __FifoStruct));
	if(NULL == fifo_ptr)
	{
		return NULL;
	}
	else
	{
		fifo_ptr->buf = (uint8_t*)malloc(size * sizeof(uint8_t));
		if(NULL == fifo_ptr->buf)
		{
			free(fifo_ptr);
			return NULL;
		}
		else
		{
			fifo_ptr->size = size;
			fifo_ptr->ReadPos = 0;
			fifo_ptr->WritePos = 0;
		}
	}
	return fifo_ptr;
}

uint32_t Fifo_GetFifoSize(FifoPtr fifo)
{
	if(NULL == fifo)
	{
		return 0;
	}
	return fifo->size;
}

uint32_t Fifo_GetDataSize(FifoPtr fifo)
{
	if(NULL == fifo)
	{
		return 0;
	}

	return (fifo->WritePos + fifo->size - fifo->ReadPos) % fifo->size;
}

void Fifo_Write(FifoPtr fifo, uint8_t *buf, uint32_t size)
{
	uint32_t i;

	if ((NULL == fifo) || (NULL == buf) || (0 == size) || (size > fifo->size))
	{
		return;  
	}

	for (i = 0; i < size; i++)
	{
		fifo->buf[fifo->WritePos++] = buf[i];
		fifo->WritePos %= fifo->size;
	}
}

void Fifo_Read(FifoPtr fifo, uint8_t *buf, uint32_t size)
{
	uint32_t i, ReadPos = fifo->ReadPos;

	if ((NULL == fifo) || (NULL == buf) || (0 == size) || (size > fifo->size))
	{
		return;  
	}

	for (i = 0; i < size; i++)
	{
		buf[i] = fifo->buf[ReadPos++];
		ReadPos %= fifo->size;
	}
}

void Fifo_UpdateReadPos(FifoPtr fifo, uint32_t inc)
{
	if(NULL == fifo)
	{
		return;
	}
	fifo->ReadPos = (fifo->ReadPos + inc) % fifo->size;
}

void Fifo_Delete(FifoPtr fifo)
{
	if(NULL == fifo)
	{
		return;
	}
	free(fifo->buf);
	free(fifo);
}
