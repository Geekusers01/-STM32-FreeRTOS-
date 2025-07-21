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
	debug_printfln("2.4Gģ���Լ�--����");
	
	debug_printfln("2.4G����Ϊ����ģʽ");
	Inf_Si24R1_RXMode();
	
	debug_printfln("ͨѶģ�������--����");
}

/**
  * @brief 2.4g��������
  * @param 
  * @retval �������ֵΪCom_OK�ɹ�����,�������ʧ��
  */
Com_Status App_Communication_ReceiveJoyStickData(void)
{
	//1.�ж��Ƿ��յ�����,
	taskENTER_CRITICAL();//�����ٽ�������ֹ�������ݱ����
	uint8_t r=Inf_Si24R1_RxPacket(RX_BUFF);
	taskEXIT_CRITICAL();	
	if(r==1) return Com_FAIL;
	//2.�ж��յ���֡ͷ�Ƿ����
	if(RX_BUFF[0]!=FRAME_0||
			RX_BUFF[1]!=FRAME_1||
			RX_BUFF[2]!=FRAME_2)
		return Com_FAIL;
	//3.�յ�����������
	JoyStick.THR=(RX_BUFF[4]<<8)|RX_BUFF[5];
	JoyStick.YAW=(RX_BUFF[6]<<8)|RX_BUFF[7];
	JoyStick.PIT=(RX_BUFF[8]<<8)|RX_BUFF[9];
	JoyStick.ROL=(RX_BUFF[10]<<8)|RX_BUFF[11];
	
	JoyStick.isPowerDown=RX_BUFF[12];
	
	//��RX_BUFF[13]Ϊ1�ҽ���ʱ���л�����ģʽ
	if(RX_BUFF[13]&&(App_Communication_RemoteUnlock(isRemoteConnected)==Com_OK))
	{
		JoyStick.isFixHeight=!JoyStick.isFixHeight;	
		isFixHeight=JoyStick.isFixHeight==1?Com_OK:Com_FAIL;
	}
	//���Խ�������
//	Com_Config_PrintJoyStick("1");	
	return Com_OK;
}

/**
  * @brief �������ң�������������
	*���ӳɹ���
	*				ֻҪ�յ�һ�����ݣ��������ӳɹ�
	*ʧ����
	*				�������1.2sû���յ����ݣ���ʾʧ��
  * @param �Ƿ��յ�ң��������
  * @retval �Ƿ����ӳɹ�
  */
Com_Status App_Communication_CheckConnection(Com_Status isReceiveData)
{//��¼�����ղ������ݴ���
	static uint8_t cnt=200;//��ֹ����ʱû�����ӳɹ���ʾ���ӳɹ����
	//1.����յ����ݣ���ʾ���ӳɹ�
	if(isReceiveData==Com_OK)	
	{
		cnt=0;
		return Com_OK;
	}
	//2.û���յ������ǣ�������ʱ1.2s��200��
	cnt++;
	if(cnt>=200)
	{
		cnt=200;//���ⳬ����Χ
		return Com_FAIL;
	}
	return Com_OK;
}

/**
  * @brief ң��������
	*������������߳���1.2s��Ȼ��������ͳ���1.2s�������ɹ�
	*����״̬�������״̬�������������>=960,�������ֵ״̬
	*���ֵ״̬��������ų���1.2s�������뿪���ֵ״̬���������ʱ�䲻��1.2s���ص�����״̬
	*�뿪���ֵ״̬�������������<=20,������Сֵ״̬
	*��Сֵ״̬��������ų���1.2s���������״̬���������ʱ�䲻��1.2s���ص�����״̬
	*����״̬���������<=20����1min,��������״̬
  * @param �Ƿ����ӳɹ�
  * @retval �Ƿ�����ɹ���Com_OKΪ�����ɹ�������Ϊ����ʧ��
  */
Com_Status App_Communication_RemoteUnlock(Com_Status isRemoteConnected)
{
	static uint8_t thrMAXDuration=0;
	static uint8_t thrMINDuration=0;
	//�������ŵ�״̬
	static Com_RemoteStatus remoteStatus=THR_FREE;
	//debug_printfln("%d",remoteStatus);
	
	//1.���ʧ��״̬��ֱ�ӽ���ʧ��
	if(isRemoteConnected!=Com_OK)
	{
		remoteStatus=THR_FREE;
		JoyStick.isFixHeight=0;
		isFixHeight=Com_FAIL;
		return Com_FAIL;		
	}
		
	switch (remoteStatus)
	{
		case THR_FREE:
		{//����״̬���������ֵ����Сֵ����ʱ��������
			thrMAXDuration=0;
			thrMINDuration=0;
			if(JoyStick.THR>=960)
			{
				remoteStatus=THR_MAX;
			}
			break;
		}
		case THR_MAX:
		{
			if(JoyStick.THR>=960)
			{
				//�жϳ���ʱ��
				thrMAXDuration++;
				if(thrMAXDuration>=200)
				{
					remoteStatus=THR_MAX_LEAVE;
				}
			}
			else
			{
				//û�г������ֵ1.2s
				remoteStatus=THR_FREE;
			}
			break;
		}
		case THR_MAX_LEAVE:
		{
			if(JoyStick.THR<=20)
			{
				remoteStatus=THR_MIN;
			}
			break;
		}
		case THR_MIN:
		{
			if(JoyStick.THR<=20)
			{
				thrMINDuration++;
				if(thrMINDuration>=200)
				{
					remoteStatus=THR_UNLOCK;
				}
			}
			else
			{
				remoteStatus=THR_FREE;
			}				
			break;
		}
		case THR_UNLOCK:
		{
			//�������״̬�£����ų�ʱ��12s����<=20,������ҡ�������������ָ�����
			static uint32_t lowDuration=0;
			if(JoyStick.THR<=20)
			{
				if(JoyStick.YAW>=600)
				{
					remoteStatus=THR_FREE;										
				}
				lowDuration++;
				if(lowDuration>=200*10)
				{
					remoteStatus=THR_FREE;					
					lowDuration=0;				
				}
			}
			else
			{
				lowDuration=0;				
			}
			//�����ɹ���ֱ�ӷ��سɹ�
			return Com_OK;			
		}
		default :
			break;
	}
	JoyStick.isFixHeight=0;
	isFixHeight=Com_FAIL;
	//Ĭ�Ϸ���ʧ��
	return Com_FAIL;			
}
