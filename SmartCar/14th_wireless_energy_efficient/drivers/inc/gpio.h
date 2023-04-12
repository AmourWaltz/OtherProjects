#ifndef __GPIO_H__
#define __GPIO_H__

#include "common.h"
#include "include.h"
#include "HW_GPIO.h"

/*===============================================================
        @note      自爱残妆晓镜中，环钗谩篸绿丝丛；
                   须臾日射燕脂颊，一朵红苏旋欲融。
                   山泉散漫绕阶流，万树桃花映小楼；
                   闲读道书慵未起，水晶帘下看梳头。
                   红罗著压逐时新，吉了花纱嫩麹尘；
                   第一莫嫌材地弱，些些纰缦最宜人。
                   曾经沧海难为水，除却巫山不是云；
                   取次花丛懒回顾，半缘修道半缘君。
                   寻常百种花齐发，偏摘梨花与白人；
                   今日江头两三树，可怜和叶度残春。
==================================================================*/

#define INPUT_L                 OUTPUT_L        
#define INPUT_H                 OUTPUT_H     

void GPIO_Init(GPIO_Type *PTx, uint32 GPIO_Pinx, uint8 Dir, uint8 Level);//初始化通用输入输出



#endif 