/*=============================================
    @file         switch.c
    @brief        拨码
    @programmer   Beyond Hsueh
=============================================*/

#include "switch.h"

/*===============================================================
        @note      静女其姝，俟我于城隅；
                   爱而不见，搔首踟蹰。
                   静女其娈，贻我彤管；
                   彤管有炜，说怿女美。
                   自牧归荑，洵美且异；
                   匪女之为美，美人之贻。
==================================================================*/     

/*===============================================================
	@brief     初始化拨码
==================================================================*/
void Switch_Init()
{
    GPIO_Init(SW0_BIT0_PORT, SW0_BIT0_PIN_MASK, DIR_INPUT, OUTPUT_L);
    GPIO_Init(SW0_BIT1_PORT, SW0_BIT1_PIN_MASK, DIR_INPUT, OUTPUT_L);

    GPIO_Init(SW1_BIT0_PORT, SW1_BIT0_PIN_MASK, DIR_INPUT, OUTPUT_L);
    GPIO_Init(SW1_BIT1_PORT, SW1_BIT1_PIN_MASK, DIR_INPUT, OUTPUT_L); 
    GPIO_Init(SW1_BIT2_PORT, SW1_BIT2_PIN_MASK, DIR_INPUT, OUTPUT_L);
    GPIO_Init(SW1_BIT3_PORT, SW1_BIT3_PIN_MASK, DIR_INPUT, OUTPUT_L);        
}

/*===============================================================
	@brief     返回开关全部位值
==================================================================*/
uint8 Get_Switch_Value(uint8 *uc_switch_pin_value, uint8 uc_switch_pin)
{
    uint8 uc_sw_bit_temp[6];
    uint8 uc_switch_value = 0;
    
    uc_sw_bit_temp[0] = LPLD_GPIO_Input_b(SW0_BIT0_PORT, SW0_BIT0_PIN);
    uc_sw_bit_temp[1] = LPLD_GPIO_Input_b(SW0_BIT1_PORT, SW0_BIT1_PIN);
    uc_sw_bit_temp[2] = LPLD_GPIO_Input_b(SW1_BIT0_PORT, SW1_BIT0_PIN);
    uc_sw_bit_temp[3] = LPLD_GPIO_Input_b(SW1_BIT1_PORT, SW1_BIT1_PIN);
    uc_sw_bit_temp[4] = LPLD_GPIO_Input_b(SW1_BIT2_PORT, SW1_BIT2_PIN);
    uc_sw_bit_temp[5] = LPLD_GPIO_Input_b(SW1_BIT3_PORT, SW1_BIT3_PIN);
    
    if (uc_switch_pin_value != NULL)
    {
        *uc_switch_pin_value = uc_sw_bit_temp[uc_switch_pin];       
    }
    
    /*获取所有位值*/
    uc_switch_value |= uc_sw_bit_temp[0]<<0;
    uc_switch_value |= uc_sw_bit_temp[1]<<1;
    uc_switch_value |= uc_sw_bit_temp[2]<<2;
    uc_switch_value |= uc_sw_bit_temp[3]<<3;
    uc_switch_value |= uc_sw_bit_temp[4]<<4;
    uc_switch_value |= uc_sw_bit_temp[5]<<5;
    
    return uc_switch_value;
}

/*===============================================================
	@brief     读取拨码开关值
==================================================================*/
uint8 Switch_Read_Value(uint8 uc_switch_pin)
{
    uint8 switch_value;
    Get_Switch_Value(&switch_value,uc_switch_pin);
    return switch_value;
}