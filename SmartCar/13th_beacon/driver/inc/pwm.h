#ifndef _PWM_H_
#define _PWM_H_

#include "include.h"
#include "common.h"

// 电机和舵机分别对应的PWM通道、引脚和死区

#define PWM_MOTOR_FTM			FTM0

#define PWM_LEFT_MOTOR_CH		FTM_Ch0
#define PWM_LEFT_MOTOR_PIN		PTC1
#define PWM_LEFT_MOTOR_CH1		FTM_Ch1
#define PWM_LEFT_MOTOR_PIN1		PTC2

#define PWM_RIGHT_MOTOR_CH		FTM_Ch2
#define PWM_RIGHT_MOTOR_PIN		PTC3
#define PWM_RIGHT_MOTOR_CH1		FTM_Ch3
#define PWM_RIGHT_MOTOR_PIN1	        PTC4

#define PWM_SERVO_FTM                   FTM1
#define PWM_SERVO_CH	  	        FTM_Ch1
#define PWM_SERVO_PIN		        PTA9

// 周期
#define PWM_Motor_PER_1S		(10000)    
#define PWM_Servo_PER_1S		(50)  

// 初始化占空比
#define RESET_DUTY	                (0)
#define HALF_DUTY                       (5000)

#define MAX_DUTY	                (10000)
#define MIN_DUTY	                (0)


void Motor_PWM_Init(void);

void Servo_PWM_Init(void);

void Change_PWM_Servo(uint32 DUTY);

void Change_PWM_Motor_Left(uint32 DUTY);

void Change_PWM_Motor_Right(uint32 DUTY);


#endif