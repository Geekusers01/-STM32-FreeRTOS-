#include "Inf_Motor.h"

/**
  * @brief ����ĳ�ʼ��
  * @param 
  * @retval 
  */
void Inf_Motor_Init(void)
{
	/*1.�����ĸ���ʱ��*/
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	
	/*2.�ĸ������������*/
	motorLeftTop.speed=0;	
	motorLeftBottom.speed=0;	
	motorRightTop.speed=0;	
	motorRightBottom.speed=0;	
	Inf_Motor_AllMotorsWork();
}	
	
/**
  * @brief ����ָ��������ٶ�
  * @param 
  * @retval 
  */
	
void Inf_Motor_SetSpeed(Motor_Struct *motor)
{
	/*�Ե�����ٶ���һ���޷�*/
	motor->speed=LIMIT(motor->speed,0,1000);
	
	if(motor->location==LEFT_TOP)
	{
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,motor->speed);
	}
	else if(motor->location==LEFT_BOTTOM)
	{
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,motor->speed);
	}
	else if(motor->location==RIGHT_TOP)
	{
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,motor->speed);
	}
	else if(motor->location==RIGHT_BOTTOM)
	{
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,motor->speed);
	}
}

void Inf_Motor_AllMotorsWork(void)
{
	Inf_Motor_SetSpeed(&motorLeftTop);
	Inf_Motor_SetSpeed(&motorLeftBottom);
	Inf_Motor_SetSpeed(&motorRightTop);
	Inf_Motor_SetSpeed(&motorRightBottom);
}
