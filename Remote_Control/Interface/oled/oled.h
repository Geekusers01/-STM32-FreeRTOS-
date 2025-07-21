#ifndef __OLED_H
#define __OLED_H

#include "main.h"
#include "gpio.h"


#define OLED_CS_GPIO GPIOA
#define OLED_CS_Pin GPIO_PIN_4

#define OLED_DC_GPIO		GPIOB	
#define OLED_DC_Pin			GPIO_PIN_12

#define OLED_RST_GPIO		GPIOB	
#define OLED_RST_Pin		GPIO_PIN_14

#define OLED_SDA_GPIO		GPIOB	
#define OLED_SDA_Pin		GPIO_PIN_15

#define OLED_SCL_GPIO		GPIOB	
#define OLED_SCL_Pin		GPIO_PIN_13

//-----------------OLED端口定义----------------
#define OLED_CS_Clr() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)   // CS
#define OLED_CS_Set() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)     // CS

#define OLED_DC_Clr() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)   // DC
#define OLED_DC_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)     // DC

#define OLED_RST_Clr() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET)   // RST
#define OLED_RST_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET)     // RST

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET)   // SDA
#define OLED_SDIN_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET)     // SDA

#define OLED_SCLK_Clr() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET)   // SCL
#define OLED_SCLK_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET)     // SCL

#define OLED_CMD 0    // 写命令
#define OLED_DATA 1   // 写数据
// OLED控制用函数
void OLED_WR_Byte(uint8_t dat, uint8_t cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode);
void OLED_ShowNumber(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);
void OLED_ShowString(uint8_t x, uint8_t y, const uint8_t *p, uint8_t size, uint8_t mode);
void OLED_ShowCH(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode);
void OLED_Show_CH(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode);
void OLED_Show_CH_String(uint8_t x, uint8_t y, const uint8_t *p, uint8_t size, uint8_t mode);
#endif
