//#include "tele.h"
//#include "include.h"
//
///* 红外初始化结构体 */
//GPIO_InitTypeDef g_tele_InitTypeDef;
//
///*=================================*
// *		函数名：TELE_Init(void)
// *		红外初始化
// *=================================*/
//void TELE_Init(void)
//{
//	//红外接收引脚配置
//	g_tele_InitTypeDef.GPIO_PTx = TELE_PORT_RECEIVE_ID;
//	g_tele_InitTypeDef.GPIO_Pins = TELE_PORT_RECEIVE_PIN;
//	g_tele_InitTypeDef.GPIO_Dir = DIR_INPUT;
//	g_tele_InitTypeDef.GPIO_PinControl = INPUT_PULL_UP | IRQC_ET;//上拉电阻，双边沿触发
//	g_tele_InitTypeDef.GPIO_Isr = TELE_HANDLER;	//端口中断函数
//	LPLD_GPIO_Init(g_tele_InitTypeDef);	//初始化
//}
//
///*===================================*
// *		函数名：PTA_handler(void)
// *		红外对应中断处理函数
// *===================================*/
//void Infrared_handler(void)
//{
//	if(LPLD_GPIO_IsPinxExt(TELE_PORT_RECEIVE_TYPE, TELE_PORT_RECEIVE_PIN))
//	{
//		infrared_decode();//红外解码
//                //guc_car_run_flag = ON;
//	}
//}
//
///*===================================*
// *		函数名：TELE_Enable(void)
// *		使能红外端口中断
// *===================================*/
//void TELE_Enable(void)
//{
//	LPLD_GPIO_EnableIrq(g_tele_InitTypeDef);//使能端口中断
//}
//
//
///*===================================*
// *		函数名：TELE_Disable(void)
// *		禁用红外端口中断
// *===================================*/
//void TELE_Disable(void)
//{
//	LPLD_GPIO_DisableIrq(g_tele_InitTypeDef);//禁用端口中断
//}
//
//uint8 GetInfraredValue()
//{
//    return LPLD_GPIO_Input_b(TELE_PORT_RECEIVE_ID, TELE_PORT_RECEIVE_NUM);
//}