#include "dir_kbi.h"


void Five_Dir_Init()
{
    GPIO_Init(FIVE_DIR_O_PORT, FIVE_DIR_O_PIN_MASK, DIR_INPUT, OUTPUT_L);
    GPIO_Init(FIVE_DIR_U_PORT, FIVE_DIR_U_PIN_MASK, DIR_INPUT, OUTPUT_L);
    GPIO_Init(FIVE_DIR_D_PORT, FIVE_DIR_D_PIN_MASK, DIR_INPUT, OUTPUT_L); 
    GPIO_Init(FIVE_DIR_L_PORT, FIVE_DIR_L_PIN_MASK, DIR_INPUT, OUTPUT_L);
    GPIO_Init(FIVE_DIR_R_PORT, FIVE_DIR_R_PIN_MASK, DIR_INPUT, OUTPUT_L);     
}

//按下电平为0
uint8 Get_Five_Dir_Value()
{


    if(!LPLD_GPIO_Input_b(FIVE_DIR_R_PORT, FIVE_DIR_R_PIN))
      return FIVE_DIR_RIGHT;
    
    if(!LPLD_GPIO_Input_b(FIVE_DIR_U_PORT, FIVE_DIR_U_PIN))
      return FIVE_DIR_UP;
    
    if(!LPLD_GPIO_Input_b(FIVE_DIR_D_PORT, FIVE_DIR_D_PIN))
      return FIVE_DIR_DOWN;

//    if(!LPLD_GPIO_Input_b(FIVE_DIR_L_PORT, FIVE_DIR_L_PIN))
//      return FIVE_DIR_LEFT;
    
    if(!LPLD_GPIO_Input_b(FIVE_DIR_O_PORT, FIVE_DIR_O_PIN))
      return FIVE_DIR_OK;
      
    else 
      return FIVE_DIR_NULL;
}

void Test_Dir()
{
    uint8 button;
    while(1)
    {
        button=button_scan();
        
        switch(button)
        {
        case FIVE_DIR_UP:
          oled_display_6x8str(1,1,"up");
          SetLEDBit(1,LED_1);
          break;
          
        case FIVE_DIR_LEFT:
          oled_display_6x8str(1,2,"left");
          SetLEDBit(1,LED_2);
          break;  
          
        case FIVE_DIR_DOWN:
          oled_display_6x8str(1,3,"down");
          SetLEDBit(1,LED_3);
          break;
          
        case FIVE_DIR_RIGHT:
          oled_display_6x8str(1,4,"right");
          SetLEDBit(1,LED_4);
          break;
        
        case FIVE_DIR_OK:
          oled_display_6x8str(1,5,"ok");
          SetLEDBit(1,LED_1);
          SetLEDBit(0,LED_2);
          SetLEDBit(1,LED_3);
          SetLEDBit(0,LED_4);
          break;
          
        default:
          oled_fill(0x00);
          SetLEDBit(LED_1,0);
          SetLEDBit(LED_2,0);
          SetLEDBit(LED_3,0);
          SetLEDBit(LED_4,0);
          break;
        }
    }
}


uint8 Five_Dir_Button(uint8 *pu8FiveDirValue)
{
    uint8 u8ValueTemp = 0;
    static uint8 last_value;
    static int scan_delay = 50; //按键延时
    static uint8 change_flag=0;  

    *pu8FiveDirValue = 0;
    if(sleep_ms(scan_delay,0))
    {
        Get_Five_Dir_Value();
        if ((u8ValueTemp>=1) & (u8ValueTemp<=5)) //值能识别为正常按键
        {
            
            *pu8FiveDirValue = u8ValueTemp;
            if(u8ValueTemp != last_value)
            {
                change_flag = 1;
            }
            else
            {
                change_flag = 0;
            }
            
        }
        if(change_flag == 1)   //确认按下一次之后，等待较长时间再进行第二次检测
        {
            scan_delay =200;
        }
        else
        {
           scan_delay = 100;
        }
        last_value = u8ValueTemp;
    }
    return u8ValueTemp;
}