#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "common.h"
#include "include.h"

/*===============================================================
        @note      长安古道马迟迟，高柳乱蝉栖；
                   夕阳岛外，秋风原上，目断四天垂。
                   归云一去无踪迹，何处是前期；
                   狎兴生疏，酒徒萧索，不似去年时。
==================================================================*/

#define GPIO_LV_HIGH                    OUTPUT_H
#define GPIO_LV_LOW                     OUTPUT_L

#define MAX_MOTOR_PWM                   8000


/* 左右电机 */
#define LEFT_MOTOR                      0	
#define RIGHT_MOTOR	                1

////节能一号
//#define LEFT_MOTOR_FORWARD_LV           GPIO_LV_LOW
//#define LEFT_MOTOR_BACKWARD_LV          GPIO_LV_HIGH
//
//#define RIGHT_MOTOR_FORWARD_LV          GPIO_LV_LOW
//#define RIGHT_MOTOR_BACKWARD_LV         GPIO_LV_HIGH


//节能二号
#define LEFT_MOTOR_FORWARD_LV           GPIO_LV_HIGH
#define LEFT_MOTOR_BACKWARD_LV          GPIO_LV_LOW

#define RIGHT_MOTOR_FORWARD_LV          GPIO_LV_HIGH
#define RIGHT_MOTOR_BACKWARD_LV         GPIO_LV_LOW


/* 引脚定义 */
#define LEFT_MOTOR_DIR_PORT             PTC
#define LEFT_MOTOR_DIR_PIN              2
#define LEFT_MOTOR_DIR_PIN_MASK         GPIO_Pin2

#define RIGHT_MOTOR_DIR_PORT            PTC
#define RIGHT_MOTOR_DIR_PIN             0
#define RIGHT_MOTOR_DIR_PIN_MASK        GPIO_Pin0



void Motor_Init();//电机输出及方向初始化

void Set_Motor_PWM(int16 i_motor_PWM, uint8 uc_motor_sele);//PWM正为正转，否则反转

void Motor_Output();//电机输出

void Car_Stop();//紧急刹车

void Car_Restart();//电机重新上电


#endif