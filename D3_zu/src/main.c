/*
 * @Copyright: Copyright (C) by Benewake Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\main.c
 * @PROJECT: P4
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2022-05-30 18:29:54
 * @LastEditTime: 2023-02-16 16:38:35
 */
#include "osif.h"
#include "app.h"
#include "utilities_plat_common.h"
#include "drivers_common.h"



void SYS_Init()
{
    Gpiopl_initall();
    PL_CTRL(0); // start PL 
    DMA_Intr_Init(&AxiDma, 0);

	Init_Intr_System(&xInterruptController);

	Setup_Intr_Exception(&xInterruptController);
	/*set interrupt of AXI DMA*/
    DMA_Setup_Intr_System(&xInterruptController,&AxiDma,RX_INTR_ID);//setup dma interrpt system

	DMA_Intr_Enable(&xInterruptController, &AxiDma);

    PL_CTRL(1);

}
int main(void)
{
    SYS_Init();
    OSIF_Init();
    ModuleInitcall(MODULE_INIT_BDEV);
    ModuleInitcall(MODULE_INIT_BVDEV);

    APP_Init();
    OSIF_Start();
    return 0;
}
