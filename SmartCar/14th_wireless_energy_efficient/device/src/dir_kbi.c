/*=============================================
    @file         dir_kbi.c
    @brief        五向按键
    @programmer   Beyond Hsueh
=============================================*/
      
#include "dir_kbi.h"

/*===============================================================
        @note      谁念西风独自凉？萧萧黄叶闭疏窗。沉思往事立残阳。
                   被酒莫惊春睡重，赌书消得泼茶香。当时只道是寻常。
==================================================================*/      

/*===============================================================
	@brief     五向按键引脚初始化
==================================================================*/
void Five_Dir_Init()
{
    GPIO_Init(FIVE_DIR_O_PORT, FIVE_DIR_O_PIN_MASK, DIR_INPUT, OUTPUT_L);
    GPIO_Init(FIVE_DIR_U_PORT, FIVE_DIR_U_PIN_MASK, DIR_INPUT, OUTPUT_L);
    GPIO_Init(FIVE_DIR_D_PORT, FIVE_DIR_D_PIN_MASK, DIR_INPUT, OUTPUT_L); 
    GPIO_Init(FIVE_DIR_L_PORT, FIVE_DIR_L_PIN_MASK, DIR_INPUT, OUTPUT_L);
    GPIO_Init(FIVE_DIR_R_PORT, FIVE_DIR_R_PIN_MASK, DIR_INPUT, OUTPUT_L);     
}

/*===============================================================
	@brief     获取五向按键引脚状态
==================================================================*/
uint8 Get_Five_Dir_Value(uint8 *switchData)
{
    if(!LPLD_GPIO_Input_b(FIVE_DIR_R_PORT, FIVE_DIR_R_PIN))
      return FIVE_DIR_RIGHT;
    
    if(!LPLD_GPIO_Input_b(FIVE_DIR_U_PORT, FIVE_DIR_U_PIN))
      return FIVE_DIR_UP;
    
    if(!LPLD_GPIO_Input_b(FIVE_DIR_D_PORT, FIVE_DIR_D_PIN))
      return FIVE_DIR_DOWN;

    if(!LPLD_GPIO_Input_b(FIVE_DIR_L_PORT, FIVE_DIR_L_PIN))
      return FIVE_DIR_LEFT;
    
    if(!LPLD_GPIO_Input_b(FIVE_DIR_O_PORT, FIVE_DIR_O_PIN))
      return FIVE_DIR_OK;
      
    else 
      return FIVE_DIR_NULL;
}

/*===============================================================
	@brief     获取五向按键引脚状态
	@note      消除按键抖动延时
==================================================================*/
uint8 Five_Dir_Button(uint8 *pu8FiveDirValue)
{
    uint8 u8ValueTemp = 0;
    static uint8 last_value;
    static int scan_delay = 50; //按键延时
    static uint8 change_flag=0;  

    *pu8FiveDirValue = 0;
    if(sleep_ms(scan_delay,0))
    {
        Get_Five_Dir_Value(&u8ValueTemp);
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