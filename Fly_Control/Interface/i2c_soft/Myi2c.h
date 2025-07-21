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

// 宏定义
#define ACK 0
#define NACK 1

// 控制SCL、SDA的输出高低电平
#define SCL_HIGH (GPIOB->ODR |= GPIO_ODR_ODR6)
#define SCL_LOW (GPIOB->ODR &= ~GPIO_ODR_ODR6)
#define SDA_HIGH (GPIOB->ODR |= GPIO_ODR_ODR7)
#define SDA_LOW (GPIOB->ODR &= ~GPIO_ODR_ODR7)

// 读取操作
#define READ_SDA (GPIOB->IDR & GPIO_IDR_IDR7)

// 定义操作的基本延迟
#define I2C_DELAY Delay_us(10)

void Delay_ms(uint16_t ms);

// 初始化
void I2C_Init(void);

// 发出起始信号
void I2C_Start(void);

// 发出停止信号
void I2C_Stop(void);

// 主机发出应答信号
void I2C_Ack(void);

// 主机发出非应答信号
void I2C_Nack(void);

// 主机等待从设备发来应答信号
uint8_t I2C_Wait4Ack(void);

// 主机发送一个字节的数据（写入）
void I2C_SendByte(uint8_t byte);

// 主机从EEPROM接收一个字节的数据（读取）
uint8_t I2C_ReadByte(void);

#endif
