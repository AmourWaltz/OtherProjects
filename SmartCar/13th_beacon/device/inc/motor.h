#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "GPIO.h"
#include "PWM.h"	

#define CAR_RUN                         1
#define CAR_STOP                        0

#define GPIO_LV_HIGH                    OUTPUT_H
#define GPIO_LV_LOW                     OUTPUT_L

#define MAX_MOTOR_PWM                   8000


/* 左右电机 */
#define LEFT_MOTOR                      0	
#define RIGHT_MOTOR	                1

#define LEFT_MOTOR_FORWARD_LV           GPIO_LV_LOW
#define LEFT_MOTOR_BACKWARD_LV          GPIO_LV_HIGH

#define RIGHT_MOTOR_FORWARD_LV          GPIO_LV_LOW
#define RIGHT_MOTOR_BACKWARD_LV         GPIO_LV_HIGH


/* 引脚定义 */
#define LEFT_MOTOR_DIR_PORT             PTC
#define LEFT_MOTOR_DIR_PIN              4
#define LEFT_MOTOR_DIR_PIN_MASK         GPIO_Pin4

#define RIGHT_MOTOR_DIR_PORT            PTC
#define RIGHT_MOTOR_DIR_PIN             2
#define RIGHT_MOTOR_DIR_PIN_MASK        GPIO_Pin2

void Motor_Init();
void Set_Motor_PWM(int16 i16MotorPWM, uint8 u8MotorSele);//PWM正为正转，否则反转
void Motor_Output();
void CarStop();
void CarRestart();
#endif