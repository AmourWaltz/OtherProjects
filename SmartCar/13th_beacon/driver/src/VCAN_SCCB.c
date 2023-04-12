/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_SCCB.c
 * @brief      OV摄像头配置总线SCCB函数库
 * @author     山外科技
 * @version    v5.0
 * @date       2013-09-01
 * @porting    2014-11-14 由野火库移植到LPLP库下（kongyu）
 */

#include "include.h"

static GPIO_InitTypeDef gpio_struct_sccb;
static void SCCB_delay(uint16 i);

/*!
 *  @brief      SCCB延迟函数
 *  @param      time    延时时间
 *  @since      v5.0
 */
static void SCCB_delay(volatile uint16 time)
{
    while(time)
    {
        time--;
    }
}

/*!
 *  @brief      SCCB管脚配置
 *  @since      v5.0
 */
void SCCB_GPIO_init(void)
{
    gpio_struct_sccb.GPIO_PTx = SCCB_SCL;
    gpio_struct_sccb.GPIO_Pins = SCCB_SCL_PIN;
    gpio_struct_sccb.GPIO_Dir = DIR_OUTPUT;
    gpio_struct_sccb.GPIO_Output = OUTPUT_H;
    gpio_struct_sccb.GPIO_PinControl = IRQC_DIS|OUTPUT_OD_EN|INPUT_PULL_UP;
    LPLD_GPIO_Init(gpio_struct_sccb);

	gpio_struct_sccb.GPIO_PTx = SCCB_SDA;
    gpio_struct_sccb.GPIO_Pins = SCCB_SDA_PIN;
	LPLD_GPIO_Init(gpio_struct_sccb);
}

/*!
 *  @brief      SCCB起始信号
 *  @since      v5.0
 */
static uint8 SCCB_Start(void)
{
    SDA_H();
    SCCB_SCL_H();
    SCCB_DELAY();

	//SDA_L();
    //SCL_L();

    SDA_DDR_IN();
    if(!SDA_IN())
    {
        SDA_DDR_OUT();
        return 0;   /* SDA线为低电平则总线忙,退出 */
    }
    SDA_DDR_OUT();
    SDA_L();

    SCCB_DELAY();
    SCCB_SCL_L();

    if(SDA_IN())
    {
        SDA_DDR_OUT();
        return 0;   /* SDA线为高电平则总线出错,退出 */
    }
    //SDA_DDR_OUT();
    //SDA_L();
    //SCCB_delay();
    return 1;
}

/*!
 *  @brief      SCCB停止信号
 *  @since      v5.0
 */
static void SCCB_Stop(void)
{
    SCCB_SCL_L();
    //SCCB_DELAY();
    SDA_L();
    SCCB_DELAY();
    SCCB_SCL_H();
    SCCB_DELAY();
    SDA_H();
    SCCB_DELAY();
}

/*!
 *  @brief      SCCB应答信号
 *  @since      v5.0
 */
static void SCCB_Ack(void)
{
    SCCB_SCL_L();
    SCCB_DELAY();
    SDA_L();
    SCCB_DELAY();
    SCCB_SCL_H();
    SCCB_DELAY();
    SCCB_SCL_L();
    SCCB_DELAY();
}

/*!
 *  @brief      SCCB无应答信号
 *  @since      v5.0
 */
static void SCCB_NoAck(void)
{
    SCCB_SCL_L();
    SCCB_DELAY();
    SDA_H();
    SCCB_DELAY();
    SCCB_SCL_H();
    SCCB_DELAY();
    SCCB_SCL_L();
    SCCB_DELAY();
}

/*!
 *  @brief      SCCB 等待应答
 *  @return     应答结果（0表示无应答，1表示有应答）
 *  @since      v5.0
 */
static int SCCB_WaitAck(void)
{
    SCCB_SCL_L();
    //SDA_H();
    SDA_DDR_IN();

    SCCB_DELAY();
    SCCB_SCL_H();

    SCCB_DELAY();

    if(SDA_IN())           //应答为高电平，异常，通信失败
    {
        SDA_DDR_OUT();
        SCCB_SCL_L();
        return 0;
    }
    SDA_DDR_OUT();
    SCCB_SCL_L();
    return 1;
}

/*!
 *  @brief      SCCB 发送的数据
 *  @param      SendByte    需要发送的数据
 *  @since      v5.0
 */
static void SCCB_SendByte(uint8 SendByte)
{
    uint8 i = 8;
    while(i--)
    {

        if(SendByte & 0x80)     //SDA 输出数据
        {
            SDA_H();
        }
        else
        {
            SDA_L();
        }
        SendByte <<= 1;
        SCCB_DELAY();
        SCCB_SCL_H();                //SCL 拉高，采集信号
        SCCB_DELAY();
        SCCB_SCL_L();                //SCL 时钟线拉低
        //SCCB_DELAY();
    }
    //SCL_L();
}

/*!
 *  @brief      接收SCCB总线的数据
 *  @return     接收到的数据
 *  @since      v5.0
 */
static int SCCB_ReceiveByte(void)
{
    uint8 i = 8;
    uint8 ReceiveByte = 0;

    //SDA_H();
    //SCCB_DELAY();
    SDA_DDR_IN();

    while(i--)
    {
        ReceiveByte <<= 1;
        SCCB_SCL_L();
        SCCB_DELAY();
        SCCB_SCL_H();
        SCCB_DELAY();

        if(SDA_IN())
        {
            ReceiveByte |= 0x01;
        }


    }
    SDA_DDR_OUT();
    SCCB_SCL_L();
    return ReceiveByte;
}

/*****************************************************************************************
* 函数名：SCCB_WriteByte
* 描述  ：写一字节数据
* 输入  ：- WriteAddress: 待写入地址    - SendByte: 待写入数据  - DeviceAddress: 器件类型
* 输出  ：返回为:=1成功写入,=0失败
* 注意  ：无
*****************************************************************************************/
static int SCCB_WriteByte_one( uint16 WriteAddress , uint8 SendByte );


int SCCB_WriteByte( uint16 WriteAddress , uint8 SendByte )            //考虑到用sccb的管脚模拟，比较容易失败，因此多试几次
{
    uint8 i = 0;
    while( 0 == SCCB_WriteByte_one ( WriteAddress, SendByte ) )
    {
        i++;
        if(i == 20)
        {
            return 0 ;
        }
    }
    return 1;
}

int SCCB_WriteByte_one( uint16 WriteAddress , uint8 SendByte )
{
    if(!SCCB_Start())
    {
        return 0;
    }
    SCCB_SendByte( DEV_ADR );                    /* 器件地址 */
    if( !SCCB_WaitAck() )
    {
        SCCB_Stop();
        return 0;
    }
    SCCB_SendByte((uint8)(WriteAddress & 0x00FF));   /* 设置低起始地址 */
    SCCB_WaitAck();
    SCCB_SendByte(SendByte);
    SCCB_WaitAck();
    SCCB_Stop();
    return 1;
}




/******************************************************************************************************************
 * 函数名：SCCB_ReadByte
 * 描述  ：读取一串数据
 * 输入  ：- pBuffer: 存放读出数据  - length: 待读出长度    - ReadAddress: 待读出地址        - DeviceAddress: 器件类型
 * 输出  ：返回为:=1成功读入,=0失败
 * 注意  ：无
 **********************************************************************************************************************/
static int SCCB_ReadByte_one(uint8 *pBuffer,   uint16 length,   uint8 ReadAddress);

int SCCB_ReadByte(uint8 *pBuffer,   uint16 length,   uint8 ReadAddress)
{
    uint8 i = 0;
    while( 0 == SCCB_ReadByte_one(pBuffer, length, ReadAddress) )
    {
        i++;
        if(i == 30)
        {
            return 0 ;
        }
    }
    return 1;
}

int SCCB_ReadByte_one(uint8 *pBuffer,   uint16 length,   uint8 ReadAddress)
{
    if(!SCCB_Start())
    {
        return 0;
    }
    SCCB_SendByte( DEV_ADR );         /* 器件地址 */
    if( !SCCB_WaitAck() )
    {
        SCCB_Stop();
        return 0;
    }
    SCCB_SendByte( ReadAddress );           /* 设置低起始地址 */
    SCCB_WaitAck();
    SCCB_Stop();

    if(!SCCB_Start())
    {
        return 0;
    }
    SCCB_SendByte( DEV_ADR + 1 );               /* 器件地址 */

    if(!SCCB_WaitAck())
    {
        SCCB_Stop();
        return 0;
    }
    while(length)
    {
        *pBuffer = SCCB_ReceiveByte();
        if(length == 1)
        {
            SCCB_NoAck();
        }
        else
        {
            SCCB_Ack();
        }
        pBuffer++;
        length--;
    }
    SCCB_Stop();
    return 1;
}




