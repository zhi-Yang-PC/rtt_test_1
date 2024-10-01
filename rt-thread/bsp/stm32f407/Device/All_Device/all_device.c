
#include <rtconfig.h>
#include <rtthread.h>
#include "userConfig.h"
#include "rtdef.h"
#include <stdint.h>
#include <led.h>
#include "led_control.h"

static int DEV_AllDeviceInit(void)
{
#ifdef  USE_LEDS
	DEV_LedsInit();
#endif
	
	APP_LedControlTask();
}

INIT_DEVICE_EXPORT(DEV_AllDeviceInit);


