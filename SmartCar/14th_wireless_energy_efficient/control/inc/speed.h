#ifndef _SPEED_H_
#define _SPEED_H_
 
#include "common.h"
#include "include.h"

/*===============================================================
        @note      富贵不知乐业，贫穷难耐凄凉，
                   可怜辜负好时光，于国于家无望；
                   天下无能第一，古今不肖无双，
                   寄言纨绔与膏粱，莫效此儿形状。
==================================================================*/

#define SPEED_PWM_MAX           8500



void Speed_Control();//增量式PI控制速度输出

int32 Set_Angle_Speed();//根据差速将速度减小至某一值

void Speed_Difference_Coefficient();//自适应差速系数

void Speed_PWM_Delta(int16 *left_expect_speed, int16 *right_expect_speed, int32 center_expect_speed);//计算左右轮差速

#endif