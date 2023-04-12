/*=============================================
    @file         inductor.c
    @brief        超声波模拟定向
    @programmer   XBY
    @note         超声波通道：前       1
                              后       2
                              左上     3
                              右上     4
                              左下     5
                              右下     6  
=============================================*/

#include "inductor.h"
#include "include.h"

/*===============================================================
	@brief      六路超声波模拟转换使能
==================================================================*/

void ADC_Ultrasonic_Init()
{
    //使能相应通道
    LPLD_ADC_Chn_Enable(ULTRA_1_ADC_ID, ULTRA_1_ADC_CH);
    LPLD_ADC_Chn_Enable(ULTRA_2_ADC_ID, ULTRA_2_ADC_CH);
    LPLD_ADC_Chn_Enable(ULTRA_3_ADC_ID, ULTRA_3_ADC_CH);
    LPLD_ADC_Chn_Enable(ULTRA_4_ADC_ID, ULTRA_4_ADC_CH);
    LPLD_ADC_Chn_Enable(ULTRA_5_ADC_ID, ULTRA_5_ADC_CH);
    LPLD_ADC_Chn_Enable(ULTRA_6_ADC_ID, ULTRA_6_ADC_CH);
}

/*===============================================================
	@brief      获取六路超声波AD转换结果
==================================================================*/

uint16 Get_Ultrasonic_ADC(uint8 inductor_num)
{
    uint16 inductor_adc_temp = 0;
    switch (inductor_num)
    {
        case ULTRA_FRONT:
                inductor_adc_temp = LPLD_ADC_Get(ULTRA_1_ADC_ID, ULTRA_1_ADC_CH);
                break;
        case ULTRA_BACK:
                inductor_adc_temp = LPLD_ADC_Get(ULTRA_2_ADC_ID, ULTRA_2_ADC_CH);
                break;
        case ULTRA_LEFT_F:
                inductor_adc_temp = LPLD_ADC_Get(ULTRA_3_ADC_ID, ULTRA_3_ADC_CH);
                break;
        case ULTRA_RIGHT_F:
                inductor_adc_temp = LPLD_ADC_Get(ULTRA_4_ADC_ID, ULTRA_4_ADC_CH);
                break;
        case ULTRA_LEFT_B:
                inductor_adc_temp = LPLD_ADC_Get(ULTRA_5_ADC_ID, ULTRA_5_ADC_CH);
                break;
        case ULTRA_RIGHT_B:
                inductor_adc_temp = LPLD_ADC_Get(ULTRA_6_ADC_ID, ULTRA_6_ADC_CH);
                break;
        default:
                break;   
    }
    return inductor_adc_temp;
}

/*===============================================================
	@brief      均值处理六路超声波AD转换结果
==================================================================*/

void Get_Ultrasonic_Value_Ave()
{
    uint16 adc_value[NUM_ULTRASONIC][AVE_TIMES] = {0};
    uint16 temp_sum = 0;
    int i = 0, j = 0;
    for (j = 0; j < AVE_TIMES; j++)
    {
        for (i = 0; i < NUM_ULTRASONIC; i++)
        {
            adc_value[i][j] = Get_Ultrasonic_ADC(i);
        }
    }
    
    for (i = 0; i < NUM_ULTRASONIC; i++)
    {
        temp_sum = 0;
        for (j = 0; j < AVE_TIMES; j++)
        {
            temp_sum += adc_value[i][j];
        }
        gi_ultrasonic_value[i] = (int16)((temp_sum+0.5*AVE_TIMES) / AVE_TIMES);
    }
}

/*===============================================================
	@brief      超声波值检测跳变最小二乘法滤波器
        @function   超声波值跳变计数，超过一定次数，认为当前信标灯被灭掉，可用于信标计数
==================================================================*/

void Filter_Ultrasonic_Value(int16 ultrasonic_value[])
{
    static int si_last_ultrasonic_value[NUM_ULTRASONIC] = {0};
    static int si_jump_ultrasonic_times[NUM_ULTRASONIC] = {0};
    static int8 sc_jump_count = 0;
    int i = 0, j = 0;
 
    for(i = 0; i < NUM_ULTRASONIC; i++)
    {
	si_last_ultrasonic_value[i] = gi_ultrasonic_history[i][0];
    }
    
    for(i = 0; i < NUM_ULTRASONIC; i++)
    {
	if(ultrasonic_value[i] <= si_last_ultrasonic_value[i])
	{
            if(ultrasonic_value[i] < (si_last_ultrasonic_value[i] - 30))
            {//用滤波后的超声波值数组滤波
                si_last_ultrasonic_value[i] = least_squares_value(gi_ultrasonic_history[i]);
		si_jump_ultrasonic_times[i] ++;
                
                //跳变超过一定次数，认为当前超声波值为正确值
		if(si_jump_ultrasonic_times[i] >= JUMP_MAX_TIMES)
		{
                    si_last_ultrasonic_value[i] = ultrasonic_value[i];
                    si_jump_ultrasonic_times[i] = 0;
                    sc_jump_count++;
		}
            }
            else if(ultrasonic_value[i] < (si_last_ultrasonic_value[i] - 15))
            {
                si_last_ultrasonic_value[i] = si_last_ultrasonic_value[i] - 5;
		si_jump_ultrasonic_times[i] = 0;
            }
            else
            {
		si_last_ultrasonic_value[i] = ultrasonic_value[i];
		si_jump_ultrasonic_times[i] = 0;
            }
        }
        else
        {
            if(ultrasonic_value[i] > (si_last_ultrasonic_value[i] + 30))
            {
		si_last_ultrasonic_value[i] = least_squares_value(gi_ultrasonic_history[i]);
		si_jump_ultrasonic_times[i] ++;

		if(si_jump_ultrasonic_times[i] >= JUMP_MAX_TIMES)
		{
                    si_last_ultrasonic_value[i] = ultrasonic_value[i];
                    si_jump_ultrasonic_times[i] = 0;
                    sc_jump_count++;
		}
            }
            else if(ultrasonic_value[i] > (si_last_ultrasonic_value[i] + 15))
            {
                si_last_ultrasonic_value[i] = si_last_ultrasonic_value[i] + 5;
                si_jump_ultrasonic_times[i] = 0;
            }
            else
            {
		si_last_ultrasonic_value[i] = ultrasonic_value[i];
		si_jump_ultrasonic_times[i] = 0;
            }
	}
    }
    //更新超声波值
    for(i = 0; i < NUM_ULTRASONIC; i++)
    {
	ultrasonic_value[i] = si_last_ultrasonic_value[i];
    }
    //更新历史超声波值
    for(i = 0; i < NUM_ULTRASONIC; i++)
    {
	for(j = NUM_ULTRASONIC_HISTORY-1; j >= 1; j--)
	{			
            gi_ultrasonic_history[i][j] = gi_ultrasonic_history[i][j-1];
	}
	gi_ultrasonic_history[i][0] = ultrasonic_value[i];
    }
    //根据跳变超声波通道数量判断是否灭灯并计数
    if(sc_jump_count > 1)
    {
        guc_beacon_count++;
    }
    
    sc_jump_count = 0;
}

/*===============================================================
	@brief      超声波值动态标定，得出差比和
==================================================================*/

void Get_Standard_Ultrasonic(int16 ultrasonic_value[])
{
    int16 sensor_front = 0;
    int16 sensor_back = 0;
//    int16 sensor_left = 0;
//    int16 sensor_right = 0;

//    float  sensor_offset_x_temp = 0;
    float  sensor_offset_y_temp = 0;
   

    sensor_front = ultrasonic_value[ULTRA_FRONT];
    sensor_back  = ultrasonic_value[ULTRA_BACK];
//    sensor_left =  ultrasonic_value[ULTRA_LEFT_F];
//    sensor_right = ultrasonic_value[ULTRA_RIGHT_F];    
    
//    sensor_left  = (int16)((ultrasonic_value[ULTRA_LEFT_F] + ultrasonic_value[ULTRA_LEFT_B]) / 2);
//    sensor_right = (int16)((ultrasonic_value[ULTRA_RIGHT_F]+ ultrasonic_value[ULTRA_RIGHT_B]) / 2);

    //纵向超声波差比和
    sensor_offset_y_temp = ((ORIGINAL_LINEAR_X)*(sensor_front - sensor_back)/(sensor_front + sensor_back));
    //横向超声波差比和
//    sensor_offset_y_temp = (ORIGINAL_LINEAR_Y)*((sensor_left - sensor_right)/(sensor_left + sensor_right));

//    gi_ultrasonic_offset_x = (int16)sensor_offset_x_temp;
    gi_ultrasonic_offset_y = (int16)sensor_offset_y_temp;
}

/*===============================================================
	@brief      比较检测超声波得出定向角度
==================================================================*/

void Directional_Calculate()
{
    int16 i;
    int8 ultra_max_beacon;
    int16 ultra_max_value;
    
    ultra_max_value = gi_ultrasonic_value[0];
    ultra_max_beacon = 0;
    
    for(i=1; i<NUM_ULTRASONIC;i++)
    {
        if(gi_ultrasonic_value[i] > ultra_max_value)
        {
            ultra_max_value = gi_ultrasonic_value[i];
            ultra_max_beacon= i;
        }
    }
    
    switch (ultra_max_beacon)
    {
        case ULTRA_FRONT:
                guc_beacon_directional_y = FRONT;
                break;
        case ULTRA_BACK:
          {
              guc_beacon_directional_y = BACK;
              if(gi_ultrasonic_value[ULTRA_LEFT_B] > gi_ultrasonic_value[ULTRA_RIGHT_B])
                guc_beacon_directional_x = LEFT;
              else 
                guc_beacon_directional_x = RIGHT;
          }
                break;
        case ULTRA_LEFT_F:
                guc_beacon_directional_x = LEFT;
                guc_beacon_directional_y = FRONT;
                break;
        case ULTRA_RIGHT_F:
                guc_beacon_directional_x = RIGHT;
                guc_beacon_directional_y = FRONT;
                break;
        case ULTRA_LEFT_B:
                guc_beacon_directional_x = LEFT;
                guc_beacon_directional_y = BACK;
                break;
        case ULTRA_RIGHT_B:
                guc_beacon_directional_x = RIGHT;
                guc_beacon_directional_y = BACK;
                break;
        default:
                break;   
    }
    
}

/*===============================================================
	@brief      超声波AD值处理综合函数
        @function   发车一定时间后进行超声波滤波以及信标计数
        @note       可外部调用
==================================================================*/

void Get_Ultrasonic_Value(void)
{     
    static int16 cnt = 0;
    int16 i = 0, j = 0;
    cnt++;
    Get_Ultrasonic_Value_Ave();
    if (cnt >= 80)
    {    
        cnt = 81;
        Filter_Ultrasonic_Value(gi_ultrasonic_value);//8位精度滤波
    }
    else
    {
        for(i = 0; i < NUM_ULTRASONIC; i++)
        {
            for(j = NUM_ULTRASONIC_HISTORY-1; j >= 1; j--)
            {			
                gi_ultrasonic_history[i][j] = gi_ultrasonic_history[i][j-1];
            }
            gi_ultrasonic_history[i][0] = gi_ultrasonic_value[i];
        }
    }
    
    //Get_Standard_Ultrasonic(gi_ultrasonic_value);
    Directional_Calculate();
}