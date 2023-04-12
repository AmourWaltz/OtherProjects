/*=============================================
    @file         process.c
    @brief        特殊路段处理
    @programmer   Beyond Hsueh
=============================================*/

#include "process.h"

/*===============================================================
        @note      纤云弄巧，飞星传恨，银汉迢迢暗度；
                   金风玉露一相逢，便胜却，人间无数。
                   柔情似水，佳期如梦，忍顾鹊桥归路；
                   两情若是久长时，又岂在，朝朝暮暮。
==================================================================*/  

/*===============================================================
	@brief     直道判断
	@note      满足一定条件超过一定次数才认为是直道
                   使用原始ad数据，动态标定并未完善，判断直道采用原始数据，判断特殊元素采用标定后数据
                   主要采用横竖电感大小关系
==================================================================*/
uint8 Judge_Straight()
{
    uint8 straight_flag = 0;
    static uint16 straight_count = 0;
    
    int16 inductor_value_x1,inductor_value_x2;
    int16 inductor_value_y1,inductor_value_y2;
    
    inductor_value_x1 = gi_inductor_value[X_LEFT];
    inductor_value_x2 = gi_inductor_value[X_RIGHT];
    inductor_value_y1 = gi_inductor_value[T_LEFT];
    inductor_value_y2 = gi_inductor_value[T_RIGHT];

    //竖电感差比和与横电感差比和在直道处大小关系，特征识别即可
    if((inductor_value_y2 + inductor_value_y1 < (inductor_value_x1 + inductor_value_x2)/40)
       &&(fabs(inductor_value_x2 - inductor_value_x1) < 20))
    {
      //直道判断次数
        if(straight_count < 5)
        {
            straight_count++;
        }
        else
        {
            straight_flag = 1;
        }
    }
    else
    {
        straight_count = 0;
        straight_flag = 0;
    }
    return straight_flag;
}

/*===============================================================
	@brief     十字判断
	@note      特征元素提取，主要防止误判
                   阈值法，比较大小，变化率相结合
==================================================================*/
uint8 Judge_Crucifix()
{
    static uint8 crucifix_flag = 0;
    
    int16 inductor_value_x1,inductor_value_x2;
    int16 inductor_value_y1,inductor_value_y2;
    int16 inductor_value_m1;
    
    inductor_value_x1 = gi_inductor_value[X_LEFT];
    inductor_value_x2 = gi_inductor_value[X_RIGHT];
    inductor_value_y1 = gi_inductor_value[T_LEFT];
    inductor_value_y2 = gi_inductor_value[T_RIGHT];
    inductor_value_m1 = gi_inductor_value[M_BACK];
    
    if((crucifix_flag == 0)
       &&(inductor_value_m1 > 400)
       &&(inductor_value_m1 < 600)
       &&(inductor_value_x2 + inductor_value_x1 > 350)
       &&(inductor_value_y1 + inductor_value_y2 > 500)
       &&(0.7*(inductor_value_y1 + inductor_value_y2 ) > (inductor_value_y1 - inductor_value_y2)))
//       ||((gi_y_signal_gradient > 500))&&(0.7*fabs(gi_y_signal_gradient) > fabs(gi_signal_gradient[T_RIGHT] - gi_signal_gradient[T_LEFT]))))
    {
        crucifix_flag = 1;
    }
    else if((inductor_value_y1 + inductor_value_y2 < 30)
//            &&(gi_y_signal_gradient > -100)
              )
    {
        crucifix_flag = 0;
    }
  
    return crucifix_flag;
}

/*===============================================================
	@brief     大弯道判断
	@note      某一边竖电感值增大，横电感差值绝对值增大，中间电感值下降
==================================================================*/
uint8 Judge_Curve()
{
    static uint8 curve_flag = 0;
    
    int16 inductor_value_x1,inductor_value_x2;
    int16 inductor_value_y1,inductor_value_y2;
    int16 inductor_value_m1;
    
    inductor_value_x1 = gi_inductor_value[X_LEFT];
    inductor_value_x2 = gi_inductor_value[X_RIGHT];
    inductor_value_y1 = gi_inductor_value[T_LEFT];
    inductor_value_y2 = gi_inductor_value[T_RIGHT];
    inductor_value_m1 = gi_inductor_value[M_BACK];
    
    if(curve_flag == 0)
    {
        if((inductor_value_y1 > 50)
           ||(inductor_value_y2 > 50)
           ||(inductor_value_m1 < 850)
           ||(fabs(inductor_value_x2 - inductor_value_x1) > 200))
        {
            curve_flag = 1;
        }
    }
    else
    {
        if((inductor_value_y1 < 50)
           &&(inductor_value_y2 < 50)
           &&(inductor_value_m1 > 850)
           &&(fabs(inductor_value_x2 - inductor_value_x1) < 200))
        {
            curve_flag = 0;
        }
    }
    return curve_flag;
}

/*===============================================================
	@brief     环岛判断
	@note      状态机，分为未入环，通过交叉点，环中，准备出环，出环五部分
==================================================================*/
uint8 Judge_Annulus()
{
    static int32 sl_annulus_distance = 0;//

    static uint8 suc_annulus_ready_flag;//检测准备入环标志
    static uint16 sui_annulus_ready_count;//检测准备入环状态计数
    static int32 sl_annulus_ready_distance;//检测环距离记录
    static uint8 suc_double_annulus_flag;
    static uint8 suc_inside_annulus_flag;//入环标志
    
    int16 inductor_value_x1,inductor_value_x2;
    int16 inductor_value_y1,inductor_value_y2;
    int16 inductor_value_m1;    
    
    inductor_value_x1 = gi_inductor_value[X_LEFT];
    inductor_value_x2 = gi_inductor_value[X_RIGHT];
    inductor_value_y1 = gi_inductor_value[T_LEFT];
    inductor_value_y2 = gi_inductor_value[T_RIGHT];
    inductor_value_m1 = gi_inductor_value[M_BACK];
    
    if((gi_m_signal_gradient > 400)&&(gi_x_signal_gradient > 700)) //如果信号强度变化率很大，说明可能有环)
    {
        sui_annulus_ready_count++;
        if((suc_annulus_ready_flag == 0)&&(sui_annulus_ready_count > 5)) //计数，看持续变化
        {
            suc_annulus_ready_flag = 1;
            sl_annulus_ready_distance = (int32)gl_car_running_distance;
            sui_annulus_ready_count = 0;
        }
    }
    else if(guc_annulus_state)
    {
        sui_annulus_ready_count = 0;
    }
    else if(gl_car_running_distance > sl_annulus_ready_distance)  //信号变化不持续
    {
        sui_annulus_ready_count = 0;
        suc_annulus_ready_flag = 0;
    }
    else
    {
        sui_annulus_ready_count = 0;
        suc_annulus_ready_flag = 0;
    }
    
//    if(suc_annulus_ready_flag)
//    {
//      Beep_On();
//    }
//    else
//      Beep_Off();

    if(guc_annulus_state == 0)//未入环,判断有没有环
    {
        if(((inductor_value_m1 > 380)
            &&(inductor_value_x1 + inductor_value_x2 > 480)
//            &&(suc_annulus_ready_flag == 1)
//            ||(inductor_value_x1 + inductor_value_x2 > 2000))            //预入环判断，防止入环前抖一下
            &&((1.4*inductor_value_m1 > inductor_value_x1)
            ||(1.4*inductor_value_m1 > inductor_value_x2))
            &&(1.2*inductor_value_m1 < inductor_value_x1 + inductor_value_x2)
            &&(inductor_value_y1 + inductor_value_y2 > 100)
            &&(inductor_value_y1 + inductor_value_y2 < 600)
            &&(fabs((inductor_value_x1 - inductor_value_x2) < 0.8*(inductor_value_x1 + inductor_value_x2)))))
//            &&(gl_car_running_distance - sl_annulus_distance > 10))        //根据是否有双环适当修改此距离
        {

            if(((inductor_value_y1 > 3*inductor_value_y2)
                  &&(inductor_value_y1 > 100)) 
//                  ||(inductor_value_y1 > 10*inductor_value_y2)
              )
            {
                guc_annulus_direction = LEFT;
                guc_annulus_state = 1;

                //gf_yaw_angle = -Limiter((gl_car_error * gf_direction_p)/ANGLE_PER_DIRECTION,-10,10);//转向不足补偿，保证角度零点为正对圆环方向
                gf_annulus_angle = -Limiter(gf_yaw_angle, -10, 10);
                
                sl_annulus_distance = gl_car_running_distance;
            }
            else if(((inductor_value_y2 > 3*inductor_value_y1)
                    &&(inductor_value_y2 > 100))
//                    ||(inductor_value_y2 > 10*inductor_value_y1)
                   )
            {
                guc_annulus_direction = RIGHT;
                guc_annulus_state = 1;

                //gf_yaw_angle = -Limiter((gl_car_error * gf_direction_p)/ANGLE_PER_DIRECTION,-10,10);//转向不足补偿，保证角度零点为正对圆环方向
                gf_annulus_angle = -Limiter(gf_yaw_angle, -10, 10);
                
                sl_annulus_distance = gl_car_running_distance;
            }
        }
        suc_double_annulus_flag = 0;
    }

    if(guc_annulus_state == 1)//检测到环，判断通过中心点
    {
        if(inductor_value_y1 + inductor_value_y2 < 200)//&& (gi_y_signal_gradient > 50))
        {
            suc_double_annulus_flag = 1;
        }
        if((inductor_value_m1 > 650)
            &&(inductor_value_x1 + inductor_value_x2 > 720)
            &&(suc_double_annulus_flag == 1)
//            &&(fabs(inductor_value_x1 - inductor_value_x2) < 35)
            &&(gl_car_running_distance - sl_annulus_distance > 40)
          )
        {
            if(
//               (inductor_value_y1 > inductor_value_y2)
                (guc_annulus_direction == LEFT)
               )
            {
                guc_annulus_direction = LEFT;
                guc_annulus_state = 2;      //竖电感算偏差
                sl_annulus_distance = gl_car_running_distance;
                guc_annulus_yaw_flag = 1;
            }
            else if(
//                  (inductor_value_y2 > inductor_value_y1) 
                  (guc_annulus_direction == RIGHT)   
                    )
            {
                guc_annulus_direction = RIGHT;
                guc_annulus_state = 2;
                sl_annulus_distance = gl_car_running_distance;
                guc_annulus_yaw_flag = 1;
            }
        }

        else if(gl_car_running_distance - sl_annulus_distance > 200)//防误判强退保护，误识别环
        {
            guc_annulus_state = 0;
        }
    }
    
    else if(guc_annulus_state == 2)  // 通过交叉点，判断是否正确姿态入环
    {
        if((inductor_value_m1 < 400)
//        &&(inductor_value_x1 + inductor_value_x2 > 170)
        &&(fabs(gf_annulus_angle) > 30)
        &&(gl_car_running_distance - sl_annulus_distance > 45))
        {
            if(guc_annulus_direction == LEFT)
//               &&(inductor_value_x2 > 1.2*inductor_value_x1))//电磁线跨过中心电感
            {
                guc_annulus_state = 3;      //正常计算偏差
                sl_annulus_distance = gl_car_running_distance;
            }
            else if(guc_annulus_direction == RIGHT)
//               &&(inductor_value_x1 > 1.2*inductor_value_x2))
            {
                guc_annulus_state = 3;      //正常计算偏差
                sl_annulus_distance = gl_car_running_distance;
            }
        }
//        if((fabs(gf_yaw_angle) > 30)
//        ||(gl_car_running_distance - sl_annulus_distance > 100))      //强退保护
//        {
//                guc_annulus_state = 3;      //正常计算偏差，或者丢线
//                sl_annulus_distance = gl_car_running_distance;
//        }
    }
    else if(guc_annulus_state == 3)//在环中，判断是否到达中心点并且要出环，
    {
        if(guc_annulus_direction == LEFT)
        {
          //入环标志，防止因入环姿势问题偏航角积分过度
          if(gl_car_running_distance - sl_annulus_distance > 150)//&& (gi_y_signal_gradient > 50))
          {
              suc_inside_annulus_flag = 1;
          }          
            if((((inductor_value_x1 > 200)
              &&(inductor_value_x1 > 0.8*inductor_value_x2)
              &&(inductor_value_y1 > 25))
//              ||(gi_y_signal_gradient > 600))
              &&(fabs(gf_annulus_angle) > 290))
              &&(suc_inside_annulus_flag == 1))               
            {
                guc_annulus_state = 4;      //打死角
                sl_annulus_distance = gl_car_running_distance;
            }
            else if((gl_car_running_distance - sl_annulus_distance > 700))//强退保护
            {

                guc_annulus_state = 0;      //打死角
                sl_annulus_distance = gl_car_running_distance;
                guc_annulus_yaw_flag = 1;

            }
        }
        else if(guc_annulus_direction == RIGHT)
        {
          if(gl_car_running_distance - sl_annulus_distance > 150)//&& (gi_y_signal_gradient > 50))
          {
              suc_inside_annulus_flag = 1;
          }     
            if((((inductor_value_x2 > 200)
              &&(inductor_value_x2 > 0.8*inductor_value_x1)
              &&(inductor_value_y2 > 25))
//              ||(gi_y_signal_gradient > 600))
              &&(fabs(gf_annulus_angle) > 290))
              &&(suc_inside_annulus_flag == 1)) 
            {
                guc_annulus_state = 4;
                sl_annulus_distance = gl_car_running_distance;
            }
            else if((gl_car_running_distance - sl_annulus_distance > 700))//强退保护
            {

                guc_annulus_state = 0;      //打死角
                sl_annulus_distance = gl_car_running_distance;
                guc_annulus_yaw_flag = 1;
            }
        }
        
        //防止因内切导致状态三无法跳出
        if((guc_annulus_direction == LEFT)
           &&(fabs(gf_annulus_angle) > 310))
        {
            guc_annulus_state = 4;//直行
            sl_annulus_distance = gl_car_running_distance;
        }
        else if((guc_annulus_direction == RIGHT)
           &&(fabs(gf_annulus_angle) > 310))
        {
            guc_annulus_state = 4;//直行
            sl_annulus_distance = gl_car_running_distance;
        }
    }
    
    else if(guc_annulus_state == 4)//准备出环，判断是否已经出环
    {
        if((guc_annulus_direction == LEFT)
           &&(fabs(gf_annulus_angle) > 345))
        {
            guc_annulus_state = 5;  //直行
            sl_annulus_distance = gl_car_running_distance;
        }
        else if((guc_annulus_direction == RIGHT)
           &&(fabs(gf_annulus_angle) > 345))
        {
            guc_annulus_state = 5;  //直行
            sl_annulus_distance = gl_car_running_distance;
        }
        
        //防止入环前抖动
        if((inductor_value_m1 > 800)
//          ||(inductor_value_x1 > 200)
//          ||(inductor_value_x2 > 200)
          &&(fabs(gf_annulus_angle) > 335))
        {
            if(guc_annulus_direction == LEFT)
            {
                if((((inductor_value_x1 - inductor_value_x2) < 400)
//                  ||((inductor_value_x1 - inductor_value_m1) < 80))
                  &&(inductor_value_y1 > 1.5 * inductor_value_y2)//10     
                  &&(inductor_value_y1 < 80)//10
                  &&(gl_car_running_distance - sl_annulus_distance > 15)))
                {
                    guc_annulus_state = 5;  //直行
                    sl_annulus_distance = gl_car_running_distance;
                }
//                else if((gl_car_running_distance - sl_annulus_distance > 100))  //一直没出去
//                {
//                    guc_annulus_state = 5;          //强退环路
//                    sl_annulus_distance = gl_car_running_distance;
//                }
            }
            else if(guc_annulus_direction == RIGHT)
            {
                if((((inductor_value_x2 - inductor_value_x1) < 400)
//                  ||(inductor_value_x2 - inductor_value_m1 < 80))
                  &&(inductor_value_y2 > 1.5 * inductor_value_y1)
                  &&(inductor_value_y2 < 80)
                  &&(gl_car_running_distance - sl_annulus_distance > 15)))
                {
                    guc_annulus_state = 5;  //直行
                    sl_annulus_distance = gl_car_running_distance;
                }
//                else if((gl_car_running_distance - sl_annulus_distance > 100))  //一直没出去
//                {
//                    guc_annulus_state = 5;          //强退环路
//                    sl_annulus_distance = gl_car_running_distance;
//                }
            }
        }
    }
    
    else if(guc_annulus_state == 5) //已出环
    {
        if((inductor_value_m1 < 460)
            &&(inductor_value_x1 + inductor_value_x2 < 550)
//            &&(gi_x_signal_gradient > -100)
            &&(gl_car_running_distance - sl_annulus_distance > 70)
//            ||(fabs(gf_annulus_angle) > 360))
//            ||(gf_car_running_distance - sl_annulus_distance > 80)//强退保护
              ) 
            {
                guc_annulus_state = 0;      //退出环岛模式
                sl_annulus_distance = gl_car_running_distance;
                gf_annulus_angle = 0;
                Beep_Off();
            }
        guc_annulus_yaw_flag = 0;
    }
    
    gl_annulus_distance_record = sl_annulus_distance;
    
    return guc_annulus_state;
}

/*===============================================================
	@brief     坡道判断
	@note      主要采用中间电感值变化判断，使用变化率效果更棒
==================================================================*/
uint8 Judge_Slope()        //坡道识别
{
    static uint8 suc_slope_flag = 0;
    static int32 sl_slope_distance = 0;
    static uint16 sui_ramp_counter;
    static uint8 suc_genttle_slope = 0;//缓和坡道标志
    
    int16 inductor_value_x1,inductor_value_x2;
    int16 inductor_value_m1;    
    
    inductor_value_x1 = gi_inductor_value[X_LEFT];
    inductor_value_x2 = gi_inductor_value[X_RIGHT];
    inductor_value_m1 = gi_inductor_value[M_BACK];

    if(suc_slope_flag == 0)
    {   
        
        if((inductor_value_m1 > 470)   //上坡
//           &&(gi_m_signal_gradient > 400)
//           &&(gi_x_signal_gradient < 200)
//           &&(gi_x_signal_gradient > -100)
//           &&(gi_m_signal_gradient > 2*gi_x_signal_gradient)
           &&(1.4 * inductor_value_m1 > (inductor_value_x1 + inductor_value_x2))
//           &&((gf_pitch_angle) > 3)       //倾角为正
           &&(gl_car_running_distance - sl_slope_distance > 20))     //两个坡道间距离
        {
            suc_genttle_slope = 1;
            gf_slope_start_angle = gf_pitch_angle;         //记录起始角度
            sl_slope_distance = gl_car_running_distance;
        }
        else if((inductor_value_m1 > 550)
                &&(gf_pitch_angle > 12))      //如果倾角很大，可能是没识别到，直接入坡
        {
            sui_ramp_counter++;
            if(sui_ramp_counter > 20)
            {
                suc_slope_flag = RAMP_UP;
            }
            else
            {
                sui_ramp_counter++; 
            }
        }
        else
        {
            sui_ramp_counter = 0;
        }
        
        if((suc_genttle_slope == 1) 
           &&((gf_slope_start_angle - gf_pitch_angle) > 2) 
           &&(gl_car_running_distance - sl_slope_distance < 40))
        {
            suc_slope_flag = RAMP_BEGIN;
            sl_slope_distance = gl_car_running_distance;
            suc_genttle_slope = 0;
            //gf_slope_start_angle = 0;
            guc_slope_pitch_flag = 1;
        }
        else if(gl_car_running_distance - sl_slope_distance > 70)
        {
            sl_slope_distance = gl_car_running_distance;
            gf_slope_start_angle = 0;
            suc_genttle_slope = 0;
            //gf_slope_start_angle = 0;
        }
        
    }    
    else if(suc_slope_flag == RAMP_BEGIN)
    {
        if(((gi_pitch_variance) > RAMP_ROT_GATE))
        {
            sui_ramp_counter++;
        }
        
        if((fabs(gf_slope_picth_angle) > 5))
        {
            suc_slope_flag = RAMP_UP;
            sui_ramp_counter = 0;
            guc_slope_complete_flag = ON;
            sl_slope_distance = gl_car_running_distance;
        }
        else if(gl_car_running_distance - sl_slope_distance > 50) //如果探到坡道一段距离没有颠簸，认为误判
        {
            suc_slope_flag = 0;
            guc_slope_pitch_flag = 0;
            Beep_Off();
        }
    }
    else if(suc_slope_flag == RAMP_UP)
    {
        if(((inductor_value_m1 < 400)//在坡上
         &&(0.7*inductor_value_m1 < (inductor_value_x1 + inductor_value_x2))
         &&(gf_slope_picth_angle < 2))//倾角为正
         &&(gl_car_running_distance - sl_slope_distance > 50))//防止状态连续跳转
        {
            suc_slope_flag = ON_RAMP;
            sl_slope_distance = gl_car_running_distance;
        }
        else if(gl_car_running_distance - sl_slope_distance > 150)
        {
            suc_slope_flag = 0;
            sl_slope_distance = gl_car_running_distance;
            guc_slope_pitch_flag = 0;
            Beep_Off();
        }
    }
    else if(suc_slope_flag == ON_RAMP)
    {
       if((inductor_value_m1 > 260)   //下坡
//        &&(0.7*inductor_value_m1 > (inductor_value_x1 + inductor_value_x2))
        &&(gf_slope_picth_angle > 6)
        &&(gl_car_running_distance - sl_slope_distance > 60))      //坡道长度
       {
           suc_slope_flag = RAMP_DOWN;
           sl_slope_distance = gl_car_running_distance;
           //guc_slope_complete_flag = ON;
       }
        else if(gl_car_running_distance - sl_slope_distance > 100)
        {
            suc_slope_flag = 0;
            sl_slope_distance = gl_car_running_distance;
            //Beep_Off();
        }
    } 
    else if(suc_slope_flag == RAMP_DOWN)
    {
        if(((inductor_value_m1 > 300)   //下坡
           &&(inductor_value_m1 < 420)
//           &&(0.75*inductor_value_m1 < (inductor_value_x1 + inductor_value_x2))
           &&(1.6*inductor_value_m1 > (inductor_value_x1 + inductor_value_x2))
           ||(fabs(gf_slope_picth_angle) < 3))
           &&(gl_car_running_distance - sl_slope_distance > 70))
        {
            suc_slope_flag = 0;
            guc_slope_pitch_flag = 0;
            sl_slope_distance = gl_car_running_distance;
            guc_slope_complete_flag = ON;
            Beep_Off();
        }
        else if(gl_car_running_distance - sl_slope_distance > 120)
        {
            suc_slope_flag = 0;
            sl_slope_distance = gl_car_running_distance;
            guc_slope_pitch_flag = 0;
            guc_slope_complete_flag = ON;
            Beep_Off();
        }
    }
    else
    {
        suc_slope_flag = 0;
        sl_slope_distance = gl_car_running_distance;
    }
    
    if(suc_slope_flag)
    {
        gl_slope_distance_record = sl_slope_distance;
    }
    
    return suc_slope_flag;
}

/*===============================================================
	@brief     丢信号判断
	@note      横电感之和小于一定值，判断电感全部偏至信号线一侧
==================================================================*/
uint8 Judge_Lost_Signal()
{
    static uint8 suc_lost_signal_flag=0;
    int16 inductor_value_x1,inductor_value_x2;
    
    inductor_value_x1 = gi_inductor_value[X_RIGHT];
    inductor_value_x2 = gi_inductor_value[X_LEFT];
    
    if(guc_car_run_flag)
    {
      //基本判别方法如上
      if(suc_lost_signal_flag == 0)
      {
          if((inductor_value_x1 + inductor_value_x2 < LOST_SIGNAL_VALUE))
          {
              suc_lost_signal_flag = 1;
          }
      }
      else if(suc_lost_signal_flag == 1)
      {
          if((inductor_value_x1 + inductor_value_x2 > GET_SIGNAL_VALUE))
          {
              suc_lost_signal_flag = 0;
          }
      }
    }
    
    return suc_lost_signal_flag;
}

/*===============================================================
	@brief     串道识别
	@note      中心值判别法，不止一种，校内赛赛道简单应该不会用
==================================================================*/
uint8 Judge_Mix_Track()
{
    uint8 mix_track_flag  = 0;//串道标志
    
    int16 inductor_value_x1,inductor_value_x2;
    int16 inductor_value_m1;
    
    inductor_value_x1 = gi_inductor_value[X_LEFT];
    inductor_value_x2 = gi_inductor_value[X_RIGHT];
    inductor_value_m1 = gi_inductor_value[M_BACK];
      
    //如果中心电感值小于左右两侧电感值，可能串道了
    if((inductor_value_x1 > inductor_value_m1) && (inductor_value_x2 > inductor_value_m1))
    {
        mix_track_flag = 1;
    }
    else
    {
        mix_track_flag = 0;
    }
    return mix_track_flag;
}

/*===============================================================
	@brief     小S判断
	@note      竖电感之和小于横电感，等速度提到2米多而且用普通差比和实在处理不了小S时再使用
==================================================================*/
uint8 Judge_Small_S()
{
    static uint8 small_S_flag=0;
    
    int16 inductor_value_x1,inductor_value_x2;
    int16 inductor_value_y1,inductor_value_y2;
    int16 inductor_value_m1;    
    
    inductor_value_x1 = gi_inductor_value[X_LEFT];
    inductor_value_x2 = gi_inductor_value[X_RIGHT];
    inductor_value_y1 = gi_inductor_value[T_LEFT];
    inductor_value_y2 = gi_inductor_value[T_RIGHT];
    inductor_value_m1 = gi_inductor_value[M_BACK];    
    
    if((small_S_flag == 0)//作为判断使用的特征值根据实际赛道调整，记录小S弯横竖电感特征值即可
       &&(inductor_value_y2 + inductor_value_y1 < 100)
       &&(fabs(inductor_value_x2 - inductor_value_x1) > 200)
       &&(inductor_value_x2 + inductor_value_x1 > 1000)
       &&(inductor_value_m1 > 900))
    {
        small_S_flag = 1;
    }
    else if((small_S_flag )//作回差判断防止震荡
              &&(inductor_value_y2 + inductor_value_y1 > 200)
              &&(inductor_value_m1 < 900)
              &&(inductor_value_m1 > 950))
    {
        small_S_flag = 0;
    }
    return small_S_flag;
}

/*===============================================================
	@brief     直道处理
	@note      正常差比和，防止误判
==================================================================*/
void General_Process()
{
    static uint8 suc_normal_flag;

    int16 inductor_value_x1,inductor_value_x2;
    int16 inductor_value_y1,inductor_value_y2;
    
    inductor_value_x1 = gi_inductor_value[X_LEFT];
    inductor_value_x2 = gi_inductor_value[X_RIGHT];
    inductor_value_y1 = gi_inductor_value[T_LEFT];
    inductor_value_y2 = gi_inductor_value[T_RIGHT];
    
    if((guc_curve_flag)||(suc_normal_flag == ABNORMAL))
    {
        gl_car_speed_set = gl_speed_curve;
    }
    else
    {
        gl_car_speed_set = gl_speed_straight;
    }
    
    //左右电感差比和算偏差
    gl_car_error = (int16)((gui_x_weight * (inductor_value_x2 - inductor_value_x1)
                    +gui_t_weight * (inductor_value_y2 - inductor_value_y1))
                    /(float)((inductor_value_x2 + inductor_value_x1)
                    +(inductor_value_y2 + inductor_value_y1)+1));
    
    if(((inductor_value_x2 + inductor_value_x1 > 1200)             //入十字，环路
       &&(inductor_value_y2 + inductor_value_y1 > 100)
       ||(inductor_value_y2 + inductor_value_y1 > inductor_value_x2 + inductor_value_x1))
       &&(fabs(gi_inductor_x_offset)<40)
       &&(gi_y_signal_gradient > 100)
       &&(suc_normal_flag == NORMAL))
    {
        suc_normal_flag = ABNORMAL;
    }
    else if(((inductor_value_x2 + inductor_value_x1 < 1000)
            ||(inductor_value_y2 + inductor_value_y1 < 50))
            &&(suc_normal_flag == ABNORMAL))
    {
        suc_normal_flag = NORMAL;
    }
    
    if(suc_normal_flag == NORMAL)       //正常偏差
    {
        gl_car_error = (int16)((gui_x_weight * (inductor_value_x2 + inductor_value_x1)
                    + gui_t_weight * (inductor_value_y2 + inductor_value_y1))
                    /(float)((inductor_value_x2 + inductor_value_x1)
                    +(inductor_value_y2 + inductor_value_y1)+1));
    }
    else                //入十字，环路
    {
        gl_car_error = (int16)((1.5*gui_x_weight * (inductor_value_x2 - inductor_value_x1))
                    /(float)((inductor_value_x2 + inductor_value_x1)
                    + (inductor_value_y2 + inductor_value_y1) + 1));
    }

}

/*===============================================================
	@brief     丢信号处理
	@note      最原始处理，直接打死角
==================================================================*/
void Lost_Signal_Process()
{
    static uint8 suc_lost_direction = 0;
    
    if(guc_lost_start_flag == ON)
    {
        if(gl_car_error > 0)//轻度丢信号处理
        {
             gl_car_error = MAX_DIR_ANGLE;
             suc_lost_direction = LEFT;
        }
        else if(gl_car_error < 0)
        {
             gl_car_error = -MAX_DIR_ANGLE;
             suc_lost_direction = RIGHT;
        }
        else if(gl_car_error == 0)//完全丢信号处理
        {
            if(suc_lost_direction == LEFT)
            {
                gl_car_error = MAX_DIR_ANGLE;
            }
            else if(suc_lost_direction == RIGHT)
            {
                gl_car_error = -MAX_DIR_ANGLE;
            }
        }
        gl_car_speed_set = gl_speed_lost_line;
    }
//    //环岛入环状态打死角处理
//    if(guc_annulus_state == 2)
//    {
//        if(guc_annulus_direction == LEFT)//轻度丢信号处理
//        {
//             gl_car_error = ANNULUS_DIR_ANGLE + 5;
//        }
//        else if(guc_annulus_direction == RIGHT)
//        {
//             gl_car_error = -(ANNULUS_DIR_ANGLE - 5);
//        }
//    }
}

/*===============================================================
	@brief     环岛处理
	@note      状态机，利用角速度积分判断转完一圈
==================================================================*/
void Annulus_Process()
{
    float error_temp;
    
    static int32 sl_annulus3_error_summation;
    static int32 sl_annulus3_error_count;
//    static float sf_annulus3_error_average;//环内偏差平均值
    
    static uint8 suc_last_annulus_state;
//    static uint8 suc_error_reset_flag = 0;//重置偏差选择标志
//    float min_error_temp = 0;
        
//    static float sf_last_error = 0;
    
    int16 inductor_value_x1,inductor_value_x2;
    int16 inductor_value_y1,inductor_value_y2;
    int16 inductor_value_m1;    
    
    inductor_value_x1 = gi_inductor_value[X_LEFT];
    inductor_value_x2 = gi_inductor_value[X_RIGHT];
    inductor_value_y1 = gi_inductor_value[T_LEFT];
    inductor_value_y2 = gi_inductor_value[T_RIGHT];
    inductor_value_m1 = gi_inductor_value[M_BACK];
    
    switch(guc_annulus_state)
    {
      case 1:
        gl_car_speed_set = gl_speed_annulus - 5;
        break;
      case 2:
        gl_car_speed_set = gl_speed_annulus - 5;
        break;
      case 3:
        gl_car_speed_set = gl_speed_annulus + 5;
        break;
      case 4:
        gl_car_speed_set = gl_speed_annulus;
        break;
      case 5:
        gl_car_speed_set = gl_speed_annulus + 5;
        break;
      default:
        break;
    }
    if(suc_last_annulus_state != guc_annulus_state)//圆环状态变化
    {
//        suc_error_reset_flag = 0;
        suc_last_annulus_state = guc_annulus_state;
    }
    if(guc_annulus_state == 1)
    {
        sl_annulus3_error_summation = 0;
        sl_annulus3_error_count = 0;
//        sf_annulus3_error_aver = 0;
//        error_temp = (gui_annulus_x_weight1 * (inductor_value_x2 - inductor_value_x1)
//                      +gui_annulus_y_weight1 * (inductor_value_y2 - inductor_value_y1))
//                      /(float)((inductor_value_x2 + inductor_value_x1)
//                      +(inductor_value_y2 + inductor_value_y1)+1);
//        if(guc_annulus_direction == LEFT)
//        {
//
//            if(inductor_value_x1 > inductor_value_m1)
//            {
//              error_temp = (gui_x_weight * (inductor_value_x1 - inductor_value_m1))
//                    /(float)((inductor_value_m1 + inductor_value_x1) + 1);
//              
//              gl_car_error = (int32)error_temp;          
//            }
//            else
//              gl_car_error = 0;    
//            
//            gl_car_error = (int32)(error_temp / 2);
//        }
//        else if(guc_annulus_direction == RIGHT)
//        {
//          if(inductor_value_x2 > inductor_value_m1)
//          {
//            error_temp = (gui_x_weight * (inductor_value_m1 - inductor_value_x2))
//                  /(float)((inductor_value_m1 + inductor_value_x2) + 1);
//            
//            gl_car_error = (int32)error_temp;          
//          }
//          else
//            gl_car_error = 0;       
//          
//          gl_car_error = (int32)(error_temp / 2);
//        }
//
//        gl_car_error = (int32)(error_temp / 10);
    }
    else if(guc_annulus_state == 2) // 通过交叉点
    {
        if(guc_annulus_direction == RIGHT)
        {
//            //入环有迟滞  gui_entry_annulus_weight = 2200
//            error_temp = (fabs(inductor_value_y2 - inductor_value_y1));
//            error_temp = -1.5*gui_entry_annulus_weight * (0.9 * error_temp + 0.1 * f_sqrt(error_temp))/100;
////            error_temp -= gui_annulus_x_weight2 * Limiter(1500 - inductor_value_m1,0,1500)/1000l;
//
//            gl_car_error = (int32)Limiter(error_temp, -440, -0);
            
            //打死角入环，由于对于不同大小的环岛不具有普适性，改为竖电感差比和入环
//          if(fabs(gf_annulus_angle) <= 27)
//          {
//              gl_car_error = -(ANNULUS_DIR_ANGLE + 10);
//          }
//          else
//              gl_car_error = 0;
          
             error_temp = (int32)((gui_entry_annulus_weight * (inductor_value_y1 - inductor_value_y2))
                    /(float)(inductor_value_y2 + inductor_value_y1 + 1));
             
            gl_car_error = (int32)Limiter(error_temp, -480, 0);
          
        }
        else if(guc_annulus_direction == LEFT)
        {
//            //入环有迟滞  gui_entry_annulus_weight = 2200
//            error_temp = (fabs(inductor_value_y2 - inductor_value_y1));
//            error_temp = 1.5*gui_entry_annulus_weight * (0.9 * error_temp + 0.1 * f_sqrt(error_temp))/100;
////            error_temp += gui_annulus_x_weight2 * Limiter(1500 - inductor_value_m1,0,1500)/1000l;
//            
//            gl_car_error = (int32)Limiter(error_temp, 0, 440);
            
            //打死角入环，由于对于不同大小的环岛不具有普适性，改为竖电感差比和入环
//          if(fabs(gf_annulus_angle) <= 27)
//          {
//              gl_car_error = (ANNULUS_DIR_ANGLE + 18);
//          }
//          else
//              gl_car_error = 0;      
            
            error_temp = (int32)((gui_entry_annulus_weight * (inductor_value_y1 - inductor_value_y2))
                    /(float)(inductor_value_y2 + inductor_value_y1 + 1));
             
            gl_car_error = (int32)Limiter(error_temp, 0, 480);
        }
    }
    else if(guc_annulus_state == 3)  // 在环中,正常跑
    {
//        if(guc_lost_signal_flag)
//        {
//            Lost_Signal_Process();
//        }
//        else
//        {
//            General_Process();
//        }
        
        sl_annulus3_error_summation += gl_car_error;
        sl_annulus3_error_count ++;
//        sf_annulus3_error_average = sl_annulus3_error_summation/sl_annulus3_error_count;
    }
    else if(guc_annulus_state == 4) //准备出环，摆正车身，防止再次入环
    {
        if(guc_annulus_direction == RIGHT)
        {
          error_temp = (int32)((4.0*gui_ready_annulus_weight * (inductor_value_x1 - inductor_value_x2))
                    /(float)(inductor_value_x2 + inductor_value_x1 + 1));
          
//          error_temp = gui_ready_annulus_weight * (0.2 * error_temp + 0.8 * f_sqrt(error_temp))/100;
          
          gl_car_error = (int32)Limiter(error_temp, -450, 0);
          
          
//            error_temp = (inductor_value_y1)/((float)inductor_value_m1);
//
//            error_temp = -gui_t_div_weight * (0.8 * error_temp + 0.2 * f_sqrt(error_temp))/gf_direction_p;
//
//            if((-error_temp < -last_err))   //偏差为负,打角不能减小
//            {
//                if((err_sel_flag == 0))
//                {
//                    error_temp = last_err;
//                }
//            }
//            else
//            {
//                suc_error_reset_flag = 1;
//            }
//            error_temp = Limiter(error_temp, -ANGLE_PER_DIRECTION*(360 + gf_annulus_angle), 100);
//            //gl_car_error = (int32)Limiter(error_temp, sf_annulus3_error_aver-50, sf_annulus3_error_aver+50);

        }
        else if(guc_annulus_direction == LEFT)
        {
          error_temp = (int32)((4.0*gui_ready_annulus_weight * (inductor_value_x1 - inductor_value_x2))
                    /(float)(inductor_value_x2 + inductor_value_x1 + 1));;
          
//          error_temp = -gui_ready_annulus_weight * (0.2 * error_temp + 0.8 * f_sqrt(error_temp))/100;
          
          gl_car_error = (int32)Limiter(error_temp, 0, 450);
//            error_temp = inductor_value_y2/((float)inductor_value_m1);
//
//            error_temp = gui_t_div_weight * (0.8 * error_temp + 0.2 * f_sqrt(error_temp))/gf_direction_p;
//            
//            if((error_temp < sf_last_error))   //偏差为正,打角不能减小
//            {
//                if((err_sel_flag == 0))
//                {
//                    error_temp = sf_last_error;
//                }
//            }
//            else
//            {
//                suc_error_reset_flag = 1;
//            }
//            error_temp = Limiter(error_temp,-100, ANGLE_PER_DIRECTION * (360 - gf_annulus_angle));
//            gl_car_error = (int32)Limiter(error_temp, sf_annulus3_error_aver - 50, sf_annulus3_error_aver + 50);
        }
    }
    else if(guc_annulus_state == 5) //已出环
    {
        if(guc_annulus_direction == RIGHT)
        {
          //左环出环时，左电感值异常大，采用右电感和中间电感差比和
            error_temp = (gui_secede_annulus_weight * (inductor_value_x1 - inductor_value_m1/2.0))
            //error_temp = (gui_secede_annulus_weight * (inductor_value_x2 - inductor_value_x1))
                        /(float)((inductor_value_m1 + inductor_value_x1) + 1);
            
//            min_error_temp = MIN(-sf_last_error, -sf_annulus3_error_average) * 0.9;
//            if((-error_temp < min_error_temp))   //偏差为负,打角不能减小
//            {
//                if((suc_error_reset_flag == 0))
//                {
//                    error_temp = -min_error_temp;
//                }
//            }
//            else
//            {
//                suc_error_reset_flag = 1;
//            }
            
//            error_temp = Limiter(error_temp, MIN(-ANGLE_PER_DIRECTION * (355 + gf_annulus_angle)/gf_direction_p, -20), 200);       //16经过计算得到
            gl_car_error = (int32)Limiter(error_temp, 0, 450);
            
            //利用中间电感与两边电感偏差出环，依赖中间电感标定值，不可取
//          if(inductor_value_x1 > inductor_value_m1)
//          {
//            error_temp = (gui_t_weight * (inductor_value_x1 - inductor_value_m1))
//                  /(float)((inductor_value_m1 + inductor_value_x1) + 1);
//            
//            gl_car_error = (int32)error_temp;          
//          }
//          else
//            gl_car_error = 0;        
          
        }
        else if(guc_annulus_direction == LEFT)
        {
            error_temp = (gui_secede_annulus_weight * (inductor_value_m1/2.0 - inductor_value_x2))
            //error_temp = (gui_secede_annulus_weight * (inductor_value_x2 - inductor_value_x1))
                        /(float)((inductor_value_m1 + inductor_value_x2) + 1);
            
//            min_error_temp = MIN(sf_last_error,sf_annulus3_error_average) * 0.9;
//            if((error_temp < min_error_temp))   //偏差为正,打角不能减小
//            {
//                if((suc_error_reset_flag == 0))
//                {
//                    error_temp = min_error_temp;
//                }
//            }
//            else
//            {
//                suc_error_reset_flag = 1;
//            }
            
//            error_temp = Limiter(error_temp,-200,MAX(ANGLE_PER_DIRECTION * (355 - gf_annulus_angle)/gf_direction_p, 20));
            gl_car_error = (int32)Limiter(error_temp, -450, 0);

            //利用中间电感与两边电感偏差出环，依赖中间电感标定值，不可取
//          if(inductor_value_x2 > inductor_value_m1)
//          {
//            error_temp = (gui_t_weight * (inductor_value_m1 - inductor_value_x2))
//                  /(float)(inductor_value_x2 + inductor_value_m1 + 1);
//            
//            gl_car_error = (int32)error_temp;          
//          }
//          else
//            gl_car_error = 0;                      
        }
    }

//    sf_last_error = gl_car_error;
}

/*===============================================================
	@brief     串道处理
	@note      同丢信号打死角
==================================================================*/
void Mix_Track_Process()
{
    Lost_Signal_Process();
}

/*===============================================================
	@brief     小S处理
	@note      减小横电感权值增大竖电感权值
==================================================================*/
void Small_S_Process()
{
    int16 inductor_value_x1,inductor_value_x2;
    int16 inductor_value_y1,inductor_value_y2;
    
    inductor_value_x1 = gi_inductor_value[X_LEFT];
    inductor_value_x2 = gi_inductor_value[X_RIGHT];
    inductor_value_y1 = gi_inductor_value[T_LEFT];
    inductor_value_y2 = gi_inductor_value[T_RIGHT];
    
    gl_car_error = (int32)((0.6 * gui_x_weight * (inductor_value_x2 - inductor_value_x1)
                   +3 * gui_t_weight * (inductor_value_y2 - inductor_value_y1))
                   /(float)((inductor_value_x2 + inductor_value_x1)
                   +(inductor_value_y2 + inductor_value_y1) + 1));//加一防止除数为零
}

/*===============================================================
	@brief     十字处理
	@note      不使用竖电感，即竖电感权值为零，此处同直道处理
==================================================================*/
void Crucifix_Process()
{
    int16 inductor_value_x1,inductor_value_x2;
    
    inductor_value_x1 = gi_inductor_value[X_LEFT];
    inductor_value_x2 = gi_inductor_value[X_RIGHT];
    
    gl_car_error = (int32)((2*gui_x_weight * (inductor_value_x1 - inductor_value_x2))
                    /(float)(inductor_value_x2 + inductor_value_x1 + 1));//加一防止除数为零
}

/*===============================================================
	@brief     坡道处理
	@note      下坡减速
==================================================================*/
void Slope_Process()
{
    switch(guc_slope_state)
    {
        case RAMP_BEGIN:
        case RAMP_UP:
        case ON_RAMP:
          break;
        case RAMP_DOWN:
          gl_car_speed_set = gl_speed_lost_line;
          break;
        default:
          break;
    }
}
