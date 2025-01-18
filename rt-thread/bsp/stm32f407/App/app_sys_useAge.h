/**
  ******************************************************************************
  * @file           : app_sys_useAge.h
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
#ifndef  _APP_SYS_USEAGE_H
#define  _APP_SYS_USEAGE_H

#include <rtdef.h>

void cpu_usage_init(void);
void cpu_usage_get(rt_uint8_t *major, rt_uint8_t *minor);
#endif
