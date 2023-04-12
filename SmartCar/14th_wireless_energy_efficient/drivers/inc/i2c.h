#ifndef _I2C_H_
#define _I2C_H_

#include "common.h"
#include "include.h"
#include "HW_SYSTICK.h"

/*===============================================================
        @note      晚日寒鸦一片愁，柳塘新绿却温柔；
                   若教眼底无离恨，不信人间有白头。
                   肠已断，泪难收，相思重上小红楼；
                   情知已被山遮断，频倚阑干不自由。
==================================================================*/

/* 应答位开关定义 */
#define ACK_ON 0
#define ACK_OFF 1

//函数名兼容
#define InitAIIC()              GPIO_I2C_Init()
#define I2C_WriteReg(A,B,C)     GPIO_I2C_Write_1(A,B,C)
#define I2C_ReadByte(A,B)       GPIO_I2C_Read_1(A,B,NULL)


/******************* 以下为设置区域 *************************/	
/* 模拟引脚选择 */ 
#define SDA_PORT                PTD
#define SDA_PIN                 GPIO_Pin15
#define SDA_NUM                 15

#define SCL_PORT                PTD
#define SCL_PIN                 GPIO_Pin14
#define SCL_NUM                 14

#define I2C_DELAY_CNT 4

/* 电平设置 */
#define SET_SDA(x) { \
                    LPLD_GPIO_Output_b(SDA_PORT,SDA_NUM ,(x)); \
					} 
#define SCL_L     { \
                    LPLD_GPIO_Output_b(SCL_PORT,SCL_NUM ,LOW); \
					} 

#define SCL_H     { \
                    LPLD_GPIO_Output_b(SCL_PORT,SCL_NUM ,HIGH);\
					}


#define GET_SDA()  LPLD_GPIO_Input_b(SDA_PORT, SDA_NUM)



#define GET_SCL()  LPLD_GPIO_Input_b(SCL_PORT, SCL_NUM)



void GPIO_I2C_Init();//初始化模拟IIC

static void GPIO_I2C_Start();//开始信号

static void GPIO_I2C_Restart();//重新开始信号

static void GPIO_I2C_Stop();//停止信号

static void GPIO_I2C_Write_Register(uint8 u8Data);//模拟写寄存器

static uint8 GPIO_I2C_Read_Register();//模拟读寄存器

static void GPIO_I2C_Response(uint8 u8AckState);//回复应答位信号

static uint8 GPIO_I2C_Wait_Ack();//等待应答信号

void GPIO_I2C_Write_1(uint8 DeviceAddr, uint8 AccessAddr, uint8 Data);//向总线设备发出1字节

uint8 GPIO_I2C_Read_1(uint8 DeviceAddr, uint8 AccessAddr, uint8 *pData);//从总线设备读取1字节

void GPIO_I2C_Write_2(uint8 DeviceAddr, uint8 AccessAddr, uint16 Data);//向总线设备发出2字节

void GPIO_I2C_Read_2(uint8 DeviceAddr, uint8 AccessAddr, uint16 *pData);//从总线设备读取2字节

void GPIO_I2C_Read_N(uint8 DeviceAddr, uint8 AccessAddr, uint8 *pData, uint16 lenth);//从总线设备读取N字节

void GPIO_I2C_Delay(uint32 u32DelayCnt);//延时



#endif
