/*=============================================
    @file         nvic.c
    @brief        中断控制
    @programmer   Beyond Hsueh
=============================================*/

#include "nvic.h"

/*===============================================================
        @note      水是眼波横，山是眉峰聚，
                   欲问行人去那边？眉眼盈盈处；
                   才始送春归，又送君归去，
                   若到江南赶上春，千万和春住。
==================================================================*/

static NVIC_InitTypeDef nvic_init_struct;

/*===============================================================
	@brief     初始化中断控制
        @note      因为初始化时所有中断均初始化为不使能中断，故应该在初始化最前初始化硬件，NVIC初始化，再使能其他中断，再使能全局中断
==================================================================*/
void NVIC_Init (void)
{
//  if(PREEMPTION ==1)
  //配置PIT0的NVIC分组
  nvic_init_struct.NVIC_IRQChannel = PIT0_IRQn;
  nvic_init_struct.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_3;
  nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 2;       //系统时钟，次高优先级
  nvic_init_struct.NVIC_IRQChannelSubPriority = 0x00;
  LPLD_NVIC_Init(nvic_init_struct);
  //配置PIT1的NVIC分组
  nvic_init_struct.NVIC_IRQChannel = PIT1_IRQn;
  nvic_init_struct.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_3;
  nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 5;
  nvic_init_struct.NVIC_IRQChannelSubPriority = 0x00;
  LPLD_NVIC_Init(nvic_init_struct);
  //配置PIT2的NVIC分组
  nvic_init_struct.NVIC_IRQChannel = PIT2_IRQn;
  nvic_init_struct.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_3;
  nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 4;
  nvic_init_struct.NVIC_IRQChannelSubPriority = 0x00;
  LPLD_NVIC_Init(nvic_init_struct);
  //配置PIT3的NVIC分组
//  nvic_init_struct.NVIC_IRQChannel = PIT3_IRQn;
//  nvic_init_struct.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_3;
//  nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 7;
//  nvic_init_struct.NVIC_IRQChannelSubPriority = 0x01;
//  LPLD_NVIC_Init(nvic_init_struct);
  
//  //配置UART_0的NVIC分组
//  nvic_init_struct.NVIC_IRQChannel = ULTRA_HANDLER_IRQn;
//  nvic_init_struct.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_3;
//  nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 3;//0;
//  nvic_init_struct.NVIC_IRQChannelSubPriority = 0x00;
//  //nvic_init_struct.NVIC_IRQChannelEnable = TRUE;
//  LPLD_NVIC_Init(nvic_init_struct);
  
  //配置TELE的NVIC分组
  nvic_init_struct.NVIC_IRQChannel = TELE_HANDLER_IRQn;
  nvic_init_struct.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_3;
  nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 3;
  nvic_init_struct.NVIC_IRQChannelSubPriority = 0x01;
  //nvic_init_struct.NVIC_IRQChannelEnable = TRUE;
  LPLD_NVIC_Init(nvic_init_struct);
  
//  //配置H_PORT的NVIC分组
//  nvic_init_struct.NVIC_IRQChannel = H_PORT_IRQn;
//  nvic_init_struct.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_3;
//  nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 1;
//  nvic_init_struct.NVIC_IRQChannelSubPriority = 0x00;
//  //nvic_init_struct.NVIC_IRQChannelEnable = TRUE;
//  LPLD_NVIC_Init(nvic_init_struct);
//  
//  //配置V_PORT的NVIC分组
//  nvic_init_struct.NVIC_IRQChannel = V_PORT_IRQn;
//  nvic_init_struct.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_3;
//  nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 0;       //最高优先级
//  nvic_init_struct.NVIC_IRQChannelSubPriority = 0x00;
//  //nvic_init_struct.NVIC_IRQChannelEnable = TRUE;
//  LPLD_NVIC_Init(nvic_init_struct);
   
//  //配置ISR_PROC_PORT的NVIC分组
//  nvic_init_struct.NVIC_IRQChannel = ISR_PROC_PORT_IRQn;
//  nvic_init_struct.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_3;
//  nvic_init_struct.NVIC_IRQChannelPreemptionPriority =6;
//  nvic_init_struct.NVIC_IRQChannelSubPriority = 0x00;
//  //nvic_init_struct.NVIC_IRQChannelEnable = TRUE;
//  LPLD_NVIC_Init(nvic_init_struct);  
  
//  //配置ADC0的NVIC分组
//  nvic_init_struct.NVIC_IRQChannel = ADC0_IRQn;
//  nvic_init_struct.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_3;
//  nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 6;
//  nvic_init_struct.NVIC_IRQChannelSubPriority = 0x00;
//  nvic_init_struct.NVIC_IRQChannelEnable = TRUE;
//  LPLD_NVIC_Init(nvic_init_struct);  

//  //配置ADC1的NVIC分组
//  nvic_init_struct.NVIC_IRQChannel = ADC1_IRQn;
//  nvic_init_struct.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_3;
//  nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 4;
//  nvic_init_struct.NVIC_IRQChannelSubPriority = 0x00;
//  nvic_init_struct.NVIC_IRQChannelEnable = TRUE;
//  LPLD_NVIC_Init(nvic_init_struct);    
  
//  //配置软件触发的NVIC分组
//  nvic_init_struct.NVIC_IRQChannel = Reserved110_IRQn;
//  nvic_init_struct.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_3;
//  nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 15;
//  nvic_init_struct.NVIC_IRQChannelSubPriority = 0x00;
//  nvic_init_struct.NVIC_IRQChannelEnable = TRUE;
//  LPLD_NVIC_Init(nvic_init_struct);
}
  
  