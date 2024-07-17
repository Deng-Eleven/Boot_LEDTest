#ifndef __LED_H__
#define __LED_H__

#include "main.h"

/*
	INFO:
	1.因为所有灯全部通过IO口进行驱动，同时根据数据手册可以得到PY32F002B的最大灌电流为Ivcc = 80mA，所以在这里对LED同时点亮的数量进行一定的限制 
	2.一颗灯珠当前流入的电流设置大概为8mA，设置最大点亮数目为6(目前程序中用户使用时同时点亮最多6颗)
*/

#define LED_LIGHT_MAX 6

typedef enum
{
	LED_OFF = 0,
	LED_ON
}LED_State_Type;


//灯的映射表
extern GPIO_TypeDef * LED_PORT[11];
extern uint16_t LED_PIN[11];

void LED_Init(void);

uint8_t LED_Get_State(uint8_t led_index);
uint8_t LED_Set_State(uint8_t led_index, LED_State_Type state);

void LED_Text(void);

#endif
