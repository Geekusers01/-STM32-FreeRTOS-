#include "App_Flight.h"

//6个姿态PID
//俯仰
PID_Struct pitchPID={.kp=-7.0f,.ki=0.0f,.kd=0.0f};
PID_Struct gyroYPID={.kp=2.0f,.ki=0.0f,.kd=0.1f};

//横滚
PID_Struct rollPID={.kp=-7.0f,.ki=0.0f,.kd=0.0f};
PID_Struct gyroXPID={.kp=-2.0f,.ki=0.0f,.kd=0.1f};

//偏航
PID_Struct yawPID={.kp=-2.2f,.ki=0.0f,.kd=0.0f};
PID_Struct gyroZPID={.kp=-1.5f,.ki=0.0f,.kd=0.0f};

//定高
PID_Struct heightPID={.kp=-1.4f,.ki=0.0f,.kd=0.0f};
PID_Struct speedZPID={.kp=-1.3f,.ki=0.0f,.kd=-0.08f};

/**
  * @brief 飞行模块的启动
  * @param 
  * @retval 
  */
void App_Flight_Start(void)
{
	//初始化电机
	debug_printfln("初始化电机--开始");
	Inf_Motor_Init();
	debug_printfln("初始化电机--结束");
	
	//初始化MPU6050
	debug_printfln("初始化MPU6050--开始");
	Inf_MPU6050_Init();
	debug_printfln("初始化MPU6050--结束");
}

/**
  * @brief 对6轴数据做滤波
	*角速度：使用一阶低通滤波，波动小，外界影响小
	*加速度：卡尔曼滤波，波动大，容易受到干扰
  * @param GyroAccel_Struct *GyroAccel 加速度和角速度结构体
  * @retval 
  */
void App_Filter_GetGyroAccelWithFilter(GyroAccel_Struct *GyroAccel)
{
	//1.读取校准后数据
	taskENTER_CRITICAL();//进入临界区防止被打断
	Inf_MPU6050_ReadGyroAccelCalibrated(GyroAccel);
	taskEXIT_CRITICAL();
	
	//2.对角速度做一阶低通滤波
	//Com_Config_PrintGyroAccel("滤波前",GyroAccel);
//	OutData[0]=GyroAccel->Gyro.GyroX;
	static int16_t lastDatas[3]={0};
	
	GyroAccel->Gyro.GyroX=Com_Filter_LowPass(GyroAccel->Gyro.GyroX,lastDatas[0]);
	GyroAccel->Gyro.GyroY=Com_Filter_LowPass(GyroAccel->Gyro.GyroY,lastDatas[1]);
	GyroAccel->Gyro.GyroZ=Com_Filter_LowPass(GyroAccel->Gyro.GyroZ,lastDatas[2]);
	
	lastDatas[0]=GyroAccel->Gyro.GyroX;
	lastDatas[1]=GyroAccel->Gyro.GyroY;
	lastDatas[2]=GyroAccel->Gyro.GyroZ;
	
//	OutData[1]=GyroAccel->Gyro.GyroX;
//	OutPut_Data();
	
	//3.对加速度做卡尔曼滤波
//	OutData[0]=GyroAccel->Accel.AccelX;
	
	GyroAccel->Accel.AccelX=Common_Filter_KalmanFilter(&kfs[0],GyroAccel->Accel.AccelX);
	GyroAccel->Accel.AccelY=Common_Filter_KalmanFilter(&kfs[1],GyroAccel->Accel.AccelY);
	GyroAccel->Accel.AccelZ=Common_Filter_KalmanFilter(&kfs[2],GyroAccel->Accel.AccelZ);
	
	//Com_Config_PrintGyroAccel("滤波后",GyroAccel);
//	OutData[1]=GyroAccel->Accel.AccelX;
//	OutPut_Data();	
}

/**
  * @brief 计算姿态PID
  * @param GyroAccel_Struct *GyroAccel 加速度和角速度结构体
  * @param EulerAngle_Struct *EulerAngle 角度结构体
  * @param float dt 采样时间
  * @retval 
  */
void App_Flight_PIDPostsure(GyroAccel_Struct *GyroAccel,EulerAngle_Struct *EulerAngle,float dt)
{
	//俯仰
	pitchPID.dt=dt;
	pitchPID.desire=(JoyStick.PIT-500)*0.04;//控制俯仰方向
	pitchPID.measure=EulerAngle->pitch;
	
	gyroYPID.dt=dt;
	gyroYPID.measure=GyroAccel->Gyro.GyroY*Gyro_G;
	
	Com_CascadePID(&pitchPID,&gyroYPID);
	
	//横滚
	rollPID.dt=dt;
	rollPID.desire=(JoyStick.ROL-500)*0.04;//控制横滚方向
	rollPID.measure=EulerAngle->roll;
	
	gyroXPID.dt=dt;
	gyroXPID.measure=GyroAccel->Gyro.GyroX*Gyro_G;
	
	Com_CascadePID(&rollPID,&gyroXPID);
	
	//偏航
	yawPID.dt=dt;
	yawPID.desire=(JoyStick.YAW-500)*0.04;//控制偏航方向
	yawPID.measure=EulerAngle->yaw;
	
	gyroZPID.dt=dt;
	gyroZPID.measure=GyroAccel->Gyro.GyroZ*Gyro_G;
	
	Com_CascadePID(&yawPID,&gyroZPID);		
}

/**
  * @brief 把姿态PID作用到电机上
  * @param Com_Status isRemoteUnlocked 是否解锁无人机
  * @retval 
  */
void App_Flight_MotorWithPosturePID(Com_Status isRemoteUnlocked)
{
	//横滚：左上+左下 对 右上+右下 GyroXPID.result
	//俯仰：左上+右上 对 左下+右下	GyroYPID.result
	//偏航：左上+右下 对 右上+左下	GyroZPID.result
	int16_t speed=JoyStick.THR*0.7; //留出30%的油门给PID控制
	motorLeftTop.speed=		speed+gyroXPID.result+gyroYPID.result+gyroZPID.result;
	motorLeftBottom.speed=	speed+gyroXPID.result-gyroYPID.result-gyroZPID.result;
	motorRightTop.speed=	speed-gyroXPID.result+gyroYPID.result-gyroZPID.result;
	motorRightBottom.speed=	speed-gyroXPID.result-gyroYPID.result+gyroZPID.result;
}

/**
  * @brief 获取欧拉角
  * @param GyroAccel_Struct *gyroAccel 加速度和角速度结构体
  * @param EulerAngle_Struct *eulerAngle 角度结构体
  * @param float dt  采样时间
  * @retval 
  */
void App_Flight_GetEulerAngle(GyroAccel_Struct  *gyroAccel,
                              EulerAngle_Struct *eulerAngle,
                              float dt)
{
	
	Common_IMU_GetEulerAngle(gyroAccel,eulerAngle,dt);
}	

/**
  * @brief 获得飞机的高度
  * @param 
  * @retval 高度mm
  */
uint16_t App_Flight_GetHeight(void)
{
	static uint16_t lastHeight=0;
	uint16_t height=Inf_VL53L1X_GetHeight();
	if(abs(height-lastHeight)>500||//如果高度有突变，可能遇到障碍，返回上次值
	   abs(JoyStick.PIT-500)>100||//如果姿态倾斜，返回上次值
		 abs(JoyStick.ROL-500)>100)
	{
		return lastHeight;
	}
	height=Com_Filter_LowPass(height,lastHeight);//对高度做一阶低通滤波
	lastHeight=height;	
	
	return height;
}

/**
  * @brief 高度PID控制
  * @param Com_Status isRemoteUnlocked 是否解锁
  * @param uint16_t height 测量高度
  * @param float dt 采样周期
  * @retval 
  */
void App_Flight_HeightPID(Com_Status isRemoteUnlocked,uint16_t height,float dt)
{
/*定高状态机：
	状态0：检测是否定高
	状态1：当前油门值是定高时的油门值，当前高度是固定时高度
	状态2：进行PID控制
*/
	static uint8_t status=0;
	static uint16_t thrHold=0;//期望油门
	static uint16_t heightHold=0;//期望高度
	static float staticAcc=0;//静态时z轴加速度
	if(isRemoteUnlocked==Com_OK&&staticAcc==0)
	{
		staticAcc=Common_IMU_GetNormAccZ();
	}
	switch(status)
	{
		case 0://定高检测
		{
			//PID重置
			heightPID.result=0;
			speedZPID.result=0;
			if(isRemoteUnlocked==Com_OK&&isFixHeight==Com_OK)
			{
				status=1;
			}
			break;
		}
		case 1://PID计算前的准备
		{
			thrHold=JoyStick.THR;
			heightHold=height;
			status=2;
			break;
		}
		case 2:
		{
			//定高时，油门变化超过100；或者定高标记为0，取消定高
			if(abs(JoyStick.THR-thrHold)>100||JoyStick.isFixHeight==Com_OK)
			{
				status=0;
				JoyStick.isFixHeight=0;
				isFixHeight=Com_FAIL;
			}
			else
			{
				//由于高度变化的周期为20ms，飞行任务周期为4ms，所以5次计算一次PID
				static uint8_t cnt=0;
				cnt++;
				if(cnt<5) return;
				cnt=0;
				dt*=5;
				//计算z轴的速度，用互补滤波
				float SpeedZ=0.9*(speedZPID.measure+(Common_IMU_GetNormAccZ()-staticAcc)*dt)+
											0.1*(height-heightPID.measure)/dt;
				//串级PID：外环为高度环，内环为z轴方向的速度环
				//外环
				heightPID.desire=heightHold;
				heightPID.measure=height;
				heightPID.dt=dt;
				//内环
				speedZPID.measure=SpeedZ;
				speedZPID.dt=dt;
				
				Com_CascadePID(&heightPID,&speedZPID);				
			}
			break;
		}
		default:
			break;
	}		
}

/**
  * @brief 把定高PID作用到电机上
  * @param Com_Status isRemoteUnlocked 是否解锁
  * @retval 
  */
void App_Flight_MotorWithHeightPID(Com_Status isRemoteUnlocked)
{
	int16_t zPID=LIMIT(speedZPID.result,-150,150);
	motorLeftTop.speed+=zPID;
	motorLeftBottom.speed+=zPID;
	motorRightTop.speed+=zPID;
	motorRightBottom.speed+=zPID;
}

/**
  * @brief 飞行器工作
  * @param Com_Status isRemoteUnlocked 是否解锁
  * @retval 
  */
void App_Flight_Work(Com_Status isRemoteUnlocked)
{
	if(isRemoteUnlocked!=Com_OK||JoyStick.THR<=30)
	{
		motorLeftTop.speed=
		motorLeftBottom.speed=
		motorRightTop.speed=
		motorRightBottom.speed=0;
	}
//	else//测试用
//	{
//		motorLeftTop.speed=
//		motorLeftBottom.speed=
//		motorRightTop.speed=
//		motorRightBottom.speed=JoyStick.THR;		
//	}
	Inf_Motor_AllMotorsWork();
}
