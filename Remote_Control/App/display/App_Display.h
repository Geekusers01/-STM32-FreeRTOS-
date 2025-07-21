#ifndef __APP_DISPLAY_H
#define __APP_DISPLAY_H
#include "Com_Debug.h"
#include "oled.h"
#include "oledfont.h"
#include "Inf_Si24R1.h"
#include "Inf_JoyStickAndKey.h"

#define Line1_Begin 29
#define Line2_Begin 5
#define Line3_Begin 5
#define Line4_Begin 30
#define Line5_Begin 2

#define X_Begin 0
#define Y_Begin 51
#define Z_Begin 103

#define Line1_Begin1 0
#define Line2_Begin1 0
#define Line3_Begin1 40
#define Line4_Begin1 0
#define Line5_Begin1 0

#define Y0 0
#define Y1 14
#define Y2 Y1 + 12
#define Y3 Y2 + 12
#define Y4 Y3 + 12
#define Y5 Y4 + 12

typedef struct
{
    uint8_t  hardware_type;
    uint8_t  oled_delay;
    uint8_t  windows;
    uint8_t  low_power;
    uint8_t  NRF_Err;
    uint8_t  Rc_num;
    uint8_t  Connect_Succeed;
    uint16_t Battery_Rc;
    uint16_t Battery_Fly;
    uint16_t test_flag, set_flag;
    int      X, Y, Z, H;
} Show_Struct;

extern Show_Struct Show;
extern uint16_t    set_temp;
extern uint16_t    test_temp;
extern uint8_t     send_flag;

void App_Display_Start(void);

void App_Display_Show(void);
#endif
