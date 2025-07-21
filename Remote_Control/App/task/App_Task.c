#include "App_Task.h"

/* 0. �������� */
void startTask(void *args);
#define START_TASK_NAME "start_task"
#define START_TASK_STACK 128
#define START_TASK_PRIORITY 10
TaskHandle_t startTaskHandle;

/*1.ͨѶ����*/
void communicationTask(void *args);
#define COMMUNICATION_TASK_NAME "communication_Task"
#define COMMUNICATION_TASK_STACK 256
#define COMMUNICATION_TASK_PRIORITY 8
TaskHandle_t communicationTaskHandle;
#define COMMUNICATION_EXEC_CYCLE 6

/*2.��������*/
void keyTask(void *args);
#define KEY_TASK_NAME "key_Task"
#define KEY_TASK_STACK 256
#define KEY_TASK_PRIORITY 7
TaskHandle_t keyTaskHandle;
#define KEY_EXEC_CYCLE 50

/*3.ҡ������*/
void joystickTask(void *args);
#define Joy_Stick_TASK_NAME "joystick_Task"
#define Joy_Stick_TASK_STACK 256
#define Joy_Stick_TASK_PRIORITY 7
TaskHandle_t joystickTaskHandle;
#define Joy_Stick_EXEC_CYCLE 10

/*4.��ʾ����*/
void displayTask(void *args);
#define Display_TASK_NAME "diaplay_Task"
#define Display_TASK_STACK 256
#define Display_TASK_PRIORITY 7
TaskHandle_t displayTaskHandle;
#define Display_EXEC_CYCLE 7

/**
 * @description: ����ʵʱϵͳ
 * @return {*}
 */
void App_Task_FreeRTOSStart(void)
{
    /*1.��ʼ��debugģ��*/
    debug_start();
	
		debug_printfln("���˻�--ң�س�ʼ����ʼ");
	
    /*2.����ͨѶģ��*/
		App_Communication_Start();
	
    /*3.�������ݴ���ģ��*/
		App_DataProcess_Start();

    /*4.������ʾģ��*/
		App_Display_Start();
	
    /* 1. ����һ����������: ��������ִ�е�ʱ��, ������ҵ����ص����� */
    BaseType_t r = xTaskCreate(startTask,        /* ���������� */
                               START_TASK_NAME,  /* ������ */
                               START_TASK_STACK, /* ջ��С: ��λ����(�����Լ�) �ֵĴ�С: 32λ�ܹ���4  START_TASK_STACK * 4 = �ֽ���  */
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
	
		/*1.����ͨѶ����*/
		xTaskCreate(communicationTask,
								COMMUNICATION_TASK_NAME,
								COMMUNICATION_TASK_STACK,
								NULL,
								COMMUNICATION_TASK_PRIORITY,
								&communicationTaskHandle);
								
		/*2.��������ɨ������*/
		xTaskCreate(keyTask,
								KEY_TASK_NAME,
								KEY_TASK_STACK,
								NULL,
								KEY_TASK_PRIORITY,
								&keyTaskHandle);
	
		/*3.����ҡ��ɨ������*/
		xTaskCreate(joystickTask,
								Joy_Stick_TASK_NAME,
								Joy_Stick_TASK_STACK,
								NULL,
								Joy_Stick_TASK_PRIORITY,
								&joystickTaskHandle);
								
		/*4.������ʾ����*/
		xTaskCreate(displayTask,
								Display_TASK_NAME,
								Display_TASK_STACK,
								NULL,
								Display_TASK_PRIORITY,
								&displayTaskHandle);
								
    /* ɾ���Լ� */
    vTaskDelete(NULL);
    debug_printfln("����ִ��");
}


/*1.ͨѶ����*/
void communicationTask(void *args)
{
	vTaskDelay(1000);//ͨѶ��ɿ����ȼ���ͬ��������ʱԤ������
	debug_printfln("ͨѶ����ʼ����");
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
//			debug_printfln ("���ͳɹ�");
//		}
//		else
//		{
//			debug_printfln ("����ʧ��");
//		}
		App_Communication_SendJoyStickData();
		vTaskDelayUntil(&pretime,COMMUNICATION_EXEC_CYCLE);
	}
}

/*2.����ɨ������*/
void keyTask(void *args)
{
	vTaskDelay(300);//��ֹ��ҡ�������ͻ
	debug_printfln("����ɨ������ʼ����");
	uint32_t pretime=xTaskGetTickCount();
	while(1)
	{
//		Com_Key KEY=Inf_JoyStickAndKey_KeyScan();
//		if(KEY==KEY_LEFT)
//		{
//			debug_printfln("�������");			
//		}
//		else if(KEY==KEY_RIGHT)
//		{
//			debug_printfln("�Ҽ�����");			
//		}
//		else if(KEY==KEY_UP)
//		{
//			debug_printfln("�ϼ�����");			
//		}
//		else if(KEY==KEY_DOWN)
//		{
//			debug_printfln("�¼�����");			
//		}
//		else if(KEY==KEY_LEFT_TOP)
//		{
//			debug_printfln("���ϼ�����");			
//		}
//		else if(KEY==KEY_RIGHT_TOP)
//		{
//			debug_printfln("���ϼ�����");			
//		}
//		else if(KEY==KEY_LEFT_TOP_LONG)
//		{
//			debug_printfln("���ϼ�����");			
//		}
//		else if(KEY==KEY_RIGHT_TOP_LONG)
//		{
//			debug_printfln("���ϼ�����");			
//		}
		App_DataProcess_KeyDataProcess();
		vTaskDelayUntil(&pretime,KEY_EXEC_CYCLE);		
	}
}

/*3.ҡ��ɨ������*/
void joystickTask(void *args)
{
	vTaskDelay(500);//��ֹ�밴�������ͻ
	debug_printfln("ҡ��ɨ������ʼ����");
	uint32_t pretime=xTaskGetTickCount();	
	while(1)
	{
		App_DataProcess_JoyStickDataProcess();
		vTaskDelayUntil(&pretime,Joy_Stick_EXEC_CYCLE);
//		Com_Config_PrintJoyStick("1");
	}
}

/*4.��ʾ����*/
void displayTask(void *args)
{
	debug_printfln("��ʾ����ʼ����");
	uint32_t pretime=xTaskGetTickCount();	
	while(1)
	{
		App_Display_Show();
		vTaskDelayUntil(&pretime,Display_EXEC_CYCLE);
	}
}
