#include "display_image.h"

static uint8 gs_u8ExitDisplayFlag = FLAG_OFF;

static uint8 gs_u8ImageLineDelayControlFlag = FLAG_OFF;


/*******************************************************
	函数名: DisplayPage_1                                                   
	功  能: 显示页                                                
	参  数: 无                                                                      
	返  回: 下一页页码                                                          
*********************************************************/
uint8 DisplayPage_1()
{
    uint8 page_num = 1;
    int8  exit_flag = 0;
    uint8 button_value = 0;

	
    oled_fill(0x00);
    oled_display_6x8str(0*OLED_CHAR_WIDTH, 0, "p1:");
    Delay_ms(DISPLAY_IMAGE_DELAY);

    while(exit_flag == 0)
    {
                oled_print_std_image(0);
        //读取按键值
                button_value=button_scan();
        
#ifdef USE_INFR 
        if (button_value == FIVE_DIR_NULL)//无五向按键按下则判断红外
        {
            button_value = get_infrared_command();
            reset_infrared();
            
            switch(button_value)
            {
                case COMMAND_UP:
                    button_value = FIVE_DIR_UP;
                    break;
                case COMMAND_DOWN:
                    button_value = FIVE_DIR_DOWN;
                    break;                    
                case COMMAND_LEFT:
                    button_value = FIVE_DIR_LEFT;
                    break;                    
                case COMMAND_RIGHT:
                    button_value = FIVE_DIR_RIGHT;
                    break;
                case COMMAND_OK:
                    button_value = FIVE_DIR_OK;
                    break;     
                default:
                    button_value = FIVE_DIR_NULL;//识别不出则认为没按下
                    break;                                    
            }
        }
#endif   

        switch(button_value)
        {
        case BT_UP_PRESS:
            page_num = page_num + SPECIAL_PAGE_OFFSET;
            exit_flag = 1;
            break;
        case BT_DN_PRESS:
            page_num = page_num + SPECIAL_PAGE_OFFSET;
            exit_flag = 1;                                  
            break;
            //左键向前翻页
        case BT_LE_PRESS:
            if (gs_u8ImageLineDelayControlFlag == 1)
            {
                g_u8DigitalCameraThreshold--;
                SCCB_WriteByte(OV7725_CNST, g_u8DigitalCameraThreshold);
            }
            
            if (gs_u8ImageLineDelayControlFlag == 0)
            {
                if(page_num > DISPLAY_PAGE_MIN)
                {
                    page_num--;
                }
                //当前页是最前一页，翻至最后一页
                else
                {
                    page_num = DISPLAY_PAGE_MAX;
                }
                //退出标志位置位
                exit_flag = 1;                
            }

            break;
            //右键向后翻页
        case BT_RI_PRESS:
            
            if (gs_u8ImageLineDelayControlFlag == 1)
            {
                g_u8DigitalCameraThreshold++;
                SCCB_WriteByte(OV7725_CNST, g_u8DigitalCameraThreshold);
            }

            if ((gs_u8ImageLineDelayControlFlag == 0) )//&& (g_carInfo.u8RoadParameterFlag == 0))
            {
                if(page_num < DISPLAY_PAGE_MAX)
                {
                    page_num++;
                }
                //当前页是最后一页，右键按下翻至最前一页
                else
                {
                    page_num = DISPLAY_PAGE_MIN;
                }
                //退出标志位置位
                exit_flag = 1;           
            }            

            break;
        case BT_OK_PRESS:
            break;
        default:
            break;
        }
    }

    return page_num;
}

/*******************************************************
	函数名: DisplayPage_2                                                   
	功  能: 显示页                                                
	参  数: 无                                                                      
	返  回: 下一页页码                                                          
*********************************************************/
uint8 DisplayPage_2()
{
	uint8 page_num = 2;
    int8  exit_flag = 0;
    uint8 button_value = 0;
	
    
	oled_fill(0x00);
    oled_display_6x8str(0*OLED_CHAR_WIDTH, 0, "p2:");
	Delay_ms(DISPLAY_IMAGE_DELAY);

    while(exit_flag == 0)
    {

        //Show_Image_Process();
        //读取按键值
                button_value=button_scan();
        
#ifdef USE_INFR 
        if (button_value == FIVE_DIR_NULL)//无五向按键按下则判断红外
        {
            button_value = get_infrared_command();
            reset_infrared();
            
            switch(button_value)
            {
                case COMMAND_UP:
                    button_value = FIVE_DIR_UP;
                    break;
                case COMMAND_DOWN:
                    button_value = FIVE_DIR_DOWN;
                    break;                    
                case COMMAND_LEFT:
                    button_value = FIVE_DIR_LEFT;
                    break;                    
                case COMMAND_RIGHT:
                    button_value = FIVE_DIR_RIGHT;
                    break;
                case COMMAND_OK:
                    button_value = FIVE_DIR_OK;
                    break;     
                default:
                    button_value = FIVE_DIR_NULL;//识别不出则认为没按下
                    break;                                    
            }
        }
#endif   

        switch(button_value)
        {
        case BT_UP_PRESS:
            page_num = page_num + SPECIAL_PAGE_OFFSET;
            exit_flag = 1;
            break;
        case BT_DN_PRESS:
            page_num = page_num + SPECIAL_PAGE_OFFSET;
            exit_flag = 1;                                  
            break;
            //左键向前翻页
        case BT_LE_PRESS:
            if (gs_u8ImageLineDelayControlFlag == 1)
            {
                g_u8DigitalCameraThreshold--;
                SCCB_WriteByte(OV7725_CNST, g_u8DigitalCameraThreshold);
            }
            
            if ((gs_u8ImageLineDelayControlFlag == 0) )
            {
                if(page_num > DISPLAY_PAGE_MIN)
                {
                    page_num--;
                }
                //当前页是最前一页，翻至最后一页
                else
                {
                    page_num = DISPLAY_PAGE_MAX;
                }
                //退出标志位置位
                exit_flag = 1;                
            }

            break;
            //右键向后翻页
        case BT_RI_PRESS:
            
            if (gs_u8ImageLineDelayControlFlag == 1)
            {
                g_u8DigitalCameraThreshold++;
                SCCB_WriteByte(OV7725_CNST, g_u8DigitalCameraThreshold);
            }
            
            
            if ((gs_u8ImageLineDelayControlFlag == 0) )//&& (g_carInfo.u8RoadParameterFlag == 0))
            {
                if(page_num < DISPLAY_PAGE_MAX)
                {
                    page_num++;
                }
                //当前页是最后一页，右键按下翻至最前一页
                else
                {
                    page_num = DISPLAY_PAGE_MIN;
                }
                //退出标志位置位
                exit_flag = 1;           
            }            

            break;
        case BT_OK_PRESS:
            break;
        default:
            break;
        }
    }

    return page_num;
}

/*******************************************************
	函数名: DisplayPage_3                                                  
	功  能: 显示页                                                
	参  数: 无                                                                      
	返  回: 下一页页码                                                          
*********************************************************/
uint8  DisplayPage_3()
{
	uint8 page_num = 3;
    int8  exit_flag = 0;
    uint8 button_value = 0;
	
    
	oled_fill(0x00);
    oled_display_6x8str(0*OLED_CHAR_WIDTH, 0, "p3:");
	Delay_ms(DISPLAY_IMAGE_DELAY);

    while(exit_flag == 0)
    {
        //读取按键值
                button_value=button_scan();
        
#ifdef USE_INFR 
        if (button_value == FIVE_DIR_NULL)//无五向按键按下则判断红外
        {
            button_value = get_infrared_command();
            reset_infrared();
            
            switch(button_value)
            {
                case COMMAND_UP:
                    button_value = FIVE_DIR_UP;
                    break;
                case COMMAND_DOWN:
                    button_value = FIVE_DIR_DOWN;
                    break;                    
                case COMMAND_LEFT:
                    button_value = FIVE_DIR_LEFT;
                    break;                    
                case COMMAND_RIGHT:
                    button_value = FIVE_DIR_RIGHT;
                    break;
                case COMMAND_OK:
                    button_value = FIVE_DIR_OK;
                    break;     
                default:
                    button_value = FIVE_DIR_NULL;//识别不出则认为没按下
                    break;                                    
            }
        }
#endif   

        switch(button_value)
        {
        case BT_UP_PRESS:
            page_num = page_num + SPECIAL_PAGE_OFFSET;
            exit_flag = 1;
            break;
        case BT_DN_PRESS:
            page_num = page_num + SPECIAL_PAGE_OFFSET;
            exit_flag = 1;                                  
            break;
            //左键向前翻页
        case BT_LE_PRESS:
            if (gs_u8ImageLineDelayControlFlag == 1)
            {
                g_u8DigitalCameraThreshold--;
                SCCB_WriteByte(OV7725_CNST, g_u8DigitalCameraThreshold);
            }
            
            if ((gs_u8ImageLineDelayControlFlag == 0) )//&& (g_carInfo.u8RoadParameterFlag == 0))
            {
                if(page_num > DISPLAY_PAGE_MIN)
                {
                    page_num--;
                }
                //当前页是最前一页，翻至最后一页
                else
                {
                    page_num = DISPLAY_PAGE_MAX;
                }
                //退出标志位置位
                exit_flag = 1;                
            }

            break;
            //右键向后翻页
        case BT_RI_PRESS:
            
            if (gs_u8ImageLineDelayControlFlag == 1)
            {
                g_u8DigitalCameraThreshold++;
                SCCB_WriteByte(OV7725_CNST, g_u8DigitalCameraThreshold);
            }
            
            if (gs_u8ImageLineDelayControlFlag == 0)
            {
                if(page_num < DISPLAY_PAGE_MAX)
                {
                    page_num++;
                }
                //当前页是最后一页，右键按下翻至最前一页
                else
                {
                    page_num = DISPLAY_PAGE_MIN;
                }
                //退出标志位置位
                exit_flag = 1;           
            }            

            break;
        case BT_OK_PRESS:
            break;
        default:
            break;
        }
    }

    return page_num;
}

/*******************************************************
	函数名: DisplayPage_4                                                  
	功  能: 显示页                                                
	参  数: 无                                                                      
	返  回: 下一页页码                                                          
*********************************************************/
uint8  DisplayPage_4()
{
	uint8 page_num = 4;
    int8  exit_flag = 0;
    uint8 button_value = 0;
	
	oled_fill(0x00);
    oled_display_6x8str(0*OLED_CHAR_WIDTH, 0, "p4:");
	Delay_ms(DISPLAY_IMAGE_DELAY);

    while(exit_flag == 0)
    {
        //读取按键值
                button_value=button_scan();
        
#ifdef USE_INFR 
        if (button_value == FIVE_DIR_NULL)//无五向按键按下则判断红外
        {
            button_value = get_infrared_command();
            reset_infrared();
            
            switch(button_value)
            {
                case COMMAND_UP:
                    button_value = FIVE_DIR_UP;
                    break;
                case COMMAND_DOWN:
                    button_value = FIVE_DIR_DOWN;
                    break;                    
                case COMMAND_LEFT:
                    button_value = FIVE_DIR_LEFT;
                    break;                    
                case COMMAND_RIGHT:
                    button_value = FIVE_DIR_RIGHT;
                    break;
                case COMMAND_OK:
                    button_value = FIVE_DIR_OK;
                    break;     
                default:
                    button_value = FIVE_DIR_NULL;//识别不出则认为没按下
                    break;                                    
            }
        }
#endif   

        switch(button_value)
        {
        case BT_UP_PRESS:
            page_num = page_num + SPECIAL_PAGE_OFFSET;
            exit_flag = 1;
            break;
        case BT_DN_PRESS:
            page_num = page_num + SPECIAL_PAGE_OFFSET;
            exit_flag = 1;                                  
            break;
            //左键向前翻页
        case BT_LE_PRESS:
            if (gs_u8ImageLineDelayControlFlag == 1)
            {
                g_u8DigitalCameraThreshold--;
                SCCB_WriteByte(OV7725_CNST, g_u8DigitalCameraThreshold);
            }
            
            if (gs_u8ImageLineDelayControlFlag == 0)
            {
                if(page_num > DISPLAY_PAGE_MIN)
                {
                    page_num--;
                }
                //当前页是最前一页，翻至最后一页
                else
                {
                    page_num = DISPLAY_PAGE_MAX;
                }
                //退出标志位置位
                exit_flag = 1;                
            }

            break;
            //右键向后翻页
        case BT_RI_PRESS:
            
            if (gs_u8ImageLineDelayControlFlag == 1)
            {
                g_u8DigitalCameraThreshold++;
                SCCB_WriteByte(OV7725_CNST, g_u8DigitalCameraThreshold);
            }
            
            if (gs_u8ImageLineDelayControlFlag == 0)
            {
                if(page_num < DISPLAY_PAGE_MAX)
                {
                    page_num++;
                }
                //当前页是最后一页，右键按下翻至最前一页
                else
                {
                    page_num = DISPLAY_PAGE_MIN;
                }
                //退出标志位置位
                exit_flag = 1;           
            }            

            break;
        case BT_OK_PRESS:
            break;
        default:
            break;
        }
    }

    return page_num;
}


/*******************************************************
	函数名: SpecialPage                                                  
	功  能: 特殊页                                                
    参  数: 编码后的上一页页码                                                                      
	返  回: 下一页页码                                                          
*********************************************************/
uint8 SpecialPage(uint8 page_num)
{
    int8  exit_flag = 0;
    uint8 button_value = 0;
    
    
	
	oled_fill(0x00);


    oled_display_6x8str_hl(0*OLED_CHAR_WIDTH, 0, "ThresholdCtrl:");
    oled_display_6x8str(0*OLED_CHAR_WIDTH, 1, "Threshold:");

    while(exit_flag == 0)
    {       
        oled_display_boolean(15*OLED_CHAR_WIDTH, 0, gs_u8ImageLineDelayControlFlag);	 
        
        oled_print_short(15*OLED_CHAR_WIDTH, 1, g_u8DigitalCameraThreshold);
        
        
        //读取按键值
                button_value=button_scan();
        
#ifdef USE_INFR 
        if (button_value == FIVE_DIR_NULL)//无五向按键按下则判断红外
        {
            button_value = get_infrared_command();
            reset_infrared();
            
            switch(button_value)
            {
                case COMMAND_UP:
                    button_value = FIVE_DIR_UP;
                    break;
                case COMMAND_DOWN:
                    button_value = FIVE_DIR_DOWN;
                    break;                    
                case COMMAND_LEFT:
                    button_value = FIVE_DIR_LEFT;
                    break;                    
                case COMMAND_RIGHT:
                    button_value = FIVE_DIR_RIGHT;
                    break;
                case COMMAND_OK:
                    button_value = FIVE_DIR_OK;
                    break;     
                default:
                    button_value = FIVE_DIR_NULL;//识别不出则认为没按下
                    break;                                    
            }
        }
#endif   

        switch(button_value)
        {
        case BT_UP_PRESS:
            page_num = page_num - SPECIAL_PAGE_OFFSET;
            
            exit_flag = 1;
            
            break;
        case BT_DN_PRESS:
            
            page_num = page_num - SPECIAL_PAGE_OFFSET;
            
            exit_flag = 1;            

            break;

        case BT_LE_PRESS:
            if (gs_u8ImageLineDelayControlFlag == 1)
            {
                g_u8DigitalCameraThreshold--;
                SCCB_WriteByte(OV7725_CNST, g_u8DigitalCameraThreshold);
            }

            break;

        case BT_RI_PRESS:
            if (gs_u8ImageLineDelayControlFlag == 1)
            {
                g_u8DigitalCameraThreshold++;
                SCCB_WriteByte(OV7725_CNST, g_u8DigitalCameraThreshold);
            }
            
            break;
        case BT_OK_PRESS:           
            gs_u8ImageLineDelayControlFlag = !gs_u8ImageLineDelayControlFlag;            
            
            break;
        default:
            break;
        }
    }

    return page_num;
}


/*******************************************************
	函数名: display_exit                                                   
	功  能: 退出页                                                
	参  数: 本页页码                                                                      
	返  回: 下一页页码                                                    
*********************************************************/
uint8 display_exit(uint8 page_num)
{
    int8  exit_flag = 0;
    uint8 button_value = 0;

    
    //显示信息
    oled_fill(0x00);
    oled_display_6x8str(10,2,"Exit display image?");

    while(!exit_flag)
    {
        //读取按键状态
                button_value=button_scan();
        
#ifdef USE_INFR 
        if (button_value == FIVE_DIR_NULL)//无五向按键按下则判断红外
        {
            button_value = get_infrared_command();
            reset_infrared();
            
            switch(button_value)
            {
                case COMMAND_UP:
                    button_value = FIVE_DIR_UP;
                    break;
                case COMMAND_DOWN:
                    button_value = FIVE_DIR_DOWN;
                    break;                    
                case COMMAND_LEFT:
                    button_value = FIVE_DIR_LEFT;
                    break;                    
                case COMMAND_RIGHT:
                    button_value = FIVE_DIR_RIGHT;
                    break;
                case COMMAND_OK:
                    button_value = FIVE_DIR_OK;
                    break;     
                default:
                    button_value = FIVE_DIR_NULL;//识别不出则认为没按下
                    break;                                    
            }
        }
#endif   

        switch(button_value)
        {
        case BT_UP_PRESS:
            break;
        case BT_DN_PRESS:
            break;
            //左键向前翻页
        case BT_LE_PRESS:
            if(page_num > DISPLAY_PAGE_MIN)
            {
                page_num--;
            }
            //如果当前页是最前页，翻至最后页
            else
            {
                page_num = DISPLAY_PAGE_MAX;
            }
            //退出标志位置位
            exit_flag = 1;
            break;
            //右键向后翻页
        case BT_RI_PRESS:
            if(page_num < DISPLAY_PAGE_MAX)
            {
                page_num++;
            }
            //如果当前页是最后一页，则翻至最前页
            else
            {
                page_num = DISPLAY_PAGE_MIN;
            }
            //退出标志位置位
            exit_flag = 1;
            break;
            //确认键按下，退出测试程序
        case BT_OK_PRESS:
            //退出标志位置位
            exit_flag = 1;
            gs_u8ExitDisplayFlag = FLAG_ON;
            break;
        default:
            break;

        }
    }
	
    return page_num;
}

/*******************************************************
	函数名: DisplayImage                                                   
	功  能: 图像显示                                                
	参  数: 无                                                                      
	返  回: 无                                                    
*********************************************************/
void Display_Image()
{
    while(1)
    {
     oled_print_std_image(0);
    }//while
}