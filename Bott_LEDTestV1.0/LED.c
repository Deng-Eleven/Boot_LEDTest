#include "LED.h"

// LED_PORT 数组存储了每个 LED 对应的 GPIO 端口指针
GPIO_TypeDef * LED_PORT[11] = 
{
    GPIOA, GPIOA, GPIOA, GPIOB, GPIOB,
    GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOC
};

// LED_PIN 数组存储了每个 LED 对应的 GPIO 引脚编号
uint16_t LED_PIN[11] = 
{
    GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_0, GPIO_PIN_1,
    GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_7, GPIO_PIN_1,
};

//测试模式状态机
typedef enum
{
	Flow = 0,
	Part,
}Test_Type;

Test_Type mode = Flow;

/**
  * @brief  初始化 LED 的 GPIO 引脚配置
  * @param  无
  * @retval 无
  */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0}; 

    // 为相应的端口使能 GPIO 时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();  
    __HAL_RCC_GPIOC_CLK_ENABLE();  

    // 配置 GPIOA 上的 LED 引脚
	HAL_GPIO_WritePin(GPIOA,LED_PIN[0] | LED_PIN[1] | LED_PIN[2],GPIO_PIN_SET);
	
    GPIO_InitStruct.Pin = LED_PIN[0] | LED_PIN[1] | LED_PIN[2];
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;         
    GPIO_InitStruct.Pull = GPIO_PULLUP;  
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;          
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 配置 GPIOB 上的 LED 引脚
	HAL_GPIO_WritePin(GPIOB,LED_PIN[3] | LED_PIN[4] |
							LED_PIN[5] | LED_PIN[6] | LED_PIN[7] |
							LED_PIN[8] | LED_PIN[9],GPIO_PIN_SET);
	
    GPIO_InitStruct.Pin = LED_PIN[3] | LED_PIN[4] | LED_PIN[5] | LED_PIN[6] | LED_PIN[7] | LED_PIN[8] | LED_PIN[9];
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;         
    GPIO_InitStruct.Pull = GPIO_PULLUP;  
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;   
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  

    // 配置 GPIOC 上的 LED 引脚
	HAL_GPIO_WritePin(GPIOC,LED_PIN[10],GPIO_PIN_SET);
	
    GPIO_InitStruct.Pin = LED_PIN[10];
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;         
    GPIO_InitStruct.Pull = GPIO_PULLUP;  
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;  
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/**
  * @brief  获取指定 LED 的当前状态
  * @param  led_index: LED 的索引（0 到 10）
  * @retval LED 的状态（GPIO_PIN_RESET 或 GPIO_PIN_SET）
  */
uint8_t LED_Get_State(uint8_t led_index)
{
    return (uint8_t)HAL_GPIO_ReadPin(LED_PORT[led_index], LED_PIN[led_index]);
}

/**
  * @brief  设置指定 LED 的状态
  * @param  led_index: LED 的索引（0 到 10）
  * @param  state: @LED_State_Type的结构体，代表了LED状态枚举
  * @retval 如果设置成功返回 1，否则返回 0
  */
uint8_t LED_Set_State(uint8_t led_index, LED_State_Type state)
{
    if(state == LED_OFF)
	{
		LED_PORT[led_index]->BSRR = LED_PIN[led_index]; // 关闭 LED
		return 1;
	}
	else
	{
	    // 计算当前点亮的 LED 数量
		uint8_t light = 0;
		for(uint8_t i = 0; i < 11; i++)
		{
			if(LED_Get_State(i) == GPIO_PIN_RESET)
			{
				light++;
				if(light >= LED_LIGHT_MAX) // 已达到同时点亮的 LED 最大数量，不允许操作
					return 0;
			}       
		}
		LED_PORT[led_index]->BRR = LED_PIN[led_index];  // 打开 LED
		return 1;
	}
}



/**
  * @brief  LED_Text函数控制LED灯的状态。
  * @param  无
  * @retval 无
  */
void LED_Text(void)
{
    static uint32_t tick = 0;    // 静态变量，存储上次操作的时间点

    // 检查是否到达LED操作的时间间隔
    if(tick <= HAL_GetTick())
    {
        tick = HAL_GetTick() + 350;    // 更新下一个操作的时间点

        // 根据当前模式进行操作
        switch(mode)
        {
            case Flow:
            {   
                static uint8_t led_index = 0;    // 静态变量，跟踪当前LED的索引

                // 熄灭当前LED
                LED_Set_State(led_index, LED_OFF);
                led_index++;

                // 如果所有LED都已经点亮过一遍，则重置并切换模式
                if(led_index == 11)
                {
                    led_index = 0;
                    mode = Part;
                    break;
                }

                // 点亮下一个LED
                LED_Set_State(led_index, LED_ON);
            }
            break;

            case Part:
            {
                static uint8_t part_num = 0;    // 静态变量，跟踪当前部分模式

                // 根据不同部分模式点亮LED
                if(part_num == 0)
                {
                    // 模式1：点亮特定的LED，其他LED熄灭
                    LED_Set_State(7, LED_OFF);
                    LED_Set_State(8, LED_OFF);
                    LED_Set_State(9, LED_OFF);
                    LED_Set_State(10, LED_OFF);
                    LED_Set_State(0, LED_ON);
                    LED_Set_State(1, LED_ON);
                    part_num = 1;
                }
                else if(part_num == 1)
                {
                    // 模式2：点亮不同的一组LED
                    LED_Set_State(0, LED_OFF);
                    LED_Set_State(1, LED_OFF);
                    LED_Set_State(2, LED_ON);
                    LED_Set_State(3, LED_ON);
                    LED_Set_State(4, LED_ON);
                    LED_Set_State(5, LED_ON);
                    LED_Set_State(6, LED_ON);
                    part_num = 2;
                }
                else if(part_num == 2)
                {
                    // 模式3：另一组LED
                    LED_Set_State(2, LED_OFF);
                    LED_Set_State(3, LED_OFF);
                    LED_Set_State(4, LED_OFF);
                    LED_Set_State(5, LED_OFF);
                    LED_Set_State(6, LED_OFF);
                    LED_Set_State(7, LED_ON);
                    LED_Set_State(8, LED_ON);
                    LED_Set_State(9, LED_ON);
                    LED_Set_State(10, LED_ON);
                    part_num = 0;
                }
            }
            break;
            default: break;
        }
    }
}
