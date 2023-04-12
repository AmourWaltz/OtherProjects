#include "adc.h"
#include "include.h"

//ADC等待最多用100us左右 故采用中断方式节省时间

static ADC_InitTypeDef adc0_init_struct;
static ADC_InitTypeDef adc1_init_struct;

void ADC_Init()
{
    ADC0_Init();
    ADC1_Init();
        
    ADC_CH_Init();
       
    //在NVIC里面使能中断
    //LPLD_ADC_EnableIrq(adc0_init_struct);
    //LPLD_ADC_EnableIrq(adc1_init_struct);
}

void ADC0_Init()
{
    //配置ADC采样参数
    adc0_init_struct.ADC_Adcx = ADC0;
    adc0_init_struct.ADC_DiffMode = ADC_SE;        //单端采集
    adc0_init_struct.ADC_BitMode = SE_12BIT;       //单端12位精度
    adc0_init_struct.ADC_SampleTimeCfg = SAMTIME_LONG;    //短采样时间
    adc0_init_struct.ADC_LongSampleTimeSel = LSAMTIME_12EX;
    adc0_init_struct.ADC_HwAvgSel = HW_16AVG;       //4次硬件平均
    adc0_init_struct.ADC_CalEnable = TRUE; //使能初始化校验
       
    adc0_init_struct.ADC_Isr = ADC_0_isr; //中断函数 
    //初始化ADC
    LPLD_ADC_Init(adc0_init_struct);
}

void ADC1_Init()
{
    //配置ADC采样参数
    adc1_init_struct.ADC_Adcx = ADC1;
    adc1_init_struct.ADC_DiffMode = ADC_SE;        //单端采集
    adc1_init_struct.ADC_BitMode = SE_12BIT;       //单端12位精度
    adc1_init_struct.ADC_SampleTimeCfg = SAMTIME_LONG;    //短采样时间
    adc1_init_struct.ADC_LongSampleTimeSel = LSAMTIME_12EX;//LSAMTIME_12EX;
    adc1_init_struct.ADC_HwAvgSel = HW_16AVG;       //4次硬件平均
    adc1_init_struct.ADC_CalEnable = TRUE;    //使能初始化校验
    adc1_init_struct.ADC_MuxSel = MUX_ADXXB; //b通道采集
    
    adc1_init_struct.ADC_Isr = ADC_1_isr; //中断函数 
    //初始化ADC
    LPLD_ADC_Init(adc1_init_struct);
}

void ADC_CH_Init()
{
    //使能相应通道
    
    //电池AD通道初始化

    LPLD_ADC_Chn_Enable(BAT_ADC_ID, BAT_ADC_CH);
    
}

void ADC_0_isr()
{

}


void ADC_1_isr()
{

}

float ADC_Bat_Vol()
{
    float result = 0;
    
    result = LPLD_ADC_Get(BAT_ADC_ID, BAT_ADC_CH);
    //gl_u16bat_voltage = 3*3300*gl_u16bat_ADC/0x0fff;
    //return 3*(result * 0.0008002 + 0.0189);
    return 3*3.3*result/0x0fff;
    //return gl_u16bat_voltage;
}





