#include "Inf_mpu6050.h"

#define abs(x) ((x)>0?(x):(-x))
static GyroAccel_Struct offsetGyroAccel;
void Inf_MPU6050_Calibrate(void);

/**
 * @description: 向指定的寄存器写入一个字节
 * @param {uint8_t} reg 寄存器地址
 * @param {uint8_t} byte 写入的字节数据
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
 * @description: 从指定的寄存器startReg写入多个字节
 * @param {uint8_t} startReg 开始寄存器地址
 * @param {uint8_t} *bytes 写入的字节数据
 * @param {uint8_t} len 写入多少个字节
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
 * @description: 从指定的寄存器读取一个字节
 * @param {uint8_t} reg 寄存器地址
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
 * @description: 从指定的寄存器读取多个字节
 * @param {uint8_t} startReg  开始寄存器地址
 * @param {uint8_t} *bytes 读取的字节数据
 * @param {uint8_t} len 读取多少个字节
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
  * @brief mpu6050初始化
  * @param 
  * @retval 
  */
void Inf_MPU6050_Init(void)
{
	//1.复位mpu6050：复位->休眠200ms->唤醒 
	Inf_MPU6050_WriteReg(MPU_PWR_MGMT1_REG,1<<7);
	HAL_Delay(200);
	Inf_MPU6050_WriteReg(MPU_PWR_MGMT1_REG,0);	
	
	//2.设置量程
	//2.1角速度量程+-2000度每秒
	Inf_MPU6050_WriteReg(MPU_GYRO_CFG_REG,3<<3);	
	
	//2.2加速度量程+2g
	Inf_MPU6050_WriteReg(MPU_ACCEL_CFG_REG,0);	
	
	//3.关闭中断 关闭第二i2c接口 禁用FIFO
	Inf_MPU6050_WriteReg(MPU_INT_EN_REG,0);	
	Inf_MPU6050_WriteReg(MPU_USER_CTRL_REG,0);	
	Inf_MPU6050_WriteReg(MPU_FIFO_EN_REG,0);	
	
	/*4.设置陀螺仪采样率和低通滤波器
		低通滤波器:数据输出速率是1kHz
		采样率:采样率多少时才能正确反应模拟信号
		香农定律:采样率>=2*信号中最高的频率
		采样频率=陀螺仪输出频率/(1+SMPLRT_DIV)
		500Hz=1000Hz/(1+x)
	*/
	Inf_MPU6050_WriteReg(MPU_SAMPLE_RATE_REG,1);		
	Inf_MPU6050_WriteReg(MPU_CFG_REG,1<<0);	
	
	//5.配置系统时钟源
	Inf_MPU6050_WriteReg(MPU_PWR_MGMT1_REG,1);	
	
	//6.使能角速度传感器和加速度传感器 禁用待机模式
	Inf_MPU6050_WriteReg(MPU_PWR_MGMT2_REG,0);	
	
	//7.校准6轴数据
	Inf_MPU6050_Calibrate();
}

/**
  * @brief 读取角速度
  * @param Gyro_Struct *Gyro 角速度结构体
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
  * @brief 读取加速度
  * @param Accel_Struct *Accel 加速度结构体
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
  * @brief 读取加速度和角速度
  * @param GyroAccel_Struct *GyroAccel 加速度和角速度结构体
  * @retval 
  */
void Inf_MPU6050_ReadGyroAccel(GyroAccel_Struct *GyroAccel)
{
	Inf_MPU6050_ReadGyro(&GyroAccel->Gyro);
	Inf_MPU6050_ReadAccel(&GyroAccel->Accel);
}

/**
  * @brief 读取校准后加速度和角速度
  * @param GyroAccel_Struct *GyroAccel 加速度和角速度结构体
  * @retval 
  */
void Inf_MPU6050_ReadGyroAccelCalibrated(GyroAccel_Struct *GyroAccel)
{
	//1.获取原始数据
	Inf_MPU6050_ReadGyroAccel(GyroAccel);
	//Com_Config_PrintGyroAccel("校准前",GyroAccel);
	
	//2.减去校准值
	GyroAccel->Gyro.GyroX-=offsetGyroAccel.Gyro.GyroX;
	GyroAccel->Gyro.GyroY-=offsetGyroAccel.Gyro.GyroY;
	GyroAccel->Gyro.GyroZ-=offsetGyroAccel.Gyro.GyroZ;
	GyroAccel->Accel.AccelX-=offsetGyroAccel.Accel.AccelX;
	GyroAccel->Accel.AccelY-=offsetGyroAccel.Accel.AccelY;
	GyroAccel->Accel.AccelZ-=offsetGyroAccel.Accel.AccelZ;
	
	//Com_Config_PrintGyroAccel("校准后",GyroAccel);
}

/**
  * @brief 在水平状态下校准6轴数据
	*1.确认飞机处于静止：多次测量角速度，本次与上次对比，如果变化小于阈值，则视为静止
	*2.多次测量计算偏移
	* @param 
  * @retval 
  */
void Inf_MPU6050_Calibrate(void)
{
    /* 1.确认静止  */
    uint8_t cnt = 30;
    GyroAccel_Struct current;
    GyroAccel_Struct last;
    debug_printfln(" 陀螺仪: 等待静止");
    while(cnt)
    {
			Inf_MPU6050_ReadGyroAccel(&current);
			if(abs(current.Gyro.GyroX - last.Gyro.GyroX) <= 60 &&
				 abs(current.Gyro.GyroY - last.Gyro.GyroY) <= 60 &&
				 abs(current.Gyro.GyroZ - last.Gyro.GyroZ) <= 60)
			{
					cnt--;
			}
			last = current; /* 上次的结构体执行这次 */
			HAL_Delay(3);
    }
    debug_printfln(" 陀螺仪: 已经静止");

    debug_printfln(" 陀螺仪: 开始校准");
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

    Com_Config_PrintGyroAccel("陀螺仪偏移值", &offsetGyroAccel);
    debug_printfln(" 陀螺仪: 结束校准");
}

