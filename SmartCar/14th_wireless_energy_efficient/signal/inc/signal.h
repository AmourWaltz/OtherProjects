#ifndef _SIGNAL_H_
#define _SIGNAL_H_

#include "common.h"
#include "include.h"

/*===============================================================
        @note      阅尽天涯离别苦，不道归来，零落花如许。
                   花底相看无一语，绿窗春与天俱莫。 
                   待把相思灯下诉，一缕新欢，旧恨千千缕。
                   最是人间留不住，朱颜辞镜花辞树。
==================================================================*/    

//信号滤波
#define    NUM_INDUCTOR_HISTORY                 (4)//电感值历史值记录长度
#define    JUMP_MAX_TIMES                       (4)//电感值防跳变滤波器最大跳变次数

#define    ORIGINAL_LINEAR_PROPORTION_X         200//横电感差比和放大比例
#define    ORIGINAL_LINEAR_PROPORTION_T         100
#define    CRITERION_SIGNAL                     3//电感零漂校准值

#define    GET_SIGNAL_VALUE                     50
#define    LOST_SIGNAL_VALUE                    20

#define    RING_BUFF_SIZE                       5//环岛检测电感通道深度
#define    POS_BUFF_SIZE                        25




void Get_Inductor_Value();//获取电感AD转换值

void Filter_Inductor_Value(vint16 inductor_value[]);//电感值防跳变最小二乘法滤波器

void Get_Sensor_Offset(vint16 sensor_value[]);//获取电感差比和

void Stop_Danger_Lost_Signals(vint16 p_inductor_value[]);//偏离赛道危险情况停车函数

void Criterion_Signal(vint16 inductor_value[]);//校准信号，消除零漂

void Get_Signal_Value();//信号获取处理判断

void Signal_Control();//信号中断调用函数

int16 Get_Signal_Derivative();//计算所有电感信号随距离变化率

void Get_Position();//获取车身姿态

void Get_Annulus_Angle();//积分得到车模偏航角度，处理环岛判断是否完成一圈

void Get_Slope_Angle();//积分得到车模俯仰角度处理坡道

void Get_Obstacle_Angle();//积分得到车模偏航角度，避障使用




#endif