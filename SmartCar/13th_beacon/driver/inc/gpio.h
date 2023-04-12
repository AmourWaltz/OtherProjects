#ifndef __GPIO_H__
#define __GPIO_H__

#include "common.h"
#include "HW_GPIO.h"


#define INPUT_L OUTPUT_L        
#define INPUT_H OUTPUT_H     


////////////////////外部调用////////////////////
void GPIO_Init(GPIO_Type *PTx, uint32 GPIO_Pinx, uint8 Dir, uint8 Level);
////////////////////////////////////////////////

#endif 