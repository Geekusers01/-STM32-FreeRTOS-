#include "Myi2c.h"

// ��ʱ������΢����Ϊ��λ������ϵͳ��શ�ʱ����72MHz��һ����� 1/72 us
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

// ��ʼ��
void I2C_Init(void)
{
    // 1. ����ʱ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // 2. GPIO����ģʽ���ã�ͨ�ÿ�©��� CNF-01��MODE-11
    GPIOB->CRH |= (GPIO_CRL_MODE6 | GPIO_CRL_MODE7);
    GPIOB->CRH &= ~(GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_1);
    GPIOB->CRH |= (GPIO_CRL_CNF6_0 | GPIO_CRL_CNF7_0);
}

// ������ʼ�ź�
void I2C_Start(void)
{
    // 1. SCL���ߣ�SDA����
    SCL_HIGH;
    SDA_HIGH;
    I2C_DELAY;

    // 2. SCL���ֲ��䣬SDA����
    SDA_LOW;
    I2C_DELAY;
}

// ����ֹͣ�ź�
void I2C_Stop(void)
{
    // 1. SCL���ߣ�SDA����
    SCL_HIGH;
    SDA_LOW;
    I2C_DELAY;

    // 2. SCL���ֲ��䣬SDA����
    SDA_HIGH;
    I2C_DELAY;
}

// ��������Ӧ���ź�
void I2C_Ack(void)
{
    // 1. SCL���ͣ�SDA���ߣ�׼�������ź�
    SCL_LOW;
    SDA_HIGH;
    I2C_DELAY;

    // 2. SCL���ֲ��䣬SDA���ͣ����Ӧ��
    SDA_LOW;
    I2C_DELAY;

    // 3. SDA���ֲ��䣬SCL���ߣ���ʼ���������źŲ���
    SCL_HIGH;
    I2C_DELAY;

    // 4. SDA���ֲ��䣬SCL���ͣ��������������źŲ���
    SCL_LOW;
    I2C_DELAY;

    // 5. SDA���ߣ��ͷ���������
    SDA_HIGH;
    I2C_DELAY;
}

// ����������Ӧ���ź�
void I2C_Nack(void)
{
    // 1. SCL���ͣ�SDA���ߣ�׼�������ź�
    SCL_LOW;
    SDA_HIGH;
    I2C_DELAY;

    // 2. SDA���ֲ��䣬SCL���ߣ���ʼ���������źŲ���
    SCL_HIGH;
    I2C_DELAY;

    // 3. SDA���ֲ��䣬SCL���ͣ��������������źŲ���
    SCL_LOW;
    I2C_DELAY;
}

// �����ȴ����豸����Ӧ���ź�
uint8_t I2C_Wait4Ack(void)
{
    // 1. SCL���ͣ�SDA���ߣ��ͷ���������
    SCL_LOW;
    SDA_HIGH;
    I2C_DELAY;

    // 2. SCL���ߣ���ʼ���ݲ���
    SCL_HIGH;
    I2C_DELAY;

    // 3. ��ȡSDA�������ϵĵ�ƽ
    uint16_t ack = READ_SDA;

    // 4. SCL���ͣ��������ݲ���
    SCL_LOW;
    I2C_DELAY;

    return ack ? NACK : ACK;
}

// ��������һ���ֽڵ����ݣ�д�룩
void I2C_SendByte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        // 1. SCL��SDA�����ͣ��ȴ����ݷ�ת
        SCL_LOW;
        SDA_LOW;
        I2C_DELAY;

        // 2. ȡ�ֽڵ����λ����SDAд������
        if (byte & 0x80)
        {
            SDA_HIGH;
        }
        else
        {
            SDA_LOW;
        }
        I2C_DELAY;

        // 3. SCL���ߣ����ݲ���
        SCL_HIGH;
        I2C_DELAY;

        // 4. SCL���ͣ���������
        SCL_LOW;
        I2C_DELAY;

        // 5. ����1λ
        byte <<= 1;
    }  
}

// ������EEPROM����һ���ֽڵ����ݣ���ȡ��
uint8_t I2C_ReadByte(void)
{
    // ����һ������������������յ�����
    uint8_t data = 0;

    // ѭ������ÿһλ
    for (uint8_t i = 0; i < 8; i++)
    {
        // 1. SCL���ͣ��ȴ����ݷ�ת
        SCL_LOW;
        I2C_DELAY;

        // 2. SCL���ߣ���ʼ����
        SCL_HIGH;
        I2C_DELAY;

        // 3. ���ݲ�������ȡSDA
        data <<= 1;     // �������ƣ��´����λ��Զ�����λ
        if (READ_SDA)
        {
            data |= 0x01;   // �ȴ������λ��Ȼ��ÿ�ζ�����1λ
        }

        // 4. SCL���ͣ���������
        SCL_LOW;
        I2C_DELAY;
    }
    
    return data;
}

