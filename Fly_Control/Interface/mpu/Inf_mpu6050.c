#include "Inf_mpu6050.h"

#define abs(x) ((x)>0?(x):(-x))
static GyroAccel_Struct offsetGyroAccel;
void Inf_MPU6050_Calibrate(void);

/**
 * @description: ��ָ���ļĴ���д��һ���ֽ�
 * @param {uint8_t} reg �Ĵ�����ַ
 * @param {uint8_t} byte д����ֽ�����
 * @return {*}
 */
void Inf_MPU6050_WriteReg(uint8_t reg, uint8_t byte)
{
    HAL_I2C_Mem_Write(&hi2c1,
                      MPU6050_ADDR_W,
                      reg,
                      I2C_MEMADD_SIZE_8BIT,
                      &byte,
                      1,
                      2000);
}

/**
 * @description: ��ָ���ļĴ���startRegд�����ֽ�
 * @param {uint8_t} startReg ��ʼ�Ĵ�����ַ
 * @param {uint8_t} *bytes д����ֽ�����
 * @param {uint8_t} len д����ٸ��ֽ�
 * @return {*}
 */
void Inf_MPU6050_WriteRegs(uint8_t startReg, uint8_t *bytes, uint8_t len)
{
    HAL_I2C_Mem_Write(&hi2c1,
                      MPU6050_ADDR_W,
                      startReg,
                      I2C_MEMADD_SIZE_8BIT,
                      bytes,
                      len,
                      2000);
}

/**
 * @description: ��ָ���ļĴ�����ȡһ���ֽ�
 * @param {uint8_t} reg �Ĵ�����ַ
 * @return {*}
 */
uint8_t Inf_MPU6050_ReadReg(uint8_t reg)
{
    uint8_t rByte = 0;
    HAL_I2C_Mem_Read(&hi2c1,
                     MPU6050_ADDR_R,
                     reg,
                     I2C_MEMADD_SIZE_8BIT,
                     &rByte,
                     1,
                     2000);
    return rByte;
}

/**
 * @description: ��ָ���ļĴ�����ȡ����ֽ�
 * @param {uint8_t} startReg  ��ʼ�Ĵ�����ַ
 * @param {uint8_t} *bytes ��ȡ���ֽ�����
 * @param {uint8_t} len ��ȡ���ٸ��ֽ�
 * @return {*}
 */
void Inf_MPU6050_ReadRegs(uint8_t startReg, uint8_t *bytes, uint8_t len)
{
    HAL_I2C_Mem_Read(&hi2c1,
                     MPU6050_ADDR_R,
                     startReg,
                     I2C_MEMADD_SIZE_8BIT,
                     bytes,
                     len,
                     2000);
}

/**
  * @brief mpu6050��ʼ��
  * @param 
  * @retval 
  */
void Inf_MPU6050_Init(void)
{
	//1.��λmpu6050����λ->����200ms->���� 
	Inf_MPU6050_WriteReg(MPU_PWR_MGMT1_REG,1<<7);
	HAL_Delay(200);
	Inf_MPU6050_WriteReg(MPU_PWR_MGMT1_REG,0);	
	
	//2.��������
	//2.1���ٶ�����+-2000��ÿ��
	Inf_MPU6050_WriteReg(MPU_GYRO_CFG_REG,3<<3);	
	
	//2.2���ٶ�����+2g
	Inf_MPU6050_WriteReg(MPU_ACCEL_CFG_REG,0);	
	
	//3.�ر��ж� �رյڶ�i2c�ӿ� ����FIFO
	Inf_MPU6050_WriteReg(MPU_INT_EN_REG,0);	
	Inf_MPU6050_WriteReg(MPU_USER_CTRL_REG,0);	
	Inf_MPU6050_WriteReg(MPU_FIFO_EN_REG,0);	
	
	/*4.���������ǲ����ʺ͵�ͨ�˲���
		��ͨ�˲���:�������������1kHz
		������:�����ʶ���ʱ������ȷ��Ӧģ���ź�
		��ũ����:������>=2*�ź�����ߵ�Ƶ��
		����Ƶ��=���������Ƶ��/(1+SMPLRT_DIV)
		500Hz=1000Hz/(1+x)
	*/
	Inf_MPU6050_WriteReg(MPU_SAMPLE_RATE_REG,1);		
	Inf_MPU6050_WriteReg(MPU_CFG_REG,1<<0);	
	
	//5.����ϵͳʱ��Դ
	Inf_MPU6050_WriteReg(MPU_PWR_MGMT1_REG,1);	
	
	//6.ʹ�ܽ��ٶȴ������ͼ��ٶȴ����� ���ô���ģʽ
	Inf_MPU6050_WriteReg(MPU_PWR_MGMT2_REG,0);	
	
	//7.У׼6������
	Inf_MPU6050_Calibrate();
}

/**
  * @brief ��ȡ���ٶ�
  * @param Gyro_Struct *Gyro ���ٶȽṹ��
  * @retval 
  */
void Inf_MPU6050_ReadGyro(Gyro_Struct *Gyro)
{
	uint8_t data[6]={0};
	Inf_MPU6050_ReadRegs(MPU_GYRO_XOUTH_REG,data,6);
	Gyro->GyroX=(int16_t)(data[0]<<8|data[1]);
	Gyro->GyroY=(int16_t)(data[2]<<8|data[3]);
	Gyro->GyroZ=(int16_t)(data[4]<<8|data[5]);
}

/**
  * @brief ��ȡ���ٶ�
  * @param Accel_Struct *Accel ���ٶȽṹ��
  * @retval 
  */
void Inf_MPU6050_ReadAccel(Accel_Struct *Accel)
{
	uint8_t data[6]={0};
	Inf_MPU6050_ReadRegs(MPU_ACCEL_XOUTH_REG,data,6);
	Accel->AccelX=(int16_t)(data[0]<<8|data[1]);
	Accel->AccelY=(int16_t)(data[2]<<8|data[3]);
	Accel->AccelZ=(int16_t)(data[4]<<8|data[5]);	
}

/**
  * @brief ��ȡ���ٶȺͽ��ٶ�
  * @param GyroAccel_Struct *GyroAccel ���ٶȺͽ��ٶȽṹ��
  * @retval 
  */
void Inf_MPU6050_ReadGyroAccel(GyroAccel_Struct *GyroAccel)
{
	Inf_MPU6050_ReadGyro(&GyroAccel->Gyro);
	Inf_MPU6050_ReadAccel(&GyroAccel->Accel);
}

/**
  * @brief ��ȡУ׼����ٶȺͽ��ٶ�
  * @param GyroAccel_Struct *GyroAccel ���ٶȺͽ��ٶȽṹ��
  * @retval 
  */
void Inf_MPU6050_ReadGyroAccelCalibrated(GyroAccel_Struct *GyroAccel)
{
	//1.��ȡԭʼ����
	Inf_MPU6050_ReadGyroAccel(GyroAccel);
	//Com_Config_PrintGyroAccel("У׼ǰ",GyroAccel);
	
	//2.��ȥУ׼ֵ
	GyroAccel->Gyro.GyroX-=offsetGyroAccel.Gyro.GyroX;
	GyroAccel->Gyro.GyroY-=offsetGyroAccel.Gyro.GyroY;
	GyroAccel->Gyro.GyroZ-=offsetGyroAccel.Gyro.GyroZ;
	GyroAccel->Accel.AccelX-=offsetGyroAccel.Accel.AccelX;
	GyroAccel->Accel.AccelY-=offsetGyroAccel.Accel.AccelY;
	GyroAccel->Accel.AccelZ-=offsetGyroAccel.Accel.AccelZ;
	
	//Com_Config_PrintGyroAccel("У׼��",GyroAccel);
}

/**
  * @brief ��ˮƽ״̬��У׼6������
	*1.ȷ�Ϸɻ����ھ�ֹ����β������ٶȣ��������ϴζԱȣ�����仯С����ֵ������Ϊ��ֹ
	*2.��β�������ƫ��
	* @param 
  * @retval 
  */
void Inf_MPU6050_Calibrate(void)
{
    /* 1.ȷ�Ͼ�ֹ  */
    uint8_t cnt = 30;
    GyroAccel_Struct current;
    GyroAccel_Struct last;
    debug_printfln(" ������: �ȴ���ֹ");
    while(cnt)
    {
			Inf_MPU6050_ReadGyroAccel(&current);
			if(abs(current.Gyro.GyroX - last.Gyro.GyroX) <= 60 &&
				 abs(current.Gyro.GyroY - last.Gyro.GyroY) <= 60 &&
				 abs(current.Gyro.GyroZ - last.Gyro.GyroZ) <= 60)
			{
					cnt--;
			}
			last = current; /* �ϴεĽṹ��ִ����� */
			HAL_Delay(3);
    }
    debug_printfln(" ������: �Ѿ���ֹ");

    debug_printfln(" ������: ��ʼУ׼");
    int32_t  sumBuff[6] = {0};
    uint16_t sumCount   = 255;
    for(uint16_t i = 0; i < sumCount; i++)
    {
			Inf_MPU6050_ReadGyroAccel(&GyroAccel);
			sumBuff[0] += GyroAccel.Gyro.GyroX - 0;
			sumBuff[1] += GyroAccel.Gyro.GyroY - 0;
			sumBuff[2] += GyroAccel.Gyro.GyroZ - 0;
			sumBuff[3] += GyroAccel.Accel.AccelX - 0;
			sumBuff[4] += GyroAccel.Accel.AccelY - 0;
			sumBuff[5] += GyroAccel.Accel.AccelZ - 16383;

			HAL_Delay(3);
    }
    offsetGyroAccel.Gyro.GyroX   = sumBuff[0] / sumCount;
    offsetGyroAccel.Gyro.GyroY   = sumBuff[1] / sumCount;
    offsetGyroAccel.Gyro.GyroZ   = sumBuff[2] / sumCount;
    offsetGyroAccel.Accel.AccelX = sumBuff[3] / sumCount;
    offsetGyroAccel.Accel.AccelY = sumBuff[4] / sumCount;
    offsetGyroAccel.Accel.AccelZ = sumBuff[5] / sumCount;

    Com_Config_PrintGyroAccel("������ƫ��ֵ", &offsetGyroAccel);
    debug_printfln(" ������: ����У׼");
}

