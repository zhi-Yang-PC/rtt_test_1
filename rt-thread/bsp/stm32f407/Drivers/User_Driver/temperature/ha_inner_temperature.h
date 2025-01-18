/**
  ******************************************************************************
  * @file           : ha_inner_temperature.h
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
#ifndef  _HA_INNER_TEMPE_H
#define  _HA_INNER_TEMPE_H

#include <rtdef.h>
//#include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_adc.h"

typedef enum 
{
  INNER_TEMPE_OK       = 0x00U,
  INNER_TEMPE_ERROR    = 0x01U,
} ha_inner_tempe_status_e;

//ha_inner_tempe_status_e  ha_inner_tempe_init(void);
ha_inner_tempe_status_e ha_inner_tempe_get(float *temperature);
#endif
