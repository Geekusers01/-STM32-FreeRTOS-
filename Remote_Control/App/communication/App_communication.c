#include "App_communication.h"

/**
  * @brief 启动通讯模块
  * @param 
  * @retval 
  */
void App_Communication_Start(void)
{
	debug_printfln("通讯模块的启动--开始");
	
	debug_printfln("2.4G模块自检--开始");
	while(Inf_Si24R1_Check()==1)
	{
		HAL_Delay(500);
	}
	debug_printfln("2.4G模块自检结果%d",Inf_Si24R1_Check());
	debug_printfln("2.4G模块自检--结束");
	
	debug_printfln("2.4G设置为发送模式");
	Inf_Si24R1_TXMode();
	
	debug_printfln("通讯模块的启动--结束");
}

/**
  * @brief 通过2.4g发送摇杆数据
	*前三个字节：0x11,0x22,0x33,帧头
	*1个字节：数据长度=10
	*10个字节：摇杆数据
	*4个字节：校验和
  * @param 
  * @retval 
  */
void App_Communication_SendJoyStickData(void)
{
	uint8_t index=0;
	//定义帧头	
	TX_BUFF[index++]=FRAME_0;
	TX_BUFF[index++]=FRAME_1;
	TX_BUFF[index++]=FRAME_2;
	
	//定义数据长度，后面再修改
	TX_BUFF[index++]=0;
	
	//摇杆数据
	TX_BUFF[index++]=JoyStick.THR>>8;
	TX_BUFF[index++]=JoyStick.THR;
	
	TX_BUFF[index++]=JoyStick.YAW>>8;
	TX_BUFF[index++]=JoyStick.YAW;
	
	TX_BUFF[index++]=JoyStick.PIT>>8;
	TX_BUFF[index++]=JoyStick.PIT;
	
	TX_BUFF[index++]=JoyStick.ROL>>8;
	TX_BUFF[index++]=JoyStick.ROL;
	
	TX_BUFF[index++]=JoyStick.isPowerDown;
	JoyStick.isPowerDown=0;								//清零关机命令
	
	TX_BUFF[index++]=JoyStick.isFixHeight;
	JoyStick.isFixHeight=0;								//接受方收到1再取反定高
	
	TX_BUFF[3]=index-4;
		
	//计算校验和
	uint32_t sum=0;
	for(uint8_t i=0;i<index;i++)
	{
		sum+=TX_BUFF[i];
	}
	TX_BUFF[index++]=sum>24;
	TX_BUFF[index++]=sum>16;
	TX_BUFF[index++]=sum>8;
	TX_BUFF[index++]=sum;
	
	taskENTER_CRITICAL();//进入临界区，防止发送数据被打断
	Inf_Si24R1_TxPacket(TX_BUFF);
	taskEXIT_CRITICAL();
}
