/*** 
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\utilities\utilities_const_macros.h
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-25 10:56:10
 * @LastEditTime: 2023-01-25 11:25:56
 */

#ifndef __UTILITIES_CONST_MACROS_H__
#define __UTILITIES_CONST_MACROS_H__

//========================================================//
#define LE_BIT0      0x00000001
#define LE_BIT1      0x00000002
#define LE_BIT2      0x00000004
#define LE_BIT3      0x00000008
#define LE_BIT4      0x00000010
#define LE_BIT5      0x00000020
#define LE_BIT6      0x00000040
#define LE_BIT7      0x00000080
#define LE_BIT8      0x00000100
#define LE_BIT9      0x00000200
#define LE_BIT10     0x00000400
#define LE_BIT11     0x00000800
#define LE_BIT12     0x00001000
#define LE_BIT13     0x00002000
#define LE_BIT14     0x00004000
#define LE_BIT15     0x00008000
#define LE_BIT16     0x00010000
#define LE_BIT17     0x00020000
#define LE_BIT18     0x00040000
#define LE_BIT19     0x00080000
#define LE_BIT20     0x00100000
#define LE_BIT21     0x00200000
#define LE_BIT22     0x00400000
#define LE_BIT23     0x00800000
#define LE_BIT24     0x01000000
#define LE_BIT25     0x02000000
#define LE_BIT26     0x04000000
#define LE_BIT27     0x08000000
#define LE_BIT28     0x10000000
#define LE_BIT29     0x20000000
#define LE_BIT30     0x40000000
#define LE_BIT31     0x80000000
#define LE_BIT(n)    LE_BIT##n

#define LE_MASK_BIT0   0x00000001
#define LE_MASK_BIT1   0x00000002
#define LE_MASK_BIT2   0x00000004
#define LE_MASK_BIT3   0x00000008
#define LE_MASK_BIT4   0x00000010
#define LE_MASK_BIT5   0x00000020
#define LE_MASK_BIT6   0x00000040
#define LE_MASK_BIT7   0x00000080
#define LE_MASK_BIT8   0x00000100
#define LE_MASK_BIT9   0x00000200
#define LE_MASK_BIT10  0x00000400
#define LE_MASK_BIT11  0x00000800
#define LE_MASK_BIT12  0x00001000
#define LE_MASK_BIT13  0x00002000
#define LE_MASK_BIT14  0x00004000
#define LE_MASK_BIT15  0x00008000
#define LE_MASK_BIT16  0x00010000
#define LE_MASK_BIT17  0x00020000
#define LE_MASK_BIT18  0x00040000
#define LE_MASK_BIT19  0x00080000
#define LE_MASK_BIT20  0x00100000
#define LE_MASK_BIT21  0x00200000
#define LE_MASK_BIT22  0x00400000
#define LE_MASK_BIT23  0x00800000
#define LE_MASK_BIT24  0x01000000
#define LE_MASK_BIT25  0x02000000
#define LE_MASK_BIT26  0x04000000
#define LE_MASK_BIT27  0x08000000
#define LE_MASK_BIT28  0x10000000
#define LE_MASK_BIT29  0x20000000
#define LE_MASK_BIT30  0x40000000
#define LE_MASK_BIT31  0x80000000

/* Generate the bit field mask from msb to lsb */
#define LE_MASK_BIT(n)            (1<<(n))
#define LE_MASK_BITS(msb,lsb)    ((0xFFFFFFFF>>(32-((msb)-(lsb)+1)))<<(lsb))

/* Generate the bit field mask with offset and length */
#define LE_MASK_BITS_OFFSET_LENGTH(offset,length)  ((((uint32_t)0xFFFFFFFF)>>(32-length))<<offset)

#define ALL4BITS     0xF
#define ALL8BITS     0xFF
#define ALL16BITS    0xFFFF
#define ALL32BITS    0xFFFFFFFF

#ifndef __ASSEMBLER__
enum {
    BLOCKING        =-1,
    NON_BLOCKING    =0,
};
#endif    /*__ASSEMBLER__*/
#endif    /*__UTILITIES_CONST_MACROS_H__*/
