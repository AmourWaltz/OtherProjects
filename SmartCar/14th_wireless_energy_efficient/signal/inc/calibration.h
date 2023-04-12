#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_

#include "common.h"
#include "include.h"

/*===============================================================
        @note      十里长亭霜满天，青丝白发度何年？
                   今生无悔今生错，来世有缘来世迁。
                   笑靥如花堪缱绻，容颜似水怎缠绵； 
                   情浓渺恰相思淡，自在蓬山舞复跹。
==================================================================*/    

#define    SENSOR_MIDDLE_NUM                   4//基准值历史值数组长度
#define    ORIGINAL_VALUE                      100//标定基准值
#define    ORIGINAL_LEAST_VALUE                80//直道横电感最小值
#define    MIDDLE_CONTINUE_TIMES               10//直道条件持续次数
#define    AD_NORMAL_ORIGINAL                  160//信号标定到0-180
#define    AD_NORMAL_MIN                       0
#define    MAX_CALI_COUNT                      100
#define    CALI_LR                             0//使用左右两边电感进行标定
#define    CALI_MID                            1//使用中间电感进行标定



void Calibrate_Once();//标定中心横电感和两侧横电感

void End_Calibration();//结束标定

void Get_Middle_Standard(int16 *sensor_middle_value);//获取中间电感准备标定

void Calibrate_All_Inductor(vint16 inductor_value[]);//归一化电感值

void Get_Standard_Inductor();//电感标定基准值


#endif