/*=============================================
    @file         signal.c
    @brief        信号处理
    @programmer   Beyond Hsueh
=============================================*/

#include "signal.h"

/*===============================================================
        @note      黄金榜上，偶失龙头望。
                   明代暂遗贤，如何向。
                   未遂风云便，争不恣狂荡，何须论得丧。
                   才子词人，自是白衣卿相。
                   烟花巷陌，依约丹青屏障，
                   幸有意中人，堪寻访，
                   且恁偎红倚翠，风流事，平生畅，青春都一饷。
                   忍把浮名，换了浅斟低唱。
==================================================================*/ 

static int16 si_inductor_value_history[NUM_INDUCTOR][NUM_INDUCTOR_HISTORY] = {0};//电感信号历史值 -- 滤波后 -- 用于信号处理,只在防跳变滤波器中更新

/*===============================================================
	@brief     信号中断调用函数
        @note      两横两竖特征变化最明显基本可以解决大部分问题
==================================================================*/
void Signal_Control()
{
  
    Get_Signal_Value();
    
    if(!guc_obstacle_flag)
    {
        Get_Sensor_Offset(gi_inductor_value);
    }

    gl_car_speed_set = gl_speed_straight;

    static int32 stopline_distance;//检测到停车线时行驶路程

    Get_Signal_Derivative();
    gi_x_signal_gradient = gi_signal_gradient[X_LEFT] + gi_signal_gradient[X_RIGHT];
    gi_m_signal_gradient = gi_signal_gradient[M_BACK];
    gi_y_signal_gradient = gi_signal_gradient[T_LEFT] + gi_signal_gradient[T_RIGHT];
    
    if(!guc_obstacle_flag)
    {
        Distinguish_Obstacle();
    }

    if((guc_car_run_flag == CAR_RUN) && (gl_car_running_distance > 3))
    {
        switch(guc_distinction_flag)     //当已经识别到一种元素后，停止识别其他元素
        {
          case GENERAL:
            guc_lost_signal_flag = Judge_Lost_Signal();//丢线识别
    //        guc_curve_flag = Judge_Curve();//弯道识别
    //        guc_straight_flag = Judge_Straight();//直道识别
            guc_slope_state = Judge_Slope();//坡道识别
    //        guc_crucifix_flag = Judge_Crucifix();//十字识别
            guc_annulus_state = Judge_Annulus();//圆环识别
            guc_obstacle_state = Jugde_Obstacle();//路障识别
            break;
          case ANNULUS:
            guc_annulus_state = Judge_Annulus();//圆环识别
    //        guc_lost_signal_flag = Judge_Lost_Signal();//丢线识别
    //        guc_obstacle_state = Jugde_Obstacle();//路障识别
            break;
          case SLOPE:
            guc_slope_state = Judge_Slope();//坡道识别
    //        guc_lost_signal_flag = Judge_Lost_Signal();//丢线识别
    //        guc_curve_flag = Judge_Curve();//弯道识别
    //        guc_crucifix_flag = Judge_Crucifix();//十字识别
            guc_annulus_state = Judge_Annulus();//圆环识别
            guc_obstacle_state = Jugde_Obstacle();//路障识别
            break;
          case OBSTACLE:
            guc_obstacle_state = Jugde_Obstacle();//路障识别
            break;        
          default :
            guc_lost_signal_flag = Judge_Lost_Signal();//丢线识别
    //        guc_curve_flag = Judge_Curve();//弯道识别
    //        guc_straight_flag = Judge_Straight();//直道识别
            guc_crucifix_flag = Judge_Crucifix();//十字识别
            guc_slope_state = Judge_Slope();//坡道识别
            guc_annulus_state = Judge_Annulus();//圆环识别
            guc_obstacle_state = Jugde_Obstacle();//路障识别
            break;
        }
        
        if((guc_distinction_flag == NORMAL) && (guc_straight_flag))    //直道标定
        {
            Calibrate_Once();
        }
        else
        {
            End_Calibration();
        }
        
    }
   


    
    if(gl_voltage_input > 20000)    //经过发射线圈,不识别特殊元素
    {
        guc_slope_state = 0;
        guc_annulus_state = 0;
        guc_obstacle_state = 0;
//        gl_car_error = 0;
    }
    
    if(guc_obstacle_state)
    {
        if(guc_obstacle_state%2)
        {
            Beep_On();
        }
        else
        {
            Beep_Off();
        }
        
        guc_distinction_flag = OBSTACLE;
        
        Obstacle_Process();
    }
    else if(guc_annulus_state)//环岛
    {
        if(guc_annulus_state%2)
        {
            Beep_On();
        }
        else
        {
            Beep_Off();
        }
        
        guc_distinction_flag = ANNULUS;
        
        Annulus_Process();
    }
    else if(guc_slope_state)
    {
        if(guc_slope_state == ON_RAMP)
        {
            Beep_Off();
        }
        else
        {
            Beep_On();
        }
        guc_distinction_flag = SLOPE;

        Slope_Process();
    }
//    else if(guc_lost_signal_flag) //丢信号
//    {
//        Lost_Signal_Process();
//        Beep_On();
//    }
//    else if(guc_crucifix_flag)    //十字
//    {
//        Beep_On();
//        guc_distinction_flag = CRUCIFIX;
//        Crucifix_Process();
//    }
    else    //正常
    {
        guc_distinction_flag = GENERAL;
        Beep_Off();
        //General_Process();
    }
    

    
    /*****************     自动停车检测，后期投入使用     *********************/
    if((guc_reed_state_value > 0) && (gul_time_start > 100000) && (gl_car_running_distance > gl_car_destination_distance))
    {
        guc_destination_flag = 1;
    }
    
    if(guc_destination_flag)
    {
        if(stopline_distance == 0)
        {
            stopline_distance = gl_car_running_distance;
        }
        else if(gl_car_running_distance - stopline_distance > 100)
        {
            gl_car_speed_set = 0;
            guc_car_run_flag = 0;
        }
        else
        {
            Beep_On();
        }
    }
    
    if(gl_car_running_distance > gl_car_stopline_distance)
    {
            guc_car_run_flag = 0;
    }

}


/*===============================================================
	@brief     获取电感AD转换值
==================================================================*/
void Get_Inductor_Value()
{   
    vint16 inductor_value[NUM_INDUCTOR];
    Inductor_All_Get(inductor_value);
    
#ifdef T_LEFT
       gi_inductor_adc[T_LEFT] = inductor_value[T_LEFT];
       gi_inductor_value[T_LEFT] = inductor_value[T_LEFT];
#endif

#ifdef T_RIGHT
       gi_inductor_adc[T_RIGHT] = inductor_value[T_RIGHT];
       gi_inductor_value[T_RIGHT] = inductor_value[T_RIGHT];
#endif
        
#ifdef M_BACK
       gi_inductor_adc[M_BACK] = inductor_value[M_BACK];
       gi_inductor_value[M_BACK] = inductor_value[M_BACK];
#endif

#ifdef M_FRONT
       gi_inductor_adc[M_FRONT] = inductor_value[M_FRONT]; 
       gi_inductor_value[M_FRONT] = inductor_value[M_FRONT];
#endif 

#ifdef X_LEFT
       gi_inductor_adc[X_LEFT] = inductor_value[X_LEFT];
       gi_inductor_value[X_LEFT] = inductor_value[X_LEFT];
#endif

#ifdef X_RIGHT
       gi_inductor_adc[X_RIGHT] = inductor_value[X_RIGHT]; 
       gi_inductor_value[X_RIGHT] = inductor_value[X_RIGHT];
#endif 
       
}

/*===============================================================
	@brief     偏离赛道危险情况停车函数
	@note      电感值跳变判断后停车保护
==================================================================*/
void Stop_Danger_Lost_Signals(vint16 p_inductor_value[])
{
    int flag = 0, i = 0;
    static int danger_count = 0;
    
    if (guc_car_run_flag == FLAG_ON)
    {
        for (i = 0; i < NUM_INDUCTOR; i++)
        {
            if (p_inductor_value[i] <= 10)
            {
                flag++;
            }
            else
            {
                break;
            }
        }

        //所有电感值信号均 < 1
        if (flag >= NUM_INDUCTOR-1)
        {
            danger_count++;   
        }
        else
        {
            danger_count = 0;
        }
    
        if (danger_count >= 200) //持续，停车
        {
            danger_count = 201;
            guc_car_run_flag = FLAG_OFF;
        } 
    }
    else
    {
        danger_count = 0;
    }  
}

/*===============================================================
	@brief     电感值防跳变最小二乘法滤波器
==================================================================*/
void Filter_Inductor_Value(vint16 inductor_value[])
{
    static int16 last_inductor_value[NUM_INDUCTOR] = {0};
    //电感值跳变计数，超过一定次数，退出电感值跳变状态，保护。
    static int inductor_value_jump_times[NUM_INDUCTOR] = {0};
    int i = 0, j = 0;
 
    for(i = 0; i < NUM_INDUCTOR; i++)
    {
	last_inductor_value[i] = si_inductor_value_history[i][0];
    }
    
    for(i = 0; i < NUM_INDUCTOR; i++)
    {
	if(inductor_value[i] <= last_inductor_value[i])
	{
            if(inductor_value[i] < (last_inductor_value[i] - 15))
            {//用滤波后的电感值数组滤波
                last_inductor_value[i] = least_squares_value(si_inductor_value_history[i]);
		inductor_value_jump_times[i] ++;
                
                //跳变超过一定次数，认为当前电感值为正确值
		if(inductor_value_jump_times[i] >= JUMP_MAX_TIMES)
		{
                    last_inductor_value[i] = inductor_value[i];
                    inductor_value_jump_times[i] = 0;
		}
            }
            else if(inductor_value[i] < (last_inductor_value[i] - 10))
            {
                last_inductor_value[i] = last_inductor_value[i] - 5;
		inductor_value_jump_times[i] = 0;
            }
            else
            {
		last_inductor_value[i] = inductor_value[i];
		inductor_value_jump_times[i] = 0;
            }
        }
        else
        {
            if(inductor_value[i] > (last_inductor_value[i] + 15))
            {
		last_inductor_value[i] = least_squares_value(si_inductor_value_history[i]);
		inductor_value_jump_times[i] ++;

		if(inductor_value_jump_times[i] >= JUMP_MAX_TIMES)
		{
                    last_inductor_value[i] = inductor_value[i];
                    inductor_value_jump_times[i] = 0;
		}
            }
            else if(inductor_value[i] > (last_inductor_value[i] + 10))
            {
                last_inductor_value[i] = last_inductor_value[i] + 5;
                inductor_value_jump_times[i] = 0;
            }
            else
            {
		last_inductor_value[i] = inductor_value[i];
		inductor_value_jump_times[i] = 0;
            }
	}
    }
    //更新电感值
    for(i = 0; i < NUM_INDUCTOR; i++)
    {
	inductor_value[i] = last_inductor_value[i];
    }
    //更新历史电感值
    for(i = 0; i < NUM_INDUCTOR; i++)
    {
	for(j = NUM_INDUCTOR_HISTORY-1; j >= 1; j--)
	{			
            si_inductor_value_history[i][j] = si_inductor_value_history[i][j-1];
	}
	si_inductor_value_history[i][0] = inductor_value[i];
    }
}

/*===============================================================
	@brief     获取电感差比和
==================================================================*/
void Get_Sensor_Offset(vint16 sensor_value[])
{
    //static int16  s_sensor_ad_value[NUM_INDUCTOR] = {0};//上次传感器的值
    int16 x0,x1,x2,x3;
    float  f_sensor_offset_x_temp = 0;
    float  f_sensor_offset_t_temp = 0;
    float  f_sensor_difference_t_temp = 0;    
    //int  i = 0;

    //int16  x_sensor_value[NUM_INDUCTOR] = {0};

//    x_sensor_value[0] = sensor_value[T_LEFT];      //X0
//    x_sensor_value[1] = sensor_value[X_LEFT];      //X1
//    x_sensor_value[2] = sensor_value[X_RIGHT];      //X2
//    x_sensor_value[3] = sensor_value[T_RIGHT];      //X3 
    
    x0 = sensor_value[T_LEFT];      //X0
    x1 = sensor_value[X_LEFT];      //X1
    x2 = sensor_value[X_RIGHT];      //X2
    x3 = sensor_value[T_RIGHT];      //X3 

    //更新上次传感器的值，前已滤波
//    for(i = 0; i < NUM_INDUCTOR; i++)
//    { 
//	if(x_sensor_value[i] < s_sensor_ad_value[i] - 5)
//	{
//            s_sensor_ad_value[i] = s_sensor_ad_value[i] - 3;
//	}
//	else if(x_sensor_value[i] > s_sensor_ad_value[i] + 5)
//	{
//            s_sensor_ad_value[i] = s_sensor_ad_value[i] + 3;
//	}
//	else
//	{ 
//            s_sensor_ad_value[i] = x_sensor_value[i];
//	}
//    }
//    
//    x0 = s_sensor_ad_value[0];
//    x1 = s_sensor_ad_value[1];
    
    f_sensor_offset_t_temp = (float)(ORIGINAL_LINEAR_PROPORTION_T) * (float)(x0 - x3)/((float)(x0 + x3 + 1));
    f_sensor_offset_x_temp = (float)(ORIGINAL_LINEAR_PROPORTION_X) * (float)(x1 - x2)/((float)(x2 + x1 + 1));
    f_sensor_difference_t_temp = (float)(ORIGINAL_LINEAR_PROPORTION_T) * (float)(x3 - x0)/((float)(x1 + x2 + 1));

    if((guc_direction_flag == LEFT)&&(f_sensor_offset_t_temp > 10))  //设置回差带，防止震荡
    {
        guc_direction_flag = RIGHT;
    }
    else if((guc_direction_flag == RIGHT)&&(f_sensor_offset_t_temp < -10))
    {
        guc_direction_flag = LEFT;
    }

    gi_inductor_t_offset = (int16)((f_sensor_offset_t_temp * gui_t_weight)/100);
    gi_inductor_x_offset = (int16)((f_sensor_offset_x_temp * gui_x_weight)/100);
    gi_inductor_t_difference = (int16)((f_sensor_difference_t_temp * gui_t_div_weight)/100);
    
    gl_car_error = gi_inductor_x_offset;
    
}

/*===============================================================
	@brief     校准信号，消除零漂
==================================================================*/
void Criterion_Signal(vint16 inductor_value[])
{
    int16 inductor_value_x1,inductor_value_x2;
    int16 inductor_value_y1,inductor_value_y2;
    int16 inductor_value_m1;
    
    inductor_value_x1 = inductor_value[X_LEFT];
    inductor_value_x2 = inductor_value[X_RIGHT];
    inductor_value_y1 = inductor_value[T_LEFT];
    inductor_value_y2 = inductor_value[T_RIGHT];
    inductor_value_m1 = inductor_value[M_BACK];
    
    inductor_value_x1 -= CRITERION_SIGNAL;
    inductor_value_x2 -= CRITERION_SIGNAL;
    inductor_value_y1 -= CRITERION_SIGNAL;
    inductor_value_y2 -= CRITERION_SIGNAL;
    inductor_value_m1 -= CRITERION_SIGNAL;
    
    inductor_value_x1 = (int16)Limiter(inductor_value_x1, 0, 4095);
    inductor_value_x2 = (int16)Limiter(inductor_value_x2, 0, 4095);
    inductor_value_y1 = (int16)Limiter(inductor_value_y1, 0, 4095);
    inductor_value_y2 = (int16)Limiter(inductor_value_y2, 0, 4095);
    inductor_value_m1 = (int16)Limiter(inductor_value_m1, 0, 4095);
    
    inductor_value[X_LEFT] = inductor_value_x1;
    inductor_value[X_RIGHT] = inductor_value_x2;
    inductor_value[T_LEFT] = inductor_value_y1;
    inductor_value[T_RIGHT] = inductor_value_y2;
    inductor_value[M_BACK] = inductor_value_m1;
  

}

/*===============================================================
	@brief     信号获取处理判断
	@note      使用前进行一定时间滤波
==================================================================*/
void Get_Signal_Value()
{
    static int16 si_cnt = 0;
    int16 i = 0, j = 0;
    si_cnt++;
    Get_Inductor_Value();
    //归一化电感   
    Calibrate_All_Inductor(gi_inductor_value);//8位精度
    Criterion_Signal(gi_inductor_value);
    if (si_cnt >= 2500)//10s
    {
        si_cnt = 2501;
        Filter_Inductor_Value(gi_inductor_value);//8位精度滤波
        if(!guc_obstacle_flag)
        {
            Stop_Danger_Lost_Signals(gi_inductor_value);
        }
    }
    else
    {
        for(i = 0; i < NUM_INDUCTOR; i++)
        {
            for(j = NUM_INDUCTOR_HISTORY-1; j >= 1; j--)
            {			
                si_inductor_value_history[i][j] = si_inductor_value_history[i][j-1];
            }
            si_inductor_value_history[i][0] = gi_inductor_value[i];
        }
    }
}

/*===============================================================
	@brief     计算所有电感信号随距离变化率
==================================================================*/
int16 Get_Signal_Derivative()
{
    static int32 sl_last_annulus_distance;//入环前车程记录
    static int32 sl_distance_sigma[RING_BUFF_SIZE] ;
    static int16 si_signal_sum[NUM_INDUCTOR][RING_BUFF_SIZE];
    static uint32 sul_signal_sum_buff[NUM_INDUCTOR];
    static uint16 sui_signal_buff_count;
    int32 signal_gradient_summation[NUM_INDUCTOR];
    static int32 sl_signal_increase_rate[NUM_INDUCTOR];
    
    int32 distance_sigma_summation = 0;
    int16 i,j;
    
    //通过横电感信号变化趋势入环
    if(sui_signal_buff_count < 10000)//平均历史信号,使用原始信号，避免动态标定带来的影响
    {  
        for(i=0; i < NUM_INDUCTOR; i++)
        {
            sul_signal_sum_buff[i] += gi_inductor_adc[i];
        }
        sui_signal_buff_count++;
    }
    if(gl_car_running_distance -  sl_last_annulus_distance > 20)//等车至少跑过一段距离再检测信号强度变化
    {
        for(i=0; i < RING_BUFF_SIZE - 1; i++)//缓存历史信号强度
        {
            for(j=0; j < NUM_INDUCTOR; j++)
            {
                si_signal_sum[j][i] = si_signal_sum[j][i+1];
            }
            sl_distance_sigma[i] = sl_distance_sigma[i+1];
        }
        for(i=0; i < NUM_INDUCTOR; i++)
        {
            si_signal_sum[i][RING_BUFF_SIZE-1] = sul_signal_sum_buff[i]/sui_signal_buff_count;
            sul_signal_sum_buff[i] = 0;
            signal_gradient_summation[i] = 0;
        }
        
        sui_signal_buff_count = 0;
        sl_distance_sigma[RING_BUFF_SIZE-1] = (int32)(gl_car_running_distance - sl_last_annulus_distance);//乘系数，提高精度
        
        for(i=0; i < RING_BUFF_SIZE / 2; i++)
        {
            for(j=0; j < NUM_INDUCTOR; j++)
            {
                signal_gradient_summation[j] += si_signal_sum[j][i + RING_BUFF_SIZE/2] - si_signal_sum[j][i];
            }
            distance_sigma_summation += sl_distance_sigma[i] + sl_distance_sigma[i+RING_BUFF_SIZE/2];
        }
        for(i=0; i < NUM_INDUCTOR; i++)
        {
            sl_signal_increase_rate[i] = (100*1500/gi_inductor_calibrate_value[CALI_MID]) * signal_gradient_summation[i]/ //归一化
                        ((RING_BUFF_SIZE/2)*(RING_BUFF_SIZE/2) * distance_sigma_summation+1); //防止除零
        }
        
        sl_last_annulus_distance = (int32)gl_car_running_distance;        
    }
    
    if(gl_car_running_distance < 20)
    {
        for(i=0; i < NUM_INDUCTOR; i++)
        {
            sl_signal_increase_rate[i] = 0; 
        }
    }
    
    for(i=0; i < NUM_INDUCTOR; i++)
    {
        sl_signal_increase_rate[i] = (int32)Limiter(sl_signal_increase_rate[i],-10000,10000);  
        gi_signal_gradient[i] = sl_signal_increase_rate[i];
    }
    return 0;
}

/*===============================================================
	@brief     获取车身姿态
==================================================================*/
void Get_Position()
{
    static int16 si_roll_dps_buff[POS_BUFF_SIZE];
    static int16 si_pitch_dps_buff[POS_BUFF_SIZE];
    static uint8 suc_buff_sequence;
//    static uint8 suc_test_yaw_flag = 0;//测试偏航角标志位，发车后清零
    
    uint32 roll_square = 0, pitch_square = 0;
    int32 roll_summation = 0, pitch_summation = 0;
    int i;
    
    if(guc_gyro_zero_flag)
    {
        //采集数据
    //    Get_Acc_X();
    //    Get_Acc_Y();
    //    Get_Acc_Z();
        Get_Gyro_X();
        Get_Gyro_Y();
        Get_Gyro_Z();
        
        Get_Annulus_Angle();
        Get_Slope_Angle();
        Get_Obstacle_Angle();

        gf_roll_angle += mpu_gyro_x * (PIT_1_PERIOD_S);
        gf_pitch_angle += mpu_gyro_y * (PIT_1_PERIOD_S);
        gf_yaw_angle += mpu_gyro_z * (PIT_1_PERIOD_S);

        gf_roll_velocity = mpu_gyro_x;
        gf_pitch_velocity = mpu_gyro_y;
        gf_yaw_velocity = mpu_gyro_z;
        
        //姿态方差计算
        si_roll_dps_buff[suc_buff_sequence] = (int16)(gf_roll_velocity * 10);
        si_pitch_dps_buff[suc_buff_sequence] = (int16)(gf_pitch_velocity * 10);
        suc_buff_sequence++;
        
        if(suc_buff_sequence >= POS_BUFF_SIZE)
        {
            suc_buff_sequence = 0;
        }
        for(i=0; i < POS_BUFF_SIZE; i++)
        {
            roll_square += (int32)(si_roll_dps_buff[i] * si_roll_dps_buff[i]/POS_BUFF_SIZE);
            roll_summation += si_roll_dps_buff[i];
            pitch_square += (int32)(si_pitch_dps_buff[i] * si_pitch_dps_buff[i]/POS_BUFF_SIZE);
            pitch_summation += si_pitch_dps_buff[i];
        }
        roll_square = roll_square - (roll_summation / POS_BUFF_SIZE) * (roll_summation / POS_BUFF_SIZE);
        pitch_square = pitch_square - (pitch_summation / POS_BUFF_SIZE) * (pitch_summation / POS_BUFF_SIZE);
        gi_pitch_variance = i_sqrt(pitch_square);
        gi_roll_variance = i_sqrt(roll_square);
    }

}

/*===============================================================
	@brief     积分得到车模偏航角度，处理环岛判断是否完成一圈
==================================================================*/
void Get_Annulus_Angle()
{ 
   if(guc_annulus_yaw_flag)
   {
       gf_annulus_angle += gf_yaw_velocity * (PIT_1_PERIOD_S);
   }
   else
   {
       gf_annulus_angle = 0;
   }
}

/*===============================================================
	@brief     积分得到车模俯仰角度处理坡道
==================================================================*/
void Get_Slope_Angle()
{ 
   if(guc_slope_pitch_flag)
   {
       gf_slope_picth_angle += gf_pitch_velocity * (PIT_1_PERIOD_S);
   }
   else
   {
       gf_slope_picth_angle = 0;
   }
}

/*===============================================================
	@brief     积分得到车模偏航角度，避障使用
==================================================================*/
void Get_Obstacle_Angle()
{ 
   if(guc_obstacle_flag)
   {
       gf_camber_angle += gf_yaw_velocity * (PIT_1_PERIOD_S);
   }
   else
   {
       gf_camber_angle = 0;
   }
}
