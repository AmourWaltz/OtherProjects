/*=============================================
    @file         adc.c
    @brief        模数转换
    @programmer   Beyond Hsueh
=============================================*/

#include "adc.h"

/*===============================================================
        @note      残雪凝辉冷画屏，落梅横笛已三更。更无人处月胧明。
                   我是人间惆怅客，知君何事泪纵横。断肠声里忆平生。
==================================================================*/
      
static ADC_InitTypeDef adc0_init_struct;
static ADC_InitTypeDef adc1_init_struct;

/*===============================================================
	@brief     初始化模数转换器
        @note      ADC等待最多用100us左右 故采用中断方式节省时间
==================================================================*/
void ADC_Init()
{
    ADC0_Init();
    ADC1_Init();
        
    ADC_CH_Init();
       
    //在NVIC里面使能中断
    //LPLD_ADC_EnableIrq(adc0_init_struct);
    //LPLD_ADC_EnableIrq(adc1_init_struct);
}

/*===============================================================
	@brief     初始化模数转换器ADC0
==================================================================*/
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
//    adc0_init_struct.ADC_MuxSel = MUX_ADXXB; //b通道采集   
//    adc0_init_struct.ADC_Isr = ADC_0_isr; //中断函数 
    //初始化ADC
    LPLD_ADC_Init(adc0_init_struct);
}

/*===============================================================
	@brief     初始化模数转换器ADC1
==================================================================*/
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
    //adc1_init_struct.ADC_MuxSel = MUX_ADXXB; //b通道采集
    
    adc1_init_struct.ADC_Isr = ADC_1_isr; //中断函数 
    //初始化ADC
    LPLD_ADC_Init(adc1_init_struct);
}

/*===============================================================
	@brief     使能电池电压通道
==================================================================*/
void ADC_CH_Init()
{
    //使能相应通道
    
    //电池AD通道初始化

//    LPLD_ADC_Chn_Enable(BAT_ADC_ID, BAT_ADC_CH);
    
}

/*===============================================================
	@brief     ADC0中断函数
==================================================================*/
void ADC_0_isr()
{

}

/*===============================================================
	@brief     ADC1中断函数
==================================================================*/
void ADC_1_isr()
{

}

/*===============================================================
	@brief     电池电压转换读取
==================================================================*/
int16 ADC_Battery_Voltage()
{
    float result = 0;
    
    result = LPLD_ADC_Get(BAT_ADC_ID, BAT_ADC_CH);

    return (int16)(4*5000*result/0x0fff);
}





