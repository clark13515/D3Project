/*** 
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\utilities\utilities_ringbuffer.h
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-25 10:56:10
 * @LastEditTime: 2023-01-25 11:25:02
 */


#ifndef __UTILITIES_RINGBUFFER_H__
#define __UTILITIES_RINGBUFFER_H__

#include "stdint.h"
#include "stddef.h"

/* ring buffer */
typedef struct utilities_ringbuffer
{
    uint8_t *buffer_ptr;
    /* use the msb of the {read,write}_index as mirror bit. You can see this as
     * if the buffer adds a virtual mirror and the pointers point either to the
     * normal or to the mirrored buffer. If the write_index has the same value
     * with the read_index, but in a different mirror, the buffer is full.
     * While if the write_index and the read_index are the same and within the
     * same mirror, the buffer is empty. The ASCII art of the ringbuffer is:
     *
     *          mirror = 0                    mirror = 1
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Full
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     *  read_idx-^                   write_idx-^
     *
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Empty
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * read_idx-^ ^-write_idx
     *
     * The tradeoff is we could only use 32KiB of buffer for 16 bit of index.
     * But it should be enough for most of the cases.
     *
     * Ref: http://en.wikipedia.org/wiki/Circular_buffer#Mirroring */
    uint16_t read_mirror : 1;
    uint16_t read_index : 15;
    uint16_t write_mirror : 1;
    uint16_t write_index : 15;
    /* as we use msb of index as mirror bit, the size should be signed and
     * could only be positive. */
    int16_t  buffer_size;
} utilities_ringbuffer_t;



/**
 * RingBuffer for DeviceDriver
 *
 * Please note that the ring buffer implementation
 * has no thread wait or resume feature.
 */
extern void   utilities_ringbuffer_init(utilities_ringbuffer_t *rb, uint8_t *pool, int16_t size);
extern size_t utilities_ringbuffer_put(utilities_ringbuffer_t *rb, const uint8_t *ptr, uint16_t length);
extern size_t utilities_ringbuffer_put_force(utilities_ringbuffer_t *rb, const uint8_t *ptr, uint16_t length);
extern size_t utilities_ringbuffer_putchar(utilities_ringbuffer_t *rb, const uint8_t ch);
extern size_t utilities_ringbuffer_putchar_force(utilities_ringbuffer_t *rb, const uint8_t ch);
extern size_t utilities_ringbuffer_get(utilities_ringbuffer_t *rb, uint8_t *ptr, uint16_t length);
extern size_t utilities_ringbuffer_getchar(utilities_ringbuffer_t *rb, uint8_t *ch);

enum utilities_ringbuffer_state
{
    UTILITIES_RINGBUFFER_EMPTY,
    UTILITIES_RINGBUFFER_FULL,
    /* half full is neither full nor empty */
    UTILITIES_RINGBUFFER_HALFFULL,
};

static inline uint16_t utilities_ringbuffer_get_size(utilities_ringbuffer_t *rb)
{
    return rb->buffer_size;
}

static inline enum utilities_ringbuffer_state utilities_ringbuffer_status(utilities_ringbuffer_t *rb)
{
    if (rb->read_index == rb->write_index)
    {
        if (rb->read_mirror == rb->write_mirror)
            return UTILITIES_RINGBUFFER_EMPTY;
        else
            return UTILITIES_RINGBUFFER_FULL;
    }
    return UTILITIES_RINGBUFFER_HALFFULL;
}

/** return the size of data in rb */
static inline uint16_t utilities_ringbuffer_data_len(utilities_ringbuffer_t *rb)
{
    switch (utilities_ringbuffer_status(rb))
    {
        case UTILITIES_RINGBUFFER_EMPTY:
            return 0;
        case UTILITIES_RINGBUFFER_FULL:
            return rb->buffer_size;
        case UTILITIES_RINGBUFFER_HALFFULL:
        default:
            if (rb->write_index > rb->read_index)
                return rb->write_index - rb->read_index;
            else
                return rb->buffer_size - (rb->read_index - rb->write_index);
    };
}

/** return the size of empty space in rb */
#define  utilities_ringbuffer_space_len(rb) ((rb)->buffer_size - utilities_ringbuffer_data_len(rb))



#endif /* __UTILITIES_RINGBUFFER_H__ */
