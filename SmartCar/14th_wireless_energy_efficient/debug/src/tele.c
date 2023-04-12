/*=============================================
    @file         tele.c
    @brief        红外文件
    @programmer   Beyond Hsueh
=============================================*/

#include "tele.h"

/*===============================================================
        @note      芦叶满汀洲，寒沙带浅流。二十年重过南楼。
                   柳下系船犹未稳，能几日，又中秋。
　　               黄鹤断矶头，故人曾到否？旧江山浑是新愁。
                   欲买桂花同载酒，终不似，少年游。
==================================================================*/

/* 红外初始化结构体 */
GPIO_InitTypeDef g_tele_InitTypeDef;

/*===============================================================
	@brief      红外初始化
==================================================================*/
void Tele_Init(void)
{
	//红外接收引脚配置
	g_tele_InitTypeDef.GPIO_PTx = TELE_PORT_RECEIVE_ID;
	g_tele_InitTypeDef.GPIO_Pins = TELE_PORT_RECEIVE_PIN;
	g_tele_InitTypeDef.GPIO_Dir = DIR_INPUT;
	g_tele_InitTypeDef.GPIO_PinControl = INPUT_PULL_UP | IRQC_ET;//上拉电阻，双边沿触发
	g_tele_InitTypeDef.GPIO_Isr = TELE_HANDLER;	//端口中断函数
	LPLD_GPIO_Init(g_tele_InitTypeDef);	//初始化
}

/*===============================================================
	@brief      红外对应中断处理函数
==================================================================*/
void Infrared_handler(void)
{
	if(LPLD_GPIO_IsPinxExt(TELE_PORT_RECEIVE_TYPE, TELE_PORT_RECEIVE_PIN))
	{
		infrared_decode();//红外解码
	}
}

/*===============================================================
	@brief      使能红外端口中断
==================================================================*/
void Tele_Enable(void)
{
	LPLD_GPIO_EnableIrq(g_tele_InitTypeDef);//使能端口中断
}

/*===============================================================
	@brief      禁用红外端口中断
==================================================================*/
void Tele_Disable(void)
{
	LPLD_GPIO_DisableIrq(g_tele_InitTypeDef);//禁用端口中断
}

/*===============================================================
	@brief      获取红外电平
==================================================================*/
uint8 Get_Infrared_Value()
{
    return LPLD_GPIO_Input_b(TELE_PORT_RECEIVE_ID, TELE_PORT_RECEIVE_NUM);
}