#ifndef _PWM_H_
#define _PWM_H_

#include "include.h"
#include "common.h"

/*===============================================================
        @note      一别都门三改火，天涯踏尽红尘。
                   依然一笑作春温；无波真古井，有节是秋筠。
                   惆怅孤帆连夜发，送行淡月微云。
                   尊前不用翠眉颦；人生如逆旅，我亦是行人。
==================================================================*/


// 电机和舵机分别对应的PWM通道、引脚和死区
#define PWM_MOTOR_FTM			FTM0

#define PWM_LEFT_MOTOR_CH		FTM_Ch2
#define PWM_LEFT_MOTOR_PIN		PTC3

#define PWM_RIGHT_MOTOR_CH		FTM_Ch0
#define PWM_RIGHT_MOTOR_PIN		PTC1


// 周期
#define PWM_Motor_PER_1S		(10000)    

// 初始化占空比
#define RESET_DUTY	                (0)
#define HALF_DUTY                       (5000)

#define MAX_DUTY	                (9000)
#define MIN_DUTY	                (0)


void Motor_PWM_Init(void);//输出PWM初始化

void Change_PWM_Motor_Left(uint32 DUTY);//修改左轮占空比

void Change_PWM_Motor_Right(uint32 DUTY);//修改右轮占空比


#endif