#ifndef _INDUCTOR_H_
#define _INDUCTOR_H_

#include "common.h"
#include "include.h"

/*===============================================================
        @note      灵台无计逃神矢，风雨如磐暗故园；
                   寄意寒星荃不察，我以我血荐轩辕。
==================================================================*/    

#define NUM_INDUCTOR                    5//电感数量
#define AVER_TIMES                      4//均值滤波次数

#define T_LEFT                          (0)
#define X_LEFT                          (1)

#define X_RIGHT                         (3)
#define T_RIGHT                         (4)

#define M_BACK                          (2)

//各电感对应ADC通道
#define LEFT_T_ADC_ID                   ADC1
#define LEFT_T_ADC_CH                   AD16

#define LEFT_X_ADC_ID                   ADC0
#define LEFT_X_ADC_CH                   DAD3

#define MID_B_ADC_ID                    ADC1
#define MID_B_ADC_CH                    DAD3

#define RIGHT_X_ADC_ID                  ADC1
#define RIGHT_X_ADC_CH                  DAD1

#define RIGHT_T_ADC_ID                  ADC0
#define RIGHT_T_ADC_CH                  DAD1



void Inductor_Init(void);//通道初始化

uint16 Inductor_Once_Get(uint8 uc_inductor_num);//单次采集单一通道值

void Inductor_All_Get(vint16 inductor_value[]);//全通道获取



#endif