#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "common.h"
#include "include.h"

/*===============================================================
        @note      风雨凄凄，鸡鸣喈喈；既见君子，云胡不夷。
                   风雨潇潇，鸡鸣胶胶；既见君子，云胡不瘳。
                   风雨如晦，鸡鸣不已；既见君子，云胡不喜。
==================================================================*/

//以前的编码器计数

//节能二号
#define PULSE_PER_METRE_R         (62059)//反跑车： //推车一米右边编码器计的脉冲数
#define PULSE_PER_METRE_L         (500196)//反跑车： //推车一米左边编码器计的脉冲数

////节能一号
//#define PULSE_PER_METRE_R         (267726)//反跑车： //推车一米右边编码器计的脉冲数
//#define PULSE_PER_METRE_L         (535452)//反跑车： //推车一米左边编码器计的脉冲数

#define PULSE_PER_CENTIMETRE      (0.2)
#define TO_CENTIMETRE             (200)

#define MOVINGAVERAGE_NUM         (4)//进行平均的脉冲采样数
#define SPEEDFILTER_NUM           (10)//滤波数组长度

#define SPD_UPDATE_PERIOD_S       (0.004)//采集速度频率为4ms  



void Get_Left_Speed();//由当前脉冲计数获取速度值

void Get_Right_Speed();//由当前脉冲计数获取速度值

void Get_Car_Speed();//获得速度

void Get_Car_Distance();//获得跑过的距离



#endif
