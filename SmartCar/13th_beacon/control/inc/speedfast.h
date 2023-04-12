#ifndef _SPEED_H_
#define _SPEED_H_
 
#include "include.h"

//#define AREA_THRESHOLD1         (15)//?õ¶????ÛúÁî
//#define AREA_THRESHOLD2         (25)//?õ¶????ÛúÁî
//#define AREA_THRESHOLD1          (70)//?õ¶????ÛúÁî
//#define AREA_THRESHOLD2          (150)//çæ?ª¶?

void Speed_Control();
int Set_Image_Speed();
int Set_Angle_Speed();
void Get_Car_Condition();
void Speed_PWM_Delta(int16 *left_expect_speed, int16 *right_expect_speed, int32 center_expect_speed);

#endif