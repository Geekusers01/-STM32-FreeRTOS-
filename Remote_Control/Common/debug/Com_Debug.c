#include "Com_Debug.h"

void Com_Debug_Start(void)
{
    // ...
}

int fputc(int c, FILE *file)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&c, 1, 1000);
    return c;
}

