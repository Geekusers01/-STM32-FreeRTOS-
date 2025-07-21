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
	debug_printfln("2.4G模块自检--结束");
	
	debug_printfln("2.4G设置为接收模式");
	Inf_Si24R1_RXMode();
	
	debug_printfln("通讯模块的启动--结束");
}

/**
  * @brief 2.4g接收数据
  * @param 
  * @retval 如果返回值为Com_OK成功接收,否则接受失败
  */
Com_Status App_Communication_ReceiveJoyStickData(void)
{
	//1.判断是否收到数据,
	taskENTER_CRITICAL();//进入临界区，防止接收数据被打断
	uint8_t r=Inf_Si24R1_RxPacket(RX_BUFF);
	taskEXIT_CRITICAL();	
	if(r==1) return Com_FAIL;
	//2.判断收到的帧头是否相等
	if(RX_BUFF[0]!=FRAME_0||
			RX_BUFF[1]!=FRAME_1||
			RX_BUFF[2]!=FRAME_2)
		return Com_FAIL;
	//3.收到真正的数据
	JoyStick.THR=(RX_BUFF[4]<<8)|RX_BUFF[5];
	JoyStick.YAW=(RX_BUFF[6]<<8)|RX_BUFF[7];
	JoyStick.PIT=(RX_BUFF[8]<<8)|RX_BUFF[9];
	JoyStick.ROL=(RX_BUFF[10]<<8)|RX_BUFF[11];
	
	JoyStick.isPowerDown=RX_BUFF[12];
	
	//当RX_BUFF[13]为1且解锁时，切换定高模式
	if(RX_BUFF[13]&&(App_Communication_RemoteUnlock(isRemoteConnected)==Com_OK))
	{
		JoyStick.isFixHeight=!JoyStick.isFixHeight;	
		isFixHeight=JoyStick.isFixHeight==1?Com_OK:Com_FAIL;
	}
	//测试接收数据
//	Com_Config_PrintJoyStick("1");	
	return Com_OK;
}

/**
  * @brief 用来检测遥控器的连接情况
	*连接成功：
	*				只要收到一条数据，就算连接成功
	*失联：
	*				如果连续1.2s没有收到数据，表示失联
  * @param 是否收到遥控器数据
  * @retval 是否连接成功
  */
Com_Status App_Communication_CheckConnection(Com_Status isReceiveData)
{//记录连续收不到数据次数
	static uint8_t cnt=200;//防止启动时没有连接成功显示连接成功情况
	//1.如果收到数据，表示连接成功
	if(isReceiveData==Com_OK)	
	{
		cnt=0;
		return Com_OK;
	}
	//2.没有收到数据是，连续计时1.2s，200次
	cnt++;
	if(cnt>=200)
	{
		cnt=200;//避免超出范围
		return Com_FAIL;
	}
	return Com_OK;
}

/**
  * @brief 遥控器解锁
	*把油门拉到最高持续1.2s，然后拉到最低持续1.2s，解锁成功
	*自由状态：在这个状态下拉到油门最高>=960,进入最大值状态
	*最大值状态：如果油门持续1.2s，进入离开最大值状态，如果持续时间不到1.2s，回到自由状态
	*离开最大值状态：如果出现油门<=20,进入最小值状态
	*最小值状态：如果油门持续1.2s，进入解锁状态，如果持续时间不到1.2s，回到自由状态
	*解锁状态：如果油门<=20持续1min,进入自由状态
  * @param 是否连接成功
  * @retval 是否解锁成功，Com_OK为解锁成功，其他为解锁失败
  */
Com_Status App_Communication_RemoteUnlock(Com_Status isRemoteConnected)
{
	static uint8_t thrMAXDuration=0;
	static uint8_t thrMINDuration=0;
	//定义油门的状态
	static Com_RemoteStatus remoteStatus=THR_FREE;
	//debug_printfln("%d",remoteStatus);
	
	//1.如果失联状态，直接解锁失败
	if(isRemoteConnected!=Com_OK)
	{
		remoteStatus=THR_FREE;
		JoyStick.isFixHeight=0;
		isFixHeight=Com_FAIL;
		return Com_FAIL;		
	}
		
	switch (remoteStatus)
	{
		case THR_FREE:
		{//自由状态，油门最大值、最小值持续时间从零计数
			thrMAXDuration=0;
			thrMINDuration=0;
			if(JoyStick.THR>=960)
			{
				remoteStatus=THR_MAX;
			}
			break;
		}
		case THR_MAX:
		{
			if(JoyStick.THR>=960)
			{
				//判断持续时间
				thrMAXDuration++;
				if(thrMAXDuration>=200)
				{
					remoteStatus=THR_MAX_LEAVE;
				}
			}
			else
			{
				//没有持续最大值1.2s
				remoteStatus=THR_FREE;
			}
			break;
		}
		case THR_MAX_LEAVE:
		{
			if(JoyStick.THR<=20)
			{
				remoteStatus=THR_MIN;
			}
			break;
		}
		case THR_MIN:
		{
			if(JoyStick.THR<=20)
			{
				thrMINDuration++;
				if(thrMINDuration>=200)
				{
					remoteStatus=THR_UNLOCK;
				}
			}
			else
			{
				remoteStatus=THR_FREE;
			}				
			break;
		}
		case THR_UNLOCK:
		{
			//如果解锁状态下，油门长时间12s处于<=20,或者右摇杆往右下掰，恢复上锁
			static uint32_t lowDuration=0;
			if(JoyStick.THR<=20)
			{
				if(JoyStick.YAW>=600)
				{
					remoteStatus=THR_FREE;										
				}
				lowDuration++;
				if(lowDuration>=200*10)
				{
					remoteStatus=THR_FREE;					
					lowDuration=0;				
				}
			}
			else
			{
				lowDuration=0;				
			}
			//解锁成功，直接返回成功
			return Com_OK;			
		}
		default :
			break;
	}
	JoyStick.isFixHeight=0;
	isFixHeight=Com_FAIL;
	//默认返回失败
	return Com_FAIL;			
}
