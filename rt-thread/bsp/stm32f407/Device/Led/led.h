
#ifndef  _LED_H
#define  _LED_H

#include <rtdef.h>

#define LED_NUMBER  4
#define LED_NAME    "leds"

typedef enum
{
	LED_ON = 0,
	LED_OFF
}ledsCmd_e;


int DEV_LedsInit(void);

#endif

