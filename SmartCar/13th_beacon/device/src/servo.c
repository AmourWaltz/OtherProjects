/*=============================================
    @file       servo.c
    @brief	舵机输出函数
    @programmer XBY
===============================================*/

#include "servo.h"
#include "include.h"


void Servo_Init(void)
{
    Servo_PWM_Init();    
    Sevro_Output(0);
}

void Servo_PWM(uint16 PWMDuty)
{
    PWMDuty = (uint16)(Limiter(PWMDuty,Servo_min_PWM , Servo_Max_PWM));//舵机输出限幅
    Change_PWM_Servo(PWMDuty);
}

void Sevro_Output(int16 dir)
{
    gl_servo_pwm =dir + SERVO_CENTER;
    Servo_PWM(gl_servo_pwm);
}

int16 Get_Servo_PWM_Delta(void)
{
    int16 temp = 0;
    temp = gl_servo_pwm - SERVO_CENTER;
    return temp;
}