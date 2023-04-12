#include "include.h"

#define LEFT_K 1.00
#define RIGHT_K 1.00

float gl_motor_left_k = LEFT_K;
float gl_motor_right_k = RIGHT_K;

void Motor_Init()
{ 
    Motor_PWM_Init();
       
    //控制方向的引脚
    GPIO_Init(RIGHT_MOTOR_DIR_PORT, RIGHT_MOTOR_DIR_PIN_MASK, DIR_OUTPUT, RIGHT_MOTOR_FORWARD_LV);
    GPIO_Init(LEFT_MOTOR_DIR_PORT, LEFT_MOTOR_DIR_PIN_MASK, DIR_OUTPUT, LEFT_MOTOR_FORWARD_LV);
}

void Set_Motor_PWM(int16 i16MotorPWM, uint8 u8MotorSele)//PWM正为正转，否则反转
{
    if (u8MotorSele == LEFT_MOTOR)//左电机
    {
        if (i16MotorPWM > 0)
        {
            Change_PWM_Motor_Left(i16MotorPWM);
            LPLD_GPIO_Output_b(LEFT_MOTOR_DIR_PORT, LEFT_MOTOR_DIR_PIN, LEFT_MOTOR_FORWARD_LV);
        }
        else if (i16MotorPWM < 0)
        {
            Change_PWM_Motor_Left(-i16MotorPWM);
            LPLD_GPIO_Output_b(LEFT_MOTOR_DIR_PORT, LEFT_MOTOR_DIR_PIN, LEFT_MOTOR_BACKWARD_LV);
        }
        else
        {
            Change_PWM_Motor_Left(0);
        }
    }
    else//右电机
    {
        if (i16MotorPWM > 0)
        {
            Change_PWM_Motor_Right(i16MotorPWM);
            LPLD_GPIO_Output_b(RIGHT_MOTOR_DIR_PORT, RIGHT_MOTOR_DIR_PIN, RIGHT_MOTOR_FORWARD_LV);
        }
        else if (i16MotorPWM < 0)
        {
            Change_PWM_Motor_Right(-i16MotorPWM);
            LPLD_GPIO_Output_b(RIGHT_MOTOR_DIR_PORT, RIGHT_MOTOR_DIR_PIN, RIGHT_MOTOR_BACKWARD_LV);
        }
        else
        {
            Change_PWM_Motor_Right(0);
        }
    }	
}

//电机输出
void Motor_Output()
{
    int16 i16CarLeftPWM;
    int16 i16CarRightPWM;
  
    Speed_PWM_Delta(&i16CarLeftPWM ,&i16CarRightPWM ,gl_pwm_out);
    
    //gl_div_pwm_set = gl_direction_set;
        
//    i16CarLeftPWM = gl_pwm_out; //+ gl_div_pwm_out;
//    i16CarRightPWM = gl_pwm_out; //- gl_div_pwm_out;
      
    //需要乘系数
    //限幅
    i16CarLeftPWM = (int16)Limiter(i16CarLeftPWM, -MAX_MOTOR_PWM, MAX_MOTOR_PWM);
    i16CarRightPWM = (int16)Limiter(i16CarRightPWM, -MAX_MOTOR_PWM, MAX_MOTOR_PWM);       
    
    if(guc_car_run_flag == CAR_STOP)
    {
        i16CarLeftPWM = i16CarRightPWM = 0;
    }
    
    Set_Motor_PWM(i16CarLeftPWM, LEFT_MOTOR);            
    Set_Motor_PWM(i16CarRightPWM, RIGHT_MOTOR);  
}
void CarStop()
{
    guc_car_run_flag = CAR_STOP;
}
void CarRestart()
{
    guc_car_run_flag = CAR_RUN ;
}