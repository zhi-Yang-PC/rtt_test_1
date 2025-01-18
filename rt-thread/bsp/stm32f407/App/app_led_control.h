
#ifndef  _APP_LED_CONTROL_H
#define  _APP_LED_CONTROL_H

#include <rtdef.h>
#include <stdint.h>

#define LEDS_CTRL_TSK_NAME    "ledsCtrl_tsk"
#define LEDS_CTRL_MQ_NAME     "ledsCtrl_mq"

typedef struct{
	uint8_t tag;
	uint8_t buff[3];
}ledsCtrl_mq_t;


rt_mq_t APP_GetLedsCtrl_MsgHandle(void);
rt_err_t APP_LedControlTask(void);
#endif

