/*
 * --------------14届无线节能K60一体板初版工程（14th_wireless_energy_saving）-----------------
 * @file     14th_wireless_energy_saving.c
 * @date     2019-6-18
 * @author   Beyond Hsueh
 * @note     K60主控，五电感（两竖，两横，一中），充电主控一体，避障：红外（备胎：激光）
 * @sign     慧极必伤，情深不寿，强极则辱；谦谦君子，温润如玉。
 */

#include "include.h"
#include "common.h"

void main (void)
{
    uint8 uc_mode_flag = 0;//模式选择标志位
    uint8 uc_running_flag = FLAG_OFF;//发车标志位
    uint8 uc_button_value = 0;//五向按键读取值
    uint8 uc_exit_flag = 0;//退出标志位
    uint16 ui_infrared_value = 0;//红外解码读取值

    guc_car_run_flag = CAR_STOP;//禁止电机输出
    guc_gyro_zero_flag = OFF;//陀螺仪未标定
    gl_car_running_distance = 0;//车程初始化
    guc_charge_complete_flag = 0;//充电标志位初始化
    guc_beep_start_flag = OFF;//蜂鸣器关闭
    guc_lost_start_flag = OFF;//丢信号关闭

    Init_All();//各模块初始化
    
    

//    while(1)
//    {
////        Change_PWM_Motor_Left(5000);//修改左轮占空比
////
////        Change_PWM_Motor_Right(5000);//修改右轮占空比
//      
//      oled_print_short(1,1,guc_reed_state_value);
//    }

    //陀螺仪标定放到充电里面，
    GYRO_Set_Zero();//陀螺仪消除零漂
    
    guc_automation_mode_flag = Switch_Read_Value(0);

    oled_fill(0x00);

    if(guc_automation_mode_flag == ON)
    {
        guc_switch_pin_value[3] = Switch_Read_Value(3);
        guc_switch_pin_value[4] = Switch_Read_Value(4);
        guc_switch_pin_value[5] = Switch_Read_Value(5);
        
        if(guc_switch_pin_value[4] == ON)
        {
            gl_voltage_start_set += 500;
        }
        else if(guc_switch_pin_value[3] == ON)
        {
            gl_voltage_start_set += 200;
        }
        
        if(guc_switch_pin_value[5] == ON)
        {
            gl_speed_straight = 230;
            gl_speed_annulus = 175;
            gl_speed_obstacle = 175;
            gl_speed_lost_line = 150;
        }

        Charge_Start_Test();

        while(1)
        {
            Smartcar_Running();
        }   
    }
    else if(guc_automation_mode_flag == OFF)
    {
        //GYRO_Set_Zero();//陀螺仪消除零漂
        
        //Stay Hungry.Stay Foolish.
        while(!uc_exit_flag)
        {

            uc_button_value = button_scan();

            ui_infrared_value = get_infrared_command();
            reset_infrared();

            if(ui_infrared_value == COMMAND_OK)
            {
              uc_exit_flag = 1;
            }

            if(uc_button_value == FIVE_DIR_OK)
            {
              uc_exit_flag = 1;
            }

            oled_display_16x8str(20,1,"Stay Hungry");
            oled_display_16x8str(15,4,"Stay Foolish");
        }

        oled_fill(0x00);

        while(uc_running_flag == FLAG_OFF)
        {
            uc_mode_flag = Select_Mode();

            switch(uc_mode_flag)
            {
                case SET_PARAM:
                      Param_Set();
                    break;
                case HARDWARE_TEST:
                      Smartcar_Test();
                    break;
                case WIRELESS_CHARGE:
                      uc_running_flag = Charge_Start_Test();
                    break;
                case CAR_RUNNING:
                      uc_running_flag = FLAG_ON;
                      guc_beep_start_flag = FLAG_ON;
                      guc_lost_start_flag = FLAG_ON;
                      gul_time_start = 0;
                    break;
                default:
                      break;
            }

            oled_fill(0x00);
        }
        while(1)
        {
            Smartcar_Running();
        }    
    }   
}