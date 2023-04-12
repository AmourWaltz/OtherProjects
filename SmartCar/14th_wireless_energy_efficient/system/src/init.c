/*=============================================
    @file         init.c
    @brief        初始化文件
    @programmer   Beyond Hsueh
=============================================*/

#include "init.h"

/*===============================================================
        @note      白酒新熟山中归，黄鸡啄黍秋正肥；
                   呼童烹鸡酌白酒，儿女嬉笑牵人衣；
                   高歌取醉欲自慰，起舞落日争光辉；
                   游说万乘苦不早，著鞭跨马涉远道；
                   会稽愚妇轻买臣，余亦辞家西入秦；
                   仰天大笑出门去，我辈岂是蓬蒿人。
==================================================================*/ 

/*===============================================================
        @brief     所有文件初始化
==================================================================*/
void Init_All()
{      
    DisableInterrupts;

//    SIM->SOPT0 &= ~SIM_SOPT0_NMIE_MASK; //屏蔽NMI引脚中断功能
//    
//    PMC->SPMSC1 |= PMC_SPMSC1_BGBE_MASK;//使能带隙基准源

    //电机初始化
    Motor_Init();

    //充电PWM初始化
    PWM_Charge_Init();    
    
    //Flash下位机参数初始化
    Flash_Init();  
    Flash_Read_Data();
    InitFlashParameter();
    
    //OLED初始化
    oled_init();
    oled_fill(0x00);
    
    //五向按键
    Switch_Init();
    
    //蜂鸣器
    Beep_Init();
    
    //拨码按键检测
    Five_Dir_Init();
    
    //干簧管初始化
    Reed_Init();
    
    //编码器初始化
    GPT_Init();
    
    //定时器
    PIT_Init();

    //红外
    Tele_Init();
    
    //蓝牙
    UART_Init();
    
    //模数转换
    ADC_Init();
    
    //电感通道初始化
    Inductor_Init();
    
    //红外测距初始化
    Infrared_AD_Init();
    
    //充电通道初始化
    Circuit_AD_Init();
    
    //中断  
    NVIC_Init();  

    //红外中断使能
    Tele_Enable();   
    
    //定时器中断使能
    PIT_ONE_Enable(PIT_0);    
    PIT_ONE_Enable(PIT_1);
//    PIT_ONE_Enable(PIT_2);
//    PIT_ONE_Enable(PIT_3);

    //模拟I2C初始化
    GPIO_I2C_Init();
    
    //加速度计陀螺仪初始化
    MPU6050_Init();

    oled_fill(0x00);
     
    //开总中断
    EnableInterrupts;
    
    
}

/*===============================================================
        @brief     模式选择
==================================================================*/
uint8 Select_Mode()
{
	uint8 uc_mode = 3;
	uint8 uc_exit_flag = FLAG_OFF;
        uint16 ui_button_value = 0;
        uint16 ui_infrared_value = 0;
	static uint8 uc_head_num = 1;//既指示当前行又指示要显示的汉字串

	//显示可选模式
	oled_display_chinese(0, 0, SET_PARAM, LOW_LIGHT_MODE);//调参
	oled_display_chinese(0, 2, HARDWARE_TEST, LOW_LIGHT_MODE);//测试
	oled_display_chinese(0, 4, WIRELESS_CHARGE, LOW_LIGHT_MODE);//充电
	oled_display_chinese(0, 6, CAR_RUNNING, LOW_LIGHT_MODE);//起飞
	oled_display_chinese(0, (uint8)(2*(uc_head_num-1)), uc_head_num+1, HIGH_LIGHT_MODE);

	while (uc_exit_flag == FLAG_OFF)//选择是否退出当前页
	{
            ui_button_value = button_scan();
       
            ui_infrared_value = get_infrared_command();
            reset_infrared();
            
            switch(ui_infrared_value)
            {
                case COMMAND_UP:
                    ui_button_value = FIVE_DIR_UP;
                    break;
                case COMMAND_DOWN:
                    ui_button_value = FIVE_DIR_DOWN;
                    break;                    
                case COMMAND_LEFT:
                    ui_button_value = FIVE_DIR_LEFT;
                    break;                    
                case COMMAND_RIGHT:
                    ui_button_value = FIVE_DIR_RIGHT;
                    break;
                case COMMAND_OK:
                    ui_button_value = FIVE_DIR_OK;
                    break;     
                default:
                    break;                                    
            }
            
            switch (ui_button_value)
            {
                case FIVE_DIR_UP:
                    //上次选中菜单去掉反白显示
                    oled_display_chinese(0, (uint8)(2*(uc_head_num-1)), uc_head_num+1, LOW_LIGHT_MODE);
                    if(uc_head_num > 1)
                    {
                            uc_head_num--;			//如果不是第一行则向上移动
                    }
                    else
                    {
                            uc_head_num = 4;		//如果是第一行则移动到最后一行
                    }
                    //本次选中菜单添加反白显示
                    oled_display_chinese(0, (uint8)(2*(uc_head_num-1)), uc_head_num+1, HIGH_LIGHT_MODE);
                    break;

                case FIVE_DIR_DOWN:
                    //上次选中菜单去掉反白显示
                    oled_display_chinese(0, (uint8)(2*(uc_head_num-1)), uc_head_num+1, LOW_LIGHT_MODE);
                    if(uc_head_num < 4)
                    {
                            uc_head_num++;			//如果不是最后一行则向下移动
                    }
                    else
                    {
                            uc_head_num = 1;		//如果是最后一行则移动到第一行
                    }
                    //本次选中菜单添加反白显示
                    oled_display_chinese(0, (uint8)(2*(uc_head_num-1)), uc_head_num+1, HIGH_LIGHT_MODE);
                    break;

                case FIVE_DIR_LEFT:
                    break;

                case FIVE_DIR_RIGHT:
                    break;

                case FIVE_DIR_OK:
                    //选定模式，退出当前页面
                    uc_mode = uc_head_num + 1;
                    uc_exit_flag = FLAG_ON;
                    break;

                default:
                    break;
                }
        }

	return uc_mode;
}