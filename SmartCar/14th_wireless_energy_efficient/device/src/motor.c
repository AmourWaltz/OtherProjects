/*=============================================
    @file         motor.c
    @brief        电机文件
    @programmer   Beyond Hsueh
=============================================*/

#include "motor.h"

/*===============================================================
        @note      春游浩荡，是年年、寒食梨花时节；
                   白锦无纹香烂漫，玉树琼葩堆雪。
                   静夜沉沉，浮光霭霭，冷浸溶溶月；
                   人间天上，烂银霞照通彻。
                   浑似姑射真人，天姿灵秀，意气舒高洁；
                   万化参差谁信道，不与群芳同列。
                   浩气清英，仙材卓荦，下土难分别；
                   瑶台归去，洞天方看清绝。
==================================================================*/     

/*===============================================================
	@brief     电机输出及方向初始化
==================================================================*/
void Motor_Init()
{ 
    Motor_PWM_Init();
       
    //控制方向的引脚
    GPIO_Init(RIGHT_MOTOR_DIR_PORT, RIGHT_MOTOR_DIR_PIN_MASK, DIR_OUTPUT, RIGHT_MOTOR_FORWARD_LV);
    GPIO_Init(LEFT_MOTOR_DIR_PORT, LEFT_MOTOR_DIR_PIN_MASK, DIR_OUTPUT, LEFT_MOTOR_FORWARD_LV);    
}

/*===============================================================
	@brief     正反转控制
	@note      PWM正为正转，否则反转，单极性控制
==================================================================*/
void Set_Motor_PWM(int16 i_motor_PWM, uint8 uc_motor_sele)
{
    if (uc_motor_sele == LEFT_MOTOR)//左电机
    {
        if (i_motor_PWM > 0)
        {
            Change_PWM_Motor_Left(i_motor_PWM);
            LPLD_GPIO_Output_b(LEFT_MOTOR_DIR_PORT, LEFT_MOTOR_DIR_PIN, LEFT_MOTOR_FORWARD_LV);
        }
        else if (i_motor_PWM < 0)
        {
            Change_PWM_Motor_Left(-i_motor_PWM);
            LPLD_GPIO_Output_b(LEFT_MOTOR_DIR_PORT, LEFT_MOTOR_DIR_PIN, LEFT_MOTOR_BACKWARD_LV);
        }
        else
        {
            Change_PWM_Motor_Left(0);
        }
    }
    else//右电机
    {
        if (i_motor_PWM > 0)
        {
            Change_PWM_Motor_Right(i_motor_PWM);
            LPLD_GPIO_Output_b(RIGHT_MOTOR_DIR_PORT, RIGHT_MOTOR_DIR_PIN, RIGHT_MOTOR_FORWARD_LV);
        }
        else if (i_motor_PWM < 0)
        {
            Change_PWM_Motor_Right(-i_motor_PWM);
            LPLD_GPIO_Output_b(RIGHT_MOTOR_DIR_PORT, RIGHT_MOTOR_DIR_PIN, RIGHT_MOTOR_BACKWARD_LV);
        }
        else
        {
            Change_PWM_Motor_Right(0);
        }
    }	
}


/*===============================================================
	@brief     电机输出
==================================================================*/
void Motor_Output()
{             
    int16 i_car_left_PWM;
    int16 i_car_right_PWM;
//    int32 car_speed = 0;
    static uint8 suc_wait_start_flag = 0;

//    car_speed = gl_car_speed;
//    Speed_PWM_Delta(&i_car_left_PWM ,&i_car_right_PWM ,gl_pwm_out);
    
    //gl_div_pwm_set = gl_direction_set;
        
    i_car_left_PWM = gl_pwm_output - gl_direction_output;
    i_car_right_PWM = gl_pwm_output + gl_direction_output;
      
    //需要乘系数
    //限幅
    i_car_left_PWM = (int16)Limiter(i_car_left_PWM, -MAX_MOTOR_PWM, MAX_MOTOR_PWM);
    i_car_right_PWM = (int16)Limiter(i_car_right_PWM, -MAX_MOTOR_PWM, MAX_MOTOR_PWM);       
    
    if(guc_car_run_flag == CAR_RUN)
    {
          suc_wait_start_flag = 1;
    }
    else if((guc_car_run_flag == CAR_STOP) && (suc_wait_start_flag == 1))//反向制动
    {
//        i_car_left_PWM = (int16)((gf_speed_i/10) * (0 - car_speed));
//        i_car_right_PWM = (int16)((gf_speed_i/10) * (0 - car_speed));  
        i_car_left_PWM = 0;
        i_car_right_PWM = 0;          
    }

    if(!suc_wait_start_flag)
    {
        i_car_left_PWM = 0;
        i_car_right_PWM = 0;      
    }
    
    Set_Motor_PWM(i_car_left_PWM, LEFT_MOTOR);            
    Set_Motor_PWM(i_car_right_PWM, RIGHT_MOTOR);   

}

/*===============================================================
	@brief     紧急刹车
==================================================================*/
void Car_Stop()
{
    guc_car_run_flag = CAR_STOP;
    Change_PWM_Motor_Left(0);
    Change_PWM_Motor_Right(0);
}

/*===============================================================
	@brief     电机重新上电
==================================================================*/
void Car_Restart()
{
//    guc_car_run_flag = CAR_RUN ;
}

