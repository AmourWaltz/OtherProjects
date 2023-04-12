#include "include.h"

static int16 s_left_pls_count[SPEED_QUEUE_LENTH];
static int16 s_right_pls_count[SPEED_QUEUE_LENTH];

float Get_Left_Speed()
{
    int16 i16Pulses; 
    float fSpeed;   
    
    float Pulses = 0; 
    static uint32 pluses_sum=0;		//总脉冲数
    static uint8 u8InitFlag = 0;
    
    static uint16 cur_speed_pos=0;	//循环队列当前位置标志

    
    i16Pulses = QD_GPT_cnv();   
    pluses_sum += i16Pulses;
    
    if (u8InitFlag == 0)
    {
        
        s_left_pls_count[0] = i16Pulses;
        s_left_pls_count[1] = i16Pulses;
        s_left_pls_count[2] = i16Pulses;
        s_left_pls_count[3] = i16Pulses;
        s_left_pls_count[4] = i16Pulses;
        
        u8InitFlag = 1;     
    }
    else
    { 
        //s_fGPT_SpdTmp[cur_speed_pos%10] = fSpeed;
        s_left_pls_count[cur_speed_pos%SPEED_QUEUE_LENTH] = i16Pulses;
        cur_speed_pos++;
    }
    
    Pulses = (float)
             ( s_left_pls_count[0]
             + s_left_pls_count[1]
             + s_left_pls_count[2]
             + s_left_pls_count[3]
             + s_left_pls_count[4]

             ) / SPEED_QUEUE_LENTH;
    
    gl_left_encoder_count += Pulses;

    fSpeed = (float)((Pulses/(float)(LEFT_PLUSES_PER_MITER*SPEED_UPDATE_PERIOD)*100.0));	//速度单位  cm/s
    //fSpeed = (((float)i16Pulses/(float)(PLUSES_PER_MITER*SPEED_UPDATE_PERIOD)*100.0));	//速度单位  cm/s

	return fSpeed;	
}

float Get_Right_Speed()
{
    static int16 i16Pulses; 
    float fSpeed;   
    //char right_dir;

    float Pulses; 
    static uint32 pluses_sum=0;		//总脉冲数
    static uint8 u8InitFlag = 0;
    
    static uint16 cur_speed_pos=0;	//循环队列当前位置标志

    i16Pulses = PLACC_GPT_cnv();
    
//    right_dir=LPLD_GPIO_Input_b(ENCODER_LE_D_ADDR,ENCODER_LE_D_NUM);
//    
//    if(!right_dir)
//    {
//      i16Pulses =-i16Pulses;
//    }
//    
    pluses_sum += i16Pulses;
    
    if (u8InitFlag == 0)
    {
        
        s_right_pls_count[0] = i16Pulses;
        s_right_pls_count[1] = i16Pulses;
        s_right_pls_count[2] = i16Pulses;
        s_right_pls_count[3] = i16Pulses;
        s_right_pls_count[4] = i16Pulses;
        
        u8InitFlag = 1;     
    }
    else
    { 
        s_right_pls_count[cur_speed_pos%SPEED_QUEUE_LENTH] = i16Pulses;
        cur_speed_pos++;
    }
    
    Pulses = (float)
             ( s_right_pls_count[0]
             + s_right_pls_count[1]
             + s_right_pls_count[2]
             + s_right_pls_count[3]
             + s_right_pls_count[4]
               
             ) / SPEED_QUEUE_LENTH;

    gl_right_encoder_count += Pulses;
    
    fSpeed = (float)((Pulses/(float)(RIGHT_PLUSES_PER_MITER*SPEED_UPDATE_PERIOD)*100));	//速度单位  cm/s
    //fSpeed = (((float)i16Pulses/(float)(PLUSES_PER_MITER*SPEED_UPDATE_PERIOD)*100));	//速度单位  cm/s


	return fSpeed;	
}