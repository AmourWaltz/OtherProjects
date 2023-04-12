/*=============================================
    @file         double_ranging.c
    @brief        红外测距检测路障
    @programmer   Beyond Hsueh
=============================================*/

#include "double_ranging.h"

/*===============================================================
        @note      寒蝉凄切，对长亭晚，骤雨初歇。
                   都门帐饮无绪，留恋处，兰舟催发。
                   执手相看泪眼，竟无语凝噎。
                   念去去，千里烟波，暮霭沉沉楚天阔。
                   多情自古伤离别，更那堪，冷落清秋节。
                   今宵酒醒何处？杨柳岸，晓风残月。
                   此去经年，应是良辰好景虚设。
                   便纵有，千种风情，更与何人说？
==================================================================*/ 

/*===============================================================
	@brief     红外测距AD初始化
==================================================================*/

void Infrared_AD_Init()
{
#ifdef INFRARED_UP
    LPLD_ADC_Chn_Enable(INFRARED_UP_ADC_ID, INFRARED_UP_ADC_CH);
#endif

#ifdef INFRARED_DOWN    
    LPLD_ADC_Chn_Enable(INFRARED_DOWN_ADC_ID, INFRARED_DOWN_ADC_CH);
#endif
}

/*===============================================================
	@brief     红外AD均值滤波
==================================================================*/

uint32 Get_Infrared_ADC_Ave(uint8 infrared_num)
{
    uint8 i;
    uint32 infrared_adc_temp;
    uint32 infrared_adc_add;
    
    infrared_adc_add = 0;
    
    for(i = 0;i < OBSTACLE_AVERAGE;i++)
    {
      switch(infrared_num)
      {
#ifdef INFRARED_UP
          case INFRARED_UP:
                infrared_adc_add += LPLD_ADC_Get(INFRARED_UP_ADC_ID, INFRARED_UP_ADC_CH);
                break;
#endif

#ifdef INFRARED_DOWN
          case INFRARED_DOWN:
                infrared_adc_add += LPLD_ADC_Get(INFRARED_DOWN_ADC_ID, INFRARED_DOWN_ADC_CH);
                break;
#endif
          default:
                break;   
      }
    }
    
    infrared_adc_temp = (uint32)(infrared_adc_add / OBSTACLE_AVERAGE);
    
    return infrared_adc_temp;
}
