/*
 * Change Logs:
 * Date           Author      
 * 2022-06-29     jiaozhu 
 */
 
#ifndef __BOARD_H__
#define __BOARD_H__
 
#include "stm32f407xx.h"

#ifdef __cplusplus
extern "C"
{
#endif	
	
/*-------------------------- ROM/RAM CONFIG BEGIN --------------------------*/
 
#define ROM_START              ((uint32_t)0x08000000)
#define ROM_SIZE               (512 * 1024)
#define ROM_END                ((uint32_t)(ROM_START + ROM_SIZE))
 
#define RAM_START              (0x20000000)
#define RAM_SIZE               (64 * 1024)
#define RAM_END                (RAM_START + RAM_SIZE)	
	
#define STM32_SRAM1_END                RAM_END	
	
/*--------------------------     GET HEAP SIZE    --------------------------*/
#if defined(__CC_ARM) || defined(__CLANG_ARM)
extern int Image$$RW_IRAM1$$ZI$$Limit;
#define HEAP_BEGIN      ((void *)&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
#pragma section="CSTACK"
#define HEAP_BEGIN      (__segment_end("CSTACK"))
#else
extern int __bss_end;
#define HEAP_BEGIN      ((void *)&__bss_end)
#endif
 
#define HEAP_END                       STM32_SRAM1_END
	
#ifdef __cplusplus
}
#endif
 
#endif /* __BOARD_H__ */	
 