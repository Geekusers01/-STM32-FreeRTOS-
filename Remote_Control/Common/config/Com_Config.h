#ifndef __COM_CONFIG_H
#define __COM_CONFIG_H

#include "stdint.h"
#include "stdio.h"
#include "string.h"

typedef enum
{
    Com_OK = 0,
    Com_TIMEOUT,
    Com_FAIL
} Com_Status;

#define LIMIT(x,min,max) (x)>=(max)?(max):(x)<=(min)?(min):(x);

typedef enum
{
	KEY_NONE=0,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT_TOP,
	KEY_RIGHT_TOP,
	KEY_LEFT_TOP_LONG,
	KEY_RIGHT_TOP_LONG
}Com_Key;

typedef struct
{
	int16_t THR;						//油门
	int16_t YAW;  				  //偏航
	int16_t PIT;						//俯仰
	int16_t ROL;						//横滚
	int16_t isPowerDown;    //是否关机，1为关机，0为不关机
	int16_t isFixHeight;		//是否定高，1为开启定高，0为关闭定高
	
}JoyStick_Struct;

extern JoyStick_Struct JoyStick;
extern JoyStick_Struct JoyStickBias;

#define FRAME_0 (0x11)
#define FRAME_1 (0x22)
#define FRAME_2 (0x33)

void Com_Config_PrintJoyStick(uint8_t *pre);

#endif
