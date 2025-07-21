#include "Inf_JoyStickAndKey.h"

#define READ_LEFT 		  	HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port,KEY_LEFT_Pin)
#define READ_RIGHT		    HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port,KEY_RIGHT_Pin)
#define READ_UP 					HAL_GPIO_ReadPin(KEY_UP_GPIO_Port,KEY_UP_Pin)
#define READ_DOWN 				HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port,KEY_DOWN_Pin)
#define READ_LEFT_TOP 		HAL_GPIO_ReadPin(KEY_LEFT_TOP_GPIO_Port,KEY_LEFT_TOP_Pin)
#define READ_RIGHT_TOP	  HAL_GPIO_ReadPin(KEY_RIGHT_TOP_GPIO_Port,KEY_RIGHT_TOP_Pin)

static uint16_t buff[4]={0};

/**
  * @brief 摇杆初始化
  * @param 
  * @retval 
  */
void Inf_JoyStickAndKey_Init(void)
{
    debug_printfln("摇杆和按键数据的初始化--开始");
	
    /* 1. ADC校准 */
    HAL_ADCEx_Calibration_Start(&hadc1);

    /* 2. 启动ADC转换 */
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)buff, 4);
	
    debug_printfln("摇杆和按键数据的初始化--结束");
}
	
  /**
  * @brief 扫描摇杆数据
  * @param 
  * @retval 
  */
void Inf_JoyStickAndKey_JoyStickScan(void)
{
	JoyStick.THR=buff[1];
	JoyStick.YAW=buff[0];
	JoyStick.PIT=buff[3];
	JoyStick.ROL=buff[2];	
}

  /**
  * @brief 扫描按键数据
  * @param 
  * @retval 按下哪个键
  */
Com_Key Inf_JoyStickAndKey_KeyScan(void)
{
	if(READ_LEFT==0|| 		
		 READ_RIGHT==0||		
     READ_UP==0|| 			
     READ_DOWN==0|| 		
     READ_LEFT_TOP==0|| 
     READ_RIGHT_TOP==0)
	{
		vTaskDelay(30);
		if(READ_LEFT==0)
		{
			while(READ_LEFT==0);
			return KEY_LEFT;	
		}
		else if(READ_RIGHT==0)
		{
			while(READ_RIGHT==0);
			return KEY_RIGHT;
		}
		else if(READ_UP==0)
		{
			while(READ_UP==0);
			return KEY_UP;
		}
		else if(READ_DOWN==0)
		{
			while(READ_DOWN==0);
			return KEY_DOWN;
		}
		else if(READ_LEFT_TOP==0)
		{
			uint16_t time=0;
			while(READ_LEFT_TOP==0&&time<12)
			{
				time++;
				vTaskDelay(100);
			}
			if(time<=5)
			{
				return KEY_LEFT_TOP;			
			}
			while(READ_LEFT_TOP==0);
			return KEY_LEFT_TOP_LONG;				
		}
		else if(READ_RIGHT_TOP==0)
		{
			uint16_t time=0;
			while(READ_RIGHT_TOP==0&&time<12)
			{
				time++;
				vTaskDelay(100);
			}
			if(time<=5)
			{
				return KEY_RIGHT_TOP;			
			}
			while(READ_RIGHT_TOP==0);
			return KEY_RIGHT_TOP_LONG;				
		}
	}
	return KEY_NONE;
}
