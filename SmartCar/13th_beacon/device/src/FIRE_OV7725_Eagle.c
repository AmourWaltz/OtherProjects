/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,野火科技
 *     All rights reserved.
 *     技术讨论：野火初学论坛 http://www.chuxue123.com
 *
 *     除注明出处外，以下所有内容版权均属野火科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留野火科技的版权声明。
 *
 * @file       FIRE_OV7725_Eagle.c
 * @brief      鹰眼ov7725驱动代码
 * @author     野火科技
 * @version    v5.0
 * @date       2013-09-07
 */

#include "include.h"

//内部函数声明
static uint8 ov7725_eagle_reg_init(void);

/*!
 *  @brief      鹰眼ov7725初始化
 *  @since      v5.0
 */
uint8 ov7725_eagle_init()
{
    
    //while(ov7725_eagle_reg_init() == 0);
    SCCB_GPIO_init();
    ov7725_eagle_reg_init();
    
    return 0;
}
//
///*!
// *  @brief      鹰眼ov7725管脚初始化（内部调用）
// *  @since      v5.0
// */
//void ov7725_eagle_port_init()
//{
//    //DMA通道0初始化，PTA27触发源(默认上升沿)，源地址为PTB_B0_IN，目的地址为：IMG_BUFF，每次传输1Byte
//    //dma_portx2buff_init(CAMERA_DMA_CH, (void *)&PTB_B0_IN, (void *)ov7725_eagle_img_buff, PTA27, DMA_BYTE1, CAMERA_DMA_NUM, DADDR_KEEPON);
//
//    //DMA_DIS(CAMERA_DMA_CH);
//    //disable_irq(PORTA_IRQn);                        //关闭PTA的中断
//    //DMA_IRQ_CLEAN(CAMERA_DMA_CH);                   //清除通道传输中断标志位
//    //DMA_IRQ_EN(CAMERA_DMA_CH);
//	
//    //port_init(PTA27, ALT1 | DMA_FALLING | PULLUP );         //PCLK
//
//    //port_init(PTA29, ALT1 | IRQ_RISING | PULLUP | PF);     //场中断，上拉，上降沿触发中断，带滤波
//
//}
//
///*!
// *  @brief      鹰眼ov7725场中断服务函数
// *  @since      v5.0
// */
//void ov7725_eagle_vsync(void)
//{
//
//    //场中断需要判断是场结束还是场开始
//    if(ov7725_eagle_img_flag == IMG_START)                   //需要开始采集图像
//    {
//        ov7725_eagle_img_flag = IMG_GATHER;                  //标记图像采集中
//        disable_irq(PORTA_IRQn);
//
//        //DMA_EN(CAMERA_DMA_CH);                  //使能通道CHn 硬件请求
//        //DMA_DADDR(CAMERA_DMA_CH) = (uint32)ov7725_eagle_img_buff;    //恢复地址
//    }
//    else                                        //图像采集错误
//    {
//        disable_irq(PORTA_IRQn);                        //关闭PTA的中断
//        ov7725_eagle_img_flag = IMG_FAIL;                    //标记图像采集失败
//    }
//}
//
///*!
// *  @brief      鹰眼ov7725 DMA中断服务函数
// *  @since      v5.0
// */
//void ov7725_eagle_dma()
//{
//    ov7725_eagle_img_flag = IMG_FINISH ;
//    //DMA_IRQ_CLEAN(CAMERA_DMA_CH);           //清除通道传输中断标志位
//}
//
///*!
// *  @brief      鹰眼ov7725采集图像（采集到的数据存储在 初始化时配置的地址上）
// *  @since      v5.0
// */
//void ov7725_eagle_get_img()
//{
//    ov7725_eagle_img_flag = IMG_START;                   //开始采集图像
//    //PORTA_ISFR = ~0;                        //写1清中断标志位(必须的，不然回导致一开中断就马上触发中断)
//    //enable_irq(PORTA_IRQn);                         //允许PTA的中断
//    while(ov7725_eagle_img_flag != IMG_FINISH)           //等待图像采集完毕
//    {
//        if(ov7725_eagle_img_flag == IMG_FAIL)            //假如图像采集错误，则重新开始采集
//        {
//            ov7725_eagle_img_flag = IMG_START;           //开始采集图像
//            //PORTA_ISFR = ~0;                //写1清中断标志位(必须的，不然回导致一开中断就马上触发中断)
//            //enable_irq(PORTA_IRQn);                 //允许PTA的中断
//        }
//    }
//}


///*OV7725初始化配置表*/
//reg_s ov7725_eagle_reg[] =
//{
//    //寄存器，寄存器值次
//    {OV7725_COM4         , 0x81},//锁相环倍频 8倍  0xC1
//    {OV7725_CLKRC        , 0x00},//分频 2分频
//    {OV7725_COM2         , 0x03},//Output drive capability 4x
//    {OV7725_COM3         , 0xD0},//Horizontal mirror image ON selection & Swap Y/UV output sequence in YUV output mode //DSP_Ctrl3 [7]
//    {OV7725_COM7         , 0x40},//Resolution selection : QVGA
//    {OV7725_HSTART       , 0x3F},//for QVGA 
//    {OV7725_HSIZE        , 0x50},//for QVGA 
//    {OV7725_VSTRT        , 0x03},//for QVGA 
//    {OV7725_VSIZE        , 0x78},//for QVGA 
//    {OV7725_HREF         , 0x00},//Image Start and Size Control
//    {OV7725_SCAL0        , 0x0A},//DCW Ratio Control : 1/4 vertical down sampling & 1/4 horizontal down sampling
//    {OV7725_AWB_Ctrl0    , 0xE0},//AWB gain enable & AWB calculate enable
//    {OV7725_DSPAuto      , 0xff},
//    {OV7725_DSP_Ctrl2    , 0x0C},//Vertical DCW enable & Horizontal DCW enable
//    {OV7725_DSP_Ctrl3    , 0x00},//DSP color bar OFF selection
//    {OV7725_DSP_Ctrl4    , 0x00},//Output selection : YUV or RGB(觉得是手册错误，只有YUV)
//
//    //H Output Size = { HOutSize [7:0] (0x29), EXHCH [1:0] (0x2A)}    
//#if (CAMERA_W == 80)
//    {OV7725_HOutSize     , 0x14},
//    
//#elif (CAMERA_W == 128)
//    {OV7725_HOutSize     , 0x20},    
//    
//#elif (CAMERA_W == 160)
//    {OV7725_HOutSize     , 0x28},
//#elif (CAMERA_W == 240)
//    {OV7725_HOutSize     , 0x3c},
//#elif (CAMERA_W == 320)
//    {OV7725_HOutSize     , 0x50},
//#else
//
//#endif
//
//#if (CAMERA_H == 60 )
//    {OV7725_VOutSize     , 0x1E},//V Output Size = { VOutSize [7:0] (0x2C), EXHCH [2] (0x2A)}
//#elif (CAMERA_H == 120 )
//    {OV7725_VOutSize     , 0x3c},
//#elif (CAMERA_H == 180 )
//    {OV7725_VOutSize     , 0x5a},
//#elif (CAMERA_H == 240 )
//    {OV7725_VOutSize     , 0x78},
//#else
//
//#endif
//
//    {OV7725_EXHCH        , 0x00},
//    {OV7725_GAM1         , 0x0c},//???
//    {OV7725_GAM2         , 0x16},
//    {OV7725_GAM3         , 0x2a},
//    {OV7725_GAM4         , 0x4e},
//    {OV7725_GAM5         , 0x61},
//    {OV7725_GAM6         , 0x6f},
//    {OV7725_GAM7         , 0x7b},
//    {OV7725_GAM8         , 0x86},
//    {OV7725_GAM9         , 0x8e},
//    {OV7725_GAM10        , 0x97},
//    {OV7725_GAM11        , 0xa4},
//    {OV7725_GAM12        , 0xaf},
//    {OV7725_GAM13        , 0xc5},
//    {OV7725_GAM14        , 0xd7},
//    {OV7725_GAM15        , 0xe8},
//    {OV7725_SLOP         , 0x20},//SLOP[7:0] = (0x100 - GAM15 [7:0]) × 4/3
//    {OV7725_LC_RADI      , 0x00},//Lens Correction Radius
//    {OV7725_LC_COEF      , 0x13},//Lens Correction Coefficient
//    {OV7725_LC_XC        , 0x08},
//    {OV7725_LC_COEFB     , 0x14},
//    {OV7725_LC_COEFR     , 0x17},
//    {OV7725_LC_CTR       , 0x05},//Lens correction enable & set by registers LC_COEF (0x49)
//    {OV7725_BDBase       , 0x99},//Banding Filter Minimum AEC Value
//    {OV7725_BDMStep      , 0x03},//Banding Filter Maximum Step
//    {OV7725_SDE          , 0x04},//Special Digital Effect (SDE) Control : Contrast/Brightness enable
//    {OV7725_BRIGHT       , 0x00},
//    {OV7725_CNST         , 0x40},//Contrast Normalized by 0x20
//    {OV7725_SIGN         , 0x06},//???
//    {OV7725_UVADJ0       , 0x11},
//    {OV7725_UVADJ1       , 0x02},//Auto UV adjust stop control
//
//};

/*OV7725初始化配置表*/
reg_s ov7725_eagle_reg[] =
{
    //寄存器，寄存器值次
	//50帧：
#if (CAMERA_FPS == 50)
    {OV7725_COM4         , 0xC1},// x8
    {OV7725_CLKRC        , 0x02},// /6=4/3
#elif (CAMERA_FPS == 75)
    //75帧：
    {OV7725_COM4         , 0x41},// x4
    {OV7725_CLKRC        , 0x00},// /2=2
#elif (CAMERA_FPS == 112)
    //112.5帧：
    {OV7725_COM4         , 0x81},// x6
    {OV7725_CLKRC        , 0x00},// /2=3
#elif (CAMERA_FPS == 150)
    //150帧：
    {OV7725_COM4         , 0xC1},// x8
    {OV7725_CLKRC        , 0x00},// /2=4
#else
#endif
    //{OV7725_COM4         , 0xC1},
    //{OV7725_CLKRC        , 0x02},
    {OV7725_COM2         , 0x03},
    {OV7725_COM3         , 0xD0},
    {OV7725_COM7         , 0x40},
    {OV7725_HSTART       , 0x3F},
    {OV7725_HSIZE        , 0x50},
    {OV7725_VSTRT        , 0x03},
    {OV7725_VSIZE        , 0x78},
    
    {OV7725_PSHFT        , 0x40},//行中断延时

    {OV7725_HREF         , 0x00},
    {OV7725_DM_LNL       , 0x22}, //加空行调帧数
    
    {OV7725_SCAL0        , 0x0A},
    {OV7725_AWB_Ctrl0    , 0xE0},
    {OV7725_DSPAuto      , 0xff},
    {OV7725_DSP_Ctrl2    , 0x0C},
    {OV7725_DSP_Ctrl3    , 0x00},
    {OV7725_DSP_Ctrl4    , 0x00},

#if (CAMERA_W == 80)
    {OV7725_HOutSize     , 0x14},
    
#elif (CAMERA_W == 128)
    {OV7725_HOutSize     , 0x20},    
    
#elif (CAMERA_W == 160)
    {OV7725_HOutSize     , 0x28},
#elif (CAMERA_W == 240)
    {OV7725_HOutSize     , 0x3c},
#elif (CAMERA_W == 320)
    {OV7725_HOutSize     , 0x50},
#else

#endif

    //{OV7725_HOutSize     , 0x21},
    
#if (CAMERA_H == 60 )
    {OV7725_VOutSize     , 0x1E},

#elif (CAMERA_H == 120 )
    {OV7725_VOutSize     , 0x3c},

#elif (CAMERA_H == 180 )
    {OV7725_VOutSize     , 0x5a},
        
#elif (CAMERA_H == 200)
    {OV7725_VOutSize     , 0x64},     
    
#elif (CAMERA_H == 240 )
    {OV7725_VOutSize     , 0x78},
    
#elif (CAMERA_H == 280 )
    {OV7725_VOutSize     , 0x8C},    
    
#elif (CAMERA_H == 320 )
    {OV7725_VOutSize     , 0xA0},

#else

#endif

    {OV7725_EXHCH        , 0x00},
    {OV7725_GAM1         , 0x0c},
    {OV7725_GAM2         , 0x16},
    {OV7725_GAM3         , 0x2a},
    {OV7725_GAM4         , 0x4e},
    {OV7725_GAM5         , 0x61},
    {OV7725_GAM6         , 0x6f},
    {OV7725_GAM7         , 0x7b},
    {OV7725_GAM8         , 0x86},
    {OV7725_GAM9         , 0x8e},
    {OV7725_GAM10        , 0x97},
    {OV7725_GAM11        , 0xa4},
    {OV7725_GAM12        , 0xaf},
    {OV7725_GAM13        , 0xc5},
    {OV7725_GAM14        , 0xd7},
    {OV7725_GAM15        , 0xe8},
    {OV7725_SLOP         , 0x20},
    {OV7725_LC_RADI      , 0x00},
    {OV7725_LC_COEF      , 0x13},
    {OV7725_LC_XC        , 0x08},
    {OV7725_LC_COEFB     , 0x14},
    {OV7725_LC_COEFR     , 0x17},
    {OV7725_LC_CTR       , 0x05},
    {OV7725_BDBase       , 0x99},
    {OV7725_BDMStep      , 0x03},
    {OV7725_SDE          , 0x04},
    {OV7725_BRIGHT       , 0x00},
	//camera threshold
    {OV7725_CNST         , 0x40},//45
    {OV7725_SIGN         , 0x06},//46
    {OV7725_UVADJ0       , 0x11},//47
    {OV7725_UVADJ1       , 0x02},//48

};


uint8 ov7725_eagle_cfgnum = ARR_SIZE( ov7725_eagle_reg ) ; /*结构体数组成员数目*/

/*!
 *  @brief      鹰眼ov7725寄存器 初始化
 *  @return     初始化结果（0表示失败，1表示成功）
 *  @since      v5.0
 */
uint8 ov7725_eagle_reg_init(void)
{
    uint16 i = 0;
    uint8 Sensor_IDCode = 0;

    
    if( 0 == SCCB_WriteByte ( OV7725_COM7, 0x80 ) )/*复位sensor */
    {
        oled_display_6x8str(0, 0, "camera write error");
        return 0 ;        
    }


    if( 0 == SCCB_ReadByte( &Sensor_IDCode, 1, OV7725_VER ) )    /* 读取sensor ID号*/
    {
        oled_display_6x8str(0, 0, "camera read error");
        return 0 ;    
    }
    
    if(Sensor_IDCode == OV7725_ID)
    {
        for( i = 0 ; i < ov7725_eagle_cfgnum ; i++ )
        {
            if( 0 == SCCB_WriteByte(ov7725_eagle_reg[i].addr, ov7725_eagle_reg[i].val) )
            {
                oled_display_6x8str(0, 0, "camera reg error");
                return 0;
            }
        }
        
        
        //if (0 == SCCB_WriteByte(OV7725_CNST, g_carInfo.CameraInfo.u8DigitalCameraThreshold))
        if (0 == SCCB_WriteByte(OV7725_CNST, g_u8DigitalCameraThreshold))
        {
            oled_display_6x8str(0, 0, "camera reg error");
            return 0;
        }
    }
    else
    {
        oled_display_6x8str(0, 0, "camera version error");
        return 0;
    }

    return 1;
}


