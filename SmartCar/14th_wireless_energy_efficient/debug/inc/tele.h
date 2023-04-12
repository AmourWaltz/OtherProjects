#ifndef _TELE_H_
#define _TELE_H_

#include "common.h"
#include "include.h"
#include "infrared_decode.h"

/*===============================================================
        @note      槛菊愁烟兰泣露，罗幕轻寒，燕子双飞去；
                   明月不谙离别苦，斜光到晓穿朱户。
                   昨夜西风凋碧树，独上高楼，望尽天涯路；
                   欲寄彩笺兼尺素，山长水阔知何处。
==================================================================*/

//红外接收引脚定义
#define TELE_PORT_RECEIVE_ID	        PTE
#define TELE_PORT_RECEIVE_TYPE	        PORTE
#define TELE_PORT_RECEIVE_PIN	        GPIO_Pin0
#define TELE_PORT_RECEIVE_NUM	        (0)

#define TELE_HANDLER		        Infrared_handler
#define TELE_HANDLER_IRQn	        PORTE_IRQn


void Tele_Init(void);//红外初始化

void Infrared_handler(void);//红外对应中断函数

void Tele_Enable(void);//使能红外端口中断

void Tele_Disable(void);//禁用红外端口中断

uint8 Get_Infrared_Value();//获取红外电平



#endif