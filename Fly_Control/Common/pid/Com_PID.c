#include "Com_PID.h"

/**
  * @brief ����PID
  * @param PID_Struct *pid PID�ṹ��
  * @retval 
  */
void Com_ComputePID(PID_Struct *pid)
{
	//1.�������
	float error=pid->measure-pid->desire;
	
	//2.������
	float pV=pid->kp*error;
	
	//3.������
	pid->integral+=pid->ki*error*pid->dt;
	
	//4.΢����
	float dV=pid->kd*(error-pid->lastError)/pid->dt;
	pid->lastError=error;
	
	//5.������
	pid->result=pV+pid->integral+dV;
}	

/**
  * @brief ���㴮��PID
  * @param	PID_Struct *out  �⻷PID
  * @param	PID_Struct *in	�ڻ�PID
  * @retval 
  */
void Com_CascadePID(PID_Struct *out,PID_Struct *in)
{
	//1.�����⻷PID
	Com_ComputePID(out);
	
	//2.���⻷�������Ϊ�ڻ�������ֵ
	in->desire=out->result;
	
	//3.�����ڻ�PID
	Com_ComputePID(in);
}
