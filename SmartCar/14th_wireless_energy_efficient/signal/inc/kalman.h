#ifndef _KALMAN_
#define _KALMAN_

#include <math.h>
#include "common.h"
#include "include.h"

/*===============================================================
        @note      钟山风雨起苍黄，百万雄师过大江；
                   虎踞龙盘今胜昔，天翻地覆慨而慷；
                   宜将剩勇追穷寇，不可沽名学霸王；
                   天若有情天亦老，人间正道是沧桑。
==================================================================*/  

#define ACCE_MAX_ERR            0.4
#define ACCE_ANGLE_NOISE        10000000.0//加速度计白噪声系数
#define GYRO_ANGLE_NOISE        1.0//陀螺仪白噪声系数
#define PI                      3.14159

#define KALMAN_CHANNEL_XY       0
#define KALMAN_CHANNEL_YZ       1
#define KALMAN_CHANNEL_ZX       2
#define KALMAN_CHANNEL_AG       3


float Kalman_Fliter(float acceAngle, float gyroAngle,char ch);//卡尔曼滤波

void Get_Car_Angle();//获取小车角度

void Get_Car_Posture();//获取车体姿态



#endif 
