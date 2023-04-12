/*=============================================
    @file         calibration.c
    @brief        电感标定
    @programmer   Beyond Hsueh
=============================================*/

#include "calibration.h"

/*===============================================================
        @note      今夕何夕兮，搴舟中流。
                   今日何日兮，得与王子同舟。
                   蒙羞被好兮，不訾诟耻。
                   心几烦而不绝兮，得知王子。
                   山有木兮木有枝，心悦君兮君不知。
==================================================================*/ 

static uint16 sui_cali_count = 0;
static uint32 sul_cali_sum_edge = 0;
static uint32 sul_cali_sum_mid = 0;

/*===============================================================
	@brief     获取中间电感准备标定
==================================================================*/
void Get_Middle_Standard(int16 *sensor_middle_value)
{
    static uint8  middle_standard_value;
    static uint8  s_first_flag = 1;//归一化基准值
    static int16  s_sensor_middle_standard_history[SENSOR_MIDDLE_NUM] = {0};
    static int16  ADoffset;
    int16 sensor_middle_total = 0;
    int8 i;    
    
      //开机第一次执行
    if(s_first_flag == 1)
    {
	s_first_flag = 0;
	for(i = SENSOR_MIDDLE_NUM - 1; i >= 0; i--)
	{
            s_sensor_middle_standard_history[i] = ORIGINAL_VALUE + ADoffset;
	}
	middle_standard_value = ORIGINAL_VALUE + ADoffset;
    }

    if(*sensor_middle_value >= ORIGINAL_LEAST_VALUE)
    {
        //更新标定基准值，直道条件满足一定次数更新一次。
        for(i = SENSOR_MIDDLE_NUM - 1; i >= 1; i--)
        {
            s_sensor_middle_standard_history[i] = s_sensor_middle_standard_history[i-1];
            sensor_middle_total += s_sensor_middle_standard_history[i];
        }
        s_sensor_middle_standard_history[0] = *sensor_middle_value;
        sensor_middle_total += s_sensor_middle_standard_history[0];

        middle_standard_value = (int16)(sensor_middle_total/SENSOR_MIDDLE_NUM);	
        s_sensor_middle_standard_history[0] = middle_standard_value;
    }	

    middle_standard_value = *sensor_middle_value;
}

/*===============================================================
	@brief     归一化电感值
	@note      使用中间横电感标定
==================================================================*/
void Calibrate_All_Inductor(vint16 inductor_value[])
{
    int i = 0;
    double middle_standard_temp = 0, calculate_temp = 0;	
    middle_standard_temp = gi_calibration_middle;
    
    for(i = 0; i < NUM_INDUCTOR; i++)
    {
        calculate_temp = inductor_value[i] * AD_NORMAL_ORIGINAL;
        calculate_temp = calculate_temp / middle_standard_temp;
        inductor_value[i] = (int16)(calculate_temp * 0.0625) ;//转换为8位精度
    }
}

/*===============================================================
	@brief     电感标定基准值
==================================================================*/
void Get_Standard_Inductor()
{
    float standard_k,standard_k_1;//中间电感及两边电感标定系数

    //使用中间横电感标定
    standard_k = 1000.0/(float)(gi_inductor_calibrate_value[CALI_MID]);
    standard_k_1 = 450.0/(float)(gi_inductor_calibrate_value[CALI_LR])/2 + standard_k/2;

    gi_inductor_value[M_BACK] = (int16)(standard_k * gi_inductor_adc[M_BACK] + 1);
    gi_inductor_value[X_LEFT] = (int16)(standard_k_1 * gi_inductor_adc[X_LEFT] + 1);
    gi_inductor_value[X_RIGHT] = (int16)(standard_k_1 * gi_inductor_adc[X_RIGHT] + 1);
    gi_inductor_value[T_LEFT] = (int16)(standard_k * gi_inductor_adc[T_LEFT] + 1);
    gi_inductor_value[T_RIGHT] = (int16)(standard_k * gi_inductor_adc[T_RIGHT] + 1);
}

/*===============================================================
	@brief     标定中心横电感和两侧横电感
==================================================================*/
void Calibrate_Once()
{
    if(sui_cali_count < MAX_CALI_COUNT)
    {
        sui_cali_count++;
        sul_cali_sum_edge += gi_inductor_adc[X_LEFT];
        sul_cali_sum_edge += gi_inductor_adc[X_RIGHT];
        sul_cali_sum_mid += gi_inductor_adc[M_BACK];
    }
    else
    {
        End_Calibration();   
    }
}

/*===============================================================
	@brief     结束标定
==================================================================*/
void End_Calibration()
{
    uint16 cali_value_temp[2];
    static int16 si_cali_times = 0;
    static int32 sl_cali_value_summation[2];//记录所有标定和
    static int32 sl_cali_weight  = 50;
    
    if((sl_cali_value_summation[CALI_LR] == 0) && (sl_cali_value_summation[CALI_MID] == 0))
    {
        sl_cali_value_summation[CALI_LR] = gi_inductor_calibrate_value[CALI_LR] * sl_cali_weight;
        sl_cali_value_summation[CALI_MID] = gi_inductor_calibrate_value[CALI_MID] * sl_cali_weight;
    }
    if((sui_cali_count > 20) && (si_cali_times < 5))
    {
        cali_value_temp[CALI_LR] = sul_cali_sum_edge / sui_cali_count / 2;       //对标定和取平均
        cali_value_temp[CALI_MID] = sul_cali_sum_mid / sui_cali_count;
      
        si_cali_times++;
        
        sl_cali_value_summation[CALI_LR] += cali_value_temp[CALI_LR] * sui_cali_count;
        sl_cali_value_summation[CALI_MID] += cali_value_temp[CALI_MID] * sui_cali_count;
        sl_cali_weight += sui_cali_count;          //记录权值
        
        //滤波，一次标定只会对标定值产生少量影响，消除偶然跳变
        gi_inductor_calibrate_value[CALI_LR] = sl_cali_value_summation[CALI_LR] / sl_cali_weight;   
        gi_inductor_calibrate_value[CALI_MID] = sl_cali_value_summation[CALI_MID] / sl_cali_weight;
    }
    
    sul_cali_sum_edge = 0;
    sul_cali_sum_mid = 0;
    sui_cali_count = 0;
}
