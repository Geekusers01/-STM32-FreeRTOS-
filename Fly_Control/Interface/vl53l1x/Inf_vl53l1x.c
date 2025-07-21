#include "Inf_vl53l1x.h"

#define DEV 0x52
/**
  * @brief ��ʼ��������оƬ
  * @param 
  * @retval 
  */
void Inf_VL53L1X_Init(void)
{
	//1.������оƬ
	HAL_GPIO_WritePin(VL53LX1_SHUT_GPIO_Port,VL53LX1_SHUT_Pin,GPIO_PIN_RESET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(VL53LX1_SHUT_GPIO_Port,VL53LX1_SHUT_Pin,GPIO_PIN_SET);
	
	//2.�ٳ�ʼ��оƬ
	VL53L1X_SensorInit(DEV);
	
	//3.���þ���ģʽ������2��̣�1
	VL53L1X_SetDistanceMode(DEV,2);
	
	//4.�����ٶ�
	VL53L1X_SetTimingBudgetInMs(DEV,20);
	
	//5.����Ƶ�ʣ�msֵ����ڵ����ٶ�ֵ
	VL53L1X_SetInterMeasurementInMs(DEV,20);
	
	//6.��ʼ����
	VL53L1X_StartRanging(DEV);
	
	uint16_t sensorID;
	VL53L1X_GetSensorId(DEV,&sensorID);
	printf("sensorID=%x\r\n",sensorID);
}

/**
  * @brief ���ز����ĸ߶�
  * @param 
  * @retval 
  */
uint16_t Inf_VL53L1X_GetHeight(void)
{
	static uint16_t height=0;
	uint8_t isDataReday;
	
	//������Ƿ����
	VL53L1X_CheckForDataReady(DEV,&isDataReday);
	if(isDataReday)
	{
		VL53L1X_ClearInterrupt(DEV);
		//��ȡ�����
		VL53L1X_GetDistance(DEV,&height);
	}	
	return height;
}
