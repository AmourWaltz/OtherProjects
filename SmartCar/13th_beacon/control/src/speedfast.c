/*=============================================
    @file         speed.c
    @brief        速度控制
    @programmer   XBY
=============================================*/
#include "speedfast.h"
 
/*===============================================================
	@brief     速度控制函数
	@function  增量式PI控制速度输出（未加编码器，暂为开环控制）
	@note      加入编码器后改为闭环控制
==================================================================*/

void Speed_Control()
{
  static int16 car_speed = 0;
//  static int16 prev_error = 0;
  static int16 last_error = 0;
  static int16 error_delta = 0;
  static int16 speed_error;
  static int16 last_pwm_out;
//  static int8 last_err_flag = 0;

   car_speed = gl_speed_set;

   car_speed -= Set_Image_Speed();
   car_speed -= Set_Angle_Speed();
   
   Get_Car_Condition();
   
   if(gl_car_speed < 185)
   {
      car_speed = 185;
   }

   speed_error = (int16)(car_speed - gl_car_speed);//给定速度-实际速度
   
//   if(last_err_flag == 0)
//   {
//      error_delta = 0;
//      last_err_flag = 1;
//   }
//   else
//   {
//      error_delta = speed_error - last_error;
//   }
   error_delta = speed_error - last_error;

   last_pwm_out = (int32)(gf_speed_p * speed_error / 100 + gf_speed_i * error_delta / 100);
   
   gl_pwm_out = (int32)(gl_pwm_out + last_pwm_out);
   
   if(gl_pwm_out > 10000)
     gl_pwm_out = 10000;
   else if(gl_pwm_out < -2000)
     gl_pwm_out = -2000;
   
   //存储误差，用于下次计算
//   prev_error=last_error;
   last_error = speed_error;
    
   Motor_Output();
}

/*===============================================================
	@brief     设定图像速度值
	@function  根据图像大小间接反映距离信标灯距离改变速度，设定一阈值，达到后减速
	@return    改变后的速度值
	@note      阈值不确定，通过建模软件分析二次函数关系确定阈值
==================================================================*/

int Set_Image_Speed()
{
  int32 area_temp;
//  static float acce_kp = 0.2;
//  static float dece_kp = 0.1;//升级为全局变量
  
  int img_speed_delta = 0;//根据图像距离设定速度变化量
  
  area_temp = gi_image_area_ave;
  
  if(area_temp < 0)
    area_temp = -area_temp;
  
  if((area_temp >= gi_area_thredhold_1)&&(area_temp <= gi_area_thredhold_2))
    img_speed_delta = (int)((area_temp - gi_area_thredhold_1) *gf_dece_kp/100);
  else if(area_temp > gi_area_thredhold_2)
    img_speed_delta = (int)((area_temp - gi_area_thredhold_1) *gf_dece_kp/80);
  else if((area_temp < gi_area_thredhold_1)&&(gi_image_center_ave > 10))
    img_speed_delta = (int)((area_temp - gi_area_thredhold_1) *gf_acce_kp/100);

  return img_speed_delta;
}

/*===============================================================
	@brief     弯道减速
	@function  根据舵机打角程度将速度减小至某一值   
	@return    改变后的速度值
	@note      只为初期预判值，后期需精调
==================================================================*/

int Set_Angle_Speed()
{   
  int32 servo_output;
  int servo_angle;
  int angle_speed_delta;
//  static float angle_speed_kp = 0.7;//升级为全局变量
  
  servo_output = gl_servo_pwm;
  //servo_angle = Servo_Angle(servo_output);
  servo_angle = gl_direction_set;
  
  if(servo_angle < 0)
    servo_angle = -servo_angle;
  
  angle_speed_delta = (int)((gf_angle_speed_kp/1000) * servo_angle);
  
  return angle_speed_delta;
}

/*===============================================================
	@brief     获取小车速度及路程
==================================================================*/

void Get_Car_Condition()
{
    static int16 si_car_speed = 0;
    
    gi_left_speed = (int16)(Get_Left_Speed());
    gi_right_speed = (int16)(Get_Right_Speed());
    
    si_car_speed = (int16)((gi_left_speed + gi_right_speed)/2);
    
    gl_car_speed = si_car_speed;
    
    gl_car_running_distance += (int32)(si_car_speed * 0.5);
}

/*===============================================================
	@brief     利用舵机增量计算左右轮差速
==================================================================*/

void Speed_PWM_Delta(int16 *left_expect_speed, int16 *right_expect_speed, int32 center_expect_speed)
{ 
        static int PWM_delta[16] = {0, 8, 15, 23,
                                     28, 35, 39, 44, 
                                     48, 52, 56, 60, 
                                     64, 68, 72, 75};

        static float Speed_Inside[16] = {0.9261,0.9161,0.9061,0.8961,
                                          0.8861,0.8761,0.8661,0.8561,
                                          0.8411,0.8311,0.8211,0.8111,
                                          0.8011,0.7861,0.7711,0.7561};
	static float Speed_Outside[16] ={1.0739,1.0839,1.0939,1.1039,
                                          1.1139,1.1239,1.1339,1.1439,
                                          1.1589,1.1689,1.1789,1.1889,
                                          1.1989,1.2139,1.2289,1.2439};

	int flag = 0, i = 0;
	int16  speed_left_temp = 0, speed_right_temp = 0;
        int16 servo_PWM_delta = 0; 
        servo_PWM_delta = Get_Servo_PWM_Delta();
        
        if (center_expect_speed == 0)
        {
   		speed_left_temp = 0;
		speed_right_temp = 0;     
        }
        else if (servo_PWM_delta >= -10 && servo_PWM_delta <= 10)
        {
            speed_left_temp = center_expect_speed;
            speed_right_temp = center_expect_speed;  
        }
        else if (servo_PWM_delta < -10)//向右转
        {
             servo_PWM_delta = -servo_PWM_delta;
             for(i = 0; i <= 14; i++)
             {
                if (servo_PWM_delta > PWM_delta[i] && servo_PWM_delta <= PWM_delta[i+1])
                {
                    flag = i+1;
                    break;
                }
             }
             
            if(i >= 15)
            {
                 flag = 15;
            }
             
            speed_left_temp = (int16)(center_expect_speed * (Speed_Outside[flag]+0.035));
            speed_right_temp = (int16)(center_expect_speed *(Speed_Inside[flag]-0.012));
        }
        else
        {
             for(i = 0; i <= 14; i++)
             {
                if (servo_PWM_delta > PWM_delta[i] && servo_PWM_delta <= PWM_delta[i+1])
                {
                    flag = i+1;
                    break;
                }
             }

            if(i >= 15)
            {
                 flag = 15;
            }

            speed_right_temp = (int16)(center_expect_speed * (Speed_Outside[flag]+0.035));
            speed_left_temp = (int16)(center_expect_speed *(Speed_Inside[flag]-0.012));
        }

	*left_expect_speed = speed_left_temp;
	*right_expect_speed = speed_right_temp;
}