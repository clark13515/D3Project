/*
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\utilities\utilities_ringbuffer.c
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-25 10:56:10
 * @LastEditTime: 2023-01-25 11:25:12
 */



#include "utilities_ringbuffer.h"

/**
 * @brief  Return the most contiguous size aligned at specified width. RT_ALIGN(13, 4)
 * would return 16.
 * 
 */
#define UTILITIES_ALIGN(size,align)         (((size) + (align) -1) & ~((align) - 1)
/**
 * @brief  Return the down number of aligned at specified width. RT_ALIGN_DOWN(13, 4)
 * would return 12.
 * 
 */
#define UTILITIES_ALIGN_DOWN(size,align)    ((size)&~((align)-1))
#define UTILITIES_ALIGN_SIZE                (4)

void utilities_ringbuffer_init(utilities_ringbuffer_t *rb, uint8_t *pool, int16_t size)
{
    /* initialize read and write index */
    rb->read_mirror = rb->read_index = 0;
    rb->write_mirror = rb->write_index = 0;

    /* set buffer pool and size */
    rb->buffer_ptr = pool;
    rb->buffer_size = UTILITIES_ALIGN_DOWN(size, UTILITIES_ALIGN_SIZE);
}


/**
 * put a block of data into ring buffer
 */
size_t utilities_ringbuffer_put(utilities_ringbuffer_t *rb, const uint8_t *ptr, uint16_t length)
{
    uint16_t size;

    /* whether has enough space */
    size = utilities_ringbuffer_space_len(rb);

    /* no space */
    if (size == 0)
        return 0;

    /* drop some data */
    if (size < length)
        length = size;

    if (rb->buffer_size - rb->write_index > length)
    {
        /* read_index - write_index = empty space */
        memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
        /* this should not cause overflow because there is enough space for
         * length of data in current mirror */
        rb->write_index += length;
        return length;
    }

    memcpy(&rb->buffer_ptr[rb->write_index],
           &ptr[0],
           rb->buffer_size - rb->write_index);
    memcpy(&rb->buffer_ptr[0],
           &ptr[rb->buffer_size - rb->write_index],
           length - (rb->buffer_size - rb->write_index));

    /* we are going into the other side of the mirror */
    rb->write_mirror = ~rb->write_mirror;
    rb->write_index = length - (rb->buffer_size - rb->write_index);

    return length;
}


/**
 * put a block of data into ring buffer
 *
 * When the buffer is full, it will discard the old data.
 */
size_t utilities_ringbuffer_put_force(utilities_ringbuffer_t *rb, const uint8_t *ptr, uint16_t length)
{
    enum utilities_ringbuffer_state old_state;

    old_state = utilities_ringbuffer_status(rb);

    if (length > rb->buffer_size)
        length = rb->buffer_size;

    if (rb->buffer_size - rb->write_index > length)
    {
        /* read_index - write_index = empty space */
        memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
        /* this should not cause overflow because there is enough space for
         * length of data in current mirror */
        rb->write_index += length;

        if (old_state == UTILITIES_RINGBUFFER_FULL)
            rb->read_index = rb->write_index;

        return length;
    }

    memcpy(&rb->buffer_ptr[rb->write_index],
           &ptr[0],
           rb->buffer_size - rb->write_index);
    memcpy(&rb->buffer_ptr[0],
           &ptr[rb->buffer_size - rb->write_index],
           length - (rb->buffer_size - rb->write_index));

    /* we are going into the other side of the mirror */
    rb->write_mirror = ~rb->write_mirror;
    rb->write_index = length - (rb->buffer_size - rb->write_index);

    if (old_state == UTILITIES_RINGBUFFER_FULL)
    {
        rb->read_mirror = ~rb->read_mirror;
        rb->read_index = rb->write_index;
    }

    return length;
}


/**
 *  get data from ring buffer
 */
size_t utilities_ringbuffer_get(utilities_ringbuffer_t *rb, uint8_t *ptr, uint16_t length)
{
    size_t size;

    /* whether has enough data  */
    size = utilities_ringbuffer_data_len(rb);

    /* no data */
    if (size == 0)
        return 0;

    /* less data */
    if (size < length)
        length = size;

    if (rb->buffer_size - rb->read_index > length)
    {
        /* copy all of data */
        memcpy(ptr, &rb->buffer_ptr[rb->read_index], length);
        /* this should not cause overflow because there is enough space for
         * length of data in current mirror */
        rb->read_index += length;
        return length;
    }

    memcpy(&ptr[0],
           &rb->buffer_ptr[rb->read_index],
           rb->buffer_size - rb->read_index);
    memcpy(&ptr[rb->buffer_size - rb->read_index],
           &rb->buffer_ptr[0],
           length - (rb->buffer_size - rb->read_index));

    /* we are going into the other side of the mirror */
    rb->read_mirror = ~rb->read_mirror;
    rb->read_index = length - (rb->buffer_size - rb->read_index);

    return length;
}

/**
 * put a character into ring buffer
 */
size_t utilities_ringbuffer_putchar(utilities_ringbuffer_t *rb, const uint8_t ch)
{
    /* whether has enough space */
    if (!utilities_ringbuffer_space_len(rb))
        return 0;

    rb->buffer_ptr[rb->write_index] = ch;

    /* flip mirror */
    if (rb->write_index == rb->buffer_size-1)
    {
        rb->write_mirror = ~rb->write_mirror;
        rb->write_index = 0;
    }
    else
    {
        rb->write_index++;
    }

    return 1;
}

/**
 * put a character into ring buffer
 *
 * When the buffer is full, it will discard one old data.
 */
size_t utilities_ringbuffer_putchar_force(utilities_ringbuffer_t *rb, const uint8_t ch)
{
    enum utilities_ringbuffer_state old_state;

    old_state = utilities_ringbuffer_status(rb);

    rb->buffer_ptr[rb->write_index] = ch;

    /* flip mirror */
    if (rb->write_index == rb->buffer_size-1)
    {
        rb->write_mirror = ~rb->write_mirror;
        rb->write_index = 0;
        if (old_state == UTILITIES_RINGBUFFER_FULL)
        {
            rb->read_mirror = ~rb->read_mirror;
            rb->read_index = rb->write_index;
        }
    }
    else
    {
        rb->write_index++;
        if (old_state == UTILITIES_RINGBUFFER_FULL)
            rb->read_index = rb->write_index;
    }

    return 1;
}

/**
 * get a character from a ringbuffer
 */
size_t utilities_ringbuffer_getchar(utilities_ringbuffer_t *rb, uint8_t *ch)
{
    /* ringbuffer is empty */
    if (!utilities_ringbuffer_data_len(rb))
        return 0;

    /* put character */
    *ch = rb->buffer_ptr[rb->read_index];

    if (rb->read_index == rb->buffer_size-1)
    {
        rb->read_mirror = ~rb->read_mirror;
        rb->read_index = 0;
    }
    else
    {
        rb->read_index++;
    }

    return 1;
}