/*=============================================
    @file         display.c
    @brief        运动参数显示文件
    @programmer   Beyond Hsueh
=============================================*/

#include "display.h"

/*===============================================================
        @note      劝君莫惜金缕衣，劝君惜取少年时；
                   花开堪折直须折，莫待无花空折枝。
==================================================================*/

/*===============================================================
	@brief      信号显示页
	@parameter  所属的页数
	@return     根据按键左右返回需要进入的下一页页数
==================================================================*/
uint8 Signal_Parameter(uint8 page_num)
{
    int8  exit_flag = 0;
    uint8 button_value = 0;

    //显示信息
    oled_fill(0x00);

    while(!exit_flag)
    {
#ifdef COMPETITION_MODE
        if(guc_car_run_flag == CAR_RUN)
        {
            
        }
        else
        {
            Signal_Test(gi_inductor_value);
        }
#else
      if(guc_automation_mode_flag == ON)
      {
          oled_fill(0x00);
      }
      else
      {
          Signal_Test(gi_inductor_value);
      }
#endif        
        
        if(guc_charge_complete_flag)
        {
            Circuit_Low_Battery_Detection();//低压检测
        }

        //读取按键值
	button_value=button_scan();
	
        gl_time_infrared = get_infrared_command();
        reset_infrared();
        
//        gf_visual_scope_data[0] = gl_car_speed*50;
//      
//        VisualScope_Send(SCOPE_DATA_MODE);
        
        if(gl_time_infrared == COMMAND_OK)
        {
          guc_car_run_flag = CAR_RUN;
        }      
        
        if(gl_time_infrared == COMMAND_POWER)
        {
          guc_car_run_flag = CAR_STOP;
          
//          uint8 i;
//          
//          for(i = 0;i < 6;i++)
//          {
//              while(!sleep_ms(300,4))
//              {
//                if(i%2)
//                  Beep_On();
//                else
//                  Beep_Off();
//              }
//          }
//          Beep_Off();
          //guc_obstacle_state = OFF; 
        }
          

        
        switch(button_value)
        {
        case BT_UP_PRESS:
            break;
        case BT_DN_PRESS:
            break;
            //左键按下向前翻页
        case BT_LE_PRESS:
        {
            if(page_num > RUNNING_PAGE_MIN)
            {
                page_num--;
            }
            else//当前页是最前一页，左键按下则翻至最后一页
            {
                page_num = RUNNING_PAGE_MAX;
            }
            //退出标志位置位
            exit_flag = 1;
        }
        break;
        //右键按下向后翻页
        case BT_RI_PRESS:
        {
            if(page_num < RUNNING_PAGE_MAX)
            {
                page_num++;
            }
            //当前页是最后一页，则翻至最前一页
            else
            {
                page_num = RUNNING_PAGE_MIN;
            }
            //退出标志位置位
            exit_flag = 1;
        }
        break;
        case BT_OK_PRESS:
            break;
        default:
            break;
        }
    }
    return page_num;
}

/*===============================================================
	@brief      环岛参数显示页
	@parameter  所属的页数
	@return     根据按键左右返回需要进入的下一页页数
==================================================================*/
uint8 Annulus_Parameter(uint8 page_num)
{
    uint8 button_value = 0;
    uint8 exit_flag = 0;
    
//    int16 databuf[4];

    //显示信息
    oled_fill(0x00);

    while(!exit_flag)
    {
	
#ifdef COMPETITION_MODE
        if(guc_car_run_flag == CAR_RUN)
        {
            oled_fill(0x00);
        }
        else
        {
            Annulus_Test(gi_inductor_value);
        }
#else
      if(guc_automation_mode_flag == ON)
      {
          oled_fill(0x00);
      }
      else
      {
          Annulus_Test(gi_inductor_value);
      }
#endif        

        if(guc_charge_complete_flag)
        {
            Circuit_Low_Battery_Detection();//低压检测
        }      
      
//      gf_VisualScopeData[0] = gi_image_center_ave*50;
//      gf_VisualScopeData[1] = 64*50;
//      gf_VisualScopeData[2] = gi_image_left_ave*50;
//      gf_VisualScopeData[3] = gi_image_right_ave*50;
      
      //VisualScope_Send(SCOPE_DATA_MODE);
      

//      UART_Sendchar(0);  
//      UART_Sendchar(databuf[0]);
//      Delay();
//      UART_Sendchar(1);  
//      UART_Sendchar(databuf[1]);
//      Delay();  
//      UART_Sendchar(2);  
//      UART_Sendchar(databuf[2]);
//      Delay();      
//      UART_Sendchar(3);  
//      UART_Sendchar(databuf[3]);
//      Delay();
      
      //读取按键值
        button_value=button_scan();
          
          gl_time_infrared = get_infrared_command();
          reset_infrared();
          
          if(gl_time_infrared == COMMAND_OK)
          {
            guc_car_run_flag = CAR_RUN;
          }      
          
          if(gl_time_infrared == COMMAND_POWER)
          {
            guc_car_run_flag = CAR_STOP;
            
//            uint8 i;
//            
//            for(i = 0;i < 6;i++)
//            {
//                while(!sleep_ms(300,4))
//                {
//                  if(i%2)
//                    Beep_On();
//                  else
//                    Beep_Off();
//                }
//            }
//            Beep_Off();
            //guc_obstacle_state = OFF; 
          }     
          
        
        switch(button_value)
        {
        case BT_UP_PRESS:
            break;
        case BT_DN_PRESS:
            break;
            //左键按下向前翻页
        case BT_LE_PRESS:
        {
            if(page_num > RUNNING_PAGE_MIN)
            {
                page_num--;
            }
            else//当前页是最前一页，左键按下则翻至最后一页
            {
                page_num = RUNNING_PAGE_MAX;
            }
            //退出标志位置位
            exit_flag = 1;
        }
        break;
        //右键按下向后翻页
        case BT_RI_PRESS:
        {
            if(page_num < RUNNING_PAGE_MAX)
            {
                page_num++;
            }
            //当前页是最后一页，则翻至最前一页
            else
            {
                page_num = RUNNING_PAGE_MIN;
            }
            //退出标志位置位
            exit_flag = 1;
        }
        break;
        case BT_OK_PRESS:
            break;
        default:
            break;
        }
    }
    return page_num;
}

/*===============================================================
	@brief      惯性测量单元参数显示页
	@parameter  所属的页数
	@return     根据按键左右返回需要进入的下一页页数
==================================================================*/
uint8 MPU6050_Parameter(uint8 page_num)
{
    uint8 button_value = 0;
    uint8 exit_flag = 0;
    
//    int16 databuf[4];

    //显示信息
    oled_fill(0x00);

    while(!exit_flag)
    {

#ifdef COMPETITION_MODE
        if(guc_car_run_flag == CAR_RUN)
        {
            oled_fill(0x00);
        }
        else
        {
            MPU6050_Test();
        }
#else
      if(guc_automation_mode_flag == ON)
      {
          oled_fill(0x00);
      }
      else
      {
        MPU6050_Test();
      }
#endif        

      if(guc_charge_complete_flag)
      {
          Circuit_Low_Battery_Detection();//低压检测
      }      
      
      //读取按键值
      button_value=button_scan();
        
        gl_time_infrared = get_infrared_command();
        reset_infrared();
        
        if(gl_time_infrared == COMMAND_OK)
        {
          guc_car_run_flag = CAR_RUN;
        }      
        
        if(gl_time_infrared == COMMAND_POWER)
        {
          guc_car_run_flag = CAR_STOP;
          
//          uint8 i;
//          
//          for(i = 0;i < 6;i++)
//          {
//              while(!sleep_ms(300,4))
//              {
//                if(i%2)
//                  Beep_On();
//                else
//                  Beep_Off();
//              }
//          }
//          Beep_Off();
          //guc_obstacle_state = OFF; 
        }

        switch(button_value)
        {
        case BT_UP_PRESS:
            break;
        case BT_DN_PRESS:
            break;
            //左键按下向前翻页
        case BT_LE_PRESS:
        {
            if(page_num > RUNNING_PAGE_MIN)
            {
                page_num--;
            }
            else//当前页是最前一页，左键按下则翻至最后一页
            {
                page_num = RUNNING_PAGE_MAX;
            }
            //退出标志位置位
            exit_flag = 1;
        }
        break;
        //右键按下向后翻页
        case BT_RI_PRESS:
        {
            if(page_num < RUNNING_PAGE_MAX)
            {
                page_num++;
            }
            //当前页是最后一页，则翻至最前一页
            else
            {
                page_num = RUNNING_PAGE_MIN;
            }
            //退出标志位置位
            exit_flag = 1;
        }
        break;
        case BT_OK_PRESS:
            break;
        default:
            break;
        }
    }
    return page_num;
}

/*===============================================================
	@brief      路障参数显示页
	@parameter  所属的页数
	@return     根据按键左右返回需要进入的下一页页数
==================================================================*/
uint8 Obstacle_Parameter(uint8 page_num)
{
    uint8 button_value = 0;
    uint8 exit_flag = 0;
    
//    int16 databuf[4];

    //显示信息
    oled_fill(0x00);

    while(!exit_flag)
    {
      Switch_Process_Selection();
      

#ifdef COMPETITION_MODE
        if(guc_car_run_flag == CAR_RUN)
        {
            oled_fill(0x00);
        }
        else
        {
            Obstacle_Test();
        }
#else
      if(guc_automation_mode_flag == ON)
      {
          oled_fill(0x00);
      }
      else
      {
          Obstacle_Test();
      }
#endif     
      
      if(guc_charge_complete_flag)
      {
          Circuit_Low_Battery_Detection();//低压检测
      }
      
      //读取按键值
      button_value=button_scan();
        
        gl_time_infrared = get_infrared_command();
        reset_infrared();
        
        if(gl_time_infrared == COMMAND_OK)
        {
          guc_car_run_flag = CAR_RUN;
//          guc_obstacle_flag = ON;
//          guc_obstacle_state = OBSTACLE_PROCESS_1;          
        }      
        
        if(gl_time_infrared == COMMAND_POWER)
        {
          guc_car_run_flag = CAR_STOP;
//          guc_obstacle_flag = OFF;
          
//          uint8 i;
//          
//          for(i = 0;i < 6;i++)
//          {
//              while(!sleep_ms(300,4))
//              {
//                if(i%2)
//                  Beep_On();
//                else
//                  Beep_Off();
//              }
//          }
//          Beep_Off();
          //guc_obstacle_state = OFF; 
        }

        
        //Obstacle_Process();

        switch(button_value)
        {
        case BT_UP_PRESS:
            break;
        case BT_DN_PRESS:
            break;
            //左键按下向前翻页
        case BT_LE_PRESS:
        {
            if(page_num > RUNNING_PAGE_MIN)
            {
                page_num--;
            }
            else//当前页是最前一页，左键按下则翻至最后一页
            {
                page_num = RUNNING_PAGE_MAX;
            }
            //退出标志位置位
            exit_flag = 1;
        }
        break;
        //右键按下向后翻页
        case BT_RI_PRESS:
        {
            if(page_num < RUNNING_PAGE_MAX)
            {
                page_num++;
            }
            //当前页是最后一页，则翻至最前一页
            else
            {
                page_num = RUNNING_PAGE_MIN;
            }
            //退出标志位置位
            exit_flag = 1;
        }
        break;
        case BT_OK_PRESS:
            break;
        default:
            break;
        }
    }
    return page_num;
}

/*===============================================================
	@brief      坡道参数显示页
	@parameter  所属的页数
	@return     根据按键左右返回需要进入的下一页页数
==================================================================*/
uint8 Slope_Parameter(uint8 page_num)
{
    uint8 button_value = 0;
    uint8 exit_flag = 0;
    
//    int16 databuf[4];

    //显示信息
    oled_fill(0x00);

    while(!exit_flag)
    {

#ifdef COMPETITION_MODE
        if(guc_car_run_flag == CAR_RUN)
        {
            oled_fill(0x00);
        }
        else
        {
            Slope_Test(gi_inductor_value);
        }
#else
      if(guc_automation_mode_flag == ON)
      {
          oled_fill(0x00);
      }
      else
      {
          Slope_Test(gi_inductor_value);
      }
#endif 

      if(guc_charge_complete_flag)
      {
          Circuit_Low_Battery_Detection();//低压检测
      }
      
      //读取按键值
      button_value=button_scan();
        
        gl_time_infrared = get_infrared_command();
        reset_infrared();
        
        if(gl_time_infrared == COMMAND_OK)
        {
          guc_car_run_flag = CAR_RUN;
          guc_obstacle_state = OBSTACLE_PROCESS_1;          
        }      
        
        if(gl_time_infrared == COMMAND_POWER)
        {
          guc_car_run_flag = CAR_STOP;
          //guc_obstacle_state = OFF; 
          
//          uint8 i;
//          
//          for(i = 0;i < 6;i++)
//          {
//              while(!sleep_ms(300,4))
//              {
//                if(i%2)
//                  Beep_On();
//                else
//                  Beep_Off();
//              }
//          }
//          Beep_Off();
          //guc_obstacle_state = OFF; 
        }
          


        switch(button_value)
        {
        case BT_UP_PRESS:
            break;
        case BT_DN_PRESS:
            break;
            //左键按下向前翻页
        case BT_LE_PRESS:
        {
            if(page_num > RUNNING_PAGE_MIN)
            {
                page_num--;
            }
            else//当前页是最前一页，左键按下则翻至最后一页
            {
                page_num = RUNNING_PAGE_MAX;
            }
            //退出标志位置位
            exit_flag = 1;
        }
        break;
        //右键按下向后翻页
        case BT_RI_PRESS:
        {
            if(page_num < RUNNING_PAGE_MAX)
            {
                page_num++;
            }
            //当前页是最后一页，则翻至最前一页
            else
            {
                page_num = RUNNING_PAGE_MIN;
            }
            //退出标志位置位
            exit_flag = 1;
        }
        break;
        case BT_OK_PRESS:
            break;
        default:
            break;
        }
    }
    return page_num;
}

/*===============================================================
	@brief      小车运动显示函数
==================================================================*/
void Smartcar_Running()
{
    uint8 page_num = TEST_PAGE_MIN;

    while(1)
    {
        //根据页数进入相应的测试
        switch(page_num)
        {
        case 1:
            page_num = Signal_Parameter(1);
            break;
        case 2:
            page_num = Annulus_Parameter(2);
            break;
        case 3:
            page_num = MPU6050_Parameter(3);
            break;        
        case 4:
            page_num = Obstacle_Parameter(4);
            break;     
        case 5:
            page_num = Slope_Parameter(5);
            break;    
        default:
            break;
        }
    }
}

/*===============================================================
	@brief      延时
==================================================================*/

void Delay()
{
    unsigned int i, j;

    for ( i=0; i<1600; i++ )
    {
        for ( j=0; j<4000; j++ )
        {
            
        }
    }
}