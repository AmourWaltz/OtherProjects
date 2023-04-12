/*=============================================
    @file         direction.c
    @brief        方向控制
    @programmer   Beyond Hsueh
=============================================*/

#include "direction.h"

/*===============================================================
        @note      独立寒秋，湘江北去，橘子洲头；
                   看万山红遍，层林尽染；漫江碧透，百舸争流。
                   鹰击长空，鱼翔浅底，万类霜天竞自由。
                   怅寥廓，问苍茫大地，谁主沉浮？
                   携来百侣曾游，忆往昔峥嵘岁月稠；
                   恰同学少年，风华正茂；书生意气，挥斥方遒。
                   指点江山，激扬文字，粪土当年万户侯。 
                   曾记否，到中流击水，浪遏飞舟？
==================================================================*/

float gf_direction_out_old;//保留上一次方向控制输出量
float gf_direction_out_new;//新的方向控制输出量
uint8  gc_direction_period;//方向控制周期

/*===============================================================
	@brief     线性化处理，差速与信号偏差成线性关系
==================================================================*/
float Linear_Offset(float offset)
{
    float offset_temp;
    
    if(offset >= 30 && offset <= 60)
    {
      offset_temp=offset - 30;
      offset=(int16)(offset_temp *2 + 30);
    }
    else if(offset > 60 && offset <= 90)
    {
      offset_temp=offset - 60;
      offset=(int16)(offset_temp*1.8 + 30 + 30*2);
    }  
    else if(offset > 90)
    {
      offset_temp=offset - 90;
      offset=(int16)(offset_temp*1.5+ 30 + 30*2 + 30*1.8);
    }    
    else if(offset <= -30 && offset >= -60)
    {
      offset_temp=offset + 30;
      offset=(int16)(offset_temp*2 - 30);
    }    
    else if(offset < -60 && offset >= -90)
    {
      offset_temp=offset + 60;
      offset=(int16)(offset_temp*1.8 - 30 - 30*2);
    }  
    else if(offset < -90)
    {
      offset_temp=offset + 90;
      offset=(int16)(offset_temp*1.5 - 30 - 30*2 - 30*1.8);
    }
    
    gl_car_error = (int32)offset;
    
    return offset;
}

/*===============================================================
	@brief     位置式PD控制输出控制车模方向
==================================================================*/
void Direction_Control() 
{
  int32 delta_offset;
  static int32 sl_offset_old;
  
  delta_offset = gl_car_error - sl_offset_old;
   
   if(gl_car_error < OFFSET_MINIMUM && gl_car_error > -OFFSET_MINIMUM) 
   {
       gl_direction_set = 0;
   } 
   else 
   {
       gl_direction_set = (int32)(gl_car_error * gf_direction_p / 10 +  delta_offset * gf_direction_d / 10 );
   }
   
   gl_direction_output = gl_direction_set;
   
   sl_offset_old = gl_car_error;
}

/*===============================================================
	@brief     方向输出周期控制
==================================================================*/
void Direction_Control_Output() 
{
   float value;
   
   value = gf_direction_out_new - gf_direction_out_old;
   
   gl_direction_output = (uint16)(value * (gc_direction_period + 1) / 
   DIRECTION_CONTROL_PERIOD + gf_direction_out_old);
}

//float g_dir_p_1,g_dir_d_1,g_dir_p_2,g_dir_d_2;

/*===============================================================
	@brief     外环赛道偏差控制方向
==================================================================*/
void Direction_Calculate()
{
    float use_offset = 0;
    float offset_delta;
    static float sf_last_offset[3]={0}; //D参数只取两个周期的差值效果不明显
    
    use_offset = gl_car_error;

    sf_last_offset[0] = sf_last_offset[1];
    sf_last_offset[1] = sf_last_offset[2];
    sf_last_offset[2] = use_offset;
    
    offset_delta = sf_last_offset[2] - sf_last_offset[0];
    
    gl_direction_set = (int16)(use_offset * gf_direction_p / 100 +  offset_delta * gf_direction_d / 100 );
 
//     //自适应系数，根据err和err_div同向反向分别给PD参数
//    if ((use_offset >=0 && offset_div >= 0)||(use_offset < 0 && offset_div <0))
//    {
//       gl_direction_set = g_dir_p_1 * use_offset + g_dir_d_1 * offset_div;//P稍大D稍小     
//    }
//    else
//    {
//       gl_direction_set = g_dir_p_2 * use_offset + g_dir_d_2 * offset_div;//P稍小D稍大      
//    }        

//    gl_direction_set = (int32)Limiter(gl_direction_set, -DIRECTION_SET_MAX, DIRECTION_SET_MAX);
    
//    gl_direction_set /= (gf_direction_p*4/100);
   
}

/*===============================================================
	@brief     内环角速度控制转向
==================================================================*/
void Angularspeed_Control_Direction(int32 set)
{
    static float sf_last_error_angluarspeed = 0.0;
    float angularspeed_error = 0.0;
    float angularspeed_error_delta = 0.0;
    
    gf_angularspeed_rotate = gf_yaw_velocity * P_ROTATE_SPEED;
       
    angularspeed_error = set - gf_angularspeed_rotate;

    angularspeed_error_delta = angularspeed_error - sf_last_error_angluarspeed;
    sf_last_error_angluarspeed = angularspeed_error;
    
    gl_direction_output = (int16)(angularspeed_error * gf_angularspeed_p / 10 + angularspeed_error_delta * gf_angularspeed_d / 10); 
     
    //方向控制输出限幅
    gl_direction_output = (int32)Limiter(gl_direction_output, -DIRECTION_PWM_MAX, DIRECTION_PWM_MAX);

}
