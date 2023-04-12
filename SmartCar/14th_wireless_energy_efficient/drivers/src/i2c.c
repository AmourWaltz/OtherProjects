/*=============================================
    @file         i2c.c
    @brief        模拟IIC
    @programmer   Beyond Hsueh
=============================================*/

#include "i2c.h"

/*===============================================================
        @note      北国风光，千里冰封，万里雪飘。
                   望长城内外,惟余莽莽；
                   大河上下，顿失滔滔。
                   山舞银蛇，原驰蜡象，欲与天公试比高。
                   须晴日，看红装素裹，分外妖娆。
                   江山如此多娇，引无数英雄竞折腰。
                   惜秦皇汉武，略输文采；
                   唐宗宋祖，稍逊风骚。
                   一代天骄，成吉思汗，只识弯弓射大雕。
                   俱往矣，数风流人物，还看今朝。
==================================================================*/

/*===============================================================
	@brief     初始化模拟IIC
        @note      统一标准，除初始化和终止之外进入每个函数时 SCL均应处于低电平，函数结束时也将SCL拉低
==================================================================*/
void GPIO_I2C_Init()
{	
    static GPIO_InitTypeDef gpio_SDA_init_struct, gpio_SCL_init_struct;
    
	//GPIO_Init(SDA_PORT, SDA_PIN, DIR_OUTPUT,OUTPUT_H);       //SDA
    gpio_SDA_init_struct.GPIO_PTx = SDA_PORT;
    gpio_SDA_init_struct.GPIO_Pins = SDA_PIN;                  //注意：初始化时得用GPIO_Pinx，而LPLD_GPIO_Output_b中直接用x
    gpio_SDA_init_struct.GPIO_Dir = DIR_OUTPUT;
    gpio_SDA_init_struct.GPIO_Output = OUTPUT_H;
    gpio_SDA_init_struct.GPIO_PinControl = INPUT_PULL_UP;      //上拉
    LPLD_GPIO_Init(gpio_SDA_init_struct);    
        
	//GPIO_Init(SCL_PORT, SCL_PIN, DIR_OUTPUT,OUTPUT_H);       //SCL
    gpio_SCL_init_struct.GPIO_PTx = SCL_PORT;
    gpio_SCL_init_struct.GPIO_Pins = SCL_PIN;                  //注意：初始化时得用GPIO_Pinx，而LPLD_GPIO_Output_b中直接用x
    gpio_SCL_init_struct.GPIO_Dir = DIR_OUTPUT;
    gpio_SCL_init_struct.GPIO_Output = OUTPUT_H;
    gpio_SCL_init_struct.GPIO_PinControl = INPUT_PULL_UP;      //上拉
    LPLD_GPIO_Init(gpio_SCL_init_struct);      
    
}

/*===============================================================
	@brief     开始信号
==================================================================*/
static void GPIO_I2C_Start()
{
	SET_SDA(HIGH);
	SCL_H;
	SET_SDA(LOW);	
}

/*===============================================================
	@brief     重新开始信号
==================================================================*/
static void GPIO_I2C_Restart()
{
	SET_SDA(HIGH);
	SCL_H;
	SET_SDA(LOW);	
}

/*===============================================================
	@brief     停止信号
==================================================================*/
static void GPIO_I2C_Stop()
{
	SET_SDA(LOW);
	SCL_H;
	SET_SDA(HIGH);	
}

/*===============================================================
	@brief     模拟写寄存器
==================================================================*/
static void GPIO_I2C_Write_Register(uint8 u8Data)
{
	uint8 u8BitTmp;
	uint8 u8Loopi;
	
	for (u8Loopi=0; u8Loopi<8; u8Loopi++)
	{
		u8BitTmp = (uint8)((u8Data >> (7-u8Loopi)) & 0x01); 
                SCL_L;
                SET_SDA(u8BitTmp);
		SCL_H;
	}

	SCL_L;  //第8字节时钟下降沿
	SET_SDA(HIGH); //释放总线以让从机可以应答
}

/*===============================================================
	@brief     模拟读寄存器
==================================================================*/
static uint8 GPIO_I2C_Read_Register()
{
	uint8 u8Data = 0;
	uint8 u8BitTmp;
	uint8 u8Loopi;
	
	SCL_L;  //交界处
	SET_SDA(HIGH); //释放总线以让从机可以发送

	for (u8Loopi=0; u8Loopi<8; u8Loopi++)
	{
		SCL_H;
		
                u8BitTmp = GET_SDA();
		u8Data = (uint8)((u8Data<<1) | u8BitTmp);
		SCL_L;		
	}	
	
	return u8Data;
}

/*===============================================================
	@brief     回复应答位信号
==================================================================*/
static void GPIO_I2C_Response(uint8 u8AckState) //ACK_ON开启应答  ACK_OFF不应答
{
	SCL_L; //交界处
	SET_SDA(u8AckState);
	SCL_H;
	SCL_L;
}

/*===============================================================
	@brief     等待应答信号
==================================================================*/
static uint8 GPIO_I2C_Wait_Ack() 
{
	uint8 u8AckRec;
	uint32 u32Loopi = 0;
	
	SCL_H; //写“寄存器”操作时已经拉低
	u8AckRec = GET_SDA();
	
	while ((u8AckRec == 1) && (u32Loopi<3))
	{
		u32Loopi++;
	}
	
	SCL_L;
	
	return u8AckRec;
}

/*===============================================================
	@brief     向总线设备发出1字节
==================================================================*/
void GPIO_I2C_Write_1(uint8 DeviceAddr, uint8 AccessAddr, uint8 Data)
{
	GPIO_I2C_Start(); //开始信号
	GPIO_I2C_Write_Register((uint8)(DeviceAddr & 0xfe)); //设备地址
	GPIO_I2C_Wait_Ack(); //等待回应
	GPIO_I2C_Write_Register(AccessAddr); //访问地址
	GPIO_I2C_Wait_Ack(); //等待回应
        GPIO_I2C_Write_Register(Data); //写数据
	GPIO_I2C_Wait_Ack(); //等待回应
	GPIO_I2C_Stop();		
}

/*===============================================================
	@brief     从总线设备读取1字节
==================================================================*/
uint8 GPIO_I2C_Read_1(uint8 DeviceAddr, uint8 AccessAddr, uint8 *pData)
{
    uint8 u8Temp;
    
	GPIO_I2C_Start(); //开始信号
	GPIO_I2C_Write_Register((uint8)(DeviceAddr & 0xfe)); //设备地址，写操作
	GPIO_I2C_Wait_Ack(); //等待回应
	GPIO_I2C_Write_Register(AccessAddr); //访问地址
	GPIO_I2C_Wait_Ack(); //等待回应

	GPIO_I2C_Restart(); //重新开始信号
	GPIO_I2C_Write_Register((uint8)(DeviceAddr | 0x01)); // 写设备地址，为读模式，通知从机改为发送数据
	GPIO_I2C_Wait_Ack(); //等待回应
    
	u8Temp = GPIO_I2C_Read_Register(); //读取数据
	GPIO_I2C_Response(ACK_OFF); //非应答信号
	GPIO_I2C_Stop();
    
    if (pData != NULL)
    {
        *pData = u8Temp;
    }
    
    return u8Temp;
}


/*===============================================================
	@brief     向总线设备发出2字节
==================================================================*/
void GPIO_I2C_Write_2(uint8 DeviceAddr, uint8 AccessAddr, uint16 Data)
{       
        uint8 msdata = 0;
        uint8 lsdata = 0;
        msdata = (uint8)((Data & 0xff00)>>8);
        lsdata = (uint8)(Data & 0xff);
             
	GPIO_I2C_Start(); //开始信号
	GPIO_I2C_Write_Register((uint8)(DeviceAddr & 0xfe)); //设备地址
	GPIO_I2C_Wait_Ack(); //等待回应
	GPIO_I2C_Write_Register(AccessAddr); //访问地址
	GPIO_I2C_Wait_Ack(); //等待回应
        
        GPIO_I2C_Write_Register(msdata); //写高八位数据
	GPIO_I2C_Wait_Ack(); //等待回应
        GPIO_I2C_Write_Register(lsdata); //写低八位数据
	GPIO_I2C_Wait_Ack(); //等待回应
        
	GPIO_I2C_Stop();		
}

/*===============================================================
	@brief     从总线设备读取2字节
==================================================================*/
void GPIO_I2C_Read_2(uint8 DeviceAddr, uint8 AccessAddr, uint16 *pData)
{
        uint16 msdata = 0;
        uint16 lsdata = 0;
        
	GPIO_I2C_Start(); //开始信号
	GPIO_I2C_Write_Register((uint8)(DeviceAddr & 0xfe)); //设备地址，写操作
	GPIO_I2C_Wait_Ack(); //等待回应
	GPIO_I2C_Write_Register(AccessAddr); //访问地址
	GPIO_I2C_Wait_Ack(); //等待回应

	GPIO_I2C_Restart(); //重新开始信号
	GPIO_I2C_Write_Register((uint8)(DeviceAddr | 0x01)); // 写设备地址，为读模式，通知从机改为发送数据
	GPIO_I2C_Wait_Ack(); //等待回应
        
	msdata = GPIO_I2C_Read_Register(); //读取高八位数据
        GPIO_I2C_Response(ACK_ON); //应答信号
        lsdata = GPIO_I2C_Read_Register(); //读取低八位数据
	GPIO_I2C_Response(ACK_OFF); //非应答信号
        
	GPIO_I2C_Stop();
        
        *pData = ( (msdata<<8) | lsdata );      
}

/*===============================================================
	@brief     从总线设备读取N字节
==================================================================*/
void GPIO_I2C_Read_N(uint8 DeviceAddr, uint8 AccessAddr, uint8 *pData, uint16 lenth)
{
        int16 i = 0;
        
	GPIO_I2C_Start(); //开始信号
	GPIO_I2C_Write_Register((uint8)(DeviceAddr & 0xfe)); //设备地址，写操作
	GPIO_I2C_Wait_Ack(); //等待回应
	GPIO_I2C_Write_Register(AccessAddr); //访问地址
	GPIO_I2C_Wait_Ack(); //等待回应

	GPIO_I2C_Restart(); //重新开始信号
	GPIO_I2C_Write_Register((uint8)(DeviceAddr | 0x01)); // 写设备地址，为读模式，通知从机改为发送数据
	GPIO_I2C_Wait_Ack(); //等待回应
        
        for(i=0;i<lenth-1;i++)
        {
                pData[i] = GPIO_I2C_Read_Register(); //读取高八位数据
                GPIO_I2C_Response(ACK_ON); //应答信号
        }
        pData[i] = GPIO_I2C_Read_Register(); //读取低八位数据
	GPIO_I2C_Response(ACK_OFF); //非应答信号
        
	GPIO_I2C_Stop();
        
        //*pData = ( (msdata<<8) | lsdata );      
}

/*===============================================================
	@brief     延时
==================================================================*/
void GPIO_I2C_Delay(uint32 u32DelayCnt)
{
    while(u32DelayCnt)
      u32DelayCnt--;
}    