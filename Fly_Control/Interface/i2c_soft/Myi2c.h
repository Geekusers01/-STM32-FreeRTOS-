/*
 * @Author: wushengran
 * @Date: 2024-05-31 11:47:31
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __I2C_H
#define __I2C_H

#include "stm32f1xx_hal.h"

// �궨��
#define ACK 0
#define NACK 1

// ����SCL��SDA������ߵ͵�ƽ
#define SCL_HIGH (GPIOB->ODR |= GPIO_ODR_ODR6)
#define SCL_LOW (GPIOB->ODR &= ~GPIO_ODR_ODR6)
#define SDA_HIGH (GPIOB->ODR |= GPIO_ODR_ODR7)
#define SDA_LOW (GPIOB->ODR &= ~GPIO_ODR_ODR7)

// ��ȡ����
#define READ_SDA (GPIOB->IDR & GPIO_IDR_IDR7)

// ��������Ļ����ӳ�
#define I2C_DELAY Delay_us(10)

void Delay_ms(uint16_t ms);

// ��ʼ��
void I2C_Init(void);

// ������ʼ�ź�
void I2C_Start(void);

// ����ֹͣ�ź�
void I2C_Stop(void);

// ��������Ӧ���ź�
void I2C_Ack(void);

// ����������Ӧ���ź�
void I2C_Nack(void);

// �����ȴ����豸����Ӧ���ź�
uint8_t I2C_Wait4Ack(void);

// ��������һ���ֽڵ����ݣ�д�룩
void I2C_SendByte(uint8_t byte);

// ������EEPROM����һ���ֽڵ����ݣ���ȡ��
uint8_t I2C_ReadByte(void);

#endif
