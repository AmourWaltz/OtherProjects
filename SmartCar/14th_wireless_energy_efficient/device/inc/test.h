#ifndef _TEST_H_
#define _TEST_H_

#include "common.h"
#include "include.h"

/*===============================================================
        @note      赵客缦胡缨，吴钩霜雪明；
                   银鞍照白马，飒沓如流星；
                   十步杀一人，千里不留行；
                   事了拂衣去，深藏身与名；
                   闲过信陵饮，脱剑膝前横；
                   将炙啖朱亥，持觞劝侯嬴；
                   三杯吐然诺，五岳倒为轻；
                   眼花耳热后，意气素霓生；
                   救赵挥金槌，邯郸先震惊；
                   千秋二壮士，烜赫大梁城；
                   纵死侠骨香，不惭世上英；
                   谁能书阁下，白首太玄经。
==================================================================*/

void Switch_Test();//拨码测试

void LED_Test();//LED测试

void Five_Dir_Test();//五向按键测试

void Encoder_Test();//编码器测试

void Signal_Test(vint16 inductor_value[]);//电感信号测试

void Test_Infrared();//红外信号测试

void MPU6050_Test();//陀螺仪矫正测试

void Gradient_Test();//电感变化率测试

void Annulus_Test(vint16 inductor_value[]);//环岛信号测试

void Obstacle_Test();//路障测试

void Slope_Test(vint16 inductor_value[]);//坡道信号测试


#endif