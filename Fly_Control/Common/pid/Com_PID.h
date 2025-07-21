#ifndef __COM_PID_H__
#define __COM_PID_H__

#include "Com_Config.h"

void Com_ComputePID(PID_Struct *pid);
void Com_CascadePID(PID_Struct *out,PID_Struct *in);

#endif
