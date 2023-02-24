/*** 
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\middleware\sys_manager\sys_manager.h
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-25 11:31:35
 * @LastEditTime: 2023-01-25 11:54:28
 */


#ifndef __SYS_MANAGER_H__
#define __SYS_MANAGER_H__
#include <stdint.h>
#include <stdbool.h>
//-------------------------------------------------------------------------------------------------
// Public Defintions
//-------------------------------------------------------------------------------------------------

/* Component startup stage definition, which is used to define starting sequence like linux */
typedef enum Startup_Stage_Type
{
    STARTUP_STAGE_0 = 0,
    STARTUP_STAGE_1 = 1,
    STARTUP_STAGE_2 = 2,
    STARTUP_STAGE_3 = 3,
    STARTUP_STAGE_4 = 4,
    STARTUP_STAGE_5 = 5
} Startup_Stage_T;

/* compatible with previous priority definition */
#define START_PRIO_HIGHEST    STARTUP_STAGE_0
#define START_PRIO_CORE       STARTUP_STAGE_1
#define START_PRIO_SYS        STARTUP_STAGE_2
#define START_PRIO_PERIPHREAL STARTUP_STAGE_3
#define START_PRIO_NETWORK    STARTUP_STAGE_3
#define START_PRIO_FIELDBUS   STARTUP_STAGE_3
#define START_PRIO_CTRLLOGIC  STARTUP_STAGE_4
#define START_PRIO_LOWEST     STARTUP_STAGE_5

/**
 * @brief  lanuch up components by stage
 * @param  pvParameters 
 * @param  i_stage
 */
void Comp_Startup(Startup_Stage_T i_stage);

/* Component entry structure, will be allocated in the component loading table */
typedef struct T_Component_Entry_Type
{
    unsigned long COMPMagic;
    char*         pCOMPName;
    void (*pfCOMPFun)(void);
    Startup_Stage_T pStage;
} Component_Entry_Type;

#define COMP_MAGIC 0xCD00CD00

/* To add a component in the system, add its Loading/Initialization Function into the component loading table */
/* by using the COMP_REGISTER operation which provided by system-manager, example code: */
/* #include "sys_manager.h" */
/* COMP_REGISTER([ComponentName], [InitializationFunction], [StartupPriority]); */

#define COMP_REGISTER(COMP_NAME, PFUNC, STAGE)                                               \
    __attribute__((section(".compt"))) const Component_Entry_Type comp_entry_##COMP_NAME = { \
        COMPMagic : COMP_MAGIC,                                                              \
        pCOMPName : #COMP_NAME,                                                              \
        pfCOMPFun : PFUNC,                                                                   \
        pStage : STAGE                                                                       \
    }

typedef const struct Comp_Configuration_tag
{
    const char* key;
    union
    {
        bool        v_b;
        int32_t     v_int;
        uint32_t    v_uint;
        float       v_float;
        const char* v_str;
        void*       v_ptr;
    } val;
} Comp_Configuration_t;
/**
 * @brief  search configuration by key string
 * @param  i_pkey target key string
 * @return comp_configuration_t* 
 */
Comp_Configuration_t* Comp_Configuration_Get(const char* i_pkey);
/**
 * @brief  component configuration table register
 * @param  i_cfgtbl start address of configuration table
 * @param  i_cnt configuration table item count
 * @return int32_t register count
 */
int32_t Comp_Configurations_Register(Comp_Configuration_t* i_cfgtbl, uint32_t i_cnt);
/**
 * @brief  embeddedV entry point, it will delete itself after launch all stages applications
 * @param  pvParameters 
 */
void App_TaskStart(void* pvParameters);
#endif
