///*=============================================
//    @file         measure.c
//    @brief        超声波测距
//    @programmer   XBY
//=============================================*/
//
//#include "measure.h"
//
//
//GPIO_InitTypeDef ultra_gpio_struct;
//
//static uint8 suc_ultra_dat[3];
//static uint8 suc_ultra_count;
//
///*===============================================================
//	@brief     初始化超声波测距串口
//==================================================================*/
//
//void Ultra_Measure_Init()
//{
//    UART_InitTypeDef struct_uart;
//    
//    struct_uart.UART_Uartx = ULTRA_UART_ID;
//    struct_uart.UART_BaudRate = UART_BAUDRATE; //设置波特率
//    struct_uart.UART_RxIntEnable = TURE;
//    struct_uart.UART_RxIsr = Ultra_Measure_Handler;
//    struct_uart.UART_RxPin = ULTRA_UART_RXD;
//    struct_uart.UART_TxPin = ULTRA_UART_TXD;
//    //初始化UART
//    LPLD_UART_Init(struct_uart);
//
//    LPLD_UART_EnableIrq(struct_uart);
//}
//
///*===============================================================
//	@brief     接收串口传来的数据
//	@function  得出距信标灯距离
//        @note      可外部调用
//==================================================================*/
//
//void Ultra_Measure_Handler()
//{
//    suc_ultra_dat[suc_ultra_count] = UART_Receivechar();
//    if(suc_ultra_dat[0] != 0xff)
//        suc_ultra_count = 0;	//检查头帧是否正确，不正确就重新接收
//    else suc_ultra_count++;
//    
//    if(suc_ultra_count==3)//接收完成，开始处理数据
//    {
//        suc_ultra_count = 0;
//        gi_ultrasonic_distance = suc_ultra_dat[1]<<8 | suc_ultra_dat[2];
//    }
//
//    if((suc_ultra_dat[1] == 0xfe) && (suc_ultra_dat[2] == 0xfe))
//      gi_ultrasonic_distance = NO_RANGE;
//}
