#include "Com_Config.h"

//ҡ�����ݽṹ��
JoyStick_Struct JoyStick;

//ҡ������ƫ�����ṹ��
JoyStick_Struct JoyStickBias;

void Com_Config_PrintJoyStick(uint8_t *pre)
{
	printf("%s THR=%d, YAW=%d, PIT=%d, ROL=%d\r\n",
								pre,
								JoyStick.THR,
								JoyStick.YAW,
								JoyStick.PIT,
								JoyStick.ROL);
}
