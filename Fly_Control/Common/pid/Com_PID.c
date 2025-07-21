#include "Com_PID.h"

/**
  * @brief 计算PID
  * @param PID_Struct *pid PID结构体
  * @retval 
  */
void Com_ComputePID(PID_Struct *pid)
{
	//1.计算误差
	float error=pid->measure-pid->desire;
	
	//2.比例项
	float pV=pid->kp*error;
	
	//3.积分项
	pid->integral+=pid->ki*error*pid->dt;
	
	//4.微分项
	float dV=pid->kd*(error-pid->lastError)/pid->dt;
	pid->lastError=error;
	
	//5.计算结果
	pid->result=pV+pid->integral+dV;
}	

/**
  * @brief 计算串级PID
  * @param	PID_Struct *out  外环PID
  * @param	PID_Struct *in	内环PID
  * @retval 
  */
void Com_CascadePID(PID_Struct *out,PID_Struct *in)
{
	//1.计算外环PID
	Com_ComputePID(out);
	
	//2.把外环的输出做为内环的期望值
	in->desire=out->result;
	
	//3.计算内环PID
	Com_ComputePID(in);
}
