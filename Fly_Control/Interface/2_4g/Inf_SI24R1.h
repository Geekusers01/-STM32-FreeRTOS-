#ifndef __INF_SI24R1_H
#define __INF_SI24R1_H
#include "Com_Debug.h"
#include "spi.h"

#define CH 111

#define SI24R1_CS_H HAL_GPIO_WritePin(Si24R1_CS_GPIO_Port, Si24R1_CS_Pin, GPIO_PIN_SET)
#define SI24R1_CS_L HAL_GPIO_WritePin(Si24R1_CS_GPIO_Port, Si24R1_CS_Pin, GPIO_PIN_RESET)
#define SI24R1_EN_H HAL_GPIO_WritePin(Si24R1_CE_GPIO_Port, Si24R1_CE_Pin, GPIO_PIN_SET)
#define SI24R1_EN_L HAL_GPIO_WritePin(Si24R1_CE_GPIO_Port, Si24R1_CE_Pin, GPIO_PIN_RESET)
// #define READ_NRF24L01_IRQ HAL_GPIO_ReadPin(SI_IRQ_GPIO_Port, SI_IRQ_Pin) // IRQ主机数据输入

/*****************************************发送接收数据宽度定义***********************************************/
#define TX_ADR_WIDTH 5      // 5字节的地址宽度
#define RX_ADR_WIDTH 5      // 5字节的地址宽度
#define TX_PLOAD_WIDTH 18   // 32字节的用户数据宽度
#define RX_PLOAD_WIDTH 18   // 32字节的用户数据宽度

/*******************************************寄存器操作指令**************************************************/
#define SPI_READ_REG 0x00    // 读配置寄存器,低5位为寄存器地址
#define SPI_WRITE_REG 0x20   // 写配置寄存器,低5位为寄存器地址
#define R_RX_PL_WID 0x60     // 读取收到的数据字节数
#define RD_RX_PLOAD 0x61     // 读RX有效数据,1~32字节
#define WR_TX_PLOAD 0xA0     // 写TX有效数据,1~32字节
#define FLUSH_TX 0xE1        // 清除TX FIFO寄存器.发射模式下用
#define FLUSH_RX 0xE2        // 清除RX FIFO寄存器.接收模式下用
#define REUSE_TX_PL 0xE3     // 重新使用上一包数据,CE为高,数据包被不断发送.
#define W_ACK_PAYLOAD 0xA8   // 接收方,将数据通过ACK的形式发出去，最多允许三帧数据存于FIFO中。
#define NOP 0xFF             // 空操作,可以用来读状态寄存器

/*******************************************寄存器地址****************************************************/
#define CONFIG 0x00       // 配置寄存器地址;bit0:1接收模式,0发射模式;bit1:电选择;bit2:CRC模式;bit3:CRC使能;
                          // bit4:中断MAX_RT(达到最大重发次数中断)使能;bit5:中断TX_DS使能;bit6:中断RX_DR使能
#define EN_AA 0x01        // 使能自动应答功能  bit0~5,对应通道0~5
#define EN_RXADDR 0x02    // 接收地址允许,bit0~5,对应通道0~5
#define SETUP_AW 0x03     // 设置地址宽度(所有数据通道):bit1,0:00,3字节;01,4字节;02,5字节;
#define SETUP_RETR 0x04   // 建立自动重发;bit3:0,自动重发计数器;bit7:4,自动重发延时 250*x+86us
#define RF_CH 0x05        // RF通道,bit6:0,工作通道频率;
#define RF_SETUP 0x06     // RF寄存器;bit3:传输速率(0:1Mbps,1:2Mbps);bit2:1,发射功率;bit0:低噪声放大器增益
#define STATUS 0x07       // 状态寄存器;bit0:TX FIFO满标志;bit3:1,接收数据通道号(最大:6);
#define MAX_TX 0x10       // 状态寄存器;bit4:达到最大发送次数中断
#define TX_OK 0x20        // 状态寄存器;bit5:TX发送完成中断
#define RX_OK 0x40        // 状态寄存器;bit6:接收到数据中断

#define OBSERVE_TX 0x08    // 发送检测寄存器,bit7:4,数据包丢失计数器;bit3:0,重发计数器
#define CD 0x09            // 载波检测寄存器,bit0,载波检测;
#define RX_ADDR_P0 0x0A    // 数据通道0接收地址,最大长度5个字节,低字节在前
#define RX_ADDR_P1 0x0B    // 数据通道1接收地址,最大长度5个字节,低字节在前
#define RX_ADDR_P2 0x0C    // 数据通道2接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P3 0x0D    // 数据通道3接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P4 0x0E    // 数据通道4接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P5 0x0F    // 数据通道5接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define TX_ADDR 0x10       // 发送地址(低字节在前),ShockBurstTM模式下,RX_ADDR_P0与此地址相等
#define RX_PW_P0 0x11      // 接收数据通道0有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P1 0x12      // 接收数据通道1有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P2 0x13      // 接收数据通道2有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P3 0x14      // 接收数据通道3有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P4 0x15      // 接收数据通道4有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P5 0x16      // 接收数据通道5有效数据宽度(1~32字节),设置为0则非法
#define FIFO_STATUS 0x17   // FIFO状态寄存器;bit0:RX FIFO寄存器空标志;bit1:RX FIFO满标志;bit2,3:保留
                           // FIFO状态寄存器;bit4:TX FIFO空标志;bit5:TX FIFO满标志;
                           // FIFO状态寄存器;bit6:1,循环发送上一数据包/0,不循环;

#define DYNPD 0x1C     // 动态负载长度寄存器;bit0——bit5对应通道0-5的动态负载长度使能
#define FEATURE 0x1D   // 特征寄存器;bit1:使能ACK负载(带负载数据的ACK包);bit2:使能动态负载长度;

/**********************************************************************************************************/
#define MODEL_RX 1    // 普通接收
#define MODEL_TX 2    // 普通发送
#define MODEL_RX2 3   // 接收模式2,用于双向传输
#define MODEL_TX2 4   // 发送模式2,用于双向传输

extern uint16_t connect_flag;
extern uint8_t  TX_BUFF[TX_PLOAD_WIDTH];
extern uint8_t  RX_BUFF[RX_PLOAD_WIDTH];

uint8_t Inf_Si24R1_WriteReg(uint8_t reg, uint8_t data);
uint8_t Inf_Si24R1_ReadReg(uint8_t reg);
uint8_t Inf_Si24R1_WriteBuf(uint8_t reg, uint8_t *pBuf, uint8_t len);
uint8_t Inf_Si24R1_ReadBuf(uint8_t reg, uint8_t *pBuf, uint8_t len);
void Inf_Si24R1_TXMode(void);
void Inf_Si24R1_RXMode(void);
uint8_t Inf_Si24R1_TxPacket(uint8_t *txBuf);
uint8_t Inf_Si24R1_RxPacket(uint8_t *txBuf);
uint8_t Inf_Si24R1_Check(void);
#endif
