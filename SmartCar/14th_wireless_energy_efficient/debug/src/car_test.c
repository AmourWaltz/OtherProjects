/*=============================================
    @file         car_test.c
    @brief        小车各个硬件模块测试头文件，下位机调试界面 
                  舵机、电机、拨码、LED、编码器
                  使用前会关闭1ms PIT中断 不关红外
    @programmer   Beyond Hsueh
=============================================*/

#include "car_test.h"

/*===============================================================
        @note      李白前时原有月，惟有李白诗能说。
                   李白如今已仙去，月在青天几圆缺?
                   今人犹歌李白诗，明月还如李白时。
                   我学李白对明月，白与明月安能知!
                   李白能诗复能酒，我今百杯复千首；
                   我愧虽无李白才，料应月不嫌我丑。
                   我也不登天子船，我也不上长安眠；
                   姑苏城外一茅屋，万树桃花月满天。
==================================================================*/

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
    uint16 infrared_value = 0;
    
    //显示信息
    oled_fill(0x00);

    while(!exit_flag)
    {
        Switch_Test();

        //读取按键值
	button_value=button_scan();
	
        infrared_value = get_infrared_command();
        reset_infrared();
        
        switch(infrared_value)
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
                break;                                    
        }

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
    uint8 button_value = 0;
    uint8 exit_flag = 0;
    uint16 infrared_value = 0;

    //显示信息
    oled_fill(0x00);

    while(!exit_flag)
    {
         LED_Test();

        //读取按键值
        button_value=button_scan();
        
        infrared_value = get_infrared_command();
        reset_infrared();
        
        switch(infrared_value)
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
                break;                                    
        }

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

    while(!exit_flag)
    {
        oled_print_short(2, 1, get_infrared_command() );

        //读取按键值
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
	@brief      编码器测试函数
	@function   按下确认键则测试程序   
	@parameter  退出测试所属的页数
	@return     根据按键左右返回需要进入的下一页页数
==================================================================*/
uint8 encoder_test(uint8 page_num)
{        
	uint8 button_value = 0;
        int8 exit_flag = 0;
        int16 infrared_value = 0;
        static uint8 suc_count_flag;
        int16 pwm_set = 2000;
        
        static int32 sl_time_count;
        static int32 sl_time_count_last;
	//显示信息
	oled_fill(0x00);
        
	oled_display_16x8str(60,0,"encoder");
        
        //PIT_ONE_Disable(PIT_1);
	while(exit_flag == FLAG_OFF)
	{
        //读取按键值
        button_value = button_scan();
                
        Encoder_Test();

        infrared_value = get_infrared_command();
        reset_infrared();
        
        switch(infrared_value)
        {
            case COMMAND_UP:
                button_value = FIVE_DIR_UP;
                pwm_set += 200;
                break;
            case COMMAND_DOWN:
                button_value = FIVE_DIR_DOWN;
                pwm_set -= 200;
                break;                    
            case COMMAND_LEFT:
                button_value = FIVE_DIR_LEFT;
                break;                    
            case COMMAND_RIGHT:
                button_value = FIVE_DIR_RIGHT;
                break;
            case COMMAND_OK:
                button_value = FIVE_DIR_OK;
                suc_count_flag = ON;
                sl_time_count_last = gul_time_100us;
                break;
            case COMMAND_POWER:
                suc_count_flag = OFF;
                break;   
            default:
                break;                                    
        }
        
        oled_print_short(0,0,suc_count_flag);
        oled_print_short(60,6,sl_time_count/10000);
        oled_print_short(60,7,pwm_set);

        
        if(suc_count_flag)
        {
            guc_car_run_flag = CAR_RUN;
            sl_time_count = gul_time_100us - sl_time_count_last;
        }
        else
        {
            guc_car_run_flag = CAR_STOP;
        }
        
        gl_pwm_output = pwm_set;
        Motor_Output();
        
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
    //PIT_ONE_Enable(PIT_1);
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
    int16 infrared_value = 0;
    
    //显示信息
    oled_fill(0x00);
    oled_display_6x8str(10,2,"Exit car test ?");

    while(!exit_flag)
    {
        //读取按键状态
        button_value=button_scan();


        infrared_value = get_infrared_command();
        reset_infrared();
        
        switch(infrared_value)
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
                break;                                    
        }

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

    suc_exit_test_flag = FLAG_OFF;
    
    //DisableInterrupts;
    while(suc_exit_test_flag != FLAG_ON)
    {
        //根据页数进入相应的测试
        switch(page_num)
        {
        case 1:
            page_num = switch_test(1);
            break;
        case 2:
            page_num = encoder_test(2);
            break;            
        case 3:
            page_num = test_exit(TEST_PAGE_MAX);
            break;
        default:
            break;
        }
    }
    suc_exit_test_flag = FLAG_OFF;
    
    //EnableInterrupts;//测试结束开总中断

}