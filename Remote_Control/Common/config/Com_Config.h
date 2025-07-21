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
	int16_t THR;						//����
	int16_t YAW;  				  //ƫ��
	int16_t PIT;						//����
	int16_t ROL;						//���
	int16_t isPowerDown;    //�Ƿ�ػ���1Ϊ�ػ���0Ϊ���ػ�
	int16_t isFixHeight;		//�Ƿ񶨸ߣ�1Ϊ�������ߣ�0Ϊ�رն���
	
}JoyStick_Struct;

extern JoyStick_Struct JoyStick;
extern JoyStick_Struct JoyStickBias;

#define FRAME_0 (0x11)
#define FRAME_1 (0x22)
#define FRAME_2 (0x33)

void Com_Config_PrintJoyStick(uint8_t *pre);

#endif
