/*=============================================
    @file         beep.c
    @brief        蜂鸣器文件
    @programmer   Beyond Hsueh
=============================================*/

#include "beep.h"

/*===============================================================
        @note      野有蔓草，零露漙兮。
                   有美一人，清扬婉兮。
                   邂逅相遇，适我愿兮。
                   野有蔓草，零露瀼瀼。
                   有美一人，婉如清扬。
                   邂逅相遇，与子偕臧。
==================================================================*/
      
/*===============================================================
	@brief     初始化蜂鸣器
==================================================================*/
void Beep_Init()
{
    GPIO_Init(BEEP_PORT, BEEP_PIN_MASK, DIR_OUTPUT, BEEP_OFF_LV);          
}

/*===============================================================
	@brief     开蜂鸣器
==================================================================*/
void Beep_On()
{
   if (guc_beep_start_flag == OFF)
    {
        Beep_Off();
    }
    else
    {
      
#ifdef COMPETITION_MODE
      LPLD_GPIO_Output_b(BEEP_PORT, BEEP_PIN, BEEP_OFF_LV);
#else
      if(guc_automation_mode_flag == ON)
      {
          LPLD_GPIO_Output_b(BEEP_PORT, BEEP_PIN, BEEP_OFF_LV);
      }
      else
      {
          LPLD_GPIO_Output_b(BEEP_PORT, BEEP_PIN, BEEP_ON_LV);
      }
#endif    
        
    }
}

/*===============================================================
	@brief     关蜂鸣器
==================================================================*/
void Beep_Off()
{
    LPLD_GPIO_Output_b(BEEP_PORT, BEEP_PIN, BEEP_OFF_LV);
}

/*===============================================================
	@brief     读取蜂鸣器状态
==================================================================*/
void Get_Beep_State(uint8 *uc_state)
{    
    *uc_state = LPLD_GPIO_Input_b(BEEP_PORT, BEEP_PIN);
}
