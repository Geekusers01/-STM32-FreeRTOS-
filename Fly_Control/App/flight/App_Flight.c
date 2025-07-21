#include "App_Flight.h"

//6����̬PID
//����
PID_Struct pitchPID={.kp=-7.0f,.ki=0.0f,.kd=0.0f};
PID_Struct gyroYPID={.kp=2.0f,.ki=0.0f,.kd=0.1f};

//���
PID_Struct rollPID={.kp=-7.0f,.ki=0.0f,.kd=0.0f};
PID_Struct gyroXPID={.kp=-2.0f,.ki=0.0f,.kd=0.1f};

//ƫ��
PID_Struct yawPID={.kp=-2.2f,.ki=0.0f,.kd=0.0f};
PID_Struct gyroZPID={.kp=-1.5f,.ki=0.0f,.kd=0.0f};

//����
PID_Struct heightPID={.kp=-1.4f,.ki=0.0f,.kd=0.0f};
PID_Struct speedZPID={.kp=-1.3f,.ki=0.0f,.kd=-0.08f};

/**
  * @brief ����ģ�������
  * @param 
  * @retval 
  */
void App_Flight_Start(void)
{
	//��ʼ�����
	debug_printfln("��ʼ�����--��ʼ");
	Inf_Motor_Init();
	debug_printfln("��ʼ�����--����");
	
	//��ʼ��MPU6050
	debug_printfln("��ʼ��MPU6050--��ʼ");
	Inf_MPU6050_Init();
	debug_printfln("��ʼ��MPU6050--����");
}

/**
  * @brief ��6���������˲�
	*���ٶȣ�ʹ��һ�׵�ͨ�˲�������С�����Ӱ��С
	*���ٶȣ��������˲��������������ܵ�����
  * @param GyroAccel_Struct *GyroAccel ���ٶȺͽ��ٶȽṹ��
  * @retval 
  */
void App_Filter_GetGyroAccelWithFilter(GyroAccel_Struct *GyroAccel)
{
	//1.��ȡУ׼������
	taskENTER_CRITICAL();//�����ٽ�����ֹ�����
	Inf_MPU6050_ReadGyroAccelCalibrated(GyroAccel);
	taskEXIT_CRITICAL();
	
	//2.�Խ��ٶ���һ�׵�ͨ�˲�
	//Com_Config_PrintGyroAccel("�˲�ǰ",GyroAccel);
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
	
	//3.�Լ��ٶ����������˲�
//	OutData[0]=GyroAccel->Accel.AccelX;
	
	GyroAccel->Accel.AccelX=Common_Filter_KalmanFilter(&kfs[0],GyroAccel->Accel.AccelX);
	GyroAccel->Accel.AccelY=Common_Filter_KalmanFilter(&kfs[1],GyroAccel->Accel.AccelY);
	GyroAccel->Accel.AccelZ=Common_Filter_KalmanFilter(&kfs[2],GyroAccel->Accel.AccelZ);
	
	//Com_Config_PrintGyroAccel("�˲���",GyroAccel);
//	OutData[1]=GyroAccel->Accel.AccelX;
//	OutPut_Data();	
}

/**
  * @brief ������̬PID
  * @param GyroAccel_Struct *GyroAccel ���ٶȺͽ��ٶȽṹ��
  * @param EulerAngle_Struct *EulerAngle �ǶȽṹ��
  * @param float dt ����ʱ��
  * @retval 
  */
void App_Flight_PIDPostsure(GyroAccel_Struct *GyroAccel,EulerAngle_Struct *EulerAngle,float dt)
{
	//����
	pitchPID.dt=dt;
	pitchPID.desire=(JoyStick.PIT-500)*0.04;//���Ƹ�������
	pitchPID.measure=EulerAngle->pitch;
	
	gyroYPID.dt=dt;
	gyroYPID.measure=GyroAccel->Gyro.GyroY*Gyro_G;
	
	Com_CascadePID(&pitchPID,&gyroYPID);
	
	//���
	rollPID.dt=dt;
	rollPID.desire=(JoyStick.ROL-500)*0.04;//���ƺ������
	rollPID.measure=EulerAngle->roll;
	
	gyroXPID.dt=dt;
	gyroXPID.measure=GyroAccel->Gyro.GyroX*Gyro_G;
	
	Com_CascadePID(&rollPID,&gyroXPID);
	
	//ƫ��
	yawPID.dt=dt;
	yawPID.desire=(JoyStick.YAW-500)*0.04;//����ƫ������
	yawPID.measure=EulerAngle->yaw;
	
	gyroZPID.dt=dt;
	gyroZPID.measure=GyroAccel->Gyro.GyroZ*Gyro_G;
	
	Com_CascadePID(&yawPID,&gyroZPID);		
}

/**
  * @brief ����̬PID���õ������
  * @param Com_Status isRemoteUnlocked �Ƿ�������˻�
  * @retval 
  */
void App_Flight_MotorWithPosturePID(Com_Status isRemoteUnlocked)
{
	//���������+���� �� ����+���� GyroXPID.result
	//����������+���� �� ����+����	GyroYPID.result
	//ƫ��������+���� �� ����+����	GyroZPID.result
	int16_t speed=JoyStick.THR*0.7; //����30%�����Ÿ�PID����
	motorLeftTop.speed=		speed+gyroXPID.result+gyroYPID.result+gyroZPID.result;
	motorLeftBottom.speed=	speed+gyroXPID.result-gyroYPID.result-gyroZPID.result;
	motorRightTop.speed=	speed-gyroXPID.result+gyroYPID.result-gyroZPID.result;
	motorRightBottom.speed=	speed-gyroXPID.result-gyroYPID.result+gyroZPID.result;
}

/**
  * @brief ��ȡŷ����
  * @param GyroAccel_Struct *gyroAccel ���ٶȺͽ��ٶȽṹ��
  * @param EulerAngle_Struct *eulerAngle �ǶȽṹ��
  * @param float dt  ����ʱ��
  * @retval 
  */
void App_Flight_GetEulerAngle(GyroAccel_Struct  *gyroAccel,
                              EulerAngle_Struct *eulerAngle,
                              float dt)
{
	
	Common_IMU_GetEulerAngle(gyroAccel,eulerAngle,dt);
}	

/**
  * @brief ��÷ɻ��ĸ߶�
  * @param 
  * @retval �߶�mm
  */
uint16_t App_Flight_GetHeight(void)
{
	static uint16_t lastHeight=0;
	uint16_t height=Inf_VL53L1X_GetHeight();
	if(abs(height-lastHeight)>500||//����߶���ͻ�䣬���������ϰ��������ϴ�ֵ
	   abs(JoyStick.PIT-500)>100||//�����̬��б�������ϴ�ֵ
		 abs(JoyStick.ROL-500)>100)
	{
		return lastHeight;
	}
	height=Com_Filter_LowPass(height,lastHeight);//�Ը߶���һ�׵�ͨ�˲�
	lastHeight=height;	
	
	return height;
}

/**
  * @brief �߶�PID����
  * @param Com_Status isRemoteUnlocked �Ƿ����
  * @param uint16_t height �����߶�
  * @param float dt ��������
  * @retval 
  */
void App_Flight_HeightPID(Com_Status isRemoteUnlocked,uint16_t height,float dt)
{
/*����״̬����
	״̬0������Ƿ񶨸�
	״̬1����ǰ����ֵ�Ƕ���ʱ������ֵ����ǰ�߶��ǹ̶�ʱ�߶�
	״̬2������PID����
*/
	static uint8_t status=0;
	static uint16_t thrHold=0;//��������
	static uint16_t heightHold=0;//�����߶�
	static float staticAcc=0;//��̬ʱz����ٶ�
	if(isRemoteUnlocked==Com_OK&&staticAcc==0)
	{
		staticAcc=Common_IMU_GetNormAccZ();
	}
	switch(status)
	{
		case 0://���߼��
		{
			//PID����
			heightPID.result=0;
			speedZPID.result=0;
			if(isRemoteUnlocked==Com_OK&&isFixHeight==Com_OK)
			{
				status=1;
			}
			break;
		}
		case 1://PID����ǰ��׼��
		{
			thrHold=JoyStick.THR;
			heightHold=height;
			status=2;
			break;
		}
		case 2:
		{
			//����ʱ�����ű仯����100�����߶��߱��Ϊ0��ȡ������
			if(abs(JoyStick.THR-thrHold)>100||JoyStick.isFixHeight==Com_OK)
			{
				status=0;
				JoyStick.isFixHeight=0;
				isFixHeight=Com_FAIL;
			}
			else
			{
				//���ڸ߶ȱ仯������Ϊ20ms��������������Ϊ4ms������5�μ���һ��PID
				static uint8_t cnt=0;
				cnt++;
				if(cnt<5) return;
				cnt=0;
				dt*=5;
				//����z����ٶȣ��û����˲�
				float SpeedZ=0.9*(speedZPID.measure+(Common_IMU_GetNormAccZ()-staticAcc)*dt)+
											0.1*(height-heightPID.measure)/dt;
				//����PID���⻷Ϊ�߶Ȼ����ڻ�Ϊz�᷽����ٶȻ�
				//�⻷
				heightPID.desire=heightHold;
				heightPID.measure=height;
				heightPID.dt=dt;
				//�ڻ�
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
  * @brief �Ѷ���PID���õ������
  * @param Com_Status isRemoteUnlocked �Ƿ����
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
  * @brief ����������
  * @param Com_Status isRemoteUnlocked �Ƿ����
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
//	else//������
//	{
//		motorLeftTop.speed=
//		motorLeftBottom.speed=
//		motorRightTop.speed=
//		motorRightBottom.speed=JoyStick.THR;		
//	}
	Inf_Motor_AllMotorsWork();
}
