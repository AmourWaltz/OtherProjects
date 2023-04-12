#ifndef _SWITCH_H_
#define _SWITCH_H_

#include "common.h"
#include "include.h"

/*===============================================================
        @note      缟素临江誓灭胡，雄师百万气吞吴；
                   试看天堑投鞭渡，不信中原不姓朱。
==================================================================*/

/*拨码器*/
#define       SW_ALL                        0xff
#define       SW0_0                         0
#define       SW0_1                         1
#define       SW1_0                         2
#define       SW1_1                         3
#define       SW1_2                         4
#define       SW1_3                         5

#define       SW0_BIT0_PORT                 PTD
#define       SW0_BIT1_PORT                 PTD
#define       SW0_BIT0_PIN                  5 
#define       SW0_BIT1_PIN                  4
#define       SW0_BIT0_PIN_MASK             GPIO_Pin5
#define       SW0_BIT1_PIN_MASK             GPIO_Pin4

#define       SW1_BIT0_PORT                 PTC
#define       SW1_BIT1_PORT                 PTC
#define       SW1_BIT2_PORT                 PTC
#define       SW1_BIT3_PORT                 PTC
#define       SW1_BIT0_PIN                  9
#define       SW1_BIT1_PIN                  8
#define       SW1_BIT2_PIN                  11
#define       SW1_BIT3_PIN                  10
#define       SW1_BIT0_PIN_MASK             GPIO_Pin9
#define       SW1_BIT1_PIN_MASK             GPIO_Pin8
#define       SW1_BIT2_PIN_MASK             GPIO_Pin11
#define       SW1_BIT3_PIN_MASK             GPIO_Pin10


void Switch_Init();//初始化拨码

uint8 Get_Switch_Value(uint8 *uc_switch_pin_value, uint8 uc_switch_pin);//返回开关全部位值

uint8 Switch_Read_Value(uint8 uc_switch_pin);//读取拨码开关值

#endif