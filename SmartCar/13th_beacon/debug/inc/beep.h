#ifndef _beep_H__
#define _beep_H__

#include "include.h"
#include "common.h"

/*·äÃùÆ÷*/

#define BEEP_PORT               PTE
#define BEEP_PIN                26
#define BEEP_PIN_MASK           GPIO_Pin26

#define BEEP_ON_LV          OUTPUT_H
#define BEEP_OFF_LV         OUTPUT_L


void Beep_Init();
void Beep_On();
void Beep_Off();
void Get_Beep_State(uint8 *u8State);

#endif 