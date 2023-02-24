/*
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\utilities\frame.c
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-25 10:56:10
 * @LastEditTime: 2023-01-25 11:26:09
 */

#include "frame.h"
#include "fifo.h"

#define DEFAULT_FRAME_HEAD    (0x44)
struct __Frame_HandleStruct
{
	FifoPtr  fifo;
	uint8_t  *frame_buf;
	uint16_t  frame_buf_size;
	Frame_CheckTypeEnum check_type;
	GetTickFuncPtr get_tick_func_ptr;
	uint32_t tick_start;
	uint32_t timeout;
	uint8_t  frame_head;
	uint8_t  frame_len_min;
	uint8_t  frame_prefetch_len;
	uint8_t  frame_head_enabled;
	uint8_t  frame_less_flag;
	uint8_t  force_frame_len;
};

static void update_static_len(Frame_HandlePtr hframe)
{
	if(hframe->force_frame_len > 0)
	{
		hframe->frame_prefetch_len = 1;  // 只判断帧头
		hframe->frame_len_min = hframe->force_frame_len;
	}
	else
	{
		hframe->frame_prefetch_len = 2 + hframe->frame_head_enabled;
		if(FRAME_CHECK_SUM == hframe->check_type)
		{
			hframe->frame_len_min = hframe->frame_prefetch_len + 1;
		}
		else if(FRAME_CHECK_CRC32 == hframe->check_type)
		{
			hframe->frame_len_min = hframe->frame_prefetch_len + 4;
		}
	}
}

Frame_HandlePtr Frame_New(uint32_t fifo_size, uint8_t *frame_buf, uint16_t frame_buf_size)
{
	if(0 == fifo_size || NULL == frame_buf || 0 == frame_buf_size)
	{
		return NULL;
	}

	Frame_HandlePtr hframe = (Frame_HandlePtr)malloc(sizeof(struct __Frame_HandleStruct));
	if(NULL == hframe)
	{
		return NULL;
	}
	else
	{
		hframe->fifo = Fifo_New(fifo_size);
		if(NULL == hframe->fifo)
		{
			free(hframe);
			return NULL;
		}
		else
		{
			hframe->frame_buf = frame_buf;
			hframe->frame_buf_size = frame_buf_size;
			hframe->check_type = FRAME_CHECK_SUM;
			hframe->get_tick_func_ptr = NULL;
			hframe->tick_start = 0;
			hframe->timeout = 0;
			hframe->frame_head = DEFAULT_FRAME_HEAD;
			hframe->frame_head_enabled = 1;
			update_static_len(hframe);
			hframe->frame_less_flag = 0;
			hframe->force_frame_len = 0;
		}
	}

	return hframe;
}

Frame_StatusEnum Frame_WriteFifo(Frame_HandlePtr hframe, uint8_t *buf, uint32_t size)
{
	if(NULL == hframe)
	{
		return FRAME_NULL_PTR;
	}
	if(size > Fifo_GetFifoSize(hframe->fifo) - Fifo_GetDataSize(hframe->fifo))
	{
		return FRAME_FIFO_OVERFLOW;
	}
	Fifo_Write(hframe->fifo, buf, size);
	
	return FRAME_OK;
}

Frame_StatusEnum Frame_Search(Frame_HandlePtr hframe)
{
	static uint8_t buf[4];

	if(NULL == hframe)
	{
		return FRAME_NULL_PTR;
	}

	while(Fifo_GetDataSize(hframe->fifo) >= hframe->frame_len_min)
	{
		Fifo_Read(hframe->fifo, buf, hframe->frame_prefetch_len);

		uint8_t i = 0;

		/* check head */
		if(hframe->frame_head_enabled)
		{
			uint8_t head = buf[i];
			if(hframe->frame_head != head)
			{
				Fifo_UpdateReadPos(hframe->fifo, 1);
				continue;
			}
			i++;
		}

		/* check frame len */
		uint8_t len;
		if(hframe->force_frame_len > 0)
		{
			len = hframe->force_frame_len;
		}else
		{
			len = buf[i];
			if(len > hframe->frame_buf_size || len < hframe->frame_len_min)
			{
				Fifo_UpdateReadPos(hframe->fifo, 1);
				continue;
			}

			/* check data enough or not */
			if(Fifo_GetDataSize(hframe->fifo) < len)
			{
				if(NULL != hframe->get_tick_func_ptr)
				{
					if(hframe->frame_less_flag)
					{
						/* timeout */
						if((*hframe->get_tick_func_ptr)() - hframe->tick_start > hframe->timeout)
						{
							hframe->frame_less_flag = 0;
							Fifo_UpdateReadPos(hframe->fifo, 1);
							continue;
						}
					}
					else
					{
						hframe->frame_less_flag = 1;
						hframe->tick_start = (*hframe->get_tick_func_ptr)();
					}
				}
				return FRAME_LESS;
			}
			else
			{
				hframe->frame_less_flag = 0;
			}
		}

		/* read one frame from fifo */
		Fifo_Read(hframe->fifo, hframe->frame_buf, len);

		/* frame check */
		if(FRAME_CHECK_SUM == hframe->check_type)
		{
			uint8_t sum = Frame_CalSum(hframe->frame_buf, len-1);
			if(sum != hframe->frame_buf[len-1])
			{
				Fifo_UpdateReadPos(hframe->fifo, 1);
				continue;
			}
		}
		else if(FRAME_CHECK_CRC32 == hframe->check_type)
		{
			uint32_t crc32 = Frame_CalCrc32(hframe->frame_buf, len-4);
			uint32_t crc32_1 = hframe->frame_buf[len-4] + ((uint32_t)hframe->frame_buf[len-3] << 8) + ((uint32_t)hframe->frame_buf[len-2] << 16) + ((uint32_t)hframe->frame_buf[len-1] << 24);
			if(crc32 != crc32_1)
			{
				Fifo_UpdateReadPos(hframe->fifo, 1);
				continue;
			}
		}

		/* got one frame */
		Fifo_UpdateReadPos(hframe->fifo, len);
		return FRAME_OK;
	}
	return FRAME_LESS;
}

void Frame_Delete(Frame_HandlePtr hframe)
{
	if(NULL == hframe)
	{
		return;
	}
	Fifo_Delete(hframe->fifo);
	free(hframe);
}

void Frame_SetHead(Frame_HandlePtr hframe, uint8_t head)
{
	if(NULL == hframe)
	{
		return;
	}
	hframe->frame_head = head;
}
void Frame_SetCheckArith(Frame_HandlePtr hframe, Frame_CheckTypeEnum check_type)
{
	if(NULL == hframe)
	{
		return;
	}
	hframe->check_type = check_type;
	update_static_len(hframe);
}

void Frame_ForceFrameLen(Frame_HandlePtr hframe, uint8_t frame_len)
{
	hframe->force_frame_len = frame_len;
	update_static_len(hframe);
}

void Frame_DiscardHead(Frame_HandlePtr hframe)
{
	if(NULL == hframe)
	{
		return;
	}
	hframe->frame_head_enabled = 0;
	update_static_len(hframe);
}

void Frame_RegGetTickFunc(Frame_HandlePtr hframe, GetTickFuncPtr func, uint32_t timeout)
{
	if(NULL == hframe)
	{
		return;
	}
	hframe->get_tick_func_ptr = func;
	hframe->timeout = timeout;
}

uint32_t Frame_GetFifoDataSize(Frame_HandlePtr hframe)
{
	if(NULL == hframe)
	{
		return 0;
	}
	return Fifo_GetDataSize(hframe->fifo);
}

uint32_t Frame_CopyFifoDataToFrameBuf(Frame_HandlePtr hframe)
{
	if(NULL == hframe)
	{
		return 0;
	}
	uint32_t data_size = Fifo_GetDataSize(hframe->fifo);
	uint32_t size;
	if(data_size > hframe->frame_buf_size)
	{
		size = hframe->frame_buf_size;
	}
	else
	{
		size = data_size;
	}
	Fifo_Read(hframe->fifo, hframe->frame_buf, size);
	return size;
}

void Frame_FifoUpdateReadPos(Frame_HandlePtr hframe, uint32_t inc)
{
	Fifo_UpdateReadPos(hframe->fifo, inc);
}

uint8_t Frame_CalSum(uint8_t *data, uint32_t size)
{
	uint8_t sum = 0;
	for(uint32_t i = 0; i < size; i++)
	{
		sum += data[i];
	}
	return sum;
}

uint32_t Frame_CalCrc32(uint8_t *data, uint32_t size)
{
    uint32_t i,j,temp,crc = 0xFFFFFFFF;
    uint32_t n, r;

    n = size / 4;
    r = size % 4;

    for(i = 0; i < n; i++)
    {
        temp = *(uint32_t*)(data + (i<<2));
        for(j = 0; j < 32; j++)
        {
            if( (crc ^ temp) & 0x80000000 )
            {
                crc = 0x04C11DB7 ^ (crc<<1);
            }
            else
            {
                crc <<=1;
            }
            temp<<=1;
        }
    }

    if(r)
    {
        temp = 0;
        for(i = 0; i < r; i++)
        {
            temp += (data[4*n+i] << (8*i));
        }

        for(j = 0; j < 32; j++)
        {
            if( (crc ^ temp) & 0x80000000 )
            {
                crc = 0x04C11DB7 ^ (crc<<1);
            }
            else
            {
                crc <<=1;
            }
            temp<<=1;
        }
    }

    return crc;
}
