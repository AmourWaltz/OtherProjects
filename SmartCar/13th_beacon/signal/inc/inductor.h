#ifndef _INDUCTOR_H_
#define _INDUCTOR_H_

#include "common.h"

#define ULTRA_1_ADC_ID                ADC0
#define ULTRA_1_ADC_CH                AD16

#define ULTRA_2_ADC_ID                ADC0
#define ULTRA_2_ADC_CH                AD17

#define ULTRA_3_ADC_ID                ADC0
#define ULTRA_3_ADC_CH                AD20

#define ULTRA_4_ADC_ID                ADC1
#define ULTRA_4_ADC_CH                AD20

#define ULTRA_5_ADC_ID                ADC1
#define ULTRA_5_ADC_CH                DAD1

#define ULTRA_6_ADC_ID                ADC1
#define ULTRA_6_ADC_CH                DAD3

#define AVE_TIMES                     (3)//均值滤波次数
#define NUM_ULTRASONIC                (6)//超声波通道数量
#define NUM_ULTRASONIC_HISTORY        (5)//超声波值历史值记录长度
#define JUMP_MAX_TIMES                (LEAST_SQUARES_NUMBERS)//超声波值防跳变滤波器最大跳变次数

#define ULTRA_FRONT                   (0)
#define ULTRA_BACK                    (1)
#define ULTRA_LEFT_F                  (2)
#define ULTRA_RIGHT_F                 (3)
#define ULTRA_LEFT_B                  (4)
#define ULTRA_RIGHT_B                 (5)

#define ORIGINAL_LINEAR_X             (10000)
#define ORIGINAL_LINEAR_Y             (1)

#define PI                            3.1415

void ADC_Ultrasonic_Init();
uint16 Get_Ultrasonic_ADC(uint8 inductor_num);
void Get_Ultrasonic_Value_Ave();
void Filter_Ultrasonic_Value(int16 ultrasonic_value[]);
void Get_Standard_Ultrasonic(int16 ultrasonic_value[]);
void Directional_Calculate();
void Get_Ultrasonic_Value(void);


#endif