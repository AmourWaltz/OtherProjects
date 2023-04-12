#include "led.h"

void LED_Init()
{
    GPIO_Init(LED_PIN_1_PORT, LED_PIN_1_MASK, DIR_OUTPUT, LED_BIT_OFF); 
    GPIO_Init(LED_PIN_2_PORT, LED_PIN_2_MASK, DIR_OUTPUT, LED_BIT_OFF); 
    GPIO_Init(LED_PIN_3_PORT, LED_PIN_3_MASK, DIR_OUTPUT, LED_BIT_OFF); 
    GPIO_Init(LED_PIN_4_PORT, LED_PIN_4_MASK, DIR_OUTPUT, LED_BIT_OFF);    
}

//ÁÁÎª  LED_BIT_ON
void SetLEDBit(uint8 u8LEDBitValue, uint8 u8LEDBit)
{
    switch (u8LEDBit)
    {
        case LED_1:                  
             LPLD_GPIO_Output_b(LED_PIN_1_PORT, LED_PIN_1, u8LEDBitValue);                
             break;
             
        case LED_2:
             LPLD_GPIO_Output_b(LED_PIN_2_PORT, LED_PIN_2, u8LEDBitValue);
             break;
             
        case LED_3:
             LPLD_GPIO_Output_b(LED_PIN_3_PORT, LED_PIN_3, u8LEDBitValue);
             break;
             
        case LED_4:
             LPLD_GPIO_Output_b(LED_PIN_4_PORT, LED_PIN_4, u8LEDBitValue);
             break;                     
        
        default:
             break;
    }
}

void ChangeLEDBit(uint8 u8LEDBit)
{
    switch (u8LEDBit)
    {
        case LED_1:     
             LPLD_GPIO_Toggle_b(LED_PIN_1_PORT, LED_PIN_1);              
             break;
             
        case LED_2:
             LPLD_GPIO_Toggle_b(LED_PIN_2_PORT, LED_PIN_2); 
             break;
             
        case LED_3:
             LPLD_GPIO_Toggle_b(LED_PIN_3_PORT, LED_PIN_3); 
             break;
             
        case LED_4:
             LPLD_GPIO_Toggle_b(LED_PIN_4_PORT, LED_PIN_4); 
             break;                                  
             
        default:
             break;
    }    
}

//ÁÁÎª1
void SetLED(uint8 u8LEDValue)
{
    static uint8 u8LEDBitTemp[4];
    
#ifdef LED_LOW_ACTIVE
    u8LEDValue = (uint8)(~u8LEDValue);
    u8LEDValue &= 0x1f;
#endif 
       
    u8LEDBitTemp[0] = (uint8)((u8LEDValue & 0x01)>>0);
    u8LEDBitTemp[1] = (uint8)((u8LEDValue & 0x02)>>1);
    u8LEDBitTemp[2] = (uint8)((u8LEDValue & 0x04)>>2);
    u8LEDBitTemp[3] = (uint8)((u8LEDValue & 0x08)>>3);

    LPLD_GPIO_Output_b(LED_PIN_1_PORT, LED_PIN_1, u8LEDBitTemp[0]);
    LPLD_GPIO_Output_b(LED_PIN_2_PORT, LED_PIN_2, u8LEDBitTemp[1]);
    LPLD_GPIO_Output_b(LED_PIN_3_PORT, LED_PIN_3, u8LEDBitTemp[2]);
    LPLD_GPIO_Output_b(LED_PIN_4_PORT, LED_PIN_4, u8LEDBitTemp[3]);
}

