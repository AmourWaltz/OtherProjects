/*=============================================
    @file         circuit.c
    @brief        电路参数
    @programmer   Beyond Hsueh
=============================================*/

#include "circuit.h"

/*===============================================================
        @note      驿外断桥边，寂寞开无主，
                   已是黄昏独自愁，更著风和雨；
                   无意苦争春，一任群芳妒，
                   零落成泥碾作尘，只有香如故。
==================================================================*/

/*===============================================================
	@brief     充电电路AD初始化
==================================================================*/
void Circuit_AD_Init()
{
    LPLD_ADC_Chn_Enable(VOLTAGE_OUTPUT_ADC_ID, VOLTAGE_OUTPUT_ADC_CH);
    LPLD_ADC_Chn_Enable(VOLTAGE_INPUT_ADC_ID, VOLTAGE_INPUT_ADC_CH);
    LPLD_ADC_Chn_Enable(CURRENT_INPUT_ADC_ID, CURRENT_INPUT_ADC_CH);    
}
  
/*===============================================================
	@brief     充电电路AD均值滤波
==================================================================*/
void Circuit_Channel_Get(vint16 channel_value[])
{
    uint16 i;
    uint32 voltage_input_adc_add;
    uint32 voltage_output_adc_add;
    uint32 current_input_adc_add;
    
    current_input_adc_add = 0;
    voltage_input_adc_add = 0;
    voltage_output_adc_add = 0;
    
    for(i = 0; i < CURRENT_INPUT_AVERAGE; i++)
    {
        current_input_adc_add += LPLD_ADC_Get(CURRENT_INPUT_ADC_ID, CURRENT_INPUT_ADC_CH);
    }
    
    for(i = 0; i < VOLTAGE_INPUT_AVERAGE; i++)
    {
        voltage_input_adc_add += LPLD_ADC_Get(VOLTAGE_INPUT_ADC_ID, VOLTAGE_INPUT_ADC_CH);
    }
    
    for(i = 0; i < VOLTAGE_OUTPUT_AVERAGE; i++)
    {
        voltage_output_adc_add += LPLD_ADC_Get(VOLTAGE_OUTPUT_ADC_ID, VOLTAGE_OUTPUT_ADC_CH);
    }
    
    channel_value[V_REF] = voltage_input_adc_add / VOLTAGE_INPUT_AVERAGE;
    channel_value[V_OUT] = voltage_output_adc_add / VOLTAGE_OUTPUT_AVERAGE;
    channel_value[I_IN] = current_input_adc_add / CURRENT_INPUT_AVERAGE;
}

/*===============================================================
	@brief     电路通道AD转换结果
==================================================================*/
void Circuit_Signal_Get()
{
////    static int32 si_last_power;
    static int32 si_last_current;
    
    int32 voltage_output;
    int32 current_input;
//    int32 power_intput;
    
    vint16 inductor_value[NUMBER_CHANNEL];
    Circuit_Channel_Get(inductor_value);
        
    
    gl_current_input = ((int32)(inductor_value[I_IN])*(5000))/4095;//输出电流

    gl_voltage_input = ((int32)inductor_value[V_REF]*16*5000)/4095;//输入电压
    
    gl_voltage_output = ((int32)inductor_value[V_OUT]*4*5000)/4095 + 0;//输出电压 ，补偿二极管压降
    gl_current_input = (20 * gl_current_input + (si_last_current - gl_current_input))/20;//惯性滤波
    
    voltage_output = gl_voltage_input;
    current_input = gl_current_input;
    
    gl_power_output = (int32)((int32)voltage_output*(int32)current_input/1000);//输入功率
    
//    power_intput = gl_power_output;
//    
//    gl_power_output = (int32)Limiter(si_last_power - power_intput,-50-power_intput/200,50+power_intput/200) + power_intput;
//    si_last_power = gl_power_output; 
    si_last_current = gl_current_input;
  
}

/*===============================================================
	@brief     电路低压检测
==================================================================*/
void Circuit_Low_Battery_Detection()
{
    int8 string1[10],string2[10],string3[10];
    uint8 ui_button_value;
    static uint16 sui_low_battery_count = 0;
    static uint8 suc_low_battery_flag = 0;

    if((ADC_Battery_Voltage() < 2700) && (gul_time_100us > 200000))   
    {
        sui_low_battery_count++;
    }
    else
    {
        sui_low_battery_count = 0;
    }
    
    if(sui_low_battery_count > 10)
    {
        //显示距离和电量
        sprintf(string1,"distance: %6d cm",gl_car_running_distance);
        sprintf(string2,"battery: %6d mV ",ADC_Battery_Voltage() );
        oled_display_16x8str(0,2,string1);
        oled_display_16x8str(0,4,string2);

        if(suc_low_battery_flag == 0)
        {
            suc_low_battery_flag = 1;
            
            ui_button_value = button_scan();
            
            if(ui_button_value == FIVE_DIR_OK)
            {
                oled_init();
                oled_fill(0x00); 
            }
            sprintf(string3, "time: %6.2d s ", gul_time_100us/10000 );
            oled_display_16x8str(0,6,string3);
            sleep_ms(1000,5);
            while(!sleep_ms(500,5))
            {
                Beep_On();
            }
        }
        if((suc_low_battery_flag == 1))
        {
            Beep_Off();

            Car_Stop();
        }
    }
}