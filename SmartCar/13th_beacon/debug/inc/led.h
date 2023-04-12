#ifndef _led_H__
#define _led_H__

#include "include.h"
#include "common.h"

/*LED*/
//#define LED_HIGH_ACTIVE
#define LED_LOW_ACTIVE

#define LED_1 1
#define LED_2 2
#define LED_3 3
#define LED_4 4


#ifdef LED_HIGH_ACTIVE
    #define LED_BIT_ON OUTPUT_H
    #define LED_BIT_OFF OUTPUT_L
#endif

#ifdef LED_LOW_ACTIVE
    #define LED_BIT_ON OUTPUT_L
    #define LED_BIT_OFF OUTPUT_H
#endif

#define LED_PIN_1_PORT PTD
#define LED_PIN_2_PORT PTD
#define LED_PIN_3_PORT PTD
#define LED_PIN_4_PORT PTD

#define LED_PIN_1 14
#define LED_PIN_2 15
#define LED_PIN_3 12
#define LED_PIN_4 13

#define LED_PIN_1_MASK GPIO_Pin14
#define LED_PIN_2_MASK GPIO_Pin15
#define LED_PIN_3_MASK GPIO_Pin12
#define LED_PIN_4_MASK GPIO_Pin13



void LED_Init();
void SetLEDBit(uint8 u8LEDBitValue, uint8 u8LEDBit);
void ChangeLEDBit(uint8 u8LEDBit);
void SetLED(uint8 u8LEDValue);   


#endif 