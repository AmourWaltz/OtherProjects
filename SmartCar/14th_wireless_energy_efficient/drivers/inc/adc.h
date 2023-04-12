#ifndef _ADC_H
#define _ADC_H

#include "common.h"
#include "include.h"

/*===============================================================
        @note      参横斗转欲三更，苦雨终风也解晴；
                   云散月明谁点缀？天容海色本澄清；
                   空余鲁叟乘桴意，粗识轩辕奏乐声；
                   九死南荒吾不恨，兹游奇绝冠平生。
==================================================================*/

#define BAT_ADC_ID                       ADC0
#define BAT_ADC_CH                       AD16

#define CURRENT_LOOP_REF_ADC_ID          ADC0
#define CURRENT_LOOP_REF_ADC_CH          AD20


void ADC_Init();//初始化模数转换器

void ADC0_Init();//初始化模数转换器

void ADC1_Init();//初始化模数转换器

void ADC_CH_Init();//使能电池电压通道

void ADC_0_isr();//ADC0中断函数

void ADC_1_isr();//ADC1中断函数

int16 ADC_Battery_Voltage();//电池电压转换读取


#endif