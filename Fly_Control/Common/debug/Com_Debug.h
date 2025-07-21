#ifndef __COM_DEBUG_H
#define __COM_DEBUG_H

#include "usart.h"
#include "string.h"
#include "stdio.h"

#define DEBUG 1

#if(DEBUG == 1)

#define FINENAME strrchr(__FILE__, '\\') ? (strrchr(__FILE__, '\\') + 1) : __FILE__

#define debug_start() Com_Debug_Start()
#define debug_printf(format, ...) printf("[%15s:%4d] -- " format, FINENAME, __LINE__ , ##__VA_ARGS__)
#define debug_printfln(format, ...) printf("[%15s:%4d] -- " format "\r\n", FINENAME, __LINE__ , ##__VA_ARGS__)
#else

#define debug_start() 
#define debug_printf(format, ...) 
#define debug_printfln(format, ...) 

#endif

void Com_Debug_Start(void);
#endif
