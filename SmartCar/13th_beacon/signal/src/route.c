/*=============================================
    @file         route.c
    @brief        路径规划
    @programmer   XBY
=============================================*/

#include "route.h"

static int8 sc_horizon_flag;//无视野标志位
static int8 sc_barrier_flag;//障碍标志位

/*===============================================================
	@brief     路径规划
	@function  路径规划函数集合
        @note      可外部调用
==================================================================*/

void Route_Planning()
{
//    Ultra_Back();
    Car_Back();
    Horizon_Off();
    //Barrier_TELE_Avoid();
  
    if(sc_horizon_flag == 0)
    {
        Direction_Control();
    }
}

/*===============================================================
	@brief     无视野特殊情况综合判断处理
	@function  分两种情况：1.纵向差比和小于0并且超声波测距大于一定距离
                               2.图像面积很小即场外噪点或反光情况未被中值滤波滤掉
                   出现以上两种情况则完全相信超声波定向，并以之作为舵机输出
	@note      以超声波判断为准
==================================================================*/

void Horizon_Off()
{
    static int8 sc_center_flag = 0;
    
    if(guc_switch_pin_val[0] == 0)
    {
        if(guc_car_run_flag == CAR_RUN)
        {
            if(guc_find_image_flag == NOT_FOUND_IMAGE)
            {
                if(guc_beacon_directional_x == LEFT)
                {
                    Sevro_Output(75);
                }
                else if(guc_beacon_directional_x == RIGHT)
                {
                    Sevro_Output(-75);
                }
                Beep_On();
                sc_horizon_flag = 1;
            }
            else
            {
                //Direction_Control();
                Beep_Off();
                sc_horizon_flag = 0;
            }
        }
    }
      
    if(guc_switch_pin_val[0] == 1)
    {
        if(guc_car_run_flag == CAR_RUN)
        {
            if(guc_find_image_flag == NOT_FOUND_IMAGE)
            {
                if(guc_center_flag == LEFT)
                {
                    Sevro_Output(75);
                }
                else if(guc_center_flag == RIGHT)
                {
                    Sevro_Output(-75);
                }
                if((guc_center_flag == 0) && (sc_center_flag == 0))
                {
                    sc_center_flag = 1;
                    guc_center_flag = guc_beacon_directional_x;
                }
                Beep_On();
                sc_horizon_flag = 1;
            }
            else
            {
                //Direction_Control();
                Beep_Off();
                sc_horizon_flag = 0;
            }
        }
    }
}

void Ultra_Back()
{
    if(guc_car_run_flag == CAR_RUN)
    {
        if(guc_beacon_directional_y == BACK)
        {
            if(guc_beacon_directional_x == LEFT)
            {
                Change_PWM_Servo(590);
            }
            else if(guc_beacon_directional_x == RIGHT)
            {
                Change_PWM_Servo(450);
            }
            Beep_On();
        }
        else
            Beep_Off();
    }
}

/*===============================================================
	@brief     红外避障，紧急输出
	@function  当检测距离小于一定值时进行紧急打角，打角方向与切灯方向相同
        @note      不使用红外避障：1.失去视野时
                                   2.与信标距离较近时（根据测距模块或信标面积）
==================================================================*/

void Barrier_TELE_Avoid()
{   
    if(guc_car_run_flag == CAR_RUN)
    {
//        if(guc_find_image_flag != NOT_FOUND_IMAGE)
    
        if(gi_image_area_ave <= 15)
        {
           if(gc_tele_barrier_count > 2)
            {
                gc_tele_barrier_count = 3;
                if(guc_center_flag == LEFT)
                {
                    Sevro_Output(75);
                }
                else if(guc_center_flag == RIGHT)
                {
                    Sevro_Output(-75);
                }
                Beep_On();
                sc_barrier_flag = 1;
            }
            Beep_Off();
            sc_barrier_flag = 0;
        } 
        else
        {
            Beep_Off();
            //Direction_Control();
            sc_barrier_flag = 0;
        }
    }    
}

/*===============================================================
	@brief     倒车程序
	@function  前期防止撞墙，撞灯基本不会，后期防撞车
==================================================================*/

void Car_Back()
{
    static int8 sc_speed_back_count = 0;//倒车检测时间计数
    static int8 sc_car_back_flag = 0;//倒车启动标志位
    static uint32 si_speed_back_start = 0;//倒车检测起始时间
    static uint32 si_speed_back_end = 0;//倒车检测结束时间
    static uint32 si_back_count_start = 0;//倒车开始时间
    static uint32 si_back_count_end = 0;//结束倒车时间
    static uint32 si_car_back_count = 0;//倒车时间计数
    static uint32 si_restart_start = 0;//重启开始时间，作为缓冲，保护电机
    static uint32 si_restart_count = 0;//重启时间计数
    static uint32 sui_start_delay = 0;//发车后倒车开启延时
    
    sui_start_delay = gul_time_100us - gul_time_start;
    
    if((guc_car_run_flag == CAR_RUN) && (sui_start_delay >= 30000))//发车十秒后检测碰撞
    {
        if(sc_car_back_flag == 0)
        {
            si_restart_count = gul_time_100us - si_restart_start;
            
            if(si_restart_count > 2000)//重新检测延时，防止低速连续启动倒车
            {
                if((gl_car_speed < 20) || (gi_left_speed < 10) || (gi_right_speed < 10))
                {
                    if(sc_speed_back_count == 0)
                    {
                        si_speed_back_start = gul_time_100us;
                    }
                    
                    sc_speed_back_count++;

                    si_speed_back_end = gul_time_100us - si_speed_back_start;
                    
                    if(si_speed_back_end > 3000)//0.4s
                    {
                        Beep_On();
                        if(sc_speed_back_count > 10)
                        {
                            sc_speed_back_count = 0;
                            sc_car_back_flag = 1;
                            
                            //Beep_On();
                            
                            PIT_ONE_Disable(PIT_1_INIT);
                            
                            if(guc_beacon_directional_x == LEFT)
                            {
                                gi_image_err = 25;
                                Set_Motor_PWM(-6000, LEFT_MOTOR);            
                                Set_Motor_PWM(-6000, RIGHT_MOTOR); 
                            }
                            else if(guc_beacon_directional_x == RIGHT)
                            {
                                gi_image_err =  -25;
                                Set_Motor_PWM(-6000, LEFT_MOTOR);
                                Set_Motor_PWM(-6000, RIGHT_MOTOR); 
                            }
                            
                            si_back_count_start = gul_time_100us;
                        }
                        else
                        {
                            sc_speed_back_count = 0;
                        }
                    } 
                    Beep_Off();
                }          
            }
        }
        if(sc_car_back_flag == 1)
        {
            si_back_count_end = gul_time_100us;
            si_car_back_count = si_back_count_end - si_back_count_start;
            
            if(si_car_back_count > 12000)//倒车2.5s
            {
                si_restart_start = gul_time_100us;
                si_car_back_count = 0;
                sc_car_back_flag = 0;
                Set_Motor_PWM(0, LEFT_MOTOR);            
                Set_Motor_PWM(0, RIGHT_MOTOR);
                PIT_ONE_Enable(PIT_1_INIT);
                //Beep_Off();
            }
        }      
    } 
}