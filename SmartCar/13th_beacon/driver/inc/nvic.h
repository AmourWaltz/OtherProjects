#ifndef _NVIC_H
#define _NVIC_H

#include "common.h"

//如果测试抢占式中断，则定义为1，否则为0
#define PREEMPTION      1

void NVIC_Init (void);

#endif