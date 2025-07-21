#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"
#include "gpio.h"

uint8_t OLED_GRAM[128][8];
void    OLED_Refresh_Gram(void)
{
    uint8_t i, n;
    for(i = 0; i < 8; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD);   // 设置页地址（0~7）
        OLED_WR_Byte(0x00, OLED_CMD);       // 设置显示位置—列低地址
        OLED_WR_Byte(0x10, OLED_CMD);       // 设置显示位置—列高地址
        for(n = 0; n < 128; n++)
            OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
    }
}

// 向OLED写入一个字节。
// dat:要写入的数据/命令
// cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(uint8_t dat, uint8_t cmd)
{
    uint8_t i;
    if(cmd)
        OLED_DC_Set();
    else
        OLED_DC_Clr();
    OLED_CS_Clr(); /* 要加：先选中 */
    for(i = 0; i < 8; i++)
    {
        OLED_SCLK_Clr();
        if(dat & 0x80)
            OLED_SDIN_Set();
        else
            OLED_SDIN_Clr();
        OLED_SCLK_Set();
        dat <<= 1;
    }

    OLED_CS_Set(); /* 要加：先释放 */
    OLED_DC_Set();
}

// 开启OLED显示
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD);   // SET DCDC命令
    OLED_WR_Byte(0X14, OLED_CMD);   // DCDC ON
    OLED_WR_Byte(0XAF, OLED_CMD);   // DISPLAY ON
}
// 关闭OLED显示
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD);   // SET DCDC命令
    OLED_WR_Byte(0X10, OLED_CMD);   // DCDC OFF
    OLED_WR_Byte(0XAE, OLED_CMD);   // DISPLAY OFF
}
// 清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void)
{
    uint8_t i, n;
    for(i = 0; i < 8; i++)
    {
        for(n = 0; n < 128; n++)
        {
            OLED_GRAM[n][i] = 0X00;
        }
    }
    OLED_Refresh_Gram();   // 更新显示
}
// 画点
// x:0~127
// y:0~63
// t:1 填充 0,清空
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t)
{
    uint8_t pos, bx, temp = 0;
    if(x > 127 || y > 63)
        return;   // 超出范围了.
    pos  = 7 - y / 8;
    bx   = y % 8;
    temp = 1 << (7 - bx);
    if(t)
        OLED_GRAM[x][pos] |= temp;
    else
        OLED_GRAM[x][pos] &= ~temp;
}

// 在指定位置显示一个字符,包括部分字符
// x:0~127
// y:0~63
// mode:0,反白显示;1,正常显示
// size:选择字体 16/12
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode)
{
    uint8_t temp, t, t1;
    uint8_t y0 = y;
    chr        = chr - ' ';   // 得到偏移后的值
    for(t = 0; t < size; t++)
    {
        if(size == 12)
            temp = oled_asc2_1206[chr][t];   // 调用1206字体
        else
            temp = oled_asc2_1608[chr][t];   // 调用1608字体
        for(t1 = 0; t1 < 8; t1++)
        {
            if(temp & 0x80)
                OLED_DrawPoint(x, y, mode);
            else
                OLED_DrawPoint(x, y, !mode);
            temp <<= 1;
            y++;
            if((y - y0) == size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}
// 显示汉字
void OLED_ShowCH(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode)
{
    uint8_t temp, t, t1;
    uint8_t y0 = y;

    for(t = 0; t < size; t++)
    {
        temp = oled_CH_1616[chr][t];   // 调用中文字库
        for(t1 = 0; t1 < 8; t1++)
        {
            if(temp & 0x80)
                OLED_DrawPoint(x, y, mode);
            else
                OLED_DrawPoint(x, y, !mode);
            temp <<= 1;
            y++;
            if((y - y0) == size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}

// 显示单个中文字符
void OLED_Show_CH(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode)
{
    OLED_ShowCH(x, y, chr * 2, size, 1);
    OLED_ShowCH(x + size / 2, y, chr * 2 + 1, size, 1);
}

// 显示一串中文字符
void OLED_Show_CH_String(uint8_t x, uint8_t y, const uint8_t *p, uint8_t size, uint8_t mode)
{
    uint8_t temp, t, t1;
    uint8_t y0 = y;

    for(t = 0; t < size; t++)
    {
        temp = p[t];   // 调用中文字库
        for(t1 = 0; t1 < 8; t1++)
        {
            if(temp & 0x80)
                OLED_DrawPoint(x, y, mode);
            else
                OLED_DrawPoint(x, y, !mode);
            temp <<= 1;
            y++;
            if((y - y0) == size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}
// m^n函数
uint32_t oled_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while(n--)
        result *= m;
    return result;
}
// 显示2个数字
// x,y :起点坐标
// len :数字的位数
// size:字体大小
// mode:模式  0,填充模式;1,叠加模式
// num:数值(0~4294967295);
void OLED_ShowNumber(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    for(t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if(enshow == 0 && t < (len - 1))
        {
            if(temp == 0)
            {
                OLED_ShowChar(x + (size / 2) * t, y, ' ', size, 1);
                continue;
            }
            else
                enshow = 1;
        }
        OLED_ShowChar(x + (size / 2) * t, y, temp + '0', size, 1);
    }
}
// 显示字符串
// x,y:起点坐标
//*p:字符串起始地址
// 用16字体
void OLED_ShowString(uint8_t x, uint8_t y, const uint8_t *p, uint8_t size, uint8_t mode)
{
#define MAX_CHAR_POSX 122
#define MAX_CHAR_POSY 58
    while(*p != '\0')
    {
        if(x > MAX_CHAR_POSX)
        {
            x = 0;
            y += 16;
        }   // 换行
        if(y > MAX_CHAR_POSY)
        {
            y = x = 0;
            // OLED_Clear();
        }
        OLED_ShowChar(x, y, *p, size, mode);
        x += 8;
        p++;
    }
}
// 显示字符串
// x,y:起点坐标
//*p:字符串起始地址
// 用16字体
void OLED_ShowString_16(uint8_t x, uint8_t y, const uint8_t *p)
{
#define MAX_CHAR_POSX 122
#define MAX_CHAR_POSY 58
    while(*p != '\0')
    {
        if(x > MAX_CHAR_POSX)
        {
            x = 0;
            y += 16;
        }   // 换行
        if(y > MAX_CHAR_POSY)
        {
            y = x = 0;
            OLED_Clear();
        }
        OLED_ShowChar(x, y, *p, 16, 1);
        x += 8;
        p++;
    }
}

// 初始化OLED
void OLED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB,  GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_SET);
		
    GPIO_InitStruct.Pin   =  GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
    OLED_RST_Clr();
    HAL_Delay(100);
    OLED_RST_Set();

    OLED_WR_Byte(0xAE, OLED_CMD);   //--turn off oled panel
    OLED_WR_Byte(0x00, OLED_CMD);   //---set low column address
    OLED_WR_Byte(0x10, OLED_CMD);   //---set high column address
    OLED_WR_Byte(0x40, OLED_CMD);   //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x81, OLED_CMD);   //--set contrast control register
    OLED_WR_Byte(0xCF, OLED_CMD);   // Set SEG Output Current Brightness
    OLED_WR_Byte(0xA1, OLED_CMD);   //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_WR_Byte(0xC0, OLED_CMD);   // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_WR_Byte(0xA6, OLED_CMD);   //--set normal display
    OLED_WR_Byte(0xA8, OLED_CMD);   //--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f, OLED_CMD);   //--1/64 duty
    OLED_WR_Byte(0xD3, OLED_CMD);   //-set display offset   Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00, OLED_CMD);   //-not offset
    OLED_WR_Byte(0xd5, OLED_CMD);   //--set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80, OLED_CMD);   //--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9, OLED_CMD);   //--set pre-charge period
    OLED_WR_Byte(0xF1, OLED_CMD);   // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA, OLED_CMD);   //--set com pins hardware configuration
    OLED_WR_Byte(0x12, OLED_CMD);
    OLED_WR_Byte(0xDB, OLED_CMD);   //--set vcomh
    OLED_WR_Byte(0x30, OLED_CMD);   // Set VCOM Deselect Level
    OLED_WR_Byte(0x20, OLED_CMD);   //-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_Byte(0x02, OLED_CMD);   //
    OLED_WR_Byte(0x8D, OLED_CMD);   //--set Charge Pump enable/disable
    OLED_WR_Byte(0x14, OLED_CMD);   //--set(0x10) disable
    OLED_Clear();
    OLED_WR_Byte(0xAF, OLED_CMD);
}
		    
