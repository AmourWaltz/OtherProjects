#ifndef _DIRECTION_H_
#define _DIRECTION_H_

#include "common.h"
#include "include.h"

#define ANGLE_TABLE_DIMENSION           73

void Direction_Control();
int16 Servo_Angle(int angle_servo_temp);
float Linear_Offset(float offset);

#endif