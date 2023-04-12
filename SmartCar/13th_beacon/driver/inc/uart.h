#ifndef _UART_H_
#define _UART_H_

#include "common.h"

//定义UART引脚
#define UART_ID                 (UART0)
#define UART_RXD                (PTB16)
#define UART_TXD                (PTB17)

//波特率设置
#define UART_BAUDRATE           (115200)


/*==============================*
 *		函数名：UART_Init()
 *		UART初始化
 *==============================*/
void UART_Init();


/*==========================================*
 *		函数名：UART_Sendchar(uint8 data)
 *		UART发送一个字符型数据
 *==========================================*/
void UART_Sendchar(uint8 data);


/*=============================================*
 *		函数名：UART_Sendstring(int8 *pstr)
 *		UART发送一个字符串数据
 *=============================================*/
void UART_Sendstring(int8 *pstr);


/*====================================*
 *		函数名：UART_Receivechar()
 *		UART接受一个字符型数据
 *====================================*/
int8 UART_Receivechar();



#endif