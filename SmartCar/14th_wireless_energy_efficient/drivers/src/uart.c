/*=============================================
    @file         uart.c
    @brief        串口
    @programmer   Beyond Hsueh
=============================================*/

#include "uart.h"

/*===============================================================
        @note      沐春风，惹一身红尘，
                   望秋月，化半缕轻烟；
                   顾盼间乾坤倒转，一霎时沧海桑田；
                   方晓，弹指红颜老，刹那芳华逝。
==================================================================*/    

/*===============================================================
	@brief     UART初始化
==================================================================*/ 
void UART_Init()
{
	UART_InitTypeDef struct_uart;
	
	struct_uart.UART_Uartx = UART_ID;
	struct_uart.UART_BaudRate = UART_BAUDRATE; //设置波特率
//	struct_uart.UART_RxIntEnable = TURE;
//	struct_uart.UART_RxIsr = UART_HANDLER;
	struct_uart.UART_RxPin = UART_RXD;
	struct_uart.UART_TxPin = UART_TXD;
	//初始化UART
	LPLD_UART_Init(struct_uart);

//	LPLD_UART_EnableIrq(struct_uart);
}

/*===============================================================
	@brief     UART发送一个字符型数据
==================================================================*/ 
void UART_Sendchar(uint8 data)
{
	LPLD_UART_PutChar(UART_ID, data);
}

/*===============================================================
	@brief     UART发送一个字符串数据
==================================================================*/ 
void UART_Sendstring(int8 *pstr)
{
	int32 length = 0;
	
	for(; pstr[length] != '\0';)
	{
		length++;
	}
	length++;

	LPLD_UART_PutCharArr(UART_ID,pstr,length);
}

/*===============================================================
	@brief     UART接受一个字符型数据
==================================================================*/ 
int8 UART_Receivechar()
{
	return LPLD_UART_GetChar(UART_ID);
}