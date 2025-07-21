#ifndef __APP_TASK_H
#define __APP_TASK_H
#include "Com_Debug.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "App_communication.h"
#include "Inf_JoyStickAndKey.h"
#include "App_DataProcess.h"
#include "App_Display.h"

void App_Task_FreeRTOSStart(void);

#endif
