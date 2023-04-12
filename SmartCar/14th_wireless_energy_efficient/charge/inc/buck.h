#ifndef _BUCK_H_
#define _BUCK_H_

#include "common.h"
#include "include.h"

/*===============================================================
        @note      腹内胎生异锦，笔端舌喷长江，
                   纵教匹绢字难偿，不屑与人称量。
                   我不求人富贵，人须求我文章，
                   风流才子占词场，真乃白衣卿相。
==================================================================*/

#define CHARGE_ON     { \
                    LPLD_GPIO_Output_b(CHARGE_CONTROL_PORT,CHARGE_CONTROL_PIN ,HIGH); \
					} 

#define CHARGE_OFF    { \
                    LPLD_GPIO_Output_b(CHARGE_CONTROL_PORT,CHARGE_CONTROL_PIN ,LOW);\
					}

/*===============================================================
        @note      风絮飘残已化萍，泥莲刚倩藕丝萦；
                   珍重别拈香一瓣，记前生。
                   情到浓时情转薄，而今真个不多情；
                   又到断肠回首处，泪偷零。
==================================================================*/


//充电相关通道
#define PWM_CHARGE_FTM		        FTM1
#define PWM_CHARGE_CH		        FTM_Ch1
#define PWM_CHARGE_PIN                  PTA13

//引脚定义 
#define CHARGE_CONTROL_PORT             PTA
#define CHARGE_CONTROL_PIN              12
#define CHARGE_CONTROL_PIN_MASK         GPIO_Pin12


#define POS_PLUSE_WIDTH_US              5
#define NEG_PLUSE_WIDTH_US              2
#define PWM_CHARGE_FREQ		        (87730) 
#define DEAD_TIME_PWM                   (int)(5.2e-7*PWM_CHARGE_FREQ*10000)//0.52us


void PWM_Charge_Init(void);//输出PWM初始化

void Change_PWM_Charge(uint32 DUTY);//修改Buck电路占空比

#endif
