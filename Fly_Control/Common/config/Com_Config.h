#ifndef __COM_CONFIG_H
#define __COM_CONFIG_H

#include <stdint.h>
#include <stdio.h>

typedef enum
{
    Com_OK = 0,
    Com_TIMEOUT,
    Com_FAIL
} Com_Status;

typedef enum
{
    LEFT_TOP,
    LEFT_BOTTOM,
    RIGHT_TOP,
    RIGHT_BOTTOM
}Com_Location;

typedef struct
{
    /*表示灯的位置*/
    Com_Location location;
    /*表示灯的状态；
        0：常亮
        1：常灭
        2+：闪烁周期
    */
    uint8_t status;
}LedStruct;

/*表示电机的结构体*/
typedef struct
{
    Com_Location location;
    int16_t speed;
}Motor_Struct;

typedef struct
{
	int16_t THR;						//油门
	int16_t YAW;  				  //偏航
	int16_t PIT;						//俯仰
	int16_t ROL;						//横滚
	int16_t isPowerDown;    //是否关机，1为关机，0为不关机
	int16_t isFixHeight;		//是否定高，1为开启定高，0为关闭定高
	
}JoyStick_Struct;

//加速度结构体
typedef struct
{
	int16_t GyroX;
	int16_t GyroY;
	int16_t GyroZ;	
}Gyro_Struct;

//角速度结构体
typedef struct
{
	int16_t AccelX;
	int16_t AccelY;
	int16_t AccelZ;	
}Accel_Struct;

//加速度角速度结构体
typedef struct
{
	Gyro_Struct Gyro;
	Accel_Struct Accel;
}GyroAccel_Struct;

#define FRAME_0 (0x11)
#define FRAME_1 (0x22)
#define FRAME_2 (0x33)


//定义油门解锁时的几个状态
typedef enum
{
	THR_FREE=0,
	THR_MAX,
	THR_MAX_LEAVE,
	THR_MIN,
	THR_UNLOCK,	
}Com_RemoteStatus;

//欧拉角
typedef struct
{
	float pitch;
	float roll;
	float yaw;
}EulerAngle_Struct;

//四元数结构体
typedef struct
{
	float q0;
	float q1;
	float q2;
	float q3;	
}Quaternion_Struct;

//PID结构体
typedef struct
{
	float kp;		//比例系数
	float ki;		//积分系数
	float kd;		//微分系数
	float	dt;		//采样时间
	float	integral;		//保存积分值
	float	lastError;	//上次误差
	float	desire;			//期望值
	float	measure;		//测量值
	float result;			//最终结果
}PID_Struct;

#define LIMIT(x,min,max) (x)>=(max)?(max):(x)<=(min)?(min):(x);

void Com_Config_PrintJoyStick(uint8_t *pre);

void Com_Config_PrintGyroAccel(uint8_t *pre,GyroAccel_Struct *GyroAccel);

void Com_Config_PrintEulerAngle(uint8_t *pre,EulerAngle_Struct *EulerAngle);

void Com_Config_PrintMotor(uint8_t *pre);

extern LedStruct ledLeftTop;
extern LedStruct ledLeftBottom;
extern LedStruct ledRightTop;
extern LedStruct ledRightBottom;

/*1.遥控器是否连接成功*/
extern Com_Status isRemoteConnected;
/*2.遥控器是否解锁*/
extern Com_Status isRemoteUnlocked;
/*3.是否开启定高模式*/
extern Com_Status isFixHeight;

/*四个电机*/
extern Motor_Struct motorLeftTop;
extern Motor_Struct motorLeftBottom;
extern Motor_Struct motorRightTop;
extern Motor_Struct motorRightBottom;

extern JoyStick_Struct JoyStick;

extern GyroAccel_Struct GyroAccel;

extern EulerAngle_Struct EulerAngle;

#endif
