#include "Com_Config.h"

/*定义4个LED*/

LedStruct ledLeftTop={LEFT_TOP,0};
LedStruct ledLeftBottom={LEFT_BOTTOM,0};
LedStruct ledRightTop={RIGHT_TOP,0};
LedStruct ledRightBottom={RIGHT_BOTTOM,0};

/*定义飞控板的状态*/
/*1.遥控器是否连接成功*/
Com_Status isRemoteConnected=Com_FAIL;
/*2.遥控器是否解锁*/
Com_Status isRemoteUnlocked=Com_FAIL;
/*3.是否开启定高模式*/
Com_Status isFixHeight=Com_FAIL;

/*四个电机*/
Motor_Struct motorLeftTop={LEFT_TOP,0};
Motor_Struct motorLeftBottom={LEFT_BOTTOM,0};
Motor_Struct motorRightTop={RIGHT_TOP,0};
Motor_Struct motorRightBottom={RIGHT_BOTTOM,0};

//摇杆数据结构体
JoyStick_Struct JoyStick;

//摇杆数据偏移量结构体
JoyStick_Struct JoyStickBias;

//加速度角速度结构体
GyroAccel_Struct GyroAccel;

//欧拉角
EulerAngle_Struct EulerAngle;

void Com_Config_PrintMotor(uint8_t *pre)
{
	printf("%s motorLeftTop=%d, motorLeftBottom=%d, motorRightTop=%d, motorRightBottom=%d\r\n",
								pre,
								motorLeftTop.speed,
								motorLeftBottom.speed,	
								motorRightTop.speed,	
								motorRightBottom.speed);
}

void Com_Config_PrintJoyStick(uint8_t *pre)
{
	printf("%s THR=%d, YAW=%d, PIT=%d, ROL=%d,isPowerDown=%d,isFixHeight=%d\r\n",
								pre,
								JoyStick.THR,
								JoyStick.YAW,
								JoyStick.PIT,
								JoyStick.ROL,
								JoyStick.isPowerDown,
								JoyStick.isFixHeight);
}

void Com_Config_PrintGyroAccel(uint8_t *pre,GyroAccel_Struct *GyroAccel)
{
	printf("%s gx=%d,gy=%d,gz=%d,ax=%d,ay=%d,az=%d\r\n",
					pre,
					GyroAccel->Gyro.GyroX,
					GyroAccel->Gyro.GyroY,
					GyroAccel->Gyro.GyroZ,
					GyroAccel->Accel.AccelX,
					GyroAccel->Accel.AccelY,
					GyroAccel->Accel.AccelZ);
}

void Com_Config_PrintEulerAngle(uint8_t *pre,EulerAngle_Struct *EulerAngle)
{
	printf("%s pitch=%.1f,roll=%.1f,yaw=%.1f\r\n",
					pre,
					EulerAngle->pitch,
					EulerAngle->roll,
					EulerAngle->yaw);
}
