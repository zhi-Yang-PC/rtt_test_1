
/**
  ******************************************************************************
  * @file           : ha_inner_temperature.c
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
//#include "stm32f4xx_hal_adc.h"
#include "stm32f4xx_hal.h"
#include "ha_inner_temperature.h"
#include <stdio.h>
/*-------------------macro------------------------*/  
#define DBG_LEVEL  DBG_WARNING
#define DBG_TAG    "inner_tempe"
#include <rtdbg.h>

#define HA_INNER_TEMPE_ADC_REGULAR_RANK_1     1  
#define HA_INNER_TEMPE_ADC_CONVERT_TIME_OUT   10 //ms
#define HA_INNER_TEMPE_INVALID_VALUE          255.0
#define V25_TYPICAL_VALUE                     0.76 //1.43
#define REFRENCE_VOL_VALUE                    2.8  //3.3
#define AVG_SLOPE_TYPICAL                     2500
/*-------------------variable---------------------*/
static ADC_HandleTypeDef g_inner_temp_hadc1;

/*-------------------function---------------------*/
void printf_float(float a);

ha_inner_tempe_status_e  ha_inner_tempe_init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};
	
  /** Common config */
  g_inner_temp_hadc1.Instance = ADC1;
	g_inner_temp_hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	g_inner_temp_hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  g_inner_temp_hadc1.Init.ScanConvMode = DISABLE;
  g_inner_temp_hadc1.Init.ContinuousConvMode = DISABLE;
  g_inner_temp_hadc1.Init.DiscontinuousConvMode = DISABLE;
  g_inner_temp_hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  g_inner_temp_hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  g_inner_temp_hadc1.Init.NbrOfConversion = 1;
	if (HAL_ADC_Init(&g_inner_temp_hadc1) != HAL_OK){
		RT_ASSERT(0);
	}
	
  /** Configure Regular Channel*/
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = HA_INNER_TEMPE_ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	if (HAL_ADC_ConfigChannel(&g_inner_temp_hadc1, &sConfig) != HAL_OK){
		RT_ASSERT(0);
	}
	
	__HAL_RCC_ADC1_CLK_ENABLE();
	
	return INNER_TEMPE_OK;
}

ha_inner_tempe_status_e  ha_inner_tempe_deinit(void)
{
	RT_ASSERT(&g_inner_temp_hadc1 != RT_NULL);
	if(HAL_OK == HAL_ADC_DeInit(&g_inner_temp_hadc1)) return INNER_TEMPE_OK;
  else                                              return INNER_TEMPE_ERROR;
}

float ha_inner_tempe_convert(void)
{	

	float convert_vol ,inner_tempe = HA_INNER_TEMPE_INVALID_VALUE;
	rt_uint32_t sample;

	HAL_ADC_Start(&g_inner_temp_hadc1);	//启动ADC转换
	HAL_ADC_PollForConversion(&g_inner_temp_hadc1,HA_INNER_TEMPE_ADC_CONVERT_TIME_OUT);	//等待转换完成，10ms表示超时时间
	sample = HAL_ADC_GetValue(&g_inner_temp_hadc1);	//读取ADC转换数据（12位数据）
	LOG_W("sample:%d",sample);
	
	convert_vol = (sample*(REFRENCE_VOL_VALUE/4096));	//AD值乘以分辨率即为电压值
	inner_tempe = (V25_TYPICAL_VALUE - convert_vol)/0.0043 + 25;	//根据公式算出温度值
	
//	convert_vol = (sample*3300.0)/4096;	//AD值乘以分辨率即为电压值
//	inner_tempe = (760 - convert_vol*1000.0)/AVG_SLOPE_TYPICAL;	//根据公式算出温度值

	char buff1[50];
	sprintf(buff1,"stm32-temp:%f vol:%f",inner_tempe,convert_vol);
	LOG_W("[temper convert] %s",buff1);
	
  return inner_tempe;	
}


ha_inner_tempe_status_e ha_inner_tempe_get(float *temperature)
{
	ha_inner_tempe_status_e state = INNER_TEMPE_OK;
	
  ha_inner_tempe_init();
	*temperature = ha_inner_tempe_convert();
	if(HA_INNER_TEMPE_INVALID_VALUE == *temperature) state = INNER_TEMPE_ERROR;
	ha_inner_tempe_deinit();

	return state;
}


