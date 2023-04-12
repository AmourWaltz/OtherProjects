/*=============================================
    @file         pit.c
    @brief        周期性定时器
    @programmer   Beyond Hsueh
=============================================*/

#include "pit.h"
#include "isr.h"

/*===============================================================
        @note      展放愁眉，休争闲气，
                   今日容颜，老于昨日，
                   浮生若梦，难逃那日，
                   受用一朝，一朝便宜。
==================================================================*/      

//定义PIT全局结构
#ifdef PIT_0_INIT
	PIT_InitTypeDef struct_PIT_0;
#endif

#ifdef PIT_1_INIT
	PIT_InitTypeDef struct_PIT_1;
#endif

#ifdef PIT_2_INIT
	PIT_InitTypeDef struct_PIT_2;
#endif
  
#ifdef PIT_3_INIT
	PIT_InitTypeDef struct_PIT_3;
#endif

/*===============================================================
	@brief     PIT初始化
==================================================================*/ 
void PIT_Init(void)
{

#ifdef PIT_0_INIT
	//配置PIT0参数
	struct_PIT_0.PIT_Pitx = PIT0;
	struct_PIT_0.PIT_PeriodUs = PIT_0_PERIOD_US;     //定时周期
	struct_PIT_0.PIT_Isr = PIT_0_handler;  //设置中断函数
	//初始化PIT0
	LPLD_PIT_Init(struct_PIT_0);

#endif

#ifdef PIT_1_INIT
	//配置PIT1参数
	struct_PIT_1.PIT_Pitx = PIT1;
	struct_PIT_1.PIT_PeriodMs = PIT_1_PERIOD_MS;     //定时周期
	struct_PIT_1.PIT_Isr = PIT_1_handler;  //设置中断函数
	//初始化PIT1
	LPLD_PIT_Init(struct_PIT_1);

#endif
  
#ifdef PIT_2_INIT
	//配置PIT2参数
	struct_PIT_2.PIT_Pitx = PIT2;
	struct_PIT_2.PIT_PeriodS = PIT_2_PERIOD_MS;     //定时周期
	struct_PIT_2.PIT_Isr = PIT_2_handler;  //设置中断函数
	//初始化PIT2
	LPLD_PIT_Init(struct_PIT_2);

#endif
  
#ifdef PIT_3_INIT
	//配置PIT3参数
	struct_PIT_3.PIT_Pitx = PIT3;
	struct_PIT_3.PIT_PeriodMs = PIT_3_PERIOD_MS;     //定时周期
	struct_PIT_3.PIT_Isr = PIT_3_handler;  //设置中断函数
	//初始化PIT3
	LPLD_PIT_Init(struct_PIT_3);

#endif
	
	//禁用PIT
	PIT_ALL_Disable();
}

/*===============================================================
	@brief     使能一个PIT
==================================================================*/ 
void PIT_ONE_Enable(uint8 pit_num)
{
	switch (pit_num)
	{
		#ifdef PIT_0_INIT
		case PIT_0_INIT:
			LPLD_PIT_EnableIrq(struct_PIT_0); 
			break;
		#endif

		#ifdef PIT_1_INIT
		case PIT_1_INIT:
			LPLD_PIT_EnableIrq(struct_PIT_1); 
			break;
		#endif
 
		#ifdef PIT_2_INIT
		case PIT_2_INIT:
			LPLD_PIT_EnableIrq(struct_PIT_2); 
			break;
		#endif
  
		#ifdef PIT_3_INIT
		case PIT_3_INIT:
			LPLD_PIT_EnableIrq(struct_PIT_3); 
			break;
		#endif
			
		default: break;
	}
}

/*===============================================================
	@brief     禁用一个PIT
==================================================================*/ 
void PIT_ONE_Disable(uint8 pit_num)
{
		switch (pit_num)
	{
		#ifdef PIT_0_INIT
		case PIT_0_INIT:
			LPLD_PIT_DisableIrq(struct_PIT_0); 
			break;
		#endif

		#ifdef PIT_1_INIT
		case PIT_1_INIT:
			LPLD_PIT_DisableIrq(struct_PIT_1); 
			break;
		#endif
 
		#ifdef PIT_2_INIT
		case PIT_2_INIT:
			LPLD_PIT_DisableIrq(struct_PIT_2); 
			break;
		#endif
  
		#ifdef PIT_3_INIT
		case PIT_3_INIT:
			LPLD_PIT_DisableIrq(struct_PIT_3); 
			break;
		#endif
			
		default: break;
	}
}

/*===============================================================
	@brief     使能所有定义的PIT
==================================================================*/ 
void PIT_ALL_Enable(void)
{
#ifdef PIT_0_INIT
	LPLD_PIT_EnableIrq(struct_PIT_0); 
#endif

#ifdef PIT_1_INIT
	LPLD_PIT_EnableIrq(struct_PIT_1); 
#endif
 
#ifdef PIT_2_INIT
	LPLD_PIT_EnableIrq(struct_PIT_2); 
#endif
  
#ifdef PIT_3_INIT
	LPLD_PIT_EnableIrq(struct_PIT_3);
#endif
}

/*===============================================================
	@brief     禁用所有定义的PIT
==================================================================*/ 
void PIT_ALL_Disable(void)
{
#ifdef PIT_0_INIT
	LPLD_PIT_DisableIrq(struct_PIT_0); 
#endif

#ifdef PIT_1_INIT
	LPLD_PIT_DisableIrq(struct_PIT_1); 
#endif
 
#ifdef PIT_2_INIT
	LPLD_PIT_DisableIrq(struct_PIT_2); 
#endif
  
#ifdef PIT_3_INIT
	LPLD_PIT_DisableIrq(struct_PIT_3); 
#endif
}

/*===============================================================
	@brief     PIT0中断
==================================================================*/ 
#ifdef PIT_0_INIT
//100us中断
void PIT_0_handler()
{
    PIT0_Process();
}
#endif

/*===============================================================
	@brief     PIT1中断
==================================================================*/ 
#ifdef PIT_1_INIT
//4ms中断
void PIT_1_handler()
{
    PIT1_Process();
}
#endif

/*===============================================================
	@brief     PIT2中断
==================================================================*/ 
#ifdef PIT_2_INIT
void PIT_2_handler()
{
    PIT2_Process();
}
#endif

/*===============================================================
	@brief     PIT3中断
==================================================================*/ 
#ifdef PIT_3_INIT
void PIT_3_handler()
{
    PIT3_Process();
}
#endif
