/**
  ******************************************************************************
  * @file           : ha_uuid.h
  * @brief          : 
  ******************************************************************************
  * @attention
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#ifndef  _HA_UUID_H
#define  _HA_UUID_H

#include <rtdef.h>

typedef struct cpu_info
{
    union{
        rt_uint8_t  byte[12];    
        rt_uint32_t id[3];
    }v;
}cpu_t;


cpu_t HA_UUID_Get(void);
#endif









