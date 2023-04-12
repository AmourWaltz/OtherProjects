#ifndef _BEEP_H__
#define _BEEP_H__

#include "common.h"
#include "include.h"

/*===============================================================
        @note      男儿何不带吴钩，收遍关山五十州；
                   请君暂上凌烟阁，若个书生万户侯。
==================================================================*/


/*蜂鸣器*/
#define BEEP_PORT               PTE
#define BEEP_PIN                4
#define BEEP_PIN_MASK           GPIO_Pin4

#define BEEP_ON_LV              OUTPUT_H
#define BEEP_OFF_LV             OUTPUT_L


void Beep_Init();//初始化蜂鸣器

void Beep_On();//开蜂鸣器

void Beep_Off();//关蜂鸣器

void Get_Beep_State(uint8 *u8State);//读取蜂鸣器状态

#endif 