/*=============================================
    @file         car_test.c
    @brief        小车各个硬件模块测试头文件 
                  舵机、电机、拨码、LED、编码器
    @programmer   XBY
=============================================*/

//使用前会关闭1ms PIT中断 不关红外

#include "car_test.h"

static uint8 suc_exit_test_flag = FLAG_OFF;

/*===============================================================
	@brief      拨码测试
	@function   将拨码值显示在oled上   
	@parameter  拨码测试所属的页数
	@return     根据按键左右返回需要进入的下一页页数
==================================================================*/
uint8 switch_test(uint8 page_num)
{
    int8  exit_flag = 0;
    uint8 button_value = 0;

    uint8 SwitchPinValTemp;

    //显示信息
    oled_fill(0x00);
    oled_display_6x8str(20,0,"switch test");

    while(!exit_flag)
    {
        //读取拨码器
        Switch_Get_Value(&SwitchPinValTemp, SW0_0);//第0位
        guc_switch_pin_val[0] = SwitchPinValTemp;
        Switch_Get_Value(&SwitchPinValTemp, SW0_1);//第1位
        guc_switch_pin_val[1] = SwitchPinValTemp;	
        
        //显示拨码器		
        oled_display_boolean(30, 4, guc_switch_pin_val[0]);
        oled_display_boolean(50, 4, guc_switch_pin_val[1]);

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
            break;
        case BT_DN_PRESS:
            break;
            //左键按下向前翻页
        case BT_LE_PRESS:
        {
            if(page_num > TEST_PAGE_MIN)
            {
                page_num--;
            }
            else//当前页是最前一页，左键按下则翻至最后一页
            {
                page_num = TEST_PAGE_MAX;
            }
            //退出标志位置位
            exit_flag = 1;
        }
        break;
        //右键按下向后翻页
        case BT_RI_PRESS:
        {
            if(page_num < TEST_PAGE_MAX)
            {
                page_num++;
            }
            //当前页是最后一页，则翻至最前一页
            else
            {
                page_num = TEST_PAGE_MIN;
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

/*===============================================================
	@brief      led灯测试
	@function   根据拨码值亮灯   
	@parameter  led测试所属的页数
	@return     根据按键左右返回需要进入的下一页页数
==================================================================*/

uint8 led_test(uint8 page_num)
{
    uint8 switch_value = 0;
    uint8 button_value = 0;
    uint8 exit_flag = 0;

    //显示信息
    oled_fill(0x00);
    oled_display_6x8str(30,0,"led test");
    oled_display_6x8str(0,3,"set switch to test led");

    while(!exit_flag)
    {
        //根据拨码值更新led灯的状态
        switch_value = Switch_Get_Value(NULL, SW_ALL);
	switch_value &= 0x1f;
        SetLED(switch_value);	

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
            break;
        case BT_DN_PRESS:
            break;
            //左键按下向前翻页
        case BT_LE_PRESS:
            if(page_num > TEST_PAGE_MIN)
            {
                page_num--;
            }
            //当前页是最前一页，左键按下则翻至最后一页
            else
            {
                page_num = TEST_PAGE_MAX;
            }
            //退出标志位置位
            exit_flag = 1;
            break;
            //右键按下向后翻页
        case BT_RI_PRESS:
            if(page_num < TEST_PAGE_MAX)
            {
                page_num++;
            }
            //如果当前页是最后一页，翻至最前一页
            else
            {
                page_num = TEST_PAGE_MIN;
            }
            //退出标志位置位
            exit_flag = 1;
            break;
        case BT_OK_PRESS:
            break;

        default:
            break;

        }
    }

    return page_num;
}

/*===============================================================
	@brief      红外测试
	@function   显示红外解码值   
	@parameter  红外测试所属的页数
	@return     根据按键左右返回需要进入的下一页页数
==================================================================*/

uint8 infrared_test(uint8 page_num)// 此处红外不能翻页
{
    int8  exit_flag = 0;
    uint8 button_value = 0;

    //显示信息
    oled_fill(0x00);
    oled_display_6x8str(20,0,"infrared test");
    oled_print_short(0,2,(int16)guc_car_run_flag);

    while(!exit_flag)
    {
        oled_print_short(0*OLED_CHAR_WIDTH, 1, get_infrared_command() );

        //读取按键值
        //FiveDirButton(&button_value);
        button_value=button_scan();

        switch(button_value)
        {
        case BT_UP_PRESS:
            break;
        case BT_DN_PRESS:
            break;
            //左键按下向前翻页
        case BT_LE_PRESS:
        {
            if(page_num > TEST_PAGE_MIN)
            {
                page_num--;
            }
            else//当前页是最前一页，左键按下则翻至最后一页
            {
                page_num = TEST_PAGE_MAX;
            }
            //退出标志位置位
            exit_flag = 1;
        }
        break;
        //右键按下向后翻页
        case BT_RI_PRESS:
        {
            if(page_num < TEST_PAGE_MAX)
            {
                page_num++;
            }
            //当前页是最后一页，则翻至最前一页
            else
            {
                page_num = TEST_PAGE_MIN;
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

/*===============================================================
	@brief      舵机测试函数，用于舵机的测试
	@function   按下确认键则测试程序   
	@parameter  退出测试所属的页数
	@return     根据按键左右返回需要进入的下一页页数
==================================================================*/

uint8 servo_test(uint8 page_num)
{
        int32 sevor_value = gl_servo_center;
        
	uint8 button_value = 0;
        int8  exit_flag = 0;

        
	//显示信息
	oled_fill(0x00);
        
	oled_display_16x8str(30,0,"servo test");
        
	while(exit_flag == FLAG_OFF)
	{
          
            //读取按键值
            button_value = button_scan();
      
            //更新舵机更改之后的输出值
            oled_print_16x8short(80,2, sevor_value);
            
            //舵机输出
            Servo_PWM(sevor_value);
              
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
            sevor_value += 1;
            break;
        case BT_DN_PRESS:
            sevor_value -= 1;
            break;
            //左键按下向前翻页
        case BT_LE_PRESS:
            if(page_num > TEST_PAGE_MIN)
            {
                page_num--;
            }
            //当前页是最前一页，左键按下则翻至最后一页
            else
            {
                page_num = TEST_PAGE_MAX;
            }
            //退出标志位置位
            exit_flag = 1;
            break;
            //右键按下向后翻页
        case BT_RI_PRESS:
            if(page_num < TEST_PAGE_MAX)
            {
                page_num++;
            }
            //如果当前页是最后一页，翻至最前一页
            else
            {
                page_num = TEST_PAGE_MIN;
            }
            //退出标志位置位
            exit_flag = 1;
            break;
        case BT_OK_PRESS:
            break;

        default:
            break;

        }
    }

    return page_num;
}

/*===============================================================
	@brief      编码器测试函数
	@function   按下确认键则测试程序   
	@parameter  退出测试所属的页数
	@return     根据按键左右返回需要进入的下一页页数
==================================================================*/

uint8 encoder_test(uint8 page_num)
{        
	uint8 button_value = 0;
        int8  exit_flag = 0;

	//显示信息
	oled_fill(0x00);
        
	oled_display_16x8str(30,0,"encoder test");
        
	while(exit_flag == FLAG_OFF)
	{
          
            gi_left_speed = (int16)(Get_Left_Speed());
            gi_right_speed = (int16)(Get_Right_Speed());
	//读取按键值
            button_value = button_scan();
            oled_display_16x8str(0,2,"left");
            oled_display_16x8str(0,4,"right");

            oled_print_16x8short(80,2, gi_left_speed);
            oled_print_16x8short(80,4, gi_right_speed);               

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
            //左键按下向前翻页
        case BT_LE_PRESS:
            if(page_num > TEST_PAGE_MIN)
            {
                page_num--;
            }
            //当前页是最前一页，左键按下则翻至最后一页
            else
            {
                page_num = TEST_PAGE_MAX;
            }
            //退出标志位置位
            exit_flag = 1;
            break;
            //右键按下向后翻页
        case BT_RI_PRESS:
            if(page_num < TEST_PAGE_MAX)
            {
                page_num++;
            }
            //如果当前页是最后一页，翻至最前一页
            else
            {
                page_num = TEST_PAGE_MIN;
            }
            //退出标志位置位
            exit_flag = 1;
            break;
        case BT_OK_PRESS:
            break;

        default:
            break;

        }
    }

    return page_num;
}

/*===============================================================
	@brief      退出测试函数
	@function   按下确认键则测试程序   
	@parameter  退出测试所属的页数
	@return     根据按键左右返回需要进入的下一页页数
==================================================================*/

uint8 test_exit(uint8 page_num)
{
    int8  exit_flag = 0;
    uint8 button_value = 0;

    //显示信息
    oled_fill(0x00);
    oled_display_6x8str(10,2,"Exit car test ?");

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
            if(page_num > TEST_PAGE_MIN)
            {
                page_num--;
            }
            //如果当前页是最前页，翻至最后页
            else
            {
                page_num = TEST_PAGE_MAX;
            }
            //退出标志位置位
            exit_flag = 1;
            break;
            //右键向后翻页
        case BT_RI_PRESS:
            if(page_num < TEST_PAGE_MAX)
            {
                page_num++;
            }
            //如果当前页是最后一页，则翻至最前页
            else
            {
                page_num = TEST_PAGE_MIN;
            }
            //退出标志位置位
            exit_flag = 1;
            break;
            //确认键按下，退出测试程序
        case BT_OK_PRESS:
            //退出标志位置位
            exit_flag = 1;
            suc_exit_test_flag = FLAG_ON;
            break;
        default:
            break;

        }
    }
    
    oled_fill(0x00);
    
    return page_num;
}

/*===============================================================
	@brief      小车硬件测试函数
	@function   led 拨码 液晶 红外   
	@parameter  退出测试所属的页数
	@note       在函数被调用前pit必须初始化，因为测速是在PIT里进行的，否则将读不到脉冲数和速度
==================================================================*/

void Smartcar_Test()
{
    uint8 page_num = TEST_PAGE_MIN;

    PIT_ONE_Disable(PIT_1_INIT);
    suc_exit_test_flag = FLAG_OFF;
    while(suc_exit_test_flag != FLAG_ON)
    {
        //根据页数进入相应的测试
        switch(page_num)
        {
        case 1:
            page_num = switch_test(1);
            break;
        case 2:
            page_num = led_test(2);
            break;
        case 3:
            page_num = infrared_test(3);
            break; 
        case 4:
            page_num = servo_test(4);
            break;
        case 5:
            page_num = encoder_test(5);
            break;            
        case 6:
            page_num = test_exit(TEST_PAGE_MAX);
            break;
        default:
            break;
        }
    }
    suc_exit_test_flag = FLAG_OFF;

    //开PIT
    PIT_ONE_Enable(PIT_1_INIT);
}