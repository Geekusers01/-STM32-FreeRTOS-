#include "App_DataProcess.h"

/**
* @brief 数据处理模块的启动
* @param 
* @retval 
*/
void App_DataProcess_Start(void)
{
	Inf_JoyStickAndKey_Init();
}

/**
* @brief 处理摇杆数据的极性和范围
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
* @brief  对摇杆做校准
* @param 
* @retval 
*/
static void App_DataProcess_JoyStickWithBias(void)
{
	//校准摇杆数据
	JoyStick.THR-=JoyStickBias.THR;
	JoyStick.YAW-=JoyStickBias.YAW;
	JoyStick.PIT-=JoyStickBias.PIT;
	JoyStick.ROL-=JoyStickBias.ROL;
	
	
	//对校准后数据做限幅
	JoyStick.THR=LIMIT(JoyStick.THR,0,1000);
	JoyStick.YAW=LIMIT(JoyStick.YAW,0,1000);
	JoyStick.PIT=LIMIT(JoyStick.PIT,0,1000);
	JoyStick.ROL=LIMIT(JoyStick.ROL,0,1000);
}

/**
* @brief 计算摇杆数据偏移量
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
		JoyStickBias.THR+=(JoyStick.THR-0);	 //0值校准
		JoyStickBias.YAW+=(JoyStick.YAW-500);//中值校准
		JoyStickBias.PIT+=(JoyStick.PIT-500);//中值校准
		JoyStickBias.ROL+=(JoyStick.ROL-500);//中值校准
		vTaskDelay(10);
	}
	JoyStickBias.THR/=100;
	JoyStickBias.YAW/=100;
	JoyStickBias.PIT/=100;
  JoyStickBias.ROL/=100;
}

/**
* @brief 摇杆数据处理
* @param 
* @retval 
*/
void App_DataProcess_JoyStickDataProcess(void)
{
	taskENTER_CRITICAL();//进入临界区，防止发送未处理的数据
	//1.扫描摇杆数据
	Inf_JoyStickAndKey_JoyStickScan();
	
	//2.处理极性和范围
	App_DataProcess_JoyStickPolarityAndRange();
	
	//3.处理摇杆漂移量
	App_DataProcess_JoyStickWithBias();
	taskEXIT_CRITICAL();
//	Com_Config_PrintJoyStick("2");
}

/**
* @brief 按键数据处理
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
			taskENTER_CRITICAL();//进入临界区，防止摇杆偏移量错误计算
			App_DataProcess_JoyStickCaclBias();
			taskEXIT_CRITICAL();
			debug_printfln("校准摇杆");			
			break;
		}
		case KEY_RIGHT_TOP:
		{
			JoyStick.isFixHeight=1;
			debug_printfln("反转定高模式");			
			break;
		}
		case KEY_RIGHT_TOP_LONG:
		{
			JoyStick.isPowerDown=1;
			debug_printfln("关机模式");			
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
