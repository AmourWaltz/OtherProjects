/*=============================================
    @file         rtc.c
    @brief        RTC万年历
    @programmer   Beyond Hsueh
=============================================*/

#include "rtc.h"
#include "TimeStamp.h"

/*===============================================================
        @note      西风烈，长空雁叫霜晨月，
                   霜晨月，马蹄声碎，喇叭声咽。
                   雄关漫道真如铁，而今迈步从头越，
                   从头越，苍山如海，残阳如血。
==================================================================*/    

RTC_InitTypeDef rtc_init_structure;
TimeStamp_FormatTypeDef time;

/*===============================================================
	@brief      日历初始化
	@note       使用拉普兰德库函数，相较山外的库，该库可直接调用编译器时间
==================================================================*/
void RTC_Init()
{   
    //开启RTC SIM 时钟，通过判断TIF获知
    //当前RTC是否发生VBAT POR或者 Software Reset
    //如果没有发生，RTC正常计数   
//    if(LPLD_RTC_IsRunning() == 0) 
//    {   
        LPLD_AutoInitTimeStamp(&time);

        rtc_init_structure.RTC_Seconds = LPLD_DateTimeToTimeStamp(&time);
        LPLD_RTC_Init(rtc_init_structure);
//    }
}

/*===============================================================
	@brief      日期显示
==================================================================*/
void Show_Time()
{
    uint32 temp;
    
    temp = LPLD_RTC_GetRealTime();
    LPLD_TimeStampToDateTime(temp,&time);
    //printf("%4d-%02d-%02d %02d:%02d:%02d\r\n", time.year, time.month, time.day, time.hour, time.minute, time.second);
    LPLD_LPTMR_DelayMs(1000);
    
    oled_print_16x8short(5,5,(int16)(time.year));
    oled_display_16x8str(55,5,"-");
    oled_print_16x8short(65,5,(int16)(time.month));
    oled_display_16x8str(85,5,"-");
    oled_print_16x8short(95,5,(int16)(time.day));
    oled_print_16x8short(35,35,(int16)(time.hour));
    oled_display_16x8str(55,35,"-");
    oled_print_16x8short(65,35,(int16)(time.minute));
    oled_display_16x8str(85,35,"-");
    oled_print_16x8short(95,35,(int16)(time.second));
}