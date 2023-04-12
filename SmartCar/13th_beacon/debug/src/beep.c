#include "beep.h"

void Beep_Init()
{
    GPIO_Init(BEEP_PORT, BEEP_PIN_MASK, DIR_OUTPUT, BEEP_OFF_LV);             
}

void Beep_On()
{
    if (guc_beep_CTL_flag == OFF)
    {
        Beep_Off();
    }
    else
    {
        LPLD_GPIO_Output_b(BEEP_PORT, BEEP_PIN, BEEP_ON_LV);
    }
}

void Beep_Off()
{
    LPLD_GPIO_Output_b(BEEP_PORT, BEEP_PIN, BEEP_OFF_LV);
}

void Get_Beep_State(uint8 *u8State)
{    
    *u8State = LPLD_GPIO_Input_b(BEEP_PORT, BEEP_PIN);
}
