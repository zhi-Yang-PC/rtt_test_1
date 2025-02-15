
#include <rtconfig.h>
#include <rtdebug.h>
#include <rtthread.h>
#include "userConfig.h"
#include "rtdef.h"
#include <stdint.h>
#include <led.h>
#include "stm32f4xx_hal.h"


#ifdef  USE_LEDS //&& RT_USING_DEVICE_OPS

/*-------------------macro------------------------*/  
#define DBG_LEVEL  DBG_WARNING
#define DBG_TAG    "leds"
#include <rtdbg.h>

/*---------------declaration----------------------*/  
rt_err_t LedsInit(rt_device_t dev);
rt_err_t LedOpen(rt_device_t dev, rt_uint16_t oflag);
rt_err_t LedClose(rt_device_t dev);
static rt_err_t LedControl(rt_device_t dev, int cmd, void *args);


/*-------------------variable---------------------*/
static rt_mutex_t ledsMutex = RT_NULL;
static struct rt_device g_leds = {0};
const static struct rt_device_ops leds =
{
    RT_NULL,//LedsInit,
    LedOpen,
    LedClose,
    RT_NULL,
    RT_NULL,
    LedControl,
};

/*-------------------function---------------------*/


/*leds init*/
rt_err_t LedsInit(rt_device_t dev)
{
		GPIO_InitTypeDef GPIO_InitStruct = {0};

		/* GPIO Ports Clock Enable */
		__HAL_RCC_GPIOH_CLK_ENABLE();
		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();

		/*Configure GPIO pin Output Level */
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_SET);

		/*Configure GPIO pins : PC4 PC5 PC6 PC7 */
		GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	  if(RT_NULL == ledsMutex) 
		{
			ledsMutex = rt_mutex_create("mutexleds", RT_IPC_FLAG_FIFO);
			LOG_W("frist init ledsMutex");
		}
    
    if(RT_NULL == ledsMutex) 
    {
        LOG_E("create mutexleds failed");
        return RT_FALSE;
    }
		
		return RT_EOK;
}

/*leds open*/
rt_err_t LedOpen(rt_device_t dev, rt_uint16_t oflag)
{  
	 if (dev == RT_NULL)  return -RT_ERROR;
	 rt_mutex_take(ledsMutex, RT_WAITING_FOREVER);
	 return RT_TRUE;
}

/*leds close*/
rt_err_t LedClose(rt_device_t dev)
{
	 if (dev == RT_NULL)  return -RT_ERROR;
	 rt_mutex_release(ledsMutex);
   return RT_TRUE;
}

/*leds device init*/
int DEV_LedsInit(void)
{
	  if(RT_EOK != LedsInit((rt_device_t)RT_NULL))  LOG_E("LedsInit fail");
#ifdef RT_USING_DEVICE_OPS
    g_leds.ops         = &leds;
#else
    g_Leds->init        = RT_NULL;
    g_Leds->open        = RT_NULL;
    g_Leds->close       = RT_NULL;
    g_Leds->read        = RT_NULL;
    g_Leds->write       = RT_NULL;
    g_Leds->control     = RT_NULL;
#endif
    g_leds.type        = RT_Device_Class_Miscellaneous;
    g_leds.rx_indicate = RT_NULL;
    g_leds.tx_complete = RT_NULL;
    g_leds.user_data   = RT_NULL;

    /* register a character device */
	  if(RT_EOK != rt_device_register(&g_leds, LED_NAME, RT_DEVICE_FLAG_RDWR))
		{
			LOG_E("LEDS INIT failed.\n");
			return -RT_ERROR;
		}			
		 LOG_E("LEDS INIT Success\n");
    return RT_EOK;
}

/*leds control*/
static rt_err_t LedControl(rt_device_t dev, int cmd, void *args)
{
	if(LED_ON == cmd)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5,GPIO_PIN_RESET);
		LOG_W("led on");
	}
	else if(LED_OFF == cmd)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5,GPIO_PIN_SET);
		LOG_W("led close");
	}
	
	return RT_EOK;
}


//INIT_DEVICE_EXPORT(DEV_LedsInit);

#endif  /*USE_LEDS*/

