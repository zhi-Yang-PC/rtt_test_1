
/**
  ******************************************************************************
  * @file           : app_sys_useAge.c
  * @brief          : calcu system use age
  ******************************************************************************
  * @attention
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include <rtconfig.h>
#include <rtdebug.h>
#include <rtthread.h>
#include "userConfig.h"
#include "rtdef.h"
#include <stdint.h>
#include "app_sys_useAge.h"
/*-------------------macro------------------------*/  
#define DBG_LEVEL  DBG_WARNING
#define DBG_TAG    "sys_useAge"
#include <rtdbg.h>

#define CPU_USAGE_CALC_TICK    1000
#define CPU_USAGE_LOOP         100

/*-------------------variable---------------------*/
static rt_uint8_t  cpu_usage_major = 0, cpu_usage_minor= 0;
static rt_uint32_t total_count = 0;

/*-------------------function---------------------*/
static void cpu_usage_idle_hook(void)
{
    rt_tick_t tick;
    rt_uint32_t count;
    volatile rt_uint32_t loop;
 
 
    if (total_count == 0)
    {
        /* get total count */
        rt_enter_critical();
        tick = rt_tick_get();
        while(rt_tick_get() - tick < CPU_USAGE_CALC_TICK)
        {
            total_count ++;
            loop = 0;
 
 
            while (loop < CPU_USAGE_LOOP) loop ++;
        }
        rt_exit_critical();
    }
 
 
    count = 0;
    /* get CPU usage */
    tick = rt_tick_get();
    while (rt_tick_get() - tick < CPU_USAGE_CALC_TICK)
    {
        count ++;
        loop  = 0;
        while (loop < CPU_USAGE_LOOP) loop ++;
    }
 
 
    /* calculate major and minor */
    if (count < total_count)
    {
        count = total_count - count;
        cpu_usage_major = (count * 100) / total_count;
        cpu_usage_minor = ((count * 100) % total_count) * 100 / total_count;
    }
    else
    {
        total_count = count;
 
 
        /* no CPU usage */
        cpu_usage_major = 0;
        cpu_usage_minor = 0;
    }
}
 
 
void cpu_usage_get(rt_uint8_t *major, rt_uint8_t *minor)
{
    RT_ASSERT(major != RT_NULL);
    RT_ASSERT(minor != RT_NULL);
 
    *major = cpu_usage_major;
    *minor = cpu_usage_minor;
}
 

void cpu_usage_init(void)
{
    /* set idle thread hook */
    rt_thread_idle_sethook(cpu_usage_idle_hook);
}
