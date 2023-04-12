#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include "common.h"
#include "include.h"

/*===============================================================
        @note      君不见黄河之水天上来，奔流到海不复回；
                   君不见高堂明镜悲白发，朝如青丝暮成雪；
                   人生得意须尽欢，莫使金樽空对月；
                   天生我材必有用，千金散尽还复来；
                   烹羊宰牛且为乐，会须一饮三百杯；
                   岑夫子、丹丘生：将进酒，杯莫停。
                   与君歌一曲，请君为我侧耳听。
                   钟鼓馔玉不足贵，但愿长醉不愿醒；
                   古来圣贤皆寂寞，唯有饮者留其名；
                   陈王昔时宴平乐，斗酒十千恣欢谑；
                   主人何为言少钱，径须沽取对君酌；
                   五花马，千金裘，呼儿将出换美酒，
                   与尔同销万古愁。
==================================================================*/  

//路障处理进程，几何法三段弧形路径
#define OBSTACLE_PROCESS_1         1
#define OBSTACLE_PROCESS_2         2
#define OBSTACLE_PROCESS_3         3

//路障处理参数，计算得出角速度，微调优化
#define ROADCENTRE_DEVIATE         18//道路宽度22.5cm，考虑车体宽度，偏离中心线距离应略小
#define PARALLEL_DISPLACEMENT      53//平行道路方向行驶距离，距离检测接近90cm时，开始偏移
//#define ROTATE_COEFFICIENT         10//期望角速度换算系数
#define OBSTACLE_RADIUS            80//路障绕行半径


//坡道路障优先级
#define OBSTACLE_FIRST             1
#define SLOPE_FIRST                2



uint8 Jugde_Obstacle();//路障进程判断

void Distinguish_Obstacle();//路障判断，主要区分路障与坡道

void Switch_Process_Selection();//拨码开关处理进程选择

void Obstacle_Detection();//路障检测

void Cambered_Trajectory_Calculate();//弧形路径几何参数计算

void Dynamic_Calculate();//动态参数计算处理路障

void Obstacle_Process();//路障处理

void Obstacle_Angle_Aviodance(float expected_angle, uint8 channel);//避障外环期望角度设定


#endif