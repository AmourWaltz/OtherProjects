#ifndef _ISR_h
#define _ISR_h

#include "common.h"
#include "include.h"

/*===============================================================
        @note      茫茫九派流中国，沉沉一线穿南北；
                   烟雨莽苍苍，龟蛇锁大江。
                   黄鹤知何去？剩有游人处；
                   把酒酹滔滔，心潮逐浪高！
==================================================================*/ 

void PIT0_Process();
void PIT1_Process();   
void PIT2_Process(); 
void PIT3_Process();
void SoftIsr_Open(IRQn_Type irqx);



#endif

