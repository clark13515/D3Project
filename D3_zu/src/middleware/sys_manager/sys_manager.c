/*
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\middleware\sys_manager\sys_manager.c
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-25 11:31:35
 * @LastEditTime: 2023-01-25 11:38:36
 */


//-------------------------------------------------------------------------------------------------
// External component include
//-------------------------------------------------------------------------------------------------
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "sys_manager.h"

volatile static uint32_t     s_config_tbl_size = 0;
static Comp_Configuration_t* s_config_tbl      = NULL;

extern Component_Entry_Type _compt_start, _compt_end;
/**
 * @brief  lanuch up components by stage
 * @param  pvParameters 
 * @param  i_stage 
 */
void Comp_Startup(Startup_Stage_T i_stage)
{
    Component_Entry_Type* comp_entry;

    for (comp_entry = &_compt_start; comp_entry < &_compt_end; comp_entry += 1)
    {
        if (comp_entry->pStage == i_stage)
        {
            if ((comp_entry->COMPMagic == COMP_MAGIC) && (comp_entry->pfCOMPFun != NULL))
            {
                comp_entry->pfCOMPFun();
            }
        }
    }
}
/**
 * @brief  search configuration by key string
 * @param  i_pkey target key string
 * @return comp_configuration_t* 
 */
Comp_Configuration_t* Comp_Configuration_Get(const char* i_pkey)
{
    uint32_t              k;
    uint32_t              keylen;
    uint32_t              tbl_size = s_config_tbl_size;
    Comp_Configuration_t* pret;
    Comp_Configuration_t* tbl = s_config_tbl;
    if ((i_pkey != NULL) && (tbl != NULL) && (tbl_size > 0))
    {
        keylen = strlen(i_pkey);
        for (k = 0; k < tbl_size; k++)
        {
            pret = &tbl[k];
            if ((pret != NULL) && (keylen == strlen(pret->key)) && (strncmp(i_pkey, pret->key, keylen) == 0))
            {
                return pret;
            }
        }
    }
    return NULL;
}
/**
 * @brief  component configuration table register
 * @param  i_cfgtbl start address of configuration table
 * @param  i_cnt configuration table item count
 * @return int32_t register count
 */
int32_t Comp_Configurations_Register(Comp_Configuration_t* i_cfgtbl, uint32_t i_cnt)
{
    if ((i_cfgtbl != NULL) && (i_cnt > 0))
    {
        s_config_tbl      = i_cfgtbl;
        s_config_tbl_size = i_cnt;
        return (int32_t)i_cnt;
    }
    return 0;
}
/**
 * @brief  embeddedV entry point, it will delete itself after launch all stages applications
 * @param  pvParameters 
 */
void App_TaskStart(void* pvParameters)
{
    (void)pvParameters;

    Comp_Startup(STARTUP_STAGE_0);
    Comp_Startup(STARTUP_STAGE_1);
    Comp_Startup(STARTUP_STAGE_2);
    Comp_Startup(STARTUP_STAGE_3);
    Comp_Startup(STARTUP_STAGE_4);
    Comp_Startup(STARTUP_STAGE_5);
    // delete the startup thread itself for heap saving
    vTaskDelete(NULL);
}
