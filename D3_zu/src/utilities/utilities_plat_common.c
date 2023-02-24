/*
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\utilities\utilities_plat_common.c
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-25 10:56:10
 * @LastEditTime: 2023-01-25 11:25:31
 */

#include "utilities_plat_common.h"

/**********Instructions for use***********************************
Note:
Add the following paragraph to the .ld file
for example

.module_init_tab : {
    . = ALIGN(64);
    __PDEV_MODULE_INITTAB_START = ABSOLUTE(.);
    KEEP (*(.module_init_pdev))
    __PDEV_MODULE_INITTAB_END = ABSOLUTE(.);

    __PVDEV_MODULE_INITTAB_START = ABSOLUTE(.);
    KEEP (*(.module_init_pvdev))
    __PVDEV_MODULE_INITTAB_END = ABSOLUTE(.);

    __BDEV_MODULE_INITTAB_START = ABSOLUTE(.);
    KEEP (*(.module_init_bdev))
    __BDEV_MODULE_INITTAB_END = ABSOLUTE(.);

    __BVDEV_MODULE_INITTAB_START = ABSOLUTE(.);
    KEEP (*(.module_init_bvdev))
    __BVDEV_MODULE_INITTAB_END = ABSOLUTE(.);

    __APP_MODULE_INITTAB_START = ABSOLUTE(.);
    KEEP (*(.module_init_app))
    __APP_MODULE_INITTAB_END = ABSOLUTE(.);

    __AAPP_MODULE_INITTAB_START = ABSOLUTE(.);
    KEEP (*(.module_init_aapp))
    __AAPP_MODULE_INITTAB_END = ABSOLUTE(.);
} > psu_ddr_0_MEM_0
*******************************************************************/


/*Platform Dev*/
extern fp_module_init __PDEV_MODULE_INITTAB_START[];
extern fp_module_init __PDEV_MODULE_INITTAB_END[];
/*Platform virtual Dev*/
extern fp_module_init __PVDEV_MODULE_INITTAB_START[];
extern fp_module_init __PVDEV_MODULE_INITTAB_END[];
/*Bus Dev*/
extern fp_module_init __BDEV_MODULE_INITTAB_START[];
extern fp_module_init __BDEV_MODULE_INITTAB_END[];
/*Bus virtual Dev*/
extern fp_module_init __BVDEV_MODULE_INITTAB_START[];
extern fp_module_init __BVDEV_MODULE_INITTAB_END[];
/* APP */
extern fp_module_init __APP_MODULE_INITTAB_START[];
extern fp_module_init __APP_MODULE_INITTAB_END[];
/* Advanced APP */
extern fp_module_init __AAPP_MODULE_INITTAB_START[];
extern fp_module_init __AAPP_MODULE_INITTAB_END[];


inline void ModuleInitcall(enum module_init_type mod)
{
    fp_module_init *func_search, *func_search_end;
    switch(mod) {
        case MODULE_INIT_PDEV:
            func_search     = __PDEV_MODULE_INITTAB_START;
            func_search_end = __PDEV_MODULE_INITTAB_END;
            break;
        case MODULE_INIT_PVDEV:
            func_search     = __PVDEV_MODULE_INITTAB_START;
            func_search_end = __PVDEV_MODULE_INITTAB_END;
            break;
        case MODULE_INIT_BDEV:
            func_search     = __BDEV_MODULE_INITTAB_START;
            func_search_end = __BDEV_MODULE_INITTAB_END;
            break;
        case MODULE_INIT_BVDEV:
            func_search     = __BVDEV_MODULE_INITTAB_START;
            func_search_end = __BVDEV_MODULE_INITTAB_END;
            break;
        case MODULE_INIT_APP:
            func_search     = __APP_MODULE_INITTAB_START;
            func_search_end = __APP_MODULE_INITTAB_END;
            break;
        case MODULE_INIT_AAPP:
            func_search     = __AAPP_MODULE_INITTAB_START;
            func_search_end = __AAPP_MODULE_INITTAB_END;
            break;
        default:
            break;
    }
    while( func_search != func_search_end) {
        (*func_search++)();
    } 
}
