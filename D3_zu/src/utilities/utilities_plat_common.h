
#ifndef __UTILITIES_PLAT_COMMON_H__
#define __UTILITIES_PLAT_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif
//============================================================================//
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//--------------------------------------------------------------------------//
typedef void (*fp_module_init)(void);
//--------------------------------------------------------------------------//
#define PLAT_ASSERT(x)  \
                    do { \
                        while(!(x)); \
                    } while(0)

//--------------------------------------------------------------------------//
#define MODULE_INIT_DECLARE(name,type) \
    static fp_module_init __ModuleInit_##name \
    __attribute__((__used__, __section__(".module_init_" #type))) = name 

#define PDEV_MODULE_INIT_DECLARE(name)   MODULE_INIT_DECLARE(name, pdev)
#define PVDEV_MODULE_INIT_DECLARE(name)  MODULE_INIT_DECLARE(name, pvdev)
#define BDEV_MODULE_INIT_DECLARE(name)   MODULE_INIT_DECLARE(name, bdev)
#define BVDEV_MODULE_INIT_DECLARE(name)  MODULE_INIT_DECLARE(name, bvdev)
#define APP_MODULE_INIT_DECLARE(name)    MODULE_INIT_DECLARE(name, app)
#define AAPP_MODULE_INIT_DECLARE(name)   MODULE_INIT_DECLARE(name, aapp) //advanced app init

enum module_init_type {
    MODULE_INIT_PDEV,
    MODULE_INIT_PVDEV,
    MODULE_INIT_BDEV,
    MODULE_INIT_BVDEV,
    MODULE_INIT_APP,
    MODULE_INIT_AAPP,
};

extern inline void ModuleInitcall(enum module_init_type mod);
//============================================================================//
#ifdef __cplusplus
}
#endif
#endif  /* __UTILITIES_PLAT_COMMON_H__ */

