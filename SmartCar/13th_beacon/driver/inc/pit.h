#ifndef _PIT_H_
#define _PIT_H_

#include "common.h"

#define PIT_0_INIT (0)
#define PIT_1_INIT (1)
#define PIT_2_INIT (2)
#define PIT_3_INIT (3)


#define PIT_0 (0)
#define PIT_1 (1)
#define PIT_2 (2)
#define PIT_3 (3)


#define PIT_0_PERIOD_US (100)
#define PIT_1_PERIOD_MS (5)
#define PIT_2_PERIOD_S  (5)
#define PIT_3_PERIOD_MS (5)


/*=================================*
 *		函数名：PIT_Init(void)
 *		PIT初始化
 *=================================*/
void PIT_Init(void);


/*=====================================*
 *		函数名：PIT_0_handler(void)
 *		PIT0处理函数
 *=====================================*/
#ifdef PIT_0_INIT
void PIT_0_handler(void);
#endif


/*=====================================*
 *		函数名：PIT_1_handler(void)
 *		PIT1处理函数
 *=====================================*/
#ifdef PIT_1_INIT
void PIT_1_handler(void);               //直立控制
#endif

/*=====================================*
 *		函数名：PIT_2_handler(void)
 *		PIT2处理函数
 *=====================================*/
#ifdef PIT_2_INIT
void PIT_2_handler(void);
#endif


/*=====================================*
 *		函数名：PIT_3_handler(void)
 *		PIT3处理函数
 *=====================================*/
#ifdef PIT_3_INIT
void PIT_3_handler(void);
#endif


/*===============================================*
 *		函数名：PIT_ONE_Enable(uint8 pit_num)
 *		使能一个PIT
 *===============================================*/
void PIT_ONE_Enable(uint8 pit_num);


/*===============================================*
 *		函数名：PIT_ONE_Disable(uint8 pit_num)
 *		禁用一个PIT
 *===============================================*/
void PIT_ONE_Disable(uint8 pit_num);


/*====================================*
 *		函数名：PIT_ALL_Enable(void)
 *		使能所有定义的PIT
 *====================================*/
void PIT_ALL_Enable(void);


/*====================================*
 *		函数名：PIT_ALL_Disable(void)
 *		禁用所有定义的PIT
 *====================================*/
void PIT_ALL_Disable(void);


#endif