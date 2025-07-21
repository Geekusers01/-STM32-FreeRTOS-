#include "App_Task.h"

/* 0. �������� */
void startTask(void *args);
#define START_TASK_NAME "start_task"
#define START_TASK_STACK 128
#define START_TASK_PRIORITY 10
TaskHandle_t startTaskHandle;

/*1.�ƿ�����*/
void ledTask(void *args);
#define LED_TASK_NAME "led_Task"
#define LED_TASK_STACK 128
#define LED_TASK_PRIORITY 2
TaskHandle_t ledTaskHandle;
#define LED_EXEC_CYCLE 50

/*2.�ɿ�����*/
void flightTask(void *args);
#define FLIGHT_TASK_NAME "flight_Task"
#define FLIGHT_TASK_STACK 256
#define FLIGHT_TASK_PRIORITY 8
TaskHandle_t flightTaskHandle;
#define FLIGHT_EXEC_CYCLE 4

/*3.ͨѶ����*/
void communicationTask(void *args);
#define COMMUNICATION_TASK_NAME "communication_Task"
#define COMMUNICATION_TASK_STACK 256
#define COMMUNICATION_TASK_PRIORITY 8
TaskHandle_t communicationTaskHandle;
#define COMMUNICATION_EXEC_CYCLE 6

/**
 * @description: ����ʵʱϵͳ
 * @return {*}
 */
void App_Task_FreeRTOSStart(void)
{
    /*1.��ʼ��debugģ��*/
    debug_start();
	
		debug_printfln("���˻�--�ɿس�ʼ����ʼ");
	
    /*2.�����ɿ�ģ��*/
		App_Flight_Start();
	
    /*3.����ͨѶģ��*/
		App_Communication_Start();
	
    /*4.����������ģ��*/
		Inf_VL53L1X_Init();
	
    /* 1. ����һ����������: ��������ִ�е�ʱ��, ������ҵ����ص����� */
    BaseType_t r = xTaskCreate(startTask,        /* ���������� */
                               START_TASK_NAME,  /* ������ */
                               START_TASK_STACK, /* ջ��С:   START_TASK_STACK * 4 = �ֽ���  */
                               NULL,             /* ָ�������������ݲ��� */
                               START_TASK_PRIORITY,
                               &startTaskHandle);
    if(r == pdPASS)
    {
        debug_printfln("�������񴴽��ɹ�...");
    }
    else
    {
        debug_printfln("�������񴴽�ʧ��...");
    }

    /* 2. ���������� */
    vTaskStartScheduler();
}

/* ���������� */
void startTask(void *args)
{
    debug_printfln("��������ʼִ�У���ʼ����");
		/*1.�����ƿ�����*/
		xTaskCreate(ledTask,
								LED_TASK_NAME,
								LED_TASK_STACK,
								NULL,
								LED_TASK_PRIORITY,
								&ledTaskHandle);

		/*2.�����ɿ�����*/
		xTaskCreate(flightTask,
								FLIGHT_TASK_NAME,
								FLIGHT_TASK_STACK,
								NULL,
								FLIGHT_TASK_PRIORITY,
								&flightTaskHandle);
	
		/*3.����ͨѶ����*/
		xTaskCreate(communicationTask,
								COMMUNICATION_TASK_NAME,
								COMMUNICATION_TASK_STACK,
								NULL,
								COMMUNICATION_TASK_PRIORITY,
								&communicationTaskHandle);
								
    /* ɾ���Լ� */
    vTaskDelete(NULL);
    debug_printfln("����ִ��");
}

/*1.�ƿ�����*/
void ledTask(void *args)
{
	debug_printfln("�ƿ�����ʼ����");
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
	static uint8_t isFirstUnlock=1;//�Ƿ��״ν������
	while(1)
	{
		/*����->����->����*/
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

/*2.�ɿ�����*/
void flightTask(void *args)
{
	debug_printfln("�ɿ�����ʼ����");	
	uint32_t preTime=xTaskGetTickCount();
	
//	motorLeftTop.speed=0;	
//	motorLeftBottom.speed=100;	
//	motorRightTop.speed=0;	
//	motorRightBottom.speed=0;	
	float dt=FLIGHT_EXEC_CYCLE/1000.0; //��������
	while(1)
	{
//		Inf_Motor_AllMotorsWork();
//		Inf_MPU6050_ReadGyroAccelCalibrated(&GyroAccel);
//		Com_Config_PrintGyroAccel("У׼��",&GyroAccel);
			
		//1.��6���������˲�
		App_Filter_GetGyroAccelWithFilter(&GyroAccel);
		//Com_Config_PrintGyroAccel("�˲���",&GyroAccel);
		
		//2.��ȡŷ����
		App_Flight_GetEulerAngle(&GyroAccel,&EulerAngle,dt);		
		//Com_Config_PrintEulerAngle("ŷ����",&EulerAngle);
			
		//3.����3������PID
		App_Flight_PIDPostsure(&GyroAccel,&EulerAngle,dt);
		
		//4.����̬PID���õ������
		App_Flight_MotorWithPosturePID(isRemoteUnlocked);
		//Com_Config_PrintMotor("PID");
		
		//5.��ȡ�߶�
		uint16_t height=App_Flight_GetHeight();
		
		//6.����߶�PID
		App_Flight_HeightPID(isRemoteUnlocked,height,dt);
		
		//7.�Ѹ߶�PID���õ������
		App_Flight_MotorWithHeightPID(isRemoteUnlocked);
		
		//8.���Ƶ��ת��
		App_Flight_Work(isRemoteUnlocked);
		vTaskDelayUntil(&preTime,FLIGHT_EXEC_CYCLE);		
	}
}

/*3.ͨѶ����*/
void communicationTask(void *args)
{
	//vTaskDelay(1000);
	debug_printfln("ͨѶ����ʼ����");
	uint32_t pretime=xTaskGetTickCount();
	while(1)
	{
		//1.��ȡ����
		Com_Status isReceiveData=App_Communication_ReceiveJoyStickData();
		
		//2.�ж�ң�����������
		isRemoteConnected=App_Communication_CheckConnection(isReceiveData);

		//3.ң��������
		isRemoteUnlocked = App_Communication_RemoteUnlock(isRemoteConnected);
		
//		uint8_t r=Inf_Si24R1_RxPacket(RX_BUFF);
//		if(r==0)
//		{
//			/*���rx_buff�е�����*/
//			debug_printfln("%d %d %d %d %d",RX_BUFF[0],RX_BUFF[1],RX_BUFF[2],RX_BUFF[3],RX_BUFF[4]);
//		}
		
//		if(isReceiveData==Com_OK)
//		{
//			debug_printfln("���յ�����");
//		}
//		else
//		{
//			debug_printfln("û���յ�����");	
//		}
//		if(isRemoteConnected==Com_OK)
//		{
//			debug_printfln("���ӳɹ�");
//		}
//		else
//		{
//			debug_printfln("����ʧ��");	
//		}
//		if(isRemoteUnlocked==Com_OK)
//		{
//			debug_printfln("�����ɹ�");
//		}
//		else
//		{
//			debug_printfln("����ʧ��");	
//		}
		vTaskDelayUntil(&pretime,COMMUNICATION_EXEC_CYCLE);
	}
}
