#ifndef __INF_JOYSTICKANDKEY_H__
#define __INF_JOYSTICKANDKEY_H__

#include "Com_Debug.h"
#include "Com_Config.h"
#include "gpio.h"
#include "adc.h"
#include "FreeRTOS.h"
#include "task.h"

void Inf_JoyStickAndKey_Init(void);
void Inf_JoyStickAndKey_JoyStickScan(void);
Com_Key Inf_JoyStickAndKey_KeyScan(void);

#endif
