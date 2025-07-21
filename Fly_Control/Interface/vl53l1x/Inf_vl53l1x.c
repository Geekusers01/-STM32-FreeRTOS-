#include "Inf_vl53l1x.h"

#define DEV 0x52
/**
  * @brief 初始化激光测距芯片
  * @param 
  * @retval 
  */
void Inf_VL53L1X_Init(void)
{
	//1.先重启芯片
	HAL_GPIO_WritePin(VL53LX1_SHUT_GPIO_Port,VL53LX1_SHUT_Pin,GPIO_PIN_RESET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(VL53LX1_SHUT_GPIO_Port,VL53LX1_SHUT_Pin,GPIO_PIN_SET);
	
	//2.再初始化芯片
	VL53L1X_SensorInit(DEV);
	
	//3.设置距离模式：长：2或短：1
	VL53L1X_SetDistanceMode(DEV,2);
	
	//4.测量速度
	VL53L1X_SetTimingBudgetInMs(DEV,20);
	
	//5.测量频率，ms值需大于等于速度值
	VL53L1X_SetInterMeasurementInMs(DEV,20);
	
	//6.开始测量
	VL53L1X_StartRanging(DEV);
	
	uint16_t sensorID;
	VL53L1X_GetSensorId(DEV,&sensorID);
	printf("sensorID=%x\r\n",sensorID);
}

/**
  * @brief 返回测量的高度
  * @param 
  * @retval 
  */
uint16_t Inf_VL53L1X_GetHeight(void)
{
	static uint16_t height=0;
	uint8_t isDataReday;
	
	//检测测距是否完成
	VL53L1X_CheckForDataReady(DEV,&isDataReday);
	if(isDataReday)
	{
		VL53L1X_ClearInterrupt(DEV);
		//读取测距结果
		VL53L1X_GetDistance(DEV,&height);
	}	
	return height;
}
