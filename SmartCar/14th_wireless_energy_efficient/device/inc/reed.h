#ifndef _REED_SWITCH_H_
#define _REED_SWITCH_H_

#include "common.h"
#include "include.h"

/*===============================================================
        @note      置酒高殿上，亲交从我游。
                   中厨办丰膳，烹羊宰肥牛。
                   秦筝何慷慨，齐瑟和且柔。
                   阳阿奏奇舞，京洛出名讴。
                   乐饮过三爵，缓带倾庶羞。
                   主称千金寿，宾奉万年酬。
                   久要不可忘，薄终义所尤。
                   谦谦君子德，磬折欲何求。
                   惊风飘白日，光景驰西流。
                   盛时不再来，百年忽我遒。
                   生存华屋处，零落归山丘。
                   先民谁不死，知命复何忧？
==================================================================*/

#define REED_NUM                        (4)

#define REED_SWITCH_0	                (0)
#define REED_SWITCH_1	                (1)
#define REED_SWITCH_2	                (2)
#define REED_SWITCH_3	                (3)

#define REED_SWITCH_ALL_ON              (0x00)
#define REED_SWITCH_ALL_OFF	        (0x0F)
#define REED_SWITCH_ON		        (0)
#define REED_SWITCH_OFF		        (1)


//干簧管引脚分配
#define REED_SWITCH_0_PORT              PTE
#define REED_SWITCH_0_PIN               GPIO_Pin6
#define REED_SWITCH_0_NUM               (6)

#define REED_SWITCH_1_PORT              PTE
#define REED_SWITCH_1_PIN               GPIO_Pin8
#define REED_SWITCH_1_NUM               (8)

#define REED_SWITCH_2_PORT              PTE
#define REED_SWITCH_2_PIN               GPIO_Pin10
#define REED_SWITCH_2_NUM               (10)

#define REED_SWITCH_3_PORT              PTE
#define REED_SWITCH_3_PIN               GPIO_Pin12
#define REED_SWITCH_3_NUM               (12)


void Reed_Init(void);//干簧管初始化

uint8 Get_Reed_State_Once(uint8 reed_switch_num);//获取一个干簧管状态

uint8 Get_All_Reed_State(void);//获取所有干簧管状态


#endif