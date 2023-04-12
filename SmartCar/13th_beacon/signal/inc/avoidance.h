#ifndef _AVOIDANCE_H_
#define _AVOIDANCE_H_

#include "common.h"
#include "math.h"
#include "include.h"

#define TELE_ADC_ID                ADC0
#define TELE_ADC_CH                AD20

#define NO_BARRIER                 0

void TELE_AD_Init();
void Get_TELE_ADC();
void Barrier_Distance();

#endif