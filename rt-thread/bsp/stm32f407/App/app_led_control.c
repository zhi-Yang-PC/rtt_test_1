
#include <rtconfig.h>
#include <rtdebug.h>
#include <rtthread.h>
#include "userConfig.h"
#include "rtdef.h"
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "app_led_control.h"
#include "led.h"

/*-------------------macro------------------------*/  
#define DBG_LEVEL  DBG_WARNING
#define DBG_TAG    "led_ctrl"
#include <rtdbg.h>

#define LEDS_CTRL_TSK_PRIORITY       25
#define LEDS_CTRL_TSK_STACK_SIZE     128 + 128
#define LEDS_CTRL_TSK_TIMESLICE      10

/*-------------------variable---------------------*/
static rt_mq_t       g_ledsCtrl_mqHandle  = RT_NULL;
static rt_thread_t   g_ledsCtrl_Tsk = RT_NULL;
static ledsCtrl_mq_t g_ledsCtrl_mq = {0}; 

/*-------------------function---------------------*/
void APP_CtrlLed(uint8_t cmd)
{
	rt_device_t g_leds = rt_device_find(LED_NAME);
	if(RT_NULL == g_leds) LOG_W("find led fail\n");
	rt_device_open(g_leds, RT_DEVICE_OFLAG_RDWR);
	
	if(LED_ON == cmd)       rt_device_control(g_leds,LED_ON,NULL);
	else if(LED_OFF == cmd) rt_device_control(g_leds,LED_OFF,NULL);
	rt_device_close(g_leds);	
}


void APP_LedControlTaskEntry(void *parameter)
{
	while(1)
	{
//		rt_uint64_t useage = 0;
//		while(1)
//		{
//			useage++;
//			if(100000 == useage) break;
//		}				
    if(RT_EOK != rt_mq_recv(g_ledsCtrl_mqHandle, &g_ledsCtrl_mq,sizeof(ledsCtrl_mq_t), 
			                      RT_WAITING_FOREVER))
		{   
			continue;
		}
		
    APP_CtrlLed(g_ledsCtrl_mq.tag);
	}
}

/*task init*/
rt_err_t APP_LedControlTaskInit(void)
{
  g_ledsCtrl_Tsk = rt_thread_create(LEDS_CTRL_TSK_NAME, 
	                                 APP_LedControlTaskEntry, 
	                                 RT_NULL,
                                   LEDS_CTRL_TSK_STACK_SIZE, 
	                                 LEDS_CTRL_TSK_PRIORITY,
                                 	 LEDS_CTRL_TSK_TIMESLICE);	

  if (g_ledsCtrl_Tsk != RT_NULL)   rt_thread_startup(g_ledsCtrl_Tsk); 
  else
  {
		LOG_E("g_ledsCtrl_Tsk thread_startup FAIL");		
		return RT_EEMPTY;		
	}	
	
  g_ledsCtrl_mqHandle = rt_mq_create(LEDS_CTRL_MQ_NAME,/* 消息队列名字 */
                        40,     /* 消息的最大长度 */
                        20,    /* 消息队列的最大容量 */
                        RT_IPC_FLAG_FIFO);/* 队列模式 FIFO(0x00)*/
	

  if (g_ledsCtrl_mqHandle == RT_NULL) 
	{
		LOG_E("g_ledsCtrl_Mq FAIL");		
		return RT_ERROR;
	}		
	
	return RT_EOK;
}

/*task start*/
rt_err_t APP_LedControlTask(void)
{
	APP_LedControlTaskInit();
	return RT_EOK;
}

rt_mq_t APP_GetLedsCtrl_MsgHandle(void)
{
	if(g_ledsCtrl_mqHandle != RT_NULL)
	{
		return g_ledsCtrl_mqHandle;
	}	
  return RT_NULL;		
}

