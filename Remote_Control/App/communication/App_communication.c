#include "App_communication.h"

/**
  * @brief ����ͨѶģ��
  * @param 
  * @retval 
  */
void App_Communication_Start(void)
{
	debug_printfln("ͨѶģ�������--��ʼ");
	
	debug_printfln("2.4Gģ���Լ�--��ʼ");
	while(Inf_Si24R1_Check()==1)
	{
		HAL_Delay(500);
	}
	debug_printfln("2.4Gģ���Լ���%d",Inf_Si24R1_Check());
	debug_printfln("2.4Gģ���Լ�--����");
	
	debug_printfln("2.4G����Ϊ����ģʽ");
	Inf_Si24R1_TXMode();
	
	debug_printfln("ͨѶģ�������--����");
}

/**
  * @brief ͨ��2.4g����ҡ������
	*ǰ�����ֽڣ�0x11,0x22,0x33,֡ͷ
	*1���ֽڣ����ݳ���=10
	*10���ֽڣ�ҡ������
	*4���ֽڣ�У���
  * @param 
  * @retval 
  */
void App_Communication_SendJoyStickData(void)
{
	uint8_t index=0;
	//����֡ͷ	
	TX_BUFF[index++]=FRAME_0;
	TX_BUFF[index++]=FRAME_1;
	TX_BUFF[index++]=FRAME_2;
	
	//�������ݳ��ȣ��������޸�
	TX_BUFF[index++]=0;
	
	//ҡ������
	TX_BUFF[index++]=JoyStick.THR>>8;
	TX_BUFF[index++]=JoyStick.THR;
	
	TX_BUFF[index++]=JoyStick.YAW>>8;
	TX_BUFF[index++]=JoyStick.YAW;
	
	TX_BUFF[index++]=JoyStick.PIT>>8;
	TX_BUFF[index++]=JoyStick.PIT;
	
	TX_BUFF[index++]=JoyStick.ROL>>8;
	TX_BUFF[index++]=JoyStick.ROL;
	
	TX_BUFF[index++]=JoyStick.isPowerDown;
	JoyStick.isPowerDown=0;								//����ػ�����
	
	TX_BUFF[index++]=JoyStick.isFixHeight;
	JoyStick.isFixHeight=0;								//���ܷ��յ�1��ȡ������
	
	TX_BUFF[3]=index-4;
		
	//����У���
	uint32_t sum=0;
	for(uint8_t i=0;i<index;i++)
	{
		sum+=TX_BUFF[i];
	}
	TX_BUFF[index++]=sum>24;
	TX_BUFF[index++]=sum>16;
	TX_BUFF[index++]=sum>8;
	TX_BUFF[index++]=sum;
	
	taskENTER_CRITICAL();//�����ٽ�������ֹ�������ݱ����
	Inf_Si24R1_TxPacket(TX_BUFF);
	taskEXIT_CRITICAL();
}
