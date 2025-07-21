#include "App_Task.h"

/* 0. 启动任务 */
void startTask(void *args);
#define START_TASK_NAME "start_task"
#define START_TASK_STACK 128
#define START_TASK_PRIORITY 10
TaskHandle_t startTaskHandle;

/*1.通讯任务*/
void communicationTask(void *args);
#define COMMUNICATION_TASK_NAME "communication_Task"
#define COMMUNICATION_TASK_STACK 256
#define COMMUNICATION_TASK_PRIORITY 8
TaskHandle_t communicationTaskHandle;
#define COMMUNICATION_EXEC_CYCLE 6

/*2.按键任务*/
void keyTask(void *args);
#define KEY_TASK_NAME "key_Task"
#define KEY_TASK_STACK 256
#define KEY_TASK_PRIORITY 7
TaskHandle_t keyTaskHandle;
#define KEY_EXEC_CYCLE 50

/*3.摇杆任务*/
void joystickTask(void *args);
#define Joy_Stick_TASK_NAME "joystick_Task"
#define Joy_Stick_TASK_STACK 256
#define Joy_Stick_TASK_PRIORITY 7
TaskHandle_t joystickTaskHandle;
#define Joy_Stick_EXEC_CYCLE 10

/*4.显示任务*/
void displayTask(void *args);
#define Display_TASK_NAME "diaplay_Task"
#define Display_TASK_STACK 256
#define Display_TASK_PRIORITY 7
TaskHandle_t displayTaskHandle;
#define Display_EXEC_CYCLE 7

/**
 * @description: 启动实时系统
 * @return {*}
 */
void App_Task_FreeRTOSStart(void)
{
    /*1.初始化debug模块*/
    debug_start();
	
		debug_printfln("无人机--遥控初始化开始");
	
    /*2.启动通讯模块*/
		App_Communication_Start();
	
    /*3.启动数据处理模块*/
		App_DataProcess_Start();

    /*4.启动显示模块*/
		App_Display_Start();
	
    /* 1. 创建一个启动任务: 启动任务执行的时候, 创建和业务相关的任务 */
    BaseType_t r = xTaskCreate(startTask,        /* 启动任务函数 */
                               START_TASK_NAME,  /* 任务名 */
                               START_TASK_STACK, /* 栈大小: 单位是字(不是自己) 字的大小: 32位架构是4  START_TASK_STACK * 4 = 字节数  */
                               NULL,             /* 指定给任务函数传递参数 */
                               START_TASK_PRIORITY,
                               &startTaskHandle);
    if(r == pdPASS)
    {
        debug_printfln("启动任务创建成功...");
    }
    else
    {
        debug_printfln("启动任务创建失败...");
    }

    /* 2. 启动调度器 */
    vTaskStartScheduler();
}

/* 启动任务函数 */
void startTask(void *args)
{
    debug_printfln("启动任务开始执行：开始调度");
	
		/*1.创建通讯任务*/
		xTaskCreate(communicationTask,
								COMMUNICATION_TASK_NAME,
								COMMUNICATION_TASK_STACK,
								NULL,
								COMMUNICATION_TASK_PRIORITY,
								&communicationTaskHandle);
								
		/*2.创建按键扫描任务*/
		xTaskCreate(keyTask,
								KEY_TASK_NAME,
								KEY_TASK_STACK,
								NULL,
								KEY_TASK_PRIORITY,
								&keyTaskHandle);
	
		/*3.创建摇杆扫描任务*/
		xTaskCreate(joystickTask,
								Joy_Stick_TASK_NAME,
								Joy_Stick_TASK_STACK,
								NULL,
								Joy_Stick_TASK_PRIORITY,
								&joystickTaskHandle);
								
		/*4.创建显示任务*/
		xTaskCreate(displayTask,
								Display_TASK_NAME,
								Display_TASK_STACK,
								NULL,
								Display_TASK_PRIORITY,
								&displayTaskHandle);
								
    /* 删除自己 */
    vTaskDelete(NULL);
    debug_printfln("不会执行");
}


/*1.通讯任务*/
void communicationTask(void *args)
{
	vTaskDelay(1000);//通讯与飞控优先级相同，设置延时预防卡死
	debug_printfln("通讯任务开始调度");
	uint32_t pretime=xTaskGetTickCount();
	
//	TX_BUFF[0]=100;
//	TX_BUFF[1]=101;
//	TX_BUFF[2]=102;
//	TX_BUFF[3]=103;
//	TX_BUFF[4]=104;
	
	while(1)
	{
//		uint8_t r=Inf_Si24R1_TxPacket(TX_BUFF);
//		if(r==0)
//		{
//			debug_printfln ("发送成功");
//		}
//		else
//		{
//			debug_printfln ("发送失败");
//		}
		App_Communication_SendJoyStickData();
		vTaskDelayUntil(&pretime,COMMUNICATION_EXEC_CYCLE);
	}
}

/*2.按键扫描任务*/
void keyTask(void *args)
{
	vTaskDelay(300);//防止与摇杆任务冲突
	debug_printfln("按键扫描任务开始调度");
	uint32_t pretime=xTaskGetTickCount();
	while(1)
	{
//		Com_Key KEY=Inf_JoyStickAndKey_KeyScan();
//		if(KEY==KEY_LEFT)
//		{
//			debug_printfln("左键按下");			
//		}
//		else if(KEY==KEY_RIGHT)
//		{
//			debug_printfln("右键按下");			
//		}
//		else if(KEY==KEY_UP)
//		{
//			debug_printfln("上键按下");			
//		}
//		else if(KEY==KEY_DOWN)
//		{
//			debug_printfln("下键按下");			
//		}
//		else if(KEY==KEY_LEFT_TOP)
//		{
//			debug_printfln("左上键按下");			
//		}
//		else if(KEY==KEY_RIGHT_TOP)
//		{
//			debug_printfln("右上键按下");			
//		}
//		else if(KEY==KEY_LEFT_TOP_LONG)
//		{
//			debug_printfln("左上键长按");			
//		}
//		else if(KEY==KEY_RIGHT_TOP_LONG)
//		{
//			debug_printfln("右上键长按");			
//		}
		App_DataProcess_KeyDataProcess();
		vTaskDelayUntil(&pretime,KEY_EXEC_CYCLE);		
	}
}

/*3.摇杆扫描任务*/
void joystickTask(void *args)
{
	vTaskDelay(500);//防止与按键任务冲突
	debug_printfln("摇杆扫描任务开始调度");
	uint32_t pretime=xTaskGetTickCount();	
	while(1)
	{
		App_DataProcess_JoyStickDataProcess();
		vTaskDelayUntil(&pretime,Joy_Stick_EXEC_CYCLE);
//		Com_Config_PrintJoyStick("1");
	}
}

/*4.显示任务*/
void displayTask(void *args)
{
	debug_printfln("显示任务开始调度");
	uint32_t pretime=xTaskGetTickCount();	
	while(1)
	{
		App_Display_Show();
		vTaskDelayUntil(&pretime,Display_EXEC_CYCLE);
	}
}
