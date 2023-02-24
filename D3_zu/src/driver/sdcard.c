/*
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\driver\sdcard.c
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-28 20:06:18
 * @LastEditTime: 2023-01-29 19:40:03
 */
#include "xil_printf.h"
#include "xparameters.h"
#include "ff.h"
#include "integer.h"
#include "xstatus.h"

 
static FATFS fatfs; //filesystem
const char src_str[30] = "time is money"; //test text
#define    FILE_NAME    "start.txt"          //name
 
/**
 * @brief: init file system
 * @return {*}
 */
int platform_init_fs()
{
	FRESULT status;
	TCHAR* path = "0:/";
	BYTE work[FF_MAX_SS]; //工作区域
	//注册一个工作区（挂载分区文件系统），在使用任何其他文件函数之前，必须使用f_mount为每个使用卷分配一个工作区
	status = f_mount(&fatfs, path, 1); //挂载 SD 卡
    if(status != FR_OK)
    {
    	xil_printf("Volume is not FAT formated; formating FAT\r\n");
    	//格式化SD卡
    	status = f_mkfs(path, FM_FAT32,0, work, sizeof work);
    	if (status != FR_OK)
    	{
    	    xil_printf("Unable to format FATfs\r\n");
    	    return -1;
    	}
    	//格式化之后，重新挂载 SD 卡
    	status = f_mount(&fatfs, path, 1);
    	if (status != FR_OK) {
    	xil_printf("Unable to mount FATfs\r\n");
    	return -1;
    	}
    }
    return 0;
 
}
//挂载 SD(TF)卡
int sd_mount()
{
	int status;
	//初始化文件系统（挂载SD卡，如果挂载不成功，则格式化SD卡）
	status = platform_init_fs();
	if(status)
	{
		xil_printf("error: f_mont returned %d!\n",status);
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}
//SD 卡写数据
int sd_write_data(char *file_name,u32 src_addr,u32 byte_len)
{
	FIL fil;        /* File object */
	UINT bw; //f_write 函数返回已写入的字节数
 
	//打开一个文件，如果不存在，就创建一个新文件
	f_open(&fil, file_name, FA_CREATE_ALWAYS|FA_WRITE);
	//移动打开的文件对象的文件读/写指针 0:指向文件开头
	f_lseek(&fil,0);
	//写入数据
	f_write (&fil, (void*)src_addr,byte_len,&bw);
	//关闭文件
	f_close(&fil);
	return 0;
}

int sd_write_data_withoffset(char *file_name,u32 src_addr,u32 byte_len,u32 offset)
{
	FIL fil;        /* File object */
	UINT bw; //f_write 函数返回已写入的字节数
 
	//打开一个文件，如果不存在，就创建一个新文件
	f_open(&fil, file_name, FA_CREATE_ALWAYS|FA_WRITE);
	//移动打开的文件对象的文件读/写指针 0:指向文件开头
	f_lseek(&fil,offset);
	//写入数据
	f_write (&fil, (void*)src_addr,byte_len,&bw);
	//关闭文件
	f_close(&fil);
	return 0;
}

//SD 卡读数据
int sd_read_data(char *file_name,u32 src_addr,u32 byte_len)
{
 FIL fil; //文件对象
 UINT br; //f_read 函数返回已读出的字节数
 
 //打开一个只读的文件
 f_open(&fil,file_name,FA_READ);
 //移动打开的文件对象的文件读/写指针 0:指向文件开头
 f_lseek(&fil,0);
 //从 SD 卡中读出数据
 f_read(&fil,(void*)src_addr,byte_len,&br);
 //关闭文件
 f_close(&fil);
 return 0;
}

int sdcard_demo()
{
	int status,len;
	char dest_str[30] = "";

	len = strlen(src_str); //计算字符串长度
	sd_write_data(FILE_NAME,(u32)src_str,len); //写入数据
	sd_read_data(FILE_NAME,(u32)dest_str,len); //读取数据
 
	if(strcmp(src_str, dest_str) == 0)
		xil_printf("src_str is equal to dest_str,SD card test success!\n");
	else
		xil_printf("src_str is not equal to dest_str,SD card test failed!\n");
 
	return 0;
}


int fsdemo()
{  
    int j,k;
    FRESULT fr;
    FIL fp_out;
    f_mount(&fatfs,"",0);
    f_mount(&fatfs,"",1);
    fr = f_open(&fp_out,"result.txt",FA_WRITE);//打开文件
    if(fr != FR_OK)      //测试是否打开文件
    {
        print("open fail\n\r");
        return 0;
    }   
    for(j=0;j<3325;j++)
    {
        for(k=0;k<2000;k++)
        {               
            f_printf(&fp_out,"%d %d %d %d %d %d\n",4000,4000,1600,55,66,77);
        }
    }
    fr = f_close(&fp_out);

    print("Hello World\n\r");
    return 0;
}
