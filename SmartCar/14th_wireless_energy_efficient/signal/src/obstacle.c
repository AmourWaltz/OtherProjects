/*=============================================
    @file         obstacle.c
    @brief        路障处理
    @programmer   Beyond Hsueh
=============================================*/

#include "obstacle.h"

/*===============================================================
        @note      暗淡轻黄体性柔，情疏迹远只香留，
                   何须浅碧深红色，自是花中第一流。
                   梅定妒，菊应羞；画阑开处冠中秋，
                   骚人可煞无情思，何事当年不见收。
==================================================================*/  

static int32 sl_obstacle_distance = 0;
static uint32 sul_obstacle_start_time = 0;

/*===============================================================
	@brief     路障进程判断
==================================================================*/
uint8 Jugde_Obstacle()
{   

    int16 inductor_value_m1;    
    
    inductor_value_m1 = gi_inductor_value[M_BACK];


        
    if(guc_obstacle_state == OBSTACLE_PROCESS_1)
    {

        
        if(guc_avoidance_direction_flag == LEFT)
        {
            if(
//               (inductor_value_x2 < 5)
//                   (inductor_value_y2 < 10)//防止串道，不对另一边电感判断
//                    &&(inductor_value_x2 < 10)//防止串道，不对另一边电感判断
//                    &&(inductor_value_m1 < 10)
//                    &&
                (gl_car_running_distance - sl_obstacle_distance > 60))
            {
              if(gf_camber_angle > (gi_aviodence_angle - 5))//设置回差，防止过量调节
              {
                 guc_obstacle_state = OBSTACLE_PROCESS_2;
                 sl_obstacle_distance = gl_car_running_distance;
              }
            } 
        }            
        else if(guc_avoidance_direction_flag == RIGHT)
        {
            if(
//               (inductor_value_x1 < 5)
//                   (inductor_value_y1 < 10)//防止串道，不对另一边电感判断
//                    &&(inductor_value_x1 < 10)//防止串道，不对另一边电感判断
//                    &&(inductor_value_m1 < 10)
//                    &&
                  (gl_car_running_distance - sl_obstacle_distance > 60))
            {
              if(gf_camber_angle < -(gi_aviodence_angle - 5))
              {
                 guc_obstacle_state = OBSTACLE_PROCESS_2;
                 sl_obstacle_distance = gl_car_running_distance;
              }
            }
        }
    }
    
    else if(guc_obstacle_state == OBSTACLE_PROCESS_2)//路障第二段，绕过障碍物
    {
        if(guc_avoidance_direction_flag == LEFT)
        {
            if(
//                   (inductor_value_y2 > 5)//防止串道，不对另一边电感判断
//                    &&(inductor_value_x2 > 5)//防止串道，不对另一边电感判断
//                    &&
                  (gl_car_running_distance - sl_obstacle_distance > 40))
            {
              if(gf_camber_angle < -(gi_regression_angle - 5))
              {
                  guc_obstacle_state = OBSTACLE_PROCESS_3;
                 sl_obstacle_distance = gl_car_running_distance;
              }
            } 
        }            
        else if(guc_avoidance_direction_flag == RIGHT)
        {
            if(
//                   (inductor_value_y1 > 5)//防止串道，不对另一边电感判断
//                    &&(inductor_value_x1 > 5)//防止串道，不对另一边电感判断
//                    &&
                  (gl_car_running_distance - sl_obstacle_distance > 30))
            {
              if(gf_camber_angle > (gi_regression_angle - 5))
              {
                 guc_obstacle_state = OBSTACLE_PROCESS_3;
                 sl_obstacle_distance = gl_car_running_distance;
              }
            } 
        }
        
        
        if(guc_avoidance_direction_flag == LEFT)
        {
            if((((inductor_value_m1 > 50)
//                &&(inductor_value_m1 > 50)
            &&(fabs(gf_camber_angle) < 40))
            ||(inductor_value_m1 > 200))
            &&(gl_car_running_distance - sl_obstacle_distance > 30))
            {
                 sl_obstacle_distance = gl_car_running_distance;
                 guc_obstacle_state = 0;
                 guc_obstacle_flag = OFF;
                 guc_obstacle_complete_flag = ON;
                 sul_obstacle_start_time = 0;
                  Beep_Off();
            }         
        }            
        else if(guc_avoidance_direction_flag == RIGHT)
        {
            if((((inductor_value_m1 > 50)
//                &&(inductor_value_m1 > 50)
            &&(fabs(gf_camber_angle) < 40))
            ||(inductor_value_m1 > 200))
            &&(gl_car_running_distance - sl_obstacle_distance > 30))
            {
                 sl_obstacle_distance = gl_car_running_distance;
                 guc_obstacle_state = 0;
                 guc_obstacle_flag = OFF;
                 guc_obstacle_complete_flag = ON;
                 sul_obstacle_start_time = 0;
                 Beep_Off();
            }         
        }
    }
    
    else if(guc_obstacle_state == OBSTACLE_PROCESS_3)//路障第三段，回归正常赛道模式
    {
      
        if(guc_avoidance_direction_flag == LEFT)
        {
            if((((inductor_value_m1 > 35)
//                &&(inductor_value_m1 > 50)
            &&(fabs(gf_camber_angle) < 30))
            ||(inductor_value_m1 > 180))
            &&(gl_car_running_distance - sl_obstacle_distance > 30))
            {
                 sl_obstacle_distance = gl_car_running_distance;
                 guc_obstacle_state = 0;
                 guc_obstacle_flag = OFF;
                 guc_obstacle_complete_flag = ON;
                 sul_obstacle_start_time = 0;
                  Beep_Off();
            }         
        }            
        else if(guc_avoidance_direction_flag == RIGHT)
        {
            if((((inductor_value_m1 > 35)
//                &&(inductor_value_m1 > 50)
            &&(fabs(gf_camber_angle) < 30))
            ||(inductor_value_m1 > 180))
            &&(gl_car_running_distance - sl_obstacle_distance > 30))
            {
                 sl_obstacle_distance = gl_car_running_distance;
                 guc_obstacle_state = 0;
                 guc_obstacle_flag = OFF;
                 guc_obstacle_complete_flag = ON;
                 sul_obstacle_start_time= 0;
                 Beep_Off();
            }         
        }
        
    }
    
    gl_obstacle_distance_record = sl_obstacle_distance;
    
    return guc_obstacle_state;
}

/*===============================================================
	@brief     路障判断，主要区分路障与坡道
==================================================================*/
void Distinguish_Obstacle()
{   
    Obstacle_Detection();//测距模块检测，是否检测到物体
    Switch_Process_Selection();
    
//    if(guc_first_obstacle_slope_flag == SLOPE_FIRST && guc_slope_complete_flag)
//    {
//      if((gl_car_running_distance - gl_slope_distance_record) > 200)//坡道完成一米以内不识别障碍
//      {
//        if(guc_infrared_ranging_flag == ON && !guc_obstacle_complete_flag)
//        {
//          if(!guc_obstacle_state)
//          {
//            guc_obstacle_flag = ON;
//            guc_obstacle_state = OBSTACLE_PROCESS_1;
//            sl_obstacle_distance = gl_car_running_distance;
//          }
//        }
//      }
//    }
//    //路障优先，坡道未完成且红外测距有效，需要加入路障完成标志防止误判
//    else if(guc_first_obstacle_slope_flag == OBSTACLE_FIRST && !guc_slope_complete_flag)
//    {
//        if(guc_infrared_ranging_flag == ON && !guc_obstacle_complete_flag)
//        {
//          if((!guc_obstacle_state) && (gl_car_running_distance > gl_obstacle_judge_distance))
//          {
//            guc_obstacle_flag = ON;
//            guc_obstacle_state = OBSTACLE_PROCESS_1;
//            sl_obstacle_distance = gl_car_running_distance;
//          }
//        }
//    }

    gl_obstacle_distance_record = sl_obstacle_distance;
}


/*===============================================================
	@brief     路障检测
==================================================================*/
void Obstacle_Detection()
{
    uint16 obstacle_temp;
    static uint16 sui_obstacle_AD_count;
    uint16 inductor_value_x1, inductor_value_x2;
      
    inductor_value_x1 = gi_inductor_value[X_LEFT];
    inductor_value_x2 = gi_inductor_value[X_RIGHT];

    obstacle_temp = (uint16)Get_Infrared_ADC_Ave(INFRARED_UP);

    
    //节能一号
    gi_infrared_AD_distance = (48120 / (obstacle_temp-9));
    
//    //节能二号
//    gi_infrared_AD_distance = (48120 / (obstacle_temp - 9));  
    
    if((gi_infrared_AD_distance < OBSTACLE_RANGING_MAX) && (gi_infrared_AD_distance > OBSTACLE_RANGING_MIN))
    {
        sui_obstacle_AD_count++;
        if((guc_infrared_ranging_flag == 0) && (sui_obstacle_AD_count > OBSTACLE_COUNT))//计数，看持续变化
        {
          if((fabs(gl_car_error) < 250) && (fabs(inductor_value_x1 - inductor_value_x2) < 200))
//          if(fabs(gl_car_error) < 300)
          {
//            if(gl_car_running_distance > gl_obstacle_judge_distance)
//            {
              guc_infrared_ranging_flag = ON;
//            }
          }
              sui_obstacle_AD_count = 0;
        }
    }
    else 
    {
        sui_obstacle_AD_count = NO_OBSTACLE;
        guc_infrared_ranging_flag = OFF;
    }
    
    uint16 obstacle2_temp;
    static uint16 sui_obstacle2_AD_count;

    obstacle2_temp = (uint16)Get_Infrared_ADC_Ave(INFRARED_DOWN);

    gi_infrared_2nd_AD_distance = (48120 / (obstacle2_temp - 9));
 

    if((gi_infrared_2nd_AD_distance < (gi_infrared_AD_distance + 5))
       && (gi_infrared_2nd_AD_distance > (gi_infrared_AD_distance - 5))
       && (guc_infrared_ranging_flag == ON)
       && (guc_annulus_state != 3)//跑环岛过程中不识别路障
       && (!guc_obstacle_state) 
//       && (!guc_obstacle_complete_flag)//路障已完成标志，后期不可用
       && (gl_car_running_distance > gl_obstacle_judge_distance))
    {
        sui_obstacle2_AD_count++;
        if((guc_obstacle_flag == 0) && (sui_obstacle_AD_count > DETECTION_COUNT))//计数，看持续变化
        {
          if(guc_first_obstacle_slope_flag == OBSTACLE_FIRST)//识别路障
          {
              guc_obstacle_flag = ON;
              sui_obstacle2_AD_count = 0;
              guc_obstacle_state = OBSTACLE_PROCESS_1;
              sul_obstacle_start_time = gul_time_100us; 
              sl_obstacle_distance = gl_car_running_distance;
          }
          else
          {
              guc_obstacle_flag = OFF;
          }
        }
    }
    else 
    {
        sui_obstacle2_AD_count = NO_OBSTACLE;
    }
}

/*===============================================================
	@brief     拨码开关处理进程选择
==================================================================*/
void Switch_Process_Selection()
{
    int8 direction_flag;
    int8 slope_obstacle_first;
    
    direction_flag = Switch_Read_Value(1);
    slope_obstacle_first = Switch_Read_Value(2);
    
    if(direction_flag)
    {
        guc_avoidance_direction_flag = LEFT;
    }
    else
        guc_avoidance_direction_flag = RIGHT;

    if(slope_obstacle_first)
    {
        guc_first_obstacle_slope_flag = OBSTACLE_FIRST;
    }
    else
        guc_first_obstacle_slope_flag = SLOPE_FIRST;
}

/*===============================================================
	@brief     弧形路径几何参数计算
==================================================================*/
void Cambered_Trajectory_Calculate()
{
    uint16 roadcentre_deviate = 0;
    uint16 parallel_displacement = 0;
    
    roadcentre_deviate = ROADCENTRE_DEVIATE * ROADCENTRE_DEVIATE;
    parallel_displacement = PARALLEL_DISPLACEMENT * PARALLEL_DISPLACEMENT;
    
    gui_camber_radius = (int16)((roadcentre_deviate + parallel_displacement) / (2 * ROADCENTRE_DEVIATE));
}

/*===============================================================
	@brief     动态参数计算处理路障
==================================================================*/
void Dynamic_Calculate()
{
    int32 camber_rotate_speed = 0;
    
    gl_car_speed_set = gl_speed_obstacle;
    
    
    //引入角速度闭环避障，利用期望角度与转过的角度的差作为输入，采用串级闭环
    if(guc_avoidance_direction_flag == LEFT)
    {
      if(guc_obstacle_state == OBSTACLE_PROCESS_1)
      {
        if(gf_camber_angle < 20)
        {
          camber_rotate_speed = (int32)(gi_camber_coefficient * gl_speed_straight / gui_camber_radius);
        }
        else
        {
          camber_rotate_speed = 0;
        }
      }
      else if(guc_obstacle_state == OBSTACLE_PROCESS_2)
      {
        if(gf_camber_angle > -13)
        {
          camber_rotate_speed = -(int32)(gi_roadblock_coefficient * gl_speed_straight / OBSTACLE_RADIUS);
        }
        else
        {
          camber_rotate_speed = 0;
        }
      }
      else if(guc_obstacle_state == OBSTACLE_PROCESS_3)
      {
        if(gf_camber_angle < -9)
        {
          camber_rotate_speed = (int32)(gi_regression_coefficient * gl_speed_straight / gui_camber_radius);
        }
        else
        {
          camber_rotate_speed = 0;
        }
      }
    }
    else if(guc_avoidance_direction_flag == RIGHT)
    {
      if(guc_obstacle_state == OBSTACLE_PROCESS_1)
      {
        if(gf_camber_angle > -20)
        {
          camber_rotate_speed = -(int32)(gi_camber_coefficient * gl_speed_straight / gui_camber_radius);
        }
        else
        {
          camber_rotate_speed = 0;
        }
     }
      else if(guc_obstacle_state == OBSTACLE_PROCESS_2)
      {
        if(gf_camber_angle < 13)
        {
          camber_rotate_speed = (int32)(gi_roadblock_coefficient * gl_speed_straight / OBSTACLE_RADIUS);
        }
        else
        {
          camber_rotate_speed = 0;
        }      
      }
      else if(guc_obstacle_state == OBSTACLE_PROCESS_3)
      {
        if(gf_camber_angle > 9)
        {
          camber_rotate_speed = -(int32)(gi_regression_coefficient * gl_speed_straight / gui_camber_radius);
        }
        else
        {
          camber_rotate_speed = 0;
        }
      }
    }
    
    gl_car_error = camber_rotate_speed * 10;
    
    Angularspeed_Control_Direction(camber_rotate_speed);

    gf_camber_angle += gf_yaw_velocity * (PIT_1_PERIOD_S);

}

/*===============================================================
	@brief     路障处理
==================================================================*/
void Obstacle_Process()
{    
    gl_car_speed_set = gl_speed_obstacle;
    
    //引入角速度闭环避障，利用期望角度与转过的角度的差作为输入，采用串级闭环
    if(guc_avoidance_direction_flag == LEFT)
    {
      if(guc_obstacle_state == OBSTACLE_PROCESS_1)
      {
          Obstacle_Angle_Aviodance(gi_aviodence_angle, OBSTACLE_PROCESS_1);
      }
      else if(guc_obstacle_state == OBSTACLE_PROCESS_2)
      {
          Obstacle_Angle_Aviodance(-gi_regression_angle, OBSTACLE_PROCESS_2);
      }
      else if(guc_obstacle_state == OBSTACLE_PROCESS_3)
      {
          Obstacle_Angle_Aviodance(0, OBSTACLE_PROCESS_3);
      }
    }
    else if(guc_avoidance_direction_flag == RIGHT)
    {
      if(guc_obstacle_state == OBSTACLE_PROCESS_1)
      {
          Obstacle_Angle_Aviodance(-gi_aviodence_angle, OBSTACLE_PROCESS_1);
     }
      else if(guc_obstacle_state == OBSTACLE_PROCESS_2)
      {
          Obstacle_Angle_Aviodance(gi_regression_angle, OBSTACLE_PROCESS_2);
      }
      else if(guc_obstacle_state == OBSTACLE_PROCESS_3)
      {
          Obstacle_Angle_Aviodance(0, OBSTACLE_PROCESS_3);
      }
    }
    
}

/*===============================================================
	@brief     避障外环期望角度设定
==================================================================*/
void Obstacle_Angle_Aviodance(float expected_angle, uint8 channel)
{
    float angle_offset;
    float angle_offset_delta;
    float camber_angle;
    static float sl_last_angle_offset;

    camber_angle = gf_camber_angle;
    angle_offset = camber_angle - expected_angle;
    angle_offset_delta = angle_offset - sl_last_angle_offset;

    switch(channel)
    {
      
       case OBSTACLE_PROCESS_1:
  gf_obstacle_error = angle_offset * gf_obstacle_avoidance_p / 10 +  angle_offset_delta * gf_obstacle_avoidance_d / 10;
              break;
              
       case OBSTACLE_PROCESS_2:
  gf_obstacle_error = angle_offset * gf_obstacle_regression_p / 10 +  angle_offset_delta * gf_obstacle_regression_d / 10;
              break;
              
       case OBSTACLE_PROCESS_3:
  gf_obstacle_error = angle_offset * gf_obstacle_straight_p / 10 +  angle_offset_delta * gf_obstacle_straight_d / 10;
              break;
              
       default:
              break;
              
    }
    
      if((gul_time_100us - sul_obstacle_start_time) >= 40000)
      {
          guc_car_run_flag = OFF;
      }
        
    gf_obstacle_error = Limiter(gf_obstacle_error, -250, 250);

    sl_last_angle_offset = angle_offset;
}
