#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "common.h"
#include "include.h"

/*===============================================================
        @note      大江东去，浪淘尽，千古风流人物，故垒西边。
                   人道是，三国周郎赤壁；
                   乱石穿空，惊涛拍岸，卷起千堆雪。
                   江山如画，一时多少豪杰。
                   遥想公瑾当年，小乔初嫁了，雄姿英发。
                   羽扇纶巾，谈笑间，樯橹灰飞烟灭。
                   故国神游，多情应笑我，早生华发。
                   人生如梦，一樽还酹江月。
==================================================================*/    

#define MAX_DIR_ANGLE           200
#define ANNULUS_DIR_ANGLE       210

#define GENERAL                 1
#define CRUCIFIX                2
#define ANNULUS                 3
#define SLOPE                   4
#define OBSTACLE                5

#define RAMP_BEGIN              1
#define RAMP_UP                 2
#define ON_RAMP                 3
#define RAMP_DOWN               4

#define NORMAL                  0
#define ABNORMAL                1

#define ANGLE_PER_DIRECTION     10000//信号偏差输出PWM对应角度
#define RAMP_ROT_GATE           500//坡道检测俯仰角标准差阈值



uint8 Judge_Straight();//直道判断

uint8 Judge_Lost_Signal();//丢信号判断

uint8 Judge_Mix_Track();//串道识别

uint8 Judge_Crucifix();//十字判断

uint8 Judge_Small_S();//小S判断

uint8 Judge_Curve();//大弯道判断

uint8 Judge_Slope();//坡道判断

uint8 Judge_Annulus();//环岛判断

void Lost_Signal_Process();//丢信号处理

void General_Process();//直道处理

void Mix_Track_Process();//串道处理

void Small_S_Process();//小S处理

void Crucifix_Process();//十字处理

void Slope_Process();//坡道处理

void Annulus_Process();//环岛处理



#endif