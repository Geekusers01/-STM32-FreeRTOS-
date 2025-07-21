#include "Myi2c.h"

// 延时函数，微秒作为单位，利用系统嘀嗒定时器，72MHz，一次嘀嗒 1/72 us
void Delay_us(uint32_t us) {
    uint32_t start = SysTick->VAL;
    uint32_t cycles = us * (SystemCoreClock / 1000000);
    while ((start - SysTick->VAL) < cycles);
}

void Delay_ms(uint16_t ms)
{
	while (ms--)
	{
		Delay_us(1000);
	}
}

// 初始化
void I2C_Init(void)
{
    // 1. 配置时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // 2. GPIO工作模式配置：通用开漏输出 CNF-01，MODE-11
    GPIOB->CRH |= (GPIO_CRL_MODE6 | GPIO_CRL_MODE7);
    GPIOB->CRH &= ~(GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_1);
    GPIOB->CRH |= (GPIO_CRL_CNF6_0 | GPIO_CRL_CNF7_0);
}

// 发出起始信号
void I2C_Start(void)
{
    // 1. SCL拉高，SDA拉高
    SCL_HIGH;
    SDA_HIGH;
    I2C_DELAY;

    // 2. SCL保持不变，SDA拉低
    SDA_LOW;
    I2C_DELAY;
}

// 发出停止信号
void I2C_Stop(void)
{
    // 1. SCL拉高，SDA拉低
    SCL_HIGH;
    SDA_LOW;
    I2C_DELAY;

    // 2. SCL保持不变，SDA拉高
    SDA_HIGH;
    I2C_DELAY;
}

// 主机发出应答信号
void I2C_Ack(void)
{
    // 1. SCL拉低，SDA拉高，准备发出信号
    SCL_LOW;
    SDA_HIGH;
    I2C_DELAY;

    // 2. SCL保持不变，SDA拉低，输出应答
    SDA_LOW;
    I2C_DELAY;

    // 3. SDA保持不变，SCL拉高，开始数据线上信号采样
    SCL_HIGH;
    I2C_DELAY;

    // 4. SDA保持不变，SCL拉低，结束数据线上信号采样
    SCL_LOW;
    I2C_DELAY;

    // 5. SDA拉高，释放数据总线
    SDA_HIGH;
    I2C_DELAY;
}

// 主机发出非应答信号
void I2C_Nack(void)
{
    // 1. SCL拉低，SDA拉高，准备发出信号
    SCL_LOW;
    SDA_HIGH;
    I2C_DELAY;

    // 2. SDA保持不变，SCL拉高，开始数据线上信号采样
    SCL_HIGH;
    I2C_DELAY;

    // 3. SDA保持不变，SCL拉低，结束数据线上信号采样
    SCL_LOW;
    I2C_DELAY;
}

// 主机等待从设备发来应答信号
uint8_t I2C_Wait4Ack(void)
{
    // 1. SCL拉低，SDA拉高，释放数据总线
    SCL_LOW;
    SDA_HIGH;
    I2C_DELAY;

    // 2. SCL拉高，开始数据采样
    SCL_HIGH;
    I2C_DELAY;

    // 3. 读取SDA数据线上的电平
    uint16_t ack = READ_SDA;

    // 4. SCL拉低，结束数据采样
    SCL_LOW;
    I2C_DELAY;

    return ack ? NACK : ACK;
}

// 主机发送一个字节的数据（写入）
void I2C_SendByte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        // 1. SCL、SDA都拉低，等待数据翻转
        SCL_LOW;
        SDA_LOW;
        I2C_DELAY;

        // 2. 取字节的最高位，向SDA写入数据
        if (byte & 0x80)
        {
            SDA_HIGH;
        }
        else
        {
            SDA_LOW;
        }
        I2C_DELAY;

        // 3. SCL拉高，数据采样
        SCL_HIGH;
        I2C_DELAY;

        // 4. SCL拉低，采样结束
        SCL_LOW;
        I2C_DELAY;

        // 5. 左移1位
        byte <<= 1;
    }  
}

// 主机从EEPROM接收一个字节的数据（读取）
uint8_t I2C_ReadByte(void)
{
    // 定义一个变量，用来保存接收的数据
    uint8_t data = 0;

    // 循环处理每一位
    for (uint8_t i = 0; i < 8; i++)
    {
        // 1. SCL拉低，等待数据翻转
        SCL_LOW;
        I2C_DELAY;

        // 2. SCL拉高，开始采样
        SCL_HIGH;
        I2C_DELAY;

        // 3. 数据采样，读取SDA
        data <<= 1;     // 先做左移，新存入的位永远在最低位
        if (READ_SDA)
        {
            data |= 0x01;   // 先存入最低位，然后每次都左移1位
        }

        // 4. SCL拉低，结束采样
        SCL_LOW;
        I2C_DELAY;
    }
    
    return data;
}

