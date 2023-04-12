/*=============================================
    @file         direction.c
    @brief        方向控制      
    @programmer   XBY
=============================================*/

#include "direction.h"

/*===============================================================
	@brief     线性化差值
	@function  线性化处理，舵机打角与图像设定中心值偏差成正比
	@parameter 打角偏差值
	@return    线性化处理后的差值
	@note      只为初期预判值，后期需精调
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
    
    gl_servo_angle = (int32)offset;
    
    return offset;
}

/*===============================================================
	@brief     舵机打角输出匹配
	@function  根据传入的舵机打角或舵机输出匹配相应的输出或打角   
	@parameter 舵机打角值或输出PWM
	@return    相匹配的舵机打角值或输出PWM
	@note      只为初期预判值，后期需精调
==================================================================*/

int16 Servo_Angle(int angle_servo_temp)
{
  //舵机打角与输出匹配数组表，待定
  static int16 gi_angle_servo[2][ANGLE_TABLE_DIMENSION]  = {						
    {-36, -35, -34, -33, -32, -31, -30, -29, -28, -27, 
    -26, -25, -24, -23, -22, -21, -20, -19, -18, -17, 
    -16, -15, -14, -13, -12, -11, -10, -9, -8, -7, 
    -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 
    8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 ,36},	
    {1530, 1500, 1497, 1494, 1491, 1488, 1485, 1483, 1479, 
    1476, 1473, 1470, 1467, 1464, 1461, 1458, 1455, 1452, 
    1449, 1446, 1443, 1440, 1438, 1436, 1433, 1430, 1427, 
    1424, 1421, 1419, 1416, 1413, 1410, 1407, 1404, 1402, 
    1400, 1399, 1397, 1392, 1386, 1380, 1374, 1368, 1362, 
    1356, 1350, 1344, 1338, 1332, 1326,	1320, 1318, 1316, 
    1314, 1312, 1310, 1308, 1306, 1304, 1302, 1300, 1298, 
    1296, 1294, 1292, 1290, 1288, 1286, 1284, 1282, 1280, 1270}
    };
  int loop_i,middle_servo;
  
  if((angle_servo_temp <= 50) && (angle_servo_temp >= -50))
  {
    angle_servo_temp = (int)Limiter(angle_servo_temp, 36 ,-36);
    for(loop_i=0; loop_i<ANGLE_TABLE_DIMENSION; loop_i++)
    {
      if(gi_angle_servo[0][loop_i] == angle_servo_temp)
        return gi_angle_servo[1][loop_i];
    }
  }
  else if((angle_servo_temp <= 1600) && (angle_servo_temp >= 1200))
  {
    angle_servo_temp = (int)Limiter(angle_servo_temp, 1530, 1270);
    for(loop_i=0; loop_i<ANGLE_TABLE_DIMENSION; loop_i++)
    {
      if(gi_angle_servo[1][loop_i] == angle_servo_temp)
        return gi_angle_servo[1][loop_i];
      else if((angle_servo_temp >= gi_angle_servo[1][loop_i]) && (angle_servo_temp <= gi_angle_servo[1][loop_i+1]))
      {
        //舵机输出值处于输出表两值之间时，总是取相对较大的那一个角度值
//        middle_servo = (gi_angle_servo[1][loop_i] + gi_angle_servo[1][loop_i+1]) / 2;
//        if((angle_servo_temp >= middle_servo) && (angle_servo_temp > SERVO_CENTER))
//           return gi_angle_servo[1][loop_i+1];
//        else 
//          return gi_angle_servo[1][loop_i];
      }
    }
  }
  else
    return 500;//只是代表一个无效数值，如果使用0可能与舵机不打角相重合
}

/*===============================================================
	@brief     舵机输出控制
	@function  位置式PD控制舵机以及最小二乘法拟合微分项
	@note      最小二乘法项数更改，后期尝试使用自适应PD系数
==================================================================*/

void Direction_Control()
{
    static int last_line_err[5];		//上两次偏差
    static float angle_error_Kp;               //图像偏差与角度值的转换系数
    float dir_err_temp;
    float dir_d_err;
    
    dir_err_temp = gi_image_err;
    
    //dir_err_temp = Linear_Offset(dir_err_temp);

    last_line_err[0] = last_line_err[1];
    last_line_err[1] = last_line_err[2];
    last_line_err[2] = last_line_err[3];
    last_line_err[3] = last_line_err[4];
    last_line_err[4] = (int)(100*dir_err_temp);
    
    least_spuare(last_line_err,3,&dir_d_err,NULL,NULL);
    dir_d_err /= 100;
    
    gl_direction_set = (int32)(dir_err_temp * gf_direction_p/100 + dir_d_err * gf_direction_d/100);
    
    Sevro_Output(gl_direction_set); 
}
