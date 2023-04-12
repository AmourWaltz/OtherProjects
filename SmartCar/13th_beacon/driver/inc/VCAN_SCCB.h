/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_SCCB.h
 * @brief      OV摄像头配置总线SCCB函数库
 * @author     山外科技
 * @version    v5.0
 * @date       2013-09-01
 */


#ifndef _VCAN_SCCB_H_
#define _VCAN_SCCB_H_

//SCCB 管脚配置

   
#define SCCB_SCL        PTA
#define SCCB_SCL_PIN    (GPIO_Pin10)
#define SCCB_SCL_NUM    (10)

#define SCCB_SDA        PTA
#define SCCB_SDA_PIN    (GPIO_Pin11)
#define SCCB_SDA_NUM    (11)
   
//#define SCCB_SCL        PTA
//#define SCCB_SCL_PIN    (GPIO_Pin12)
//#define SCCB_SCL_NUM    (12)
//
//#define SCCB_SDA        PTA
//#define SCCB_SDA_PIN    (GPIO_Pin13)
//#define SCCB_SDA_NUM    (13)

#define SCCB_SCL_L()         (SCCB_SCL->PDOR &= ~(0x01L<<SCCB_SCL_NUM))          //PTXn_T(SCCB_SCL,OUT) = 1
#define SCCB_SCL_H()         (SCCB_SCL->PDOR |=  (0x01L<<SCCB_SCL_NUM))          //PTXn_T(SCCB_SCL,OUT) = 0
#define SCL_DDR_OUT()        (SCCB_SCL->PDDR |= SCCB_SCL_PIN)                    //PTXn_T(SCCB_SCL,DDR) = 1
#define SCL_DDR_IN()         (SCCB_SCL->PDDR &= ~(SCCB_SCL_PIN))                      //PTXn_T(SCCB_SCL,DDR) = 0

#define SDA_L()              (SCCB_SDA->PDOR &= ~(0x01L<<SCCB_SDA_NUM))          //PTXn_T(SCCB_SDA,OUT) = 1
#define SDA_H()         (SCCB_SDA->PDOR |=  (0x01L<<SCCB_SDA_NUM))          //PTXn_T(SCCB_SDA,OUT) = 0
#define SDA_IN()        ((uint8)((SCCB_SDA->PDIR>>SCCB_SDA_NUM)&0x01L))                //PTXn_T(SCCB_SDA,IN)
#define SDA_DDR_OUT()   (SCCB_SDA->PDDR |= SCCB_SDA_PIN)                   //PTXn_T(SCCB_SDA,DDR) = 1
#define SDA_DDR_IN()    (SCCB_SDA->PDDR &= ~(SCCB_SDA_PIN))                //PTXn_T(SCCB_SDA,DDR) = 0

#define ADDR_OV7725   0x42
#define ADDR_OV7620   0x42

#define DEV_ADR  ADDR_OV7725             /*设备地址定义*/

#define SCCB_DELAY()    SCCB_delay(1000)


extern void SCCB_GPIO_init(void);
extern int SCCB_WriteByte( uint16 WriteAddress , uint8 SendByte);
extern int SCCB_ReadByte(uint8 *pBuffer,   uint16 length,   uint8 ReadAddress);


#endif      //_VCAN_SCCB_H_
