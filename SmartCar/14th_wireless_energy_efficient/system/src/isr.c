/*=============================================
    @file         isr.c
    @brief        中断函数文件
    @programmer   Beyond Hsueh
=============================================*/

#include "isr.h"

/*===============================================================
        @note      花间一壶酒，独酌无相亲；
                   举杯邀明月，对影成三人。
                   月既不解饮，影徒随我身；
                   暂伴月将影，行乐须及春。
                   我歌月徘徊，我舞影零乱；
                   醒时相交欢，醉后各分散；
                   永结无情游，相期邈云汉。
==================================================================*/ 

uint8 guc_control_count = 0;//主控中断函数计数
uint8 gc_speed_control_count;//速度输出计数
uint8 gc_direction_control_count;//方向输出计数

void PIT0_Process()
{
    gul_time_100us++;
    gul_time_start++;
}

//4ms控制中断
void PIT1_Process()
{
    //信号处理控制
    Signal_Control();
    
    //车身姿态获取
    Get_Position();
    
    //速度及车程获取
    Get_Car_Distance();

    //角速度闭环：外环计算偏差+内环控制输出
//    Direction_Calculate();
    //避障特殊处理，输入期望角度偏差，其余情况为电感偏差
    if(!guc_obstacle_flag)
    {
//        Angularspeed_Control_Direction(gl_direction_set);
      Direction_Control();
    }
    else 
    {
        Angularspeed_Control_Direction(((int32)gf_obstacle_error));
    }
    
    //速度控制及电机输出
    Speed_Control();
    
    //读取干簧管状态
    guc_reed_state_value = Get_All_Reed_State();
     
    //电路参数计算及低压检测
    Circuit_Signal_Get();
    
    //充电控制
    Charge_Control();
    Constant_Power_Control(gl_power_set);
    Change_PWM_Charge(gl_charge_pwm_set);
    
}

void PIT2_Process()
{

}

void PIT3_Process()
{

}

void SoftIsr_Open(IRQn_Type irqx)
{
  if (irqx < 32)
  {
    NVIC->ISPR[0] |= ((uint32)1 << (irqx % 32));
  }
  else if (irqx < 64)
  {
    NVIC->ISPR[1] |= ((uint32)1 << (irqx % 32));
  }
  else if (irqx < 96)
  {
    NVIC->ISPR[2] |= ((uint32)1 << (irqx % 32));
  }
  else
  {
    NVIC->ISPR[3] |= ((uint32)1 << (irqx % 32));
  }
}

/*
中断函数名称，用于设置对应功能的中断函数
Sample usage:当前启用了周期定时器 通道0得中断
void PIT_CH0_IRQHandler(void)
{
    ;
}
记得进入中断后清除标志位

FTMRE_IRQHandler      
PMC_IRQHandler        
IRQ_IRQHandler        
I2C0_IRQHandler       
I2C1_IRQHandler       
SPI0_IRQHandler       
SPI1_IRQHandler       
UART0_IRQHandler 
UART1_IRQHandler 
UART2_IRQHandler 
ADC0_IRQHandler       
ACMP0_IRQHandler      
FTM0_IRQHandler       
FTM1_IRQHandler       
FTM2_IRQHandler       
RTC_IRQHandler        
ACMP1_IRQHandler      
PIT_CH0_IRQHandler    
PIT_CH1_IRQHandler    
KBI0_IRQHandler       
KBI1_IRQHandler       
Reserved26_IRQHandler 
ICS_IRQHandler        
WDG_IRQHandler        
PWT_IRQHandler        
MSCAN_Rx_IRQHandler   
MSCAN_Tx_IRQHandler   
*/

