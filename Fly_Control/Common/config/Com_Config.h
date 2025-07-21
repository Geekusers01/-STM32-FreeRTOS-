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
    /*��ʾ�Ƶ�λ��*/
    Com_Location location;
    /*��ʾ�Ƶ�״̬��
        0������
        1������
        2+����˸����
    */
    uint8_t status;
}LedStruct;

/*��ʾ����Ľṹ��*/
typedef struct
{
    Com_Location location;
    int16_t speed;
}Motor_Struct;

typedef struct
{
	int16_t THR;						//����
	int16_t YAW;  				  //ƫ��
	int16_t PIT;						//����
	int16_t ROL;						//���
	int16_t isPowerDown;    //�Ƿ�ػ���1Ϊ�ػ���0Ϊ���ػ�
	int16_t isFixHeight;		//�Ƿ񶨸ߣ�1Ϊ�������ߣ�0Ϊ�رն���
	
}JoyStick_Struct;

//���ٶȽṹ��
typedef struct
{
	int16_t GyroX;
	int16_t GyroY;
	int16_t GyroZ;	
}Gyro_Struct;

//���ٶȽṹ��
typedef struct
{
	int16_t AccelX;
	int16_t AccelY;
	int16_t AccelZ;	
}Accel_Struct;

//���ٶȽ��ٶȽṹ��
typedef struct
{
	Gyro_Struct Gyro;
	Accel_Struct Accel;
}GyroAccel_Struct;

#define FRAME_0 (0x11)
#define FRAME_1 (0x22)
#define FRAME_2 (0x33)


//�������Ž���ʱ�ļ���״̬
typedef enum
{
	THR_FREE=0,
	THR_MAX,
	THR_MAX_LEAVE,
	THR_MIN,
	THR_UNLOCK,	
}Com_RemoteStatus;

//ŷ����
typedef struct
{
	float pitch;
	float roll;
	float yaw;
}EulerAngle_Struct;

//��Ԫ���ṹ��
typedef struct
{
	float q0;
	float q1;
	float q2;
	float q3;	
}Quaternion_Struct;

//PID�ṹ��
typedef struct
{
	float kp;		//����ϵ��
	float ki;		//����ϵ��
	float kd;		//΢��ϵ��
	float	dt;		//����ʱ��
	float	integral;		//�������ֵ
	float	lastError;	//�ϴ����
	float	desire;			//����ֵ
	float	measure;		//����ֵ
	float result;			//���ս��
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

/*1.ң�����Ƿ����ӳɹ�*/
extern Com_Status isRemoteConnected;
/*2.ң�����Ƿ����*/
extern Com_Status isRemoteUnlocked;
/*3.�Ƿ�������ģʽ*/
extern Com_Status isFixHeight;

/*�ĸ����*/
extern Motor_Struct motorLeftTop;
extern Motor_Struct motorLeftBottom;
extern Motor_Struct motorRightTop;
extern Motor_Struct motorRightBottom;

extern JoyStick_Struct JoyStick;

extern GyroAccel_Struct GyroAccel;

extern EulerAngle_Struct EulerAngle;

#endif
