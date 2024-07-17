#include "User.h"
#include "LED.h"

#define OB_GPIO_PIN_MODE OB_SWD_PB6_GPIO_PC0

TIM_HandleTypeDef TimHandle;

/**
 * @brief  定时器配置
 * @param  无
 * @retval 无

 */
void APP_TimConfig(void)
{ 
  /* TIM1 */
  TimHandle.Instance = TIM1;                                                  
  
  /* Period = 3000 - 1 */
  TimHandle.Init.Period            = 500 - 1;                                     

  /* Prescaler = 8000 - 1 */
  TimHandle.Init.Prescaler         = 24 - 1;                                 

  /* ClockDivision = 0 */
  TimHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;                  

  /* Counter direction = Up */
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;                      

  /* Repetition = 0 */
  TimHandle.Init.RepetitionCounter = 1 - 1;                                   

  /* Auto-reload register not buffered */
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;          

  /* Initialize TIM1 */
  HAL_TIM_Base_Init(&TimHandle);     
  HAL_TIM_Base_Start_IT(&TimHandle);
  
}


/**
  * @brief  Option Program function
  * @param  None
  * @retval None
  */
static void APP_FlashOBProgram(void)
{
  FLASH_OBProgramInitTypeDef OBInitCfg = {0};

  HAL_FLASH_Unlock();        /* Unlock Flash */
  HAL_FLASH_OB_Unlock();     /* Unlock Option */
  
  OBInitCfg.OptionType = OPTIONBYTE_USER;

  OBInitCfg.USERType = OB_USER_BOR_EN | OB_USER_BOR_LEV | OB_USER_IWDG_SW | OB_USER_SWD_NRST_MODE;
  OBInitCfg.USERConfig = OB_BOR_DISABLE | OB_BOR_LEVEL_3p1_3p2 | OB_IWDG_SW | OB_GPIO_PIN_MODE;

  /* Option Program */
  HAL_FLASH_OBProgram(&OBInitCfg);

  HAL_FLASH_Lock();      /* Lock Flash */
  HAL_FLASH_OB_Lock();   /* Lock Option */

  /* Option Launch */
  HAL_FLASH_OB_Launch();
}

void User_Init(void)
{
	//将MCU的NRST功能失能，此时GPIOC PIN0 口可以用作输入输出IO功能
	if(READ_BIT(FLASH->OPTR, OB_USER_SWD_NRST_MODE)!= OB_GPIO_PIN_MODE )	
	{
		APP_FlashOBProgram();
	}
  LED_Init();
}

void User_Loop(void)
{
	LED_Text();
}
