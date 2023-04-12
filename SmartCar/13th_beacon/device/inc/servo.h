/*=============================================
    @file  servo.h
    @brief	
    @function
===============================================
*/

#ifndef _SERVO_H_
#define _SERVO_H_

#include "common.h"
#include "pwm.h"


#define  SERVO_CENTER            520//舵机中心值对应的PWM
//#define  SERVO_CENTER            665//舵机中心值对应的PWM

#define  Servo_Max_PWM          (SERVO_CENTER+75)
#define  Servo_min_PWM          (SERVO_CENTER-75)



void Servo_Init(void);
void Servo_PWM(uint16 PWMDuty);
void Sevro_Output(int16 dir);
int16 Get_Servo_PWM_Delta(void);

#endif