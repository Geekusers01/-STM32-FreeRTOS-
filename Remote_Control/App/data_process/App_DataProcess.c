#include "App_DataProcess.h"

/**
* @brief ���ݴ���ģ�������
* @param 
* @retval 
*/
void App_DataProcess_Start(void)
{
	Inf_JoyStickAndKey_Init();
}

/**
* @brief ����ҡ�����ݵļ��Ժͷ�Χ
* @param 
* @retval 
*/
static void App_DataProcess_JoyStickPolarityAndRange(void)
{
	//[0,4095]->[0,1000]
	JoyStick.THR=JoyStick.THR*1000/4095;
	JoyStick.YAW=1000-JoyStick.YAW*1000/4095;
	JoyStick.PIT=JoyStick.PIT*1000/4095;
	JoyStick.ROL=JoyStick.ROL*1000/4095;
}

/**
* @brief  ��ҡ����У׼
* @param 
* @retval 
*/
static void App_DataProcess_JoyStickWithBias(void)
{
	//У׼ҡ������
	JoyStick.THR-=JoyStickBias.THR;
	JoyStick.YAW-=JoyStickBias.YAW;
	JoyStick.PIT-=JoyStickBias.PIT;
	JoyStick.ROL-=JoyStickBias.ROL;
	
	
	//��У׼���������޷�
	JoyStick.THR=LIMIT(JoyStick.THR,0,1000);
	JoyStick.YAW=LIMIT(JoyStick.YAW,0,1000);
	JoyStick.PIT=LIMIT(JoyStick.PIT,0,1000);
	JoyStick.ROL=LIMIT(JoyStick.ROL,0,1000);
}

/**
* @brief ����ҡ������ƫ����
* @param 
* @retval 
*/
void App_DataProcess_JoyStickCaclBias(void)
{
	JoyStickBias.THR=0;
	JoyStickBias.YAW=0;
	JoyStickBias.PIT=0;
	JoyStickBias.ROL=0;
	for(uint8_t i=0;i<100;i++)
	{
		Inf_JoyStickAndKey_JoyStickScan();
		App_DataProcess_JoyStickPolarityAndRange();
		JoyStickBias.THR+=(JoyStick.THR-0);	 //0ֵУ׼
		JoyStickBias.YAW+=(JoyStick.YAW-500);//��ֵУ׼
		JoyStickBias.PIT+=(JoyStick.PIT-500);//��ֵУ׼
		JoyStickBias.ROL+=(JoyStick.ROL-500);//��ֵУ׼
		vTaskDelay(10);
	}
	JoyStickBias.THR/=100;
	JoyStickBias.YAW/=100;
	JoyStickBias.PIT/=100;
  JoyStickBias.ROL/=100;
}

/**
* @brief ҡ�����ݴ���
* @param 
* @retval 
*/
void App_DataProcess_JoyStickDataProcess(void)
{
	taskENTER_CRITICAL();//�����ٽ�������ֹ����δ���������
	//1.ɨ��ҡ������
	Inf_JoyStickAndKey_JoyStickScan();
	
	//2.�����Ժͷ�Χ
	App_DataProcess_JoyStickPolarityAndRange();
	
	//3.����ҡ��Ư����
	App_DataProcess_JoyStickWithBias();
	taskEXIT_CRITICAL();
//	Com_Config_PrintJoyStick("2");
}

/**
* @brief �������ݴ���
* @param 
* @retval 
*/
void App_DataProcess_KeyDataProcess(void)
{
	Com_Key key=Inf_JoyStickAndKey_KeyScan();
	switch(key)
	{
		case KEY_LEFT_TOP_LONG:
		{
			taskENTER_CRITICAL();//�����ٽ�������ֹҡ��ƫ�����������
			App_DataProcess_JoyStickCaclBias();
			taskEXIT_CRITICAL();
			debug_printfln("У׼ҡ��");			
			break;
		}
		case KEY_RIGHT_TOP:
		{
			JoyStick.isFixHeight=1;
			debug_printfln("��ת����ģʽ");			
			break;
		}
		case KEY_RIGHT_TOP_LONG:
		{
			JoyStick.isPowerDown=1;
			debug_printfln("�ػ�ģʽ");			
			break;
		}
		case KEY_LEFT:
		{
			JoyStickBias.ROL+=10;
			break;
		}
		case KEY_RIGHT:
		{
			JoyStickBias.ROL-=10;
			break;
		}
		case KEY_UP:
		{
			JoyStickBias.PIT-=10;
			break;
		}
		case KEY_DOWN:
		{
			JoyStickBias.PIT+=10;
			break;
		}		
		default :
			break;
	}	
}
