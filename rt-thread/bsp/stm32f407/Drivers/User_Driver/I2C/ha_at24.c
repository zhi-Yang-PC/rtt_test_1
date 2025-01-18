
#include <rtconfig.h>
#include <rtdebug.h>
#include <rtthread.h>
#include "userConfig.h"
#include "rtdef.h"
#include <stdint.h>
#include <led.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"

#ifdef  USE_AT24 //&& RT_USING_DEVICE_OPS

/*-------------------macro------------------------*/  
#define DBG_LEVEL  DBG_WARNING
#define DBG_TAG    "at24"
#include <rtdbg.h>

/*---------------declaration----------------------*/  



/*-------------------variable---------------------*/


/*-------------------function---------------------*/
HAL_StatusTypeDef HA_AT24_Init(void)
{
	I2C_HandleTypeDef *hi2c = RT_NULL;
	hi2c->Instance = I2C1;
	
	HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c);
}


#endif











