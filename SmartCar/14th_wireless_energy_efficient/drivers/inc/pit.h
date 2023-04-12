#ifndef _PIT_H_
#define _PIT_H_

#include "common.h"
#include "include.h"

/*===============================================================
        @note      细雨斜风作晓寒。淡烟疏柳媚晴滩。入淮清洛渐漫漫。
                   雪沫乳花浮午盏，蓼茸蒿笋试春盘。人间有味是清欢。
==================================================================*/

#define PIT_0_INIT              (0)
#define PIT_1_INIT              (1)
#define PIT_2_INIT              (2)
#define PIT_3_INIT              (3)


#define PIT_0                   (0)
#define PIT_1                   (1)
#define PIT_2                   (2)
#define PIT_3                   (3)


#define PIT_0_PERIOD_US         (100)
#define PIT_1_PERIOD_MS         (4)
#define PIT_1_PERIOD_US         (40000)
#define PIT_1_PERIOD_S          (0.004)
#define PIT_2_PERIOD_MS         (2)
#define PIT_3_PERIOD_MS         (5)





void PIT_Init(void);//PIT初始化

#ifdef PIT_0_INIT
void PIT_0_handler(void);//PIT0处理函数
#endif

#ifdef PIT_1_INIT
void PIT_1_handler(void);//PIT1处理函数
#endif

#ifdef PIT_2_INIT
void PIT_2_handler(void);//PIT2处理函数
#endif

#ifdef PIT_3_INIT
void PIT_3_handler(void);//PIT3处理函数
#endif

void PIT_ONE_Enable(uint8 pit_num);//使能一个PIT

void PIT_ONE_Disable(uint8 pit_num);//禁用一个PIT

void PIT_ALL_Enable(void);//使能所有定义的PIT

void PIT_ALL_Disable(void);//禁用所有定义的PIT



#endif