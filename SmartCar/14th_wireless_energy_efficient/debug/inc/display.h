#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common.h"
#include "include.h"

/*===============================================================
        @note      待到秋来九月八，我花开时百花杀；
                   冲天香阵透长安，满城尽带黄金甲。
==================================================================*/

#define RUNNING_PAGE_MIN        1
#define RUNNING_PAGE_MAX        5


uint8  Signal_Parameter(uint8 page_num);//信号显示页

uint8  Annulus_Parameter(uint8 page_num);//环岛参数显示页

uint8  MPU6050_Parameter(uint8 page_num);//惯性测量单元参数显示页

uint8  Slope_Parameter(uint8 page_num);//坡道参数显示页

uint8 Obstacle_Parameter(uint8 page_num);//路障参数显示页

void Smartcar_Running();//小车运动显示函数

void Delay();//延时



#endif