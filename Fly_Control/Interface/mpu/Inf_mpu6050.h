#ifndef __INF_MPU6050_H__
#define __INF_MPU6050_H__
#include "Com_Debug.h"
#include "i2c.h"
#include "Com_Config.h"

#define MPU6050_ADDR (0x68)
#define MPU6050_ADDR_W (0x68<<1)
#define MPU6050_ADDR_R ((0x68<<1)+1)

#define MPU_SELF_TESTX_REG 0X0D 	 // �Լ�Ĵ���X
#define MPU_SELF_TESTY_REG 0X0E 	 // �Լ�Ĵ���Y
#define MPU_SELF_TESTZ_REG 0X0F 	 // �Լ�Ĵ���Z
#define MPU_SELF_TESTA_REG 0X10 	 // �Լ�Ĵ���A
#define MPU_SAMPLE_RATE_REG 0X19 	 // ����Ƶ�ʷ�Ƶ��
#define MPU_CFG_REG 0X1A 					 // ���üĴ���
#define MPU_GYRO_CFG_REG 0X1B 		 // ���������üĴ���
#define MPU_ACCEL_CFG_REG 0X1C 		 // ���ٶȼ����üĴ���
#define MPU_MOTION_DET_REG 0X1F 	 // �˶���ֵⷧ���üĴ���
#define MPU_FIFO_EN_REG 0X23 			 // FIFOʹ�ܼĴ���
#define MPU_I2CMST_CTRL_REG 0X24 	 // IIC�������ƼĴ��� 
#define MPU_I2CSLV0_ADDR_REG 0X25  // IIC�ӻ�0������ַ�Ĵ���
#define MPU_I2CSLV0_REG 0X26 			 // IIC�ӻ�0���ݵ�ַ�Ĵ���
#define MPU_I2CSLV0_CTRL_REG 0X27  // IIC�ӻ�0���ƼĴ���
#define MPU_I2CSLV1_ADDR_REG 0X28  // IIC�ӻ�1������ַ�Ĵ���
#define MPU_I2CSLV1_REG 0X29 			 // IIC�ӻ�1���ݵ�ַ�Ĵ���
#define MPU_I2CSLV1_CTRL_REG 0X2A  // IIC�ӻ�1���ƼĴ���
#define MPU_I2CSLV2_ADDR_REG 0X2B  // IIC�ӻ�2������ַ�Ĵ���
#define MPU_I2CSLV2_REG 0X2C 			 // IIC�ӻ�2���ݵ�ַ�Ĵ���
#define MPU_I2CSLV2_CTRL_REG 0X2D	 // IIC�ӻ�2���ƼĴ���
#define MPU_I2CSLV3_ADDR_REG 0X2E  // IIC�ӻ�3������ַ�Ĵ���
#define MPU_I2CSLV3_REG 0X2F 			 // IIC�ӻ�3���ݵ�ַ�Ĵ���
#define MPU_I2CSLV3_CTRL_REG 0X30  // IIC�ӻ�3���ƼĴ���
#define MPU_I2CSLV4_ADDR_REG 0X31  // IIC�ӻ�4������ַ�Ĵ���
#define MPU_I2CSLV4_REG 0X32 			 // IIC�ӻ�4���ݵ�ַ�Ĵ���
#define MPU_I2CSLV4_DO_REG 0X33 	 // IIC�ӻ�4д���ݼĴ���
#define MPU_I2CSLV4_CTRL_REG 0X34  // IIC�ӻ�4���ƼĴ���
#define MPU_I2CSLV4_DI_REG 0X35 	 // IIC�ӻ�4�����ݼĴ���

#define MPU_I2CMST_STA_REG 0X36 	 // IIC����״̬�Ĵ���
#define MPU_INTBP_CFG_REG 0X37 		 // �ж�/��·���üĴ���
#define MPU_INT_EN_REG 0X38 	 		 // �ж�ʹ�ܼĴ���
#define MPU_INT_STA_REG 0X3A 			 // �ж�״̬�Ĵ���

#define MPU_ACCEL_XOUTH_REG 0X3B 	 // ���ٶ�ֵ,X���8λ�Ĵ���
#define MPU_ACCEL_XOUTL_REG 0X3C 	 // ���ٶ�ֵ,X���8λ�Ĵ���
#define MPU_ACCEL_YOUTH_REG 0X3D 	 // ���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU_ACCEL_YOUTL_REG 0X3E 	 // ���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU_ACCEL_ZOUTH_REG 0X3F 	 // ���ٶ�ֵ,Z���8λ�Ĵ���
#define MPU_ACCEL_ZOUTL_REG 0X40 	 // ���ٶ�ֵ,Z���8λ�Ĵ���

#define MPU_TEMP_OUTH_REG 0X41 	 	 // �¶�ֵ�߰�λ�Ĵ���
#define MPU_TEMP_OUTL_REG 0X42 	 	 // �¶�ֵ��8λ�Ĵ���
																	 
#define MPU_GYRO_XOUTH_REG 0X43  	 // ������ֵ,X���8λ�Ĵ���
#define MPU_GYRO_XOUTL_REG 0X44  	 // ������ֵ,X���8λ�Ĵ���
#define MPU_GYRO_YOUTH_REG 0X45  	 // ������ֵ,Y���8λ�Ĵ���
#define MPU_GYRO_YOUTL_REG 0X46  	 // ������ֵ,Y���8λ�Ĵ���
#define MPU_GYRO_ZOUTH_REG 0X47  	 // ������ֵ,Z���8λ�Ĵ���
#define MPU_GYRO_ZOUTL_REG 0X48  	 // ������ֵ,Z���8λ�Ĵ���
																	 
#define MPU_I2CSLV0_DO_REG 0X63  	 // IIC�ӻ�0���ݼĴ���
#define MPU_I2CSLV1_DO_REG 0X64  	 // IIC�ӻ�1���ݼĴ���
#define MPU_I2CSLV2_DO_REG 0X65  	 // IIC�ӻ�2���ݼĴ���
#define MPU_I2CSLV3_DO_REG 0X66  	 // IIC�ӻ�3���ݼĴ���

#define MPU_I2CMST_DELAY_REG 0X67	 // IIC������ʱ�����Ĵ���
#define MPU_SIGPATH_RST_REG 0X68 	 // �ź�ͨ����λ�Ĵ���
#define MPU_MDETECT_CTRL_REG 0X69	 // �˶������ƼĴ���
#define MPU_USER_CTRL_REG 0X6A 	 	 // �û����ƼĴ���
#define MPU_PWR_MGMT1_REG 0X6B 	 	 // ��Դ�����Ĵ���1
#define MPU_PWR_MGMT2_REG 0X6C 	 	 // ��Դ�����Ĵ���2
#define MPU_FIFO_CNTH_REG 0X72 	 	 // FIFO�����Ĵ����߰�λ
#define MPU_FIFO_CNTL_REG 0X73 	 	 // FIFO�����Ĵ����Ͱ�λ
#define MPU_FIFO_RW_REG 0X74  	 	 // FIFO��д�Ĵ���
#define MPU_DEVICE_ID_REG 0X75 	 	 // ����ID�Ĵ���

void Inf_MPU6050_Init(void);
void Inf_MPU6050_ReadGyroAccel(GyroAccel_Struct *GyroAccel);
void Inf_MPU6050_ReadGyroAccelCalibrated(GyroAccel_Struct *GyroAccel);

#endif
