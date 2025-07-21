#include "App_Task.h"

/* 0. 启动任务 */
void startTask(void *args);
#define START_TASK_NAME "start_task"
#define START_TASK_STACK 128
#define START_TASK_PRIORITY 10
TaskHandle_t startTaskHandle;

/*1.灯控任务*/
void ledTask(void *args);
#define LED_TASK_NAME "led_Task"
#define LED_TASK_STACK 128
#define LED_TASK_PRIORITY 2
TaskHandle_t ledTaskHandle;
#define LED_EXEC_CYCLE 50

/*2.飞控任务*/
void flightTask(void *args);
#define FLIGHT_TASK_NAME "flight_Task"
#define FLIGHT_TASK_STACK 256
#define FLIGHT_TASK_PRIORITY 8
TaskHandle_t flightTaskHandle;
#define FLIGHT_EXEC_CYCLE 4

/*3.通讯任务*/
void communicationTask(void *args);
#define COMMUNICATION_TASK_NAME "communication_Task"
#define COMMUNICATION_TASK_STACK 256
#define COMMUNICATION_TASK_PRIORITY 8
TaskHandle_t communicationTaskHandle;
#define COMMUNICATION_EXEC_CYCLE 6

/**
 * @description: 启动实时系统
 * @return {*}
 */
void App_Task_FreeRTOSStart(void)
{
    /*1.初始化debug模块*/
    debug_start();
	
		debug_printfln("无人机--飞控初始化开始");
	
    /*2.启动飞控模块*/
		App_Flight_Start();
	
    /*3.启动通讯模块*/
		App_Communication_Start();
	
    /*4.启动激光测距模块*/
		Inf_VL53L1X_Init();
	
    /* 1. 创建一个启动任务: 启动任务执行的时候, 创建和业务相关的任务 */
    BaseType_t r = xTaskCreate(startTask,        /* 启动任务函数 */
                               START_TASK_NAME,  /* 任务名 */
                               START_TASK_STACK, /* 栈大小:   START_TASK_STACK * 4 = 字节数  */
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
		/*1.创建灯控任务*/
		xTaskCreate(ledTask,
								LED_TASK_NAME,
								LED_TASK_STACK,
								NULL,
								LED_TASK_PRIORITY,
								&ledTaskHandle);

		/*2.创建飞控任务*/
		xTaskCreate(flightTask,
								FLIGHT_TASK_NAME,
								FLIGHT_TASK_STACK,
								NULL,
								FLIGHT_TASK_PRIORITY,
								&flightTaskHandle);
	
		/*3.创建通讯任务*/
		xTaskCreate(communicationTask,
								COMMUNICATION_TASK_NAME,
								COMMUNICATION_TASK_STACK,
								NULL,
								COMMUNICATION_TASK_PRIORITY,
								&communicationTaskHandle);
								
    /* 删除自己 */
    vTaskDelete(NULL);
    debug_printfln("不会执行");
}

/*1.灯控任务*/
void ledTask(void *args)
{
	debug_printfln("灯控任务开始调度");
//	ledLeftTop.status=0;
//	ledLeftBottom.status=1;
//	ledRightTop.status=1;
//	ledRightBottom.status=1;
//	Inf_LED_SetStatus(&ledLeftTop);
//	Inf_LED_SetStatus(&ledLeftBottom);
//	Inf_LED_SetStatus(&ledRightTop);
//	Inf_LED_SetStatus(&ledRightBottom);
	
//	isRemoteConnected=Com_OK;
//	isRemoteUnlocked=Com_OK;
//	isFixHeight=Com_OK;	
	
	uint32_t preTime=xTaskGetTickCount();
	uint32_t cnt=0;
	static uint8_t isFirstUnlock=1;//是否首次进入解锁
	while(1)
	{
		/*连接->解锁->定高*/
		if(isFixHeight==Com_OK)
		{
			ledLeftTop.status=
			ledLeftBottom.status=
			ledRightTop.status=
			ledRightBottom.status=10;	
		}
		else if(isRemoteUnlocked==Com_OK)
		{
			if(isFirstUnlock==1)
			{
				ledLeftTop.status=
				ledLeftBottom.status=
				ledRightTop.status=
				ledRightBottom.status=1;
				isFirstUnlock=0;
			}
			else
			{
				ledLeftTop.status=
				ledLeftBottom.status=
				ledRightTop.status=
				ledRightBottom.status=0;
			}				
		}
		else if(isRemoteConnected==Com_OK)
		{
			isFirstUnlock=1;
			ledLeftTop.status=ledRightTop.status=15;
			ledLeftBottom.status=ledRightBottom.status=1;		
		}
		else
		{
			ledLeftTop.status=ledRightTop.status=1;
			ledLeftBottom.status=ledRightBottom.status=15;		
		}
		cnt++;
		if(ledLeftTop.status<=1||cnt%ledLeftTop.status==0)
		{
			Inf_LED_SetStatus(&ledLeftTop);
		}
		if(ledLeftBottom.status<=1||cnt%ledLeftBottom.status==0)
		{
			Inf_LED_SetStatus(&ledLeftBottom);
		}
		if(ledRightTop.status<=1||cnt%ledRightTop.status==0)
		{
			Inf_LED_SetStatus(&ledRightTop);
		}
		if(ledRightBottom.status<=1||cnt%ledRightBottom.status==0)
		{
			Inf_LED_SetStatus(&ledRightBottom);
		}
		vTaskDelayUntil(&preTime,LED_EXEC_CYCLE);
	}
}

/*2.飞控任务*/
void flightTask(void *args)
{
	debug_printfln("飞控任务开始调度");	
	uint32_t preTime=xTaskGetTickCount();
	
//	motorLeftTop.speed=0;	
//	motorLeftBottom.speed=100;	
//	motorRightTop.speed=0;	
//	motorRightBottom.speed=0;	
	float dt=FLIGHT_EXEC_CYCLE/1000.0; //调度周期
	while(1)
	{
//		Inf_Motor_AllMotorsWork();
//		Inf_MPU6050_ReadGyroAccelCalibrated(&GyroAccel);
//		Com_Config_PrintGyroAccel("校准后",&GyroAccel);
			
		//1.对6轴数据做滤波
		App_Filter_GetGyroAccelWithFilter(&GyroAccel);
		//Com_Config_PrintGyroAccel("滤波后",&GyroAccel);
		
		//2.获取欧拉角
		App_Flight_GetEulerAngle(&GyroAccel,&EulerAngle,dt);		
		//Com_Config_PrintEulerAngle("欧拉角",&EulerAngle);
			
		//3.计算3个串级PID
		App_Flight_PIDPostsure(&GyroAccel,&EulerAngle,dt);
		
		//4.把姿态PID作用到电机上
		App_Flight_MotorWithPosturePID(isRemoteUnlocked);
		//Com_Config_PrintMotor("PID");
		
		//5.获取高度
		uint16_t height=App_Flight_GetHeight();
		
		//6.计算高度PID
		App_Flight_HeightPID(isRemoteUnlocked,height,dt);
		
		//7.把高度PID作用到电机上
		App_Flight_MotorWithHeightPID(isRemoteUnlocked);
		
		//8.控制电机转动
		App_Flight_Work(isRemoteUnlocked);
		vTaskDelayUntil(&preTime,FLIGHT_EXEC_CYCLE);		
	}
}

/*3.通讯任务*/
void communicationTask(void *args)
{
	//vTaskDelay(1000);
	debug_printfln("通讯任务开始调度");
	uint32_t pretime=xTaskGetTickCount();
	while(1)
	{
		//1.读取数据
		Com_Status isReceiveData=App_Communication_ReceiveJoyStickData();
		
		//2.判断遥控器连接情况
		isRemoteConnected=App_Communication_CheckConnection(isReceiveData);

		//3.遥控器解锁
		isRemoteUnlocked = App_Communication_RemoteUnlock(isRemoteConnected);
		
//		uint8_t r=Inf_Si24R1_RxPacket(RX_BUFF);
//		if(r==0)
//		{
//			/*输出rx_buff中的数据*/
//			debug_printfln("%d %d %d %d %d",RX_BUFF[0],RX_BUFF[1],RX_BUFF[2],RX_BUFF[3],RX_BUFF[4]);
//		}
		
//		if(isReceiveData==Com_OK)
//		{
//			debug_printfln("接收到数据");
//		}
//		else
//		{
//			debug_printfln("没有收到数据");	
//		}
//		if(isRemoteConnected==Com_OK)
//		{
//			debug_printfln("连接成功");
//		}
//		else
//		{
//			debug_printfln("连接失败");	
//		}
//		if(isRemoteUnlocked==Com_OK)
//		{
//			debug_printfln("解锁成功");
//		}
//		else
//		{
//			debug_printfln("解锁失败");	
//		}
		vTaskDelayUntil(&pretime,COMMUNICATION_EXEC_CYCLE);
	}
}
