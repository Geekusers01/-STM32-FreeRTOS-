#ifndef __APP_FLIGHT_H__
#define __APP_FLIGHT_H__
#include "Com_Debug.h"
#include "Inf_Motor.h"
#include "Inf_mpu6050.h"
#include "Com_Filter.h"
#include "FreeRTOS.h"
#include "task.h"
#include "outputdata.h"
#include "Com_Config.h"
#include "Com_IMU.h"
#include "Com_PID.h"
#include "Inf_vl53l1x.h"

void App_Flight_Start(void);
void App_Flight_Work(Com_Status isRemoteUnlocked);
void App_Filter_GetGyroAccelWithFilter(GyroAccel_Struct *GyroAccel);
void App_Flight_GetEulerAngle(GyroAccel_Struct  *gyroAccel,
                              EulerAngle_Struct *eulerAngle,
                              float dt);
void App_Flight_PIDPostsure(GyroAccel_Struct *GyroAccel,EulerAngle_Struct *EulerAngle,float dt);
void App_Flight_MotorWithPosturePID(Com_Status isRemoteUnlocked);
uint16_t App_Flight_GetHeight(void);
void App_Flight_HeightPID(Com_Status isRemoteUnlocked,uint16_t height,float dt);
void App_Flight_MotorWithHeightPID(Com_Status isRemoteUnlocked);

#endif
