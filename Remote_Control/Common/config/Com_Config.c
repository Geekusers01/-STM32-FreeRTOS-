#include "Com_Config.h"

//摇杆数据结构体
JoyStick_Struct JoyStick;

//摇杆数据偏移量结构体
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
