/*** 
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\driver\sdcard.h
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-28 20:09:09
 * @LastEditTime: 2023-01-28 21:40:31
 */
#ifndef SDCARD_H
#define SDCARD_H

int platform_init_fs();
int sd_mount();
int sd_write_data(char *file_name,u32 src_addr,u32 byte_len);
int sd_read_data(char *file_name,u32 src_addr,u32 byte_len);
int sd_write_data_withoffset(char *file_name,u32 src_addr,u32 byte_len,u32 offset);
int sdcard_demo();
int fsdemo();

#endif
