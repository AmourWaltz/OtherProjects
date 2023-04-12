#ifndef _switch_H_
#define _switch_H_

#include "include.h"
#include "common.h"

/*拨码器*/
//#define SWITCH_HIGH_ACTIVE  //高电平触发
#define SWITCH_LOW_ACTIVE     //低电平触发

#define       SW_ALL                        0xff
#define       SW0_0                         0
#define       SW0_1                         1

#define       SW0_BIT0_PORT                 PTC
#define       SW0_BIT1_PORT                 PTC
#define       SW0_BIT0_PIN                  18 
#define       SW0_BIT1_PIN                  19
#define       SW0_BIT0_PIN_MASK             GPIO_Pin18
#define       SW0_BIT1_PIN_MASK             GPIO_Pin19

#ifdef SWITCH_HIGH_ACTIVE
    #define     SWITCH_ON                   1
    #define     SWITCH_OFF                  0
#endif

#ifdef SWITCH_LOW_ACTIVE
    #define     SWITCH_ON                   0
    #define     SWITCH_OFF                  1
#endif

void  Switch_Init();
uint8 Switch_Get_Value(uint8 *pu8SwitchPinValue, uint8 u8SwitchPin);
uint8 Switch_Read_Value(uint8 u8SwitchPin);


#endif