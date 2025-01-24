
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
/*-------------------variable---------------------*/
static ADC_HandleTypeDef g_inner_temp_hadc1;

/*-------------------function---------------------*/


ha_inner_tempe_status_e  ha_inner_tempe_init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};
	
  /** Common config */
  g_inner_temp_hadc1.Instance = ADC1;
  g_inner_temp_hadc1.Init.ScanConvMode = DISABLE;
  g_inner_temp_hadc1.Init.ContinuousConvMode = DISABLE;
  g_inner_temp_hadc1.Init.DiscontinuousConvMode = DISABLE;
  g_inner_temp_hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  g_inner_temp_hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  g_inner_temp_hadc1.Init.NbrOfConversion = 1;
	//  if (HAL_ADC_Init(&hadc1) != HAL_OK){
	//    Error_Handler();
	//  }
	//RT_ASSERT(HAL_ADC_Init(&g_inner_temp_hadc1) == HAL_OK);
	HAL_StatusTypeDef sta = HAL_ADC_Init(&g_inner_temp_hadc1);
	//LOG_W("[inner temp] init sta:%d\r\n",sta);
  /** Configure Regular Channel*/
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = HA_INNER_TEMPE_ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
	//  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) == HAL_OK){
	//    Error_Handler();
	//  }
	//RT_ASSERT(HAL_ADC_ConfigChannel(&g_inner_temp_hadc1, &sConfig) != HAL_OK);
	sta = HAL_ADC_ConfigChannel(&g_inner_temp_hadc1, &sConfig);
	//LOG_W("[inner temp] config chann:%d\r\n",sta);
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
	LOG_W("sample:%d\n",sample);
	convert_vol = ((float)sample*3.3/4096);	//AD值乘以分辨率即为电压值
	//LOG_W("vol:%f tt:%2f\r\n",convert_vol,tttt);
	inner_tempe = (1.43 - convert_vol)/0.0043 + 25;	//根据公式算出温度值
	//LOG_W("MCU Internal Temperature: %.2lf \r\n",inner_tempe);
	float tttt=3.3;
	rt_uint8_t d1;
	if(tttt-3<0) d1 = (tttt-3)*10;
	
//	char buff1[10];
//	rt_sprintf(buff1,"%f",tttt);
//	for(int i=0;i<10;i++)
//	{
//		LOG_W("%c",buff1[i]);
//	}
	LOG_W("[temper convert]--%d.%d ",(uint8_t)tttt/10,d1);
	//rt_kprintf("[temper convert]%f",tttt);
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

