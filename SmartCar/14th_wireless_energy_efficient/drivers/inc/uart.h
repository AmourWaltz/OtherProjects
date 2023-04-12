#ifndef _UART_H_
#define _UART_H_

#include "common.h"
#include "include.h"

/*===============================================================
        @note      对潇潇暮雨洒江天，一番洗清秋。
                   渐霜风凄紧，关河冷落，残照当楼，
                   是处红衰翠减，苒苒物华休。
                   惟有长江水，无语东流。
                   不忍登高临远，望故乡渺邈，归思难收。
                   叹年来踪迹，何事苦淹留？
                   想佳人、妆楼颙望，误几回、天际识归舟？
                   争知我、倚阑干处，正恁凝愁？
==================================================================*/

//定义UART引脚
#define UART_ID                 (UART2)
#define UART_RXD                (PTD2)
#define UART_TXD                (PTD3)

//波特率设置
#define UART_BAUDRATE           (115200)


void UART_Init();//UART初始化

void UART_Sendchar(uint8 data);//UART发送一个字符型数据

void UART_Sendstring(int8 *pstr);//UART发送一个字符串数据

int8 UART_Receivechar();//UART接受一个字符型数据



#endif