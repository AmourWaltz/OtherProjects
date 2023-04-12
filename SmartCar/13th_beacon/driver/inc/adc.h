#ifndef _ADC_H
#define _ADC_H

#include "common.h"

#define BAT_ADC_ID        ADC1
#define BAT_ADC_CH        AD16

#define CURRENT_LOOP_REF_ADC_ID          ADC0
#define CURRENT_LOOP_REF_ADC_CH          AD12

#define CURRENT1_LOOP_ADC_ID         ADC1
#define CURRENT1_LOOP_ADC_CH         AD15

#define CURRENT2_LOOP_ADC_ID         ADC1
#define CURRENT2_LOOP_ADC_CH         AD14

void ADC_Init();

void ADC0_Init();
void ADC1_Init();

void ADC_CH_Init();

void ADC_0_isr();
void ADC_1_isr();

float ADC_Bat_Vol(); 

#endif