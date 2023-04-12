#include "pit.h"
#include "interrupt.h"

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

  
/*=================================*
 *		函数名：PIT_Init(void)
 *		PIT初始化
 *=================================*/
void PIT_Init(void)
{
	static NVIC_InitTypeDef NVIC_typeDefTemp;


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
	struct_PIT_2.PIT_PeriodS = PIT_2_PERIOD_S;     //定时周期
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

/*===============================================*
 *		函数名：PIT_ONE_Enable(uint8 pit_num)
 *		使能一个PIT
 *===============================================*/
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


/*===============================================*
 *		函数名：PIT_ONE_Disable(uint8 pit_num)
 *		禁用一个PIT
 *===============================================*/
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

/*====================================*
 *		函数名：PIT_ALL_Enable(void)
 *		使能所有定义的PIT
 *====================================*/
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


/*====================================*
 *		函数名：PIT_ALL_Disable(void)
 *		禁用所有定义的PIT
 *====================================*/
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


//PIT0中断
#ifdef PIT_0_INIT
//100us中断
void PIT_0_handler()
{
    PIT_Process_100us();
}
#endif


//PIT1中断
#ifdef PIT_1_INIT
//1ms中断
void PIT_1_handler()
{
    PIT_Process_5ms();
}
#endif


//PIT2中断
#ifdef PIT_2_INIT
void PIT_2_handler()
{
    PIT_Process_5s();
}
#endif


//PIT3中断
#ifdef PIT_3_INIT
void PIT_3_handler()
{
    PIT_Process_10us();
}
#endif
