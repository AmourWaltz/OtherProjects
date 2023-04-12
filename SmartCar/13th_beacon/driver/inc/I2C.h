//#include "common.h"
//
//#ifndef _I2C_H_
//#define _I2C_H_
//
//#define SCLPIN 1
//#define SDAPIN 0
//
//
//void I2C_init();
//void I2C_start();
//void I2C_stop();
//void I2C_Ack();
//void I2C_m_Ack();
//void I2C_m_nAck();
//void I2C_sendbyte(uint8 data);
//uint8 I2C_readbyte();
////void delay();
//
///*******************************************************
//	函数名: I2C_write_1                                                   
//	功  能: 向I2C总线设备写一个字节                                                
//	参  数: addr : 设备地址 
//            cmd  : 指令 
//            data : 数据                                                  
//	返  回: 无                                                          
//*********************************************************/
//void I2C_write_1(uint8 addr,uint8 cmd, uint8 data);
///*******************************************************
//	函数名: I2C_write_N                                                   
//	功  能: 向I2C总线设备写n个字节                                               
//	参  数: addr : 设备地址 
//            cmd  : 指令 
//            n     : 传输字节数 
//            *data : 数据块位置                                                                          
//	返  回: 无                                                          
//*********************************************************/
//void I2C_write_N(uint8 addr,uint8 cmd, uint8 n, uint8 *data);
///*******************************************************
//	函数名: I2C_read_N                                                   
//	功  能: 向I2C总线设备读n个字节                                             
//	参  数: addr : 设备地址 
//            cmd  : 指令 
//            n     : 传输字节数 
//            *data : 数据块位置                                                                 
//	返  回: 无                                                                 
//*********************************************************/
//void I2C_read_N(uint8 addr,uint8 cmd, uint8 n, uint8 *data);
//
//#endif
