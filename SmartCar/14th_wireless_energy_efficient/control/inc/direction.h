#ifndef _DIRECTION_H__
#define _DIRECTION_H__

#include "common.h"
#include "include.h"

/*===============================================================
        @note      桃花坞里桃花庵，桃花庵下桃花仙；
                   桃花仙人种桃树，又折花枝当酒钱；
                   酒醒只在花前坐，酒醉还须花下眠；
                   花前花后日复日，酒醉酒醒年复年；
                   不愿鞠躬车马前，但愿老死花酒间；
                   车尘马足贵者趣，酒盏花枝贫者缘；
                   若将富贵比贫贱，一在平地一在天；
                   若将贫贱比车马，他得驱驰我得闲；
                   世人笑我忒疯癫，我笑世人看不穿；
                   记得五陵豪杰墓，无酒无花锄作田。
==================================================================*/

#define DIRECTION_CONTROL_PERIOD        20//方向输出周期
#define OFFSET_MINIMUM                  3//方向最小输出限幅
#define DIRECTION_PWM_MAX               4000//方向分量限幅
#define DIRECTION_SET_MAX               3000//方向设定限幅
#define P_ROTATE_SPEED                  1.0//角速度测量系数


extern float gf_direction_out_old;//保留上一次方向控制输出量
extern float gf_direction_out_new;//新的方向控制输出量
extern uint8 gc_direction_period;//方向控制周期


float Linear_Offset(float offset);//线性化处理，差速与信号偏差成线性关系

void Direction_Control();//位置式PD控制输出控制车模方向

void Direction_Control_Output();//方向输出周期控制

void Direction_Calculate();//外环赛道偏差控制方向

void Angularspeed_Control_Direction(int32 set);//内环角速度控制转向



#endif 