#include "include.h"

#define ADC_QUEUE_LENTH 50
static int s_left_current_buffer[ADC_QUEUE_LENTH];
static int s_right_current_buffer[ADC_QUEUE_LENTH];

float left_current;
float right_current;
void CurrentLoop()
{
    
}
void buff_ADC_data()
{
    uint8 i;
    static uint8 buffer_pos = 0;
    static int flag = 0;
    
    int16 left_ADC_data;
    int16 right_ADC_data;
    
    left_ADC_data = g_u16LeftCurrentADC;
    right_ADC_data = g_u16RightCurrentADC;
    
    /*if(flag == 0)
    {
        for(i=0;i<ADC_QUEUE_LENTH;i++)
        {
            s_left_current_buffer[i] = left_ADC_data;
            s_right_current_buffer[i] = right_ADC_data;
        }
        flag = 1;
    }*/
    //else
    {
        buffer_pos++;
        if(buffer_pos>=ADC_QUEUE_LENTH)
        {
           buffer_pos = 0;
        }
        
        s_left_current_buffer[buffer_pos] = left_ADC_data;
        s_right_current_buffer[buffer_pos] = right_ADC_data;
    }
}
void UpdateLeftCurrent()
{
    int16 left_ADC_data;
    
    float left_ADC_data_temp = 0;
     
    uint8 i;
    /*static uint8 buffer_pos = 0;
    static int flag = 0;
    
    left_ADC_data = g_carInfo.u16LeftCurrentADC;
    if(flag == 0)
    {
        for(i=0;i<ADC_QUEUE_LENTH;i++)
        {
            s_left_current_buffer[i] = left_ADC_data;
        }
        flag = 1;
    }
    else
    {
        buffer_pos++;
        if(buffer_pos>=ADC_QUEUE_LENTH)
        {
           buffer_pos = 0;
        }
        
        s_left_current_buffer[buffer_pos] = left_ADC_data;
    }*/
    for(i=0;i<ADC_QUEUE_LENTH;i++)
    {
        left_ADC_data_temp += s_left_current_buffer[i];
    }
    left_current = (float)(left_ADC_data_temp/ADC_QUEUE_LENTH - LEFT_CURRENT_ZERO)*(REF_V/SAMPLE_R)/CUR_GAIN/0x0fff;
}
void UpdateRightCurrent()
{
    
}
void UpdateRefVoltage()
{
    
}
void UpdateBatteryADC()
{
    
}