#ifndef __COM_IMU_H
#define __COM_IMU_H
#include "Com_Debug.h"
#include "Com_Config.h"


/* 计算欧拉角用到的3个参数 */
extern float RtA;   // 弧度->度
// 陀螺仪初始化量程+-2000度/秒于 1/(65536 / 4000) = 0.03051756*2
// float Gyro_G = 0.03051756f * 2;
extern float Gyro_G;   // 度/s
// 度每秒,转换弧度每秒则 2*0.03051756 * 0.0174533f = 0.0005326*2
// float Gyro_Gr = 0.0005326f * 2;
extern float Gyro_Gr;   // 弧度/s
void Common_IMU_GetEulerAngle(GyroAccel_Struct  *gyroAccel,
                              EulerAngle_Struct *eulerAngle,
                              float              dt);

float Common_IMU_GetNormAccZ(void);

#endif

