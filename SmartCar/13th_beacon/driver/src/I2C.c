//#include "I2C.h"
//#include "include.h"
///**************************************************
//统一标准，除初始化和终止之外进入每个函数时
//SCL均应处于低电平，函数结束时也将SCL拉低 
//***************************************************/
//
//static GPIO_InitTypeDef gpio_SDA_init_struct, gpio_SCL_init_struct;
//
///******************* 以下为设置区域 *************************/	
///* 模拟引脚选择 */ 
//#define SDA_PORT PTE
//#define SDA_PIN  GPIO_Pin0
//#define SDA_NUM  0
//
//#define SCL_PORT PTE
//#define SCL_PIN  GPIO_Pin1
//#define SCL_NUM  1
//
//#define I2C_DELAY_CNT 4
//
///* 电平设置 */
//#define SDA_SET(x) { \
//                    LPLD_GPIO_Output_b(SDA_PORT,SDA_NUM ,(x)); \
//				} 
//#define SDAHI           SDA_SET(1)
//#define SDALO           SDA_SET(0)
//#define SCLLO     { \
//                    LPLD_GPIO_Output_b(SCL_PORT,SCL_NUM ,LOW); \
//					} 
//
//#define SCLHI     { \
//                    LPLD_GPIO_Output_b(SCL_PORT,SCL_NUM ,HIGH);\
//					}
//
//
//#define SDA1_IN  {\
//                    gpio_SDA_init_struct.GPIO_Dir = DIR_INPUT;\
//                      LPLD_GPIO_Init(gpio_SDA_init_struct);\
//                    }
//#define SDA1_OUT {\
//                    gpio_SDA_init_struct.GPIO_Dir = DIR_OUTPUT;\
//                      LPLD_GPIO_Init(gpio_SDA_init_struct);\
//                    }
//
//#define GET_SDA()  LPLD_GPIO_Input_b(SDA_PORT, SDA_NUM)
//
//
//#define GET_SCL()  LPLD_GPIO_Input_b(SCL_PORT, SCL_NUM)
//
////---------------------------------------------------------------------*
////函数名: I2C_init *
////功 能: 初始化I2C *
////参 数: 无 *
////返 回: 无 *
////---------------------------------------------------------------------*
//void I2C_init()
//{
//    gpio_SDA_init_struct.GPIO_PTx = SDA_PORT;
//    gpio_SDA_init_struct.GPIO_Pins = SDA_PIN;                  //注意：初始化时得用GPIO_Pinx，而LPLD_GPIO_Output_b中直接用x
//    gpio_SDA_init_struct.GPIO_Dir = DIR_OUTPUT;
//    gpio_SDA_init_struct.GPIO_Output = OUTPUT_H;
//    gpio_SDA_init_struct.GPIO_PinControl = INPUT_PULL_UP;      //上拉
//    LPLD_GPIO_Init(gpio_SDA_init_struct);    
//    
//    gpio_SCL_init_struct.GPIO_PTx = SCL_PORT;
//    gpio_SCL_init_struct.GPIO_Pins = SCL_PIN;                  //注意：初始化时得用GPIO_Pinx，而LPLD_GPIO_Output_b中直接用x
//    gpio_SCL_init_struct.GPIO_Dir = DIR_OUTPUT;
//    gpio_SCL_init_struct.GPIO_Output = OUTPUT_H;
//    gpio_SCL_init_struct.GPIO_PinControl = INPUT_PULL_UP;      //上拉
//    LPLD_GPIO_Init(gpio_SCL_init_struct);      
//}
//
////---------------------------------------------------------------------*
////函数名: I2C_start *
////功 能: 产生开始信号 *
////参 数: 无 *
////返 回: 无 *
////---------------------------------------------------------------------*
//void I2C_start()
//{ 
//    SDA1_OUT;
//    SDAHI;
//    SCLHI;
//    SDALO;
//}
//
//
////---------------------------------------------------------------------*
////函数名: I2C_stop *
////功 能: 产生停止信号 *
////参 数: 无 *
////返 回: 无 *
////---------------------------------------------------------------------*
//void I2C_stop()
//{
//    SDA1_OUT;
//    SDALO;
//    SCLHI;
//    SDAHI;
//}
//
////---------------------------------------------------------------------*
////函数名: I2C_Ack *
////功 能: 接受从机应答信号 *
////参 数: 无 *
////返 回: 无 *
////---------------------------------------------------------------------*
//void I2C_Ack()
//{
//    uint8 i=0;
//    SDA1_IN;    
//    SCLHI;
//    while((GET_SDA()) && (i < 10))
//    {
//        i++;
//    }
//    SCLLO;
//}
//
////---------------------------------------------------------------------*
////函数名: I2C_m_Ack *
////功 能: 产生主机应答信号 *
////参 数: 无 *
////返 回: 无 *
////---------------------------------------------------------------------*
//void I2C_m_Ack()
//{    
//    SCLLO;
//    SDA1_OUT;
//    SDALO;
//    SCLHI;
//    SCLLO;
//}
//
////---------------------------------------------------------------------*
////函数名: I2C_m_nAck *
////功 能: 产生主机不应答信号 *
////参 数: 无 *
////返 回: 无 *
////---------------------------------------------------------------------*
//void I2C_m_nAck()
//{
//    SCLLO;
//    SDA1_OUT;
//    SDAHI;
//    SCLHI;
//    SCLLO;
//}
//
////---------------------------------------------------------------------*
////函数名: I2C_sendbyte *
////功 能: 像I2C总线写一字节数据 *
////参 数: 数据 *
////返 回: 无 *
////---------------------------------------------------------------------*
//void I2C_sendbyte(uint8 data)
//{
//    uint8 i;
//    SCLLO;
//    SDA1_OUT;
//    for(i=0; i<8; i++)
//    {
//        /***发送数据***/
//        SDA_SET(data&0x80);
//        SCLHI;
//        data <<= 1;
//        SCLLO;
//    }
//    SDAHI;
//}
//
////---------------------------------------------------------------------*
////函数名: I2C_readbyte *
////功 能: 从I2C总线读一字节数据 *
////参 数: 无 *
////返 回: 1byte数据 * 
////---------------------------------------------------------------------*
//uint8 I2C_readbyte()
//{
//    uint8 i=0, data=0;
//    SDA1_IN;
//    for(i=0, data=0; i<8; i++)
//    {
//        /***接受数据***/
//        SCLHI;
//        data <<= 1;
//        data |= GET_SDA();
//        SCLLO;
//    }
//    return data;
//}
//
////---------------------------------------------------------------------*
////函数名:  delay *
////功 能:  短暂延时*
////参 数:  无*
////返 回: 无 *
////---------------------------------------------------------------------*
//void delay()
//{
//    int16 i;
//    for(i = 5; i>0; i--);
//}
//
//
//
///*******************************************************
//	函数名: I2C_write_1                                                   
//	功  能: 向I2C总线设备写一个字节                                                
//	参  数: addr : 设备地址 
//            cmd  : 指令 
//            data : 数据                                                  
//	返  回: 无                                                          
//*********************************************************/
//void I2C_write_1(uint8 addr,uint8 cmd, uint8 data)
//{
//    I2C_start();
//    I2C_sendbyte(addr);
//    I2C_Ack();
//    I2C_sendbyte(cmd);
//    I2C_Ack();
//    I2C_sendbyte(data);
//    I2C_Ack();
//    I2C_stop();
// } 
///*******************************************************
//	函数名: I2C_write_N                                                   
//	功  能: 向I2C总线设备写n个字节                                               
//	参  数: addr : 设备地址 
//            cmd  : 指令 
//            n     : 传输字节数 
//            *data : 数据块位置                                                                          
//	返  回: 无                                                          
//*********************************************************/
//void I2C_write_N(uint8 addr,uint8 cmd, uint8 n, uint8 *data)
//{ 
//    int16 i;
//    I2C_start();
//    I2C_sendbyte(addr);
//    I2C_Ack();
//    I2C_sendbyte(cmd);
//    I2C_Ack();
//    for(i = 0; i < n ; i++)
//    {        
//        I2C_sendbyte(data[i]);
//        I2C_Ack();
//    }
//    I2C_stop();
//}
///*******************************************************
//	函数名: I2C_read_N                                                   
//	功  能: 向I2C总线设备读n个字节                                             
//	参  数: addr : 设备地址 
//            cmd  : 指令 
//            n     : 传输字节数 
//            *data : 数据块位置                                                                 
//	返  回: 无                                                                 
//*********************************************************/
//void I2C_read_N(uint8 addr,uint8 cmd, uint8 n, uint8 *data)
//{
//    int16 i;
//    I2C_start();
//    I2C_sendbyte(addr&0xfe);
//    I2C_Ack();
//    I2C_sendbyte(cmd);
//    I2C_Ack();
//    
//    I2C_start();
//    I2C_sendbyte(addr|0x01);
//    I2C_Ack();
//    for(i = 0; i < n-1 ; i++)
//    {        
//        data[i] = I2C_readbyte();
//        I2C_m_Ack();
//    }
//    data[i] = I2C_readbyte();
//    I2C_m_nAck();
//    I2C_stop();
//}
