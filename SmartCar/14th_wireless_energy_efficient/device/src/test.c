/*=============================================
    @file         test.c
    @brief        各模块测试函数
    @programmer   Beyond Hsueh
=============================================*/

#include "test.h"

/*===============================================================
        @note      春未老，风细柳斜斜；
                   试上超然台上望，半壕春水一城花，烟雨暗千家。
                   寒食后，酒醒却咨嗟；
                   休对故人思故国，且将新火试新茶，诗酒趁年华。
==================================================================*/   

/*===============================================================
	@brief      拨码测试
	@function   将拨码值显示在oled上   
==================================================================*/
void Switch_Test()
{
    uint8 uc_switch_pin_temp;

    //OLED_Fill(0x00);
    oled_display_6x8str(20,0,"switch test");
    //读取拨码器
    Get_Switch_Value(&uc_switch_pin_temp, SW0_0);//第0位
    guc_switch_pin_value[0] = uc_switch_pin_temp;
    Get_Switch_Value(&uc_switch_pin_temp, SW0_1);//第1位
    guc_switch_pin_value[1] = uc_switch_pin_temp;
    Get_Switch_Value(&uc_switch_pin_temp, SW1_0);//第2位
    guc_switch_pin_value[2] = uc_switch_pin_temp;
    Get_Switch_Value(&uc_switch_pin_temp, SW1_1);//第3位
    guc_switch_pin_value[3] = uc_switch_pin_temp;	
    Get_Switch_Value(&uc_switch_pin_temp, SW1_2);//第4位
    guc_switch_pin_value[4] = uc_switch_pin_temp;		
    Get_Switch_Value(&uc_switch_pin_temp, SW1_3);//第5位
    guc_switch_pin_value[5] = uc_switch_pin_temp;	
    
    //显示拨码器		
    oled_display_boolean(30, 4, guc_switch_pin_value[0]);
    oled_display_boolean(50, 4, guc_switch_pin_value[1]);
    oled_display_boolean(70, 4, guc_switch_pin_value[2]);
    
    oled_display_boolean(30, 5, guc_switch_pin_value[3]);
    oled_display_boolean(50, 5, guc_switch_pin_value[4]);
    oled_display_boolean(70, 5, guc_switch_pin_value[5]);	

}

/*===============================================================
	@brief      LED测试
	@function   对应拨码值
==================================================================*/
void LED_Test()
{
    uint8 uc_switch_value = 0;
    
    //OLED_Fill(0x00);
    oled_display_6x8str(30,0,"led test");
    oled_display_6x8str(0,3,"set switch to test led");

    //根据拨码值更新led灯的状态
    uc_switch_value = Get_Switch_Value(NULL,0);
    uc_switch_value &= 0x1f;
    Set_LED(uc_switch_value);	
}

/*===============================================================
	@brief      五向按键测试
	@function   将按键值显示在oled上   
==================================================================*/
void Five_Dir_Test()
{
    uint8 uc_button_value;
    
    while(1)
    {
        uc_button_value = Get_Five_Dir_Value(&uc_button_value);
        
        switch(uc_button_value)
        {
          case FIVE_DIR_UP:
            oled_display_6x8str(1,1,"up");
//            LPLD_GPIO_Output_b(LED_PIN_1_PORT, LED_PIN_1, LED_BIT_ON);
//            LPLD_GPIO_Output_b(LED_PIN_2_PORT, LED_PIN_2, LED_BIT_OFF);
//            LPLD_GPIO_Output_b(LED_PIN_3_PORT, LED_PIN_3, LED_BIT_OFF);
//            LPLD_GPIO_Output_b(LED_PIN_4_PORT, LED_PIN_4, LED_BIT_OFF);
            break;
            
          case FIVE_DIR_LEFT:
            oled_display_6x8str(1,2,"left");
//            LPLD_GPIO_Output_b(LED_PIN_1_PORT, LED_PIN_1, LED_BIT_ON);
//            LPLD_GPIO_Output_b(LED_PIN_2_PORT, LED_PIN_2, LED_BIT_OFF);
//            LPLD_GPIO_Output_b(LED_PIN_3_PORT, LED_PIN_3, LED_BIT_OFF);
//            LPLD_GPIO_Output_b(LED_PIN_4_PORT, LED_PIN_4, LED_BIT_OFF);
            break;  
            
          case FIVE_DIR_DOWN:
            oled_display_6x8str(1,3,"down");
//            LPLD_GPIO_Output_b(LED_PIN_1_PORT, LED_PIN_1, LED_BIT_OFF);
//            LPLD_GPIO_Output_b(LED_PIN_2_PORT, LED_PIN_2, LED_BIT_ON);
//            LPLD_GPIO_Output_b(LED_PIN_3_PORT, LED_PIN_3, LED_BIT_OFF);
//            LPLD_GPIO_Output_b(LED_PIN_4_PORT, LED_PIN_4, LED_BIT_OFF);
            break;
            
          case FIVE_DIR_RIGHT:
            oled_display_6x8str(1,4,"right");
//            LPLD_GPIO_Output_b(LED_PIN_1_PORT, LED_PIN_1, LED_BIT_OFF);
//            LPLD_GPIO_Output_b(LED_PIN_2_PORT, LED_PIN_2, LED_BIT_OFF);
//            LPLD_GPIO_Output_b(LED_PIN_3_PORT, LED_PIN_3, LED_BIT_ON);
//            LPLD_GPIO_Output_b(LED_PIN_4_PORT, LED_PIN_4, LED_BIT_OFF);
            break;
          
          case FIVE_DIR_OK:
            oled_display_6x8str(1,5,"ok");
//            LPLD_GPIO_Output_b(LED_PIN_1_PORT, LED_PIN_1, LED_BIT_OFF);
//            LPLD_GPIO_Output_b(LED_PIN_2_PORT, LED_PIN_2, LED_BIT_OFF);
//            LPLD_GPIO_Output_b(LED_PIN_3_PORT, LED_PIN_3, LED_BIT_OFF);
//            LPLD_GPIO_Output_b(LED_PIN_4_PORT, LED_PIN_4, LED_BIT_ON);
            break;
            
          default:
            oled_fill(0x00);
//            LPLD_GPIO_Output_b(LED_PIN_1_PORT, LED_PIN_1, LED_BIT_ON);
//            LPLD_GPIO_Output_b(LED_PIN_2_PORT, LED_PIN_2, LED_BIT_ON);
//            LPLD_GPIO_Output_b(LED_PIN_3_PORT, LED_PIN_3, LED_BIT_ON);
//            LPLD_GPIO_Output_b(LED_PIN_4_PORT, LED_PIN_4, LED_BIT_ON);
            break;
        }
    }  
}

/*===============================================================
	@brief      编码器测试
	@function   显示编码盘计数和转速   
==================================================================*/
void Encoder_Test()
{
      //显示小数值
  
    //以前计数存在bug，不在计时器中进行
    //Get_Car_Distance();
      
    oled_print_short(60,2,gl_car_speed);
    oled_print_short(60,3,gl_car_running_distance);
    
    oled_display_6x8str(0,2,"speed");
    oled_display_6x8str(0,3,"distance");
    oled_display_6x8str(0,4,"left");
    oled_display_6x8str(0,5,"right");
    oled_display_6x8str(0,6,"time");
    oled_display_6x8str(0,7,"pwm");

    oled_print_short(60,4,gl_left_encoder_value);
    oled_print_short(60,5,gl_right_encoder_value);


    //VisualScope_Send(SCOPE_DATA_MODE);//待测试蓝牙
}

/*===============================================================
	@brief      电感信号测试
==================================================================*/
void Signal_Test(vint16 inductor_value[])
{
    int8 i;
    
    for(i = NUM_INDUCTOR - 1; i >= 0; i--)
    {
      oled_print_short(20, i, inductor_value[i]);
      gf_visual_scope_data[i]=inductor_value[i]*10;
    }
    oled_print_short(20,5,gi_inductor_x_offset);
    oled_print_short(20,6,guc_reed_state_value);
    oled_print_short(20,7,gl_direction_output);
    
    oled_print_short(85,0,gl_car_error);
    oled_print_short(85,1,gl_pwm_output);
    oled_print_short(85,2,gl_car_speed);
    oled_print_short(85,3,gl_car_speed_set);
    
    if(guc_distinction_flag == GENERAL)
    {
        oled_display_6x8str(85,4,"GENERAL");
    }
    else if(guc_distinction_flag == CRUCIFIX)
    {
        oled_display_6x8str(85,4,"CRUCIFIX");
    }
    else if(guc_distinction_flag == ANNULUS)
    {
        oled_display_6x8str(85,4,"ANNULUS");
    }
    else if(guc_distinction_flag == SLOPE)
    {
        oled_display_6x8str(85,4,"SLOPE");
    }
    else if(guc_distinction_flag == OBSTACLE)
    {
        oled_display_6x8str(85,4,"OBSTACLE");
    }
    else
    {
        oled_display_6x8str(85,4,"NO");
    }
    
    oled_print_short(85,5,guc_car_run_flag);
    oled_print_short(85,6,gl_car_running_distance);
    oled_print_short(85,7,(int16)gf_angularspeed_rotate);
    
    oled_display_6x8str(1,0,"y1");
    oled_display_6x8str(1,1,"x1");
    oled_display_6x8str(1,2,"m1");
    oled_display_6x8str(1,3,"x2");
    oled_display_6x8str(1,4,"y2");
    oled_display_6x8str(1,5,"xe");
    oled_display_6x8str(1,6,"rd");
    oled_display_6x8str(1,7,"dr");
    
    oled_display_6x8str(65,0,"err");
    oled_display_6x8str(65,1,"pwm");
    oled_display_6x8str(65,2,"spd");
    oled_display_6x8str(65,3,"set");
    oled_display_6x8str(65,4,"sta");
    oled_display_6x8str(65,5,"run");
    oled_display_6x8str(65,6,"dis");
    oled_display_6x8str(65,7,"yaw");
    
//    VisualScope_Send(SCOPE_DATA_MODE);//待测试蓝牙
}

/*===============================================================
	@brief      环岛信号测试
==================================================================*/
void Annulus_Test(vint16 inductor_value[])
{
    int8 i;

    for(i = NUM_INDUCTOR - 1; i >= 0; i--)
    {
      oled_print_short(20, i, inductor_value[i]);
      gf_visual_scope_data[i]=inductor_value[i]*10;
    }
    oled_print_short(20,5,gi_inductor_x_offset);
    oled_print_short(20,6,gi_inductor_t_offset);
    oled_print_short(20,7,gl_car_error);
    
    oled_print_short(85,0,guc_annulus_state);
    
    if(guc_annulus_direction == LEFT)
    {
        oled_display_6x8str(85,1,"left");
    }
    else if(guc_annulus_direction == RIGHT)
    {
        oled_display_6x8str(85,1,"right");
    }
    else
        oled_display_6x8str(85,1," no ");
    
    oled_print_short(85,2,gl_car_speed);
    oled_print_short(85,3,(int16)(gf_yaw_angle));
    oled_print_short(85,4,(int16)(10*gf_yaw_velocity));
    oled_print_short(85,5,(int16)(gf_annulus_angle));
    oled_print_short(85,6,gl_annulus_distance_record);
    oled_print_short(85,7,gl_car_running_distance);
    
    oled_display_6x8str(1,0,"y1");
    oled_display_6x8str(1,1,"x1");
    oled_display_6x8str(1,2,"m1");
    oled_display_6x8str(1,3,"x2");
    oled_display_6x8str(1,4,"y2");
    oled_display_6x8str(1,5,"xe");
    oled_display_6x8str(1,6,"ye");
    oled_display_6x8str(1,7,"er");
    
    oled_display_6x8str(65,0,"ans");
    oled_display_6x8str(65,1,"dir");
    oled_display_6x8str(65,2,"spd");
    oled_display_6x8str(65,3,"g");
    oled_display_6x8str(65,4,"v");
    oled_display_6x8str(65,5,"aus");
    oled_display_6x8str(65,6,"rs");
    oled_display_6x8str(65,7,"ds");
    
    VisualScope_Send(SCOPE_DATA_MODE);//待测试蓝牙
}

/*===============================================================
	@brief      电感变化率测试
==================================================================*/
void Gradient_Test()
{
    int8 i;

    for(i = NUM_INDUCTOR - 1; i >= 0; i--)
    {
      oled_print_short(20, NUM_INDUCTOR - 1 - i, gi_signal_gradient[i]);
      gf_visual_scope_data[i] = gi_signal_gradient[i]*10;
    }
    oled_print_short(20,5,gi_x_signal_gradient);
    oled_print_short(20,6,gi_y_signal_gradient);
    oled_print_short(20,7,gl_pwm_output);
    
    for(i = NUM_INDUCTOR - 1; i >= 0; i--)
    {
      oled_print_short(85,NUM_INDUCTOR - 1 - i,gi_inductor_adc[i]);
      gf_visual_scope_data[i] = gi_inductor_adc[i]*10;
    }
    oled_print_short(85,5,gi_inductor_x_offset);
    oled_print_short(85,6,gi_inductor_t_offset);
    oled_print_short(85,7,gl_car_running_distance);
    
    oled_display_6x8str(1,0,"y1");
    oled_display_6x8str(1,1,"x1");
    oled_display_6x8str(1,2,"m1");
    oled_display_6x8str(1,3,"x2");
    oled_display_6x8str(1,4,"y2");
    oled_display_6x8str(1,5,"xe");
    oled_display_6x8str(1,6,"ye");
    oled_display_6x8str(1,7,"ds");
    
    oled_display_6x8str(65,0,"y1");
    oled_display_6x8str(65,1,"x1");
    oled_display_6x8str(65,2,"m1");
    oled_display_6x8str(65,3,"x2");
    oled_display_6x8str(65,4,"y2");
    oled_display_6x8str(65,5,"xe");
    oled_display_6x8str(65,6,"ye");
    oled_display_6x8str(65,7,"sp");
    
    VisualScope_Send(SCOPE_DATA_MODE);//待测试蓝牙
}

/*===============================================================
	@brief      陀螺仪矫正测试
==================================================================*/
void MPU6050_Test()
{
    oled_print_short(20,0,(int16)(10*gf_yaw_velocity));
    oled_print_short(20,1,(int16)(gf_yaw_angle));
    oled_print_short(20,2,(int16)(10*gf_gyro_bias_z));
    oled_print_short(20,3,(int16)(10*gf_pitch_velocity));
    oled_print_short(20,4,(int16)(gf_pitch_angle));
    oled_print_short(20,5,(int16)(10*gf_roll_velocity));
    oled_print_short(20,6,(int16)(gf_roll_angle));

    
    oled_print_short(85,0,gl_car_error);
    oled_print_short(85,1,gl_pwm_output);
    oled_print_short(85,2,gl_car_speed);
    oled_print_short(85,3,gl_car_speed_set);
    oled_print_short(85,4,gl_time_infrared);
    
    oled_display_6x8str(1,0,"yv");
    oled_display_6x8str(1,1,"ya");
    oled_display_6x8str(1,2,"gzb");
    oled_display_6x8str(1,3,"pv");
    oled_display_6x8str(1,4,"pa");
    oled_display_6x8str(1,5,"rv");
    oled_display_6x8str(1,6,"ra");
    
    oled_display_6x8str(70,0,"er");
    oled_display_6x8str(70,1,"pw");
    oled_display_6x8str(70,2,"sp");
    oled_display_6x8str(70,3,"st");
    oled_display_6x8str(70,4,"te");
    
    VisualScope_Send(SCOPE_DATA_MODE);//待测试蓝牙
}

/*===============================================================
	@brief      路障测试
==================================================================*/
void Obstacle_Test()
{
    oled_print_short(20,0,gi_infrared_AD_distance);
    oled_print_short(20,1,guc_infrared_ranging_flag);
    oled_print_short(20,2,guc_obstacle_state);
    oled_print_short(20,3,(int16)(gf_camber_angle));
    oled_print_short(20,4,(int16)(10*gf_yaw_velocity));
    oled_print_short(20,5,guc_obstacle_flag);
    
    if(guc_avoidance_direction_flag == LEFT)
    {
        oled_display_6x8str(20,6,"left");
    }
    else if(guc_avoidance_direction_flag == RIGHT)
    {
        oled_display_6x8str(20,6,"right");
    }
    else
    {
        oled_display_6x8str(20,6," no ");
    }
        
    
    oled_print_short(20,7,guc_obstacle_complete_flag);
    
    oled_print_short(85,0,gi_infrared_2nd_AD_distance);
    
    if(guc_slope_complete_flag == ON)
    {
        oled_display_6x8str(85,1,"ok");
    }
    else if(guc_slope_complete_flag == OFF)
    {
        oled_display_6x8str(85,1,"no");
    }
    else
    {
        oled_display_6x8str(85,1,"error");
    }
        
    if(guc_first_obstacle_slope_flag == SLOPE_FIRST)
    {
        oled_display_6x8str(85,2,"s_first");
    }
    else if(guc_first_obstacle_slope_flag == OBSTACLE_FIRST)
    {
        oled_display_6x8str(85,2,"o_first");
    }
    else
    {
        oled_display_6x8str(85,2,"error");
    }
    
    oled_print_short(85,3,(int16)gf_obstacle_error);
    oled_print_short(85,4,gi_inductor_value[M_BACK]);
    
    if(guc_obstacle_state == 1)
    {
        oled_print_short(85,5,gi_aviodence_angle);
    }
    else if(guc_obstacle_state == 2)
    {
        oled_print_short(85,5,gi_regression_angle);
    }
    else
    {
        oled_print_short(85,5,0);
    }
    
    oled_print_short(85,6,gl_obstacle_distance_record);
    oled_print_short(85,7,gl_car_running_distance);
    
    oled_display_6x8str(1,0,"IU");
    oled_display_6x8str(1,1,"cf");
    oled_display_6x8str(1,2,"os");
    oled_display_6x8str(1,3,"ag");
    oled_display_6x8str(1,4,"yv");
    oled_display_6x8str(1,5,"of");
    oled_display_6x8str(1,6,"dr");
    oled_display_6x8str(1,7,"oc");
    
    oled_display_6x8str(70,0,"ID");
    oled_display_6x8str(70,1,"sc");
    oled_display_6x8str(70,2,"st");
    oled_display_6x8str(70,3,"ce");
    oled_display_6x8str(70,4,"m1");
    
    if(guc_obstacle_state == 1)
    {
        oled_display_6x8str(70,5,"s1");
    }
    else if(guc_obstacle_state == 2)
    {
        oled_display_6x8str(70,5,"s2");
    }
    else
    {
        oled_display_6x8str(70,5,"no");
    }
    
    oled_display_6x8str(70,6,"rs");
    oled_display_6x8str(70,7,"ds");
    
    VisualScope_Send(SCOPE_DATA_MODE);//待测试蓝牙
}

/*===============================================================
	@brief      红外信号测试
	@function   显示红外数值   
==================================================================*/
void Test_Infrared()
{ 
  while(1)
  {
    gl_time_infrared = get_infrared_command();
    oled_print_short(1,1,(int16)gl_time_infrared);    
  }
}

/*===============================================================
	@brief      坡道信号测试
==================================================================*/
void Slope_Test(vint16 inductor_value[])
{
    int8 i;

    for(i = NUM_INDUCTOR - 1; i >= 0; i--)
    {
      oled_print_short(20,NUM_INDUCTOR - 1 - i,inductor_value[i]);
      gf_visual_scope_data[i]=inductor_value[i]*10;
    }
    oled_print_short(20,5,gl_slope_distance_record);
    oled_print_short(20,6,gl_car_running_distance);
    oled_print_short(20,7,gl_car_error);
    
    oled_print_short(85,0,guc_slope_state);
    oled_print_short(85,1,gi_pitch_variance);
    oled_print_short(85,2,gl_car_speed);
    oled_print_short(85,3,(int16)(gf_pitch_angle));
    oled_print_short(85,4,(int16)(gf_pitch_velocity));
    oled_print_short(85,5,(int16)(gf_slope_start_angle));
    oled_print_short(85,6,(int16)(gf_slope_picth_angle));
    oled_print_short(85,7,guc_slope_pitch_flag);
    
    oled_display_6x8str(1,0,"y1");
    oled_display_6x8str(1,1,"x1");
    oled_display_6x8str(1,2,"m1");
    oled_display_6x8str(1,3,"x2");
    oled_display_6x8str(1,4,"y2");
    oled_display_6x8str(1,5,"rs");
    oled_display_6x8str(1,6,"ds");
    oled_display_6x8str(1,7,"er");
    
    oled_display_6x8str(65,0,"st");
    oled_display_6x8str(65,1,"ps");
    oled_display_6x8str(65,2,"spd");
    oled_display_6x8str(65,3,"pa");
    oled_display_6x8str(65,4,"pv");
    oled_display_6x8str(65,5,"ba");
    oled_display_6x8str(65,6,"sa");
    oled_display_6x8str(65,7,"if");
    
    VisualScope_Send(SCOPE_DATA_MODE);//待测试蓝牙
}
