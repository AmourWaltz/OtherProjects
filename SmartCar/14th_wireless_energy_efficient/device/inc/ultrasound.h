//#ifndef _KS103_DRVER_H_
//#define _KS103_DRVER_H_
//
//#include "common.h"
//#include "include.h"
//
//
//#define ULTRASOUND_UART_ID              UART3
//#define ULTRASOUND_UART_TX              PTB11
//#define ULTRASOUND_UART_RX              PTB10
//#define ULTRASOUND_UART_BUND            9600
//
//#define ULTRASOUND_PORT                 0xe8//KS103的串口地址
//#define ULTRASOUND_REG2                 0x02//寄存器2地址
//
//#define KS103Time1200                   0x0c      //量程1200mm，返时间
//#define KS103Time1000                   0x0a      //量程1000mm，返时间
//#define KS103Time900                    0x09      //量程900mm，返时间
//
//
////超声波KS103信息结构体
//typedef struct
//{
//  uint8 Send_flag :1;         //显示已经发送查询信息的标志
//  uint8 Rev_flag :1;          //显示已经接收的标志
//  uint8 u8_ErrorCounter :4;   //显示发送查询信息但尚未接收的计数信息（不要溢出！）
//  uint8 RevSeq_flag :1;       //表示接收数据时是接收高8位还是低8位
//  uint8 GetData_flag :1;      //标识是否得到可用数据
//                              //接收到新数据置1，处理完该次数据后用户置0
//}KS103Infor;
//
//extern uint16 gl_u16_KS103Time;
//extern uint16 gl_u16_KS103Dis;
//extern KS103Infor gl_KS103GetData_flag;
//
//void init_KS103();//初始化KS103
//void AskKS103Dis();//查询KS103距离信息
//void RevKS103Dis();//接受KS103距离信息（在串口中断中调用）
//void ResetKS103Infor(KS103Infor *infor);         //重置信息结构体
//
//#endif
