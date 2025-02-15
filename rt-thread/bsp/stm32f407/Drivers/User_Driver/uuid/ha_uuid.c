/**
  ******************************************************************************
  * @file           : ha_uuid.c
  * @brief          : calcu stm32 chip inner temperature
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
#include <stdio.h>
//#include "stm32f4xx_hal_adc.h"
#include "stm32f4xx_hal.h"
#include "ha_inner_temperature.h"
#include "ha_uuid.h"
/*-------------------macro------------------------*/  
#define DBG_LEVEL  DBG_WARNING
#define DBG_TAG    "uuid"
#include <rtdbg.h>

#define  HA_CPU_ID_BASE_ADDR_0   (rt_uint32_t*)(0x1FFF7A10 + 0)
#define  HA_CPU_ID_BASE_ADDR_1   (rt_uint32_t*)(0x1FFF7A10 + 0x04)
#define  HA_CPU_ID_BASE_ADDR_2   (rt_uint32_t*)(0x1FFF7A10 + 0x08)
/*-------------------variable---------------------*/
static cpu_t my;

/*-------------------function---------------------*/
cpu_t HA_UUID_Get(void)
{
	my.v.id[0] = (rt_uint32_t)*HA_CPU_ID_BASE_ADDR_0;
	my.v.id[1] = (rt_uint32_t)*HA_CPU_ID_BASE_ADDR_1;
	my.v.id[2] = (rt_uint32_t)*HA_CPU_ID_BASE_ADDR_2;	
	
	LOG_W("[get uuid] %d %d %d",my.v.id[0],my.v.id[1],my.v.id[2]);
	
//	char buff_uuid[100];
//	sprintf(buff_uuid,"%d%d%d",my.v.id[0],my.v.id[1],my.v.id[2]);
//	LOG_W("[get uuid c] %s",buff_uuid);
	return my;
}











