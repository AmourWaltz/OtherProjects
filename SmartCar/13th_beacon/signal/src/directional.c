/*=============================================
    @file         directional.c
    @brief        红外定向
    @programmer   XBY
    @note         红外通道：前       1
                            后       5
                            左       3
                            右       7
                            左上     2
                            左下     4
                            右上     8  
                            右下     6  

备用车：
    @note         红外通道：左一      8
                            左二      7
                            左三      6
                            左四      5
                            右一      1
                            右二      2
                            右三      3  
                            右四      4  
=============================================*/

#include "directional.h"
#include "include.h"

/*===============================================================
	@brief      八路红外接收初始化
==================================================================*/

void Infrared_Init()
{
    GPIO_Init(INFRA_1_PORT_ID, INFRA_1_PORT_PIN, DIR_INPUT, OUTPUT_L);
    GPIO_Init(INFRA_2_PORT_ID, INFRA_2_PORT_PIN, DIR_INPUT, OUTPUT_L);
    GPIO_Init(INFRA_3_PORT_ID, INFRA_3_PORT_PIN, DIR_INPUT, OUTPUT_L); 
    GPIO_Init(INFRA_4_PORT_ID, INFRA_4_PORT_PIN, DIR_INPUT, OUTPUT_L);
    GPIO_Init(INFRA_5_PORT_ID, INFRA_5_PORT_PIN, DIR_INPUT, OUTPUT_L);
    GPIO_Init(INFRA_6_PORT_ID, INFRA_6_PORT_PIN, DIR_INPUT, OUTPUT_L);
    GPIO_Init(INFRA_7_PORT_ID, INFRA_7_PORT_PIN, DIR_INPUT, OUTPUT_L);
    GPIO_Init(INFRA_8_PORT_ID, INFRA_8_PORT_PIN, DIR_INPUT, OUTPUT_L);
}

/*===============================================================
	@brief      八路红外接收数据读取
==================================================================*/

void Infrared_Value_Get()
{
    guc_infrared_value[0] = LPLD_GPIO_Input_b(INFRA_1_PORT_ID, INFRA_1_PORT_NUM);
    guc_infrared_value[1] = LPLD_GPIO_Input_b(INFRA_2_PORT_ID, INFRA_2_PORT_NUM);
    guc_infrared_value[2] = LPLD_GPIO_Input_b(INFRA_3_PORT_ID, INFRA_3_PORT_NUM);
    guc_infrared_value[3] = LPLD_GPIO_Input_b(INFRA_4_PORT_ID, INFRA_4_PORT_NUM);
    guc_infrared_value[4] = LPLD_GPIO_Input_b(INFRA_5_PORT_ID, INFRA_5_PORT_NUM);
    guc_infrared_value[5] = LPLD_GPIO_Input_b(INFRA_6_PORT_ID, INFRA_6_PORT_NUM);
    guc_infrared_value[6] = LPLD_GPIO_Input_b(INFRA_7_PORT_ID, INFRA_7_PORT_NUM);
    guc_infrared_value[7] = LPLD_GPIO_Input_b(INFRA_8_PORT_ID, INFRA_8_PORT_NUM);
    
}

/*===============================================================
	@brief      红外信号发车
==================================================================*/

uint8 Infrared_Start()
{
    uint8 infrared_value;
    
    infrared_value = 0;
    
    Infrared_Value_Get();
    
    infrared_value |= guc_infrared_value[0]<<0;
    infrared_value |= guc_infrared_value[1]<<1;
    infrared_value |= guc_infrared_value[2]<<2;
    infrared_value |= guc_infrared_value[3]<<3;
    infrared_value |= guc_infrared_value[4]<<4;
    infrared_value |= guc_infrared_value[5]<<5;
    infrared_value |= guc_infrared_value[6]<<6;
    infrared_value |= guc_infrared_value[7]<<7;
    
    return infrared_value;
}

/*===============================================================
	@brief      检测红外信号定向
==================================================================*/

//#ifndef BEACON_MAIN

void Infrared_Directional()
{
    int8 i;
    
    static int8 sc_directional_x;
    
    int8 front_count;
    int8 left_count;
    int8 right_count;
    int8 back_count;
    
    front_count = 0;
    left_count = 0;
    right_count = 0;
    back_count = 0;
    
    for(i = 0;i < 5;i++)
    {
        Infrared_Value_Get();
//        if(guc_infrared_value[0])
//        {
//            front_count++;
//        }
//        if(guc_infrared_value[1])
//        {
//            left_count++;
//            front_count++;
//        }
        if(guc_infrared_value[2])
        {
            left_count++;
        }
        
        if(guc_switch_pin_val[1] == 1)
        {
            if(guc_infrared_value[3])
            {
                back_count++;
                left_count++;
            }
            if(guc_infrared_value[4])
            {
                back_count++;
            }      
            if(guc_infrared_value[5])
            {
                back_count++;
                right_count++;
            }
        }
        
        if(guc_infrared_value[6])
        {
            right_count++;
        }
//        if(guc_infrared_value[7])
//        {
//            right_count++;
//            front_count++;
//        }
    }
    
    if(right_count > left_count)
    {
        guc_beacon_directional_x = RIGHT;
        sc_directional_x = guc_beacon_directional_x;
    }
    else if(right_count < left_count)
    {
        guc_beacon_directional_x = LEFT;
        sc_directional_x = guc_beacon_directional_x;
    }
    else if(right_count == left_count)
    {
        guc_beacon_directional_x = sc_directional_x;
    }
    
    if(guc_beacon_directional_x == 0)
    {
        guc_beacon_directional_x = guc_center_flag;
    }
        
    if(front_count > back_count)
    {
        guc_beacon_directional_y = FRONT;
    }
    else if(front_count < back_count)
    {
        guc_beacon_directional_y = BACK;
    }
    else if(front_count == back_count)
    {
        guc_beacon_directional_y = FRONT;
    }
}

//#else
//
//void Infrared_Directional()
//{
//    int8 i;
//    
//    static int8 sc_directional_x;
//    
//    int8 left_count;
//    int8 right_count;
//    
//    left_count = 0;
//    right_count = 0;
//    
//    for(i = 0;i < 5;i++)
//    {
//        Infrared_Value_Get();
////        if(guc_infrared_value[0])
////        {
////            front_count++;
////        }
//        if(guc_infrared_value[1])
//        {
//            left_count++;
//        }
//        if(guc_infrared_value[2])
//        {
//            left_count++;
//        }
//        if(guc_infrared_value[3])
//        {
//            left_count++;
//        }
//        if(guc_infrared_value[4])
//        {
//            right_count++;
//        }      
//        if(guc_infrared_value[5])
//        {
//            right_count++;
//        }
//        if(guc_infrared_value[6])
//        {
//            right_count++;
//        }
////        if(guc_infrared_value[7])
////        {
////            right_count++;
////            front_count++;
////        }
//    }
//    
//    if(right_count > left_count)
//    {
//        guc_beacon_directional_x = RIGHT;
//        sc_directional_x = guc_beacon_directional_x;
//    }
//    else if(right_count < left_count)
//    {
//        guc_beacon_directional_x = LEFT;
//        sc_directional_x = guc_beacon_directional_x;
//    }
//    else if(right_count == left_count)
//    {
//        guc_beacon_directional_x = sc_directional_x;
//    }
//        
////    if(front_count > back_count)
////    {
////        guc_beacon_directional_y = FRONT;
////    }
////    else if(front_count < back_count)
////    {
////        guc_beacon_directional_y = BACK;
////    }
////    else if(front_count == back_count)
////    {
////        guc_beacon_directional_y = FRONT;
////    }
//}
//
//#endif