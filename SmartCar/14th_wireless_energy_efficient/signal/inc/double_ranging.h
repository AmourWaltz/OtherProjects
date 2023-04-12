#ifndef _RANGING_H_
#define _RANGING_H_

#include "common.h"
#include "math.h"
#include "include.h"

/*===============================================================
        @note      东风夜放花千树，更吹落、星如雨。
                   宝马雕车香满路，
                   凤箫声动，玉壶光转，一夜鱼龙舞。
                   蛾儿雪柳黄金缕，笑语盈盈暗香去。
                   众里寻他千百度，
                   蓦然回首，那人却在，灯火阑珊处。
==================================================================*/    

#define INFRARED_UP                     0
#define INFRARED_DOWN                   1

#define INFRARED_UP_ADC_ID              ADC0
#define INFRARED_UP_ADC_CH              AD17

#define INFRARED_BOTTOM_ADC_ID          ADC1
#define INFRARED_BOTTOM_ADC_CH          AD15

#define NO_OBSTACLE                0
#define OBSTACLE_RANGING_MAX       105//路障测量最大阈值
#define OBSTACLE_RANGING_MIN       45//路障测量最大阈值
#define OBSTACLE_COUNT             2//连续检测障碍计数
#define DETECTION_COUNT            2//区分障碍坡道计数
#define OBSTACLE_AVERAGE           3//均值滤波次数 



void Infrared_AD_Init();//红外测距AD初始化

uint32 Get_Infrared_ADC_Ave(uint8 infrared_num);//红外AD均值滤波



#endif