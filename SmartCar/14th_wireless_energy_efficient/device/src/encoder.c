/*=============================================
    @file         encoder.c
    @brief        编码器测速
    @programmer   Beyond Hsueh
=============================================*/

#include "encoder.h"

/*===============================================================
        @note      甚矣吾衰矣。怅平生、交游零落，只今余几。
                   白发空垂三千丈，一笑人间万事。
                   问何物、能令公喜。
                   我见青山多妩媚，料青山、见我应如是。情与貌，略相似。
                   一尊搔首东窗里。想渊明、停云诗就，此时风味。
                   江左沈酣求名者，岂识浊醪妙理。
                   回首叫、云飞风起。
                   不恨古人吾不见，恨古人、不见吾狂耳。知我者，二三子。
==================================================================*/

/*===============================================================
	@brief     由当前脉冲计数获取速度值
	@note      脉冲计数滤波当前采取移动平均滤波，后期尝试巴斯沃特数字滤波
==================================================================*/
void Get_Right_Speed()
{
    uint8 pulse_loop;
    int16 pulse_temp_r;
    int16 pulsesum_r = 0;
    float pulse_r;
    static int16 rightspeed_queue[MOVINGAVERAGE_NUM]={0};
    static uint8 position=0;
    static uint8 init_flag = 0;
    /*用滑动平均法求得当前速度*/
    
//    //节能一号
//    pulse_temp_r = PLACC_GPT_cnv();  //记录当前采得的脉冲数
    
    //节能二号
    pulse_temp_r = -PLACC_GPT_cnv();  //记录当前采得的脉冲数
    
    if(init_flag == 0)
    {
        for(pulse_loop = 0;pulse_loop<MOVINGAVERAGE_NUM; pulse_loop++)
        {
            rightspeed_queue[pulse_loop] = pulse_temp_r;
        }
        init_flag = 1;
    }
    else
    {
        rightspeed_queue[position] = pulse_temp_r;
        if(position == (MOVINGAVERAGE_NUM-1))
        {
            position = 0;
        }
        else
        {
            position ++;
        }
    }
    for(pulse_loop = 0;pulse_loop<MOVINGAVERAGE_NUM; pulse_loop++)
    {
        pulsesum_r += rightspeed_queue[pulse_loop];
    }
    
    gl_right_encoder_count += pulsesum_r;
    
    pulse_r = (float)pulsesum_r/MOVINGAVERAGE_NUM;

    gf_right_speed = pulse_r/(float)(PULSE_PER_METRE_R*SPD_UPDATE_PERIOD_S);     
    
}

void Get_Left_Speed()
{
    uint8 pulse_loop;
    int16 pulse_temp_l;
    int16 pulsesum_l = 0;
    float pulse_l;
    static int16 leftspeed_queue[MOVINGAVERAGE_NUM]={0};
    static uint8 position=0;
    static uint8 init_flag = 0;
    /*用滑动平均法求得当前速度*/
    
//    //节能一号
//    pulse_temp_l = QD_GPT_cnv();   //记录当前采得的脉冲数
    
    //节能二号
    pulse_temp_l = QD_GPT_cnv();   //记录当前采得的脉冲数
    
    
    if(init_flag == 0)
    {
        for(pulse_loop = 0;pulse_loop<MOVINGAVERAGE_NUM; pulse_loop++)
        {
            leftspeed_queue[pulse_loop] = pulse_temp_l;
        }
        init_flag = 1;
    }
    else
    {
        leftspeed_queue[position] = pulse_temp_l;
        if(position == (MOVINGAVERAGE_NUM-1))
        {
            position = 0;
        }
        else
        {
            position ++;
        }
    }
    for(pulse_loop = 0;pulse_loop<MOVINGAVERAGE_NUM; pulse_loop++)
    {
        pulsesum_l += leftspeed_queue[pulse_loop];
    }
    
    gl_left_encoder_count += pulsesum_l;
    
    pulse_l = (float)pulsesum_l/MOVINGAVERAGE_NUM;

    gf_left_speed =(float)(pulse_l/(float)(PULSE_PER_METRE_L*SPD_UPDATE_PERIOD_S));   
}

/*===============================================================
	@brief     获得速度
==================================================================*/
void Get_Car_Speed()
{
    Get_Right_Speed();
    Get_Left_Speed();
    gf_car_encoder_value = (gf_right_speed + gf_left_speed)/2;    //得到当前编码器返回速度，速度单位：m/s
}

/*===============================================================
	@brief     获得跑过的距离
==================================================================*/
void Get_Car_Distance()
{
    float f_speed_temp;
    static float sf_car_distance = 0;
    static float sf_car_left_distance = 0;
    static float sf_car_right_distance = 0;

    Get_Car_Speed();
    f_speed_temp = gf_car_encoder_value;
    
    gl_car_speed = (int32)(gf_car_encoder_value * TO_CENTIMETRE);
    
    sf_car_distance += f_speed_temp * PIT_1_PERIOD_MS;//路程单位：m
    sf_car_left_distance += gf_left_speed * PIT_1_PERIOD_MS;//路程单位：m
    sf_car_right_distance += gf_right_speed * PIT_1_PERIOD_MS;//路程单位：m
    gl_car_running_distance = (int32)(sf_car_distance * PULSE_PER_CENTIMETRE);
    gl_left_encoder_value = (int32)(sf_car_left_distance * PULSE_PER_CENTIMETRE);
    gl_right_encoder_value = (int32)(sf_car_right_distance * PULSE_PER_CENTIMETRE);
}