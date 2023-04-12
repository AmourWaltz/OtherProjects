#include "include.h"

void Init_All()
{      
    DisableInterrupts;
     
    Servo_Init();
    Motor_Init();
    //Flash
    FLASH_Init();  
    FlashReadData();
    InitFlashParameter();

    //OLED初始化
    oled_init();  

    //摄像头初始化    
    SCCB_GPIO_init();    
    ov7725_eagle_init();
    Camera_Init();
    
    gc_not_found_count = 4;//连续检测不到的次数，远距离时为5，近距离为4，初始设为5

    //五向按键，蜂鸣器，LED灯，拨码按键检测
    Switch_Init();
    Beep_Init();
    LED_Init();
    Five_Dir_Init();
    Infrared_Init();

    //中断
    PIT_Init();
    
    //ADC
    ADC_Init();
    
    oled_fill(0x00); //去掉后液晶就不能正常显示了，不知为何
    
    //GPT
    GPT_Init();
    
    //红外
    TELE_AD_Init();
    
    //超声波定向
//    ADC_Ultrasonic_Init();

    //UART
    UART_Init();
    
//    //RTC
//    //RTC_Init();
    
    //中断优先级    
    NVIC_Init();  

    PIT_ONE_Enable(PIT_0);    
    
//    PIT_ONE_Enable(PIT_2);
//    PIT_ONE_Enable(PIT_3);

    //使能场中断
    enable_irq(V_PORT_IRQn);
    enable_irq(H_PORT_IRQn);
    enable_irq(Reserved110_IRQn);
        
    EnableInterrupts;

    //需要延时函数   
    
    Delay_ms(500);//需要pit所以最后才延时
    oled_fill(0x00);
}

/*===============================================================
	@brief     等待发车
	@function  检测到图像面积后启动发车
	@note      也可以改为检测到红外信号或超声波信号后启动
==================================================================*/

void Wait_Beacon()
{
//  while(1)
//  {
//    if(gi_image_area == 0)
//    {
//      //Beep_On();
//      guc_car_run_flag = CAR_STOP;      
//    }
//    else
//    {
//      //Beep_Off();
//      guc_car_run_flag = CAR_RUN; 
//      PIT_ONE_Enable(PIT_1);
//      break;
//    }
//  }

    while(1)
    {
        if((!Infrared_Start()) && (!gi_image_center))
        {
            Beep_On();
            guc_car_run_flag = CAR_STOP;      
        }
        else
        {
            Beep_Off();
            guc_car_run_flag = CAR_RUN;
            gul_time_start = gul_time_100us;
            break;
        }
    }
}
