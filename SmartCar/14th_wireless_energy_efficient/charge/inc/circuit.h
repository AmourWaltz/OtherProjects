#ifndef _CIRCUIT_H_
#define _CIRCUIT_H_

#include "common.h"
#include "include.h"

/*===============================================================
        @note      飞絮飞花何处是，层冰积雪摧残；
                   疏疏一树五更寒，爱他明月好，憔悴也相关。
                   最是繁丝摇落后，转教人忆春山；
                   湔裙梦断续应难，西风多少恨，吹不散眉弯。
==================================================================*/

#define NUMBER_CHANNEL                  3
#define VOLTAGE_INPUT_AVERAGE           6
#define VOLTAGE_OUTPUT_AVERAGE          6
#define CURRENT_INPUT_AVERAGE           6

#define VOLTAGE_OUTPUT_ADC_ID           ADC0
#define VOLTAGE_OUTPUT_ADC_CH           AD16

#define VOLTAGE_INPUT_ADC_ID            ADC0
#define VOLTAGE_INPUT_ADC_CH            AD10

#define CURRENT_INPUT_ADC_ID            ADC0
#define CURRENT_INPUT_ADC_CH            AD20

#define V_REF                           0
#define V_OUT                           1
#define I_IN                            2
#define V_BAT                           3



void Circuit_AD_Init();//充电电路AD初始化

void Circuit_Channel_Get(vint16 channel_value[]);//充电电路AD均值滤波

void Circuit_Signal_Get();//电路通道AD转换结果

void Circuit_Low_Battery_Detection();//电路低压检测



#endif