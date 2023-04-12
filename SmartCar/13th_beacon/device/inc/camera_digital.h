#ifndef _CAMERA_DIGITAL_H
#define _CAMERA_DIGITAL_H

#include "common.h"

//摄像头引脚分配
//#define H_GPIO_PORT          (PTB)
//#define H_PIN                (GPIO_Pin20)
//#define H_NUM                (20)
//#define H_PORT               (PORTB)
//#define H_PORT_IRQn          (PORTB_IRQn)
//
//#define V_GPIO_PORT          (PTA)
//#define V_PIN                (GPIO_Pin7)
//#define V_NUM                (7)
//#define V_PORT               (PORTA)
//#define V_PORT_IRQn          (PORTA_IRQn)
//
//#define DATA_GPIO_PORT       (PTE)
//#define DATA_PIN             (GPIO_Pin0_7)
//
//
//#define PCLK_GPIO_PORT       (PTA)
//#define PCLK_PIN             (GPIO_Pin6)
//#define PCLK_NUM             (6)
//#define PCLK_PORT            (PORTA)


#define H_GPIO_PORT          (PTB)
#define H_PIN                (GPIO_Pin21)
#define H_NUM                (21)
#define H_PORT               (PORTB)
#define H_PORT_IRQn          (PORTB_IRQn)

#define V_GPIO_PORT          (PTA)
#define V_PIN                (GPIO_Pin29)
#define V_NUM                (29)
#define V_PORT               (PORTA)
#define V_PORT_IRQn          (PORTA_IRQn)

#define DATA_GPIO_PORT       (PTD)
#define DATA_PIN             (GPIO_Pin0_7)


#define PCLK_GPIO_PORT       (PTB)
#define PCLK_PIN             (GPIO_Pin23)
#define PCLK_NUM             (23)
#define PCLK_PORT            (PORTB)


//DMA 
#define CAMERA_DMA_CH                   DMA_CH0                         //DMA选择通道
#define CAMERA_DMA_IRQn                 DMA0_IRQn

#define DMA_REQ                         (PORTB_DMAREQ)    //请求源 pclk引脚


#define DMA_MAJOR_LOOP            CAMERA_ROW_DMA_NUM              //DMA主循环次数，每行传输完毕后自动关闭
#define DMA_MINER_BYTE            1                               //DMA副循环传输字节数
#define DMA_SOURCE_ADDRESS        (DATA_GPIO_PORT->PDIR)          //传输源地址：PTD0~7
#define DMA_DEST_ADDR             (uint32)compressImage0          //传输目的地址
#define DMA_SOURCE_DATA_SIZE      DMA_DST_8BIT                    //源数据传输宽度，1个字节
#define DMA_SOURCE_ADDR_OFFSET    0                               //源地址偏移量
#define DMA_DEST_DATA_SIZE        DMA_DST_8BIT                    //目的数据传输宽度
#define DMA_DEST_ADDR_OFFSET      1                               //目的地址偏移量
#define LPLD_DMA_ClearIRQ(CHX)   (DMA0->INT |= 0x1u<<CHX)


//图像宏定义
#define IMAGE_HEIGHT 64   //提取后图像高度，必须是四的倍数
#define IMAGE_WIDTH  128  //提取后图像宽度

//摄像头的白色为0，黑色为1
//而液晶的白色为1，黑色为0，最终图像由液晶显示，故如下定义
#define IMAGE_WHITE 1    //白像素
#define IMAGE_BLACK 0    //黑像素


#define IMAGE_SIZE      (IMAGE_WIDTH * IMAGE_HEIGHT)  //压缩后一副图像大小



//配置摄像头 属性
#define CAMERA_W            320                                  //定义摄像头图像宽度
#define CAMERA_H            240                                  //定义摄像头图像高度
#define CAMERA_SIZE         (CAMERA_W * CAMERA_H/8)              //图像占用空间大小
#define CAMERA_DMA_NUM      (CAMERA_SIZE)                        //DMA采集次数
#define CAMERA_ROW_DMA_NUM  (CAMERA_W / 8)                       //行DMA采集次数
#define CAMERA_DATA_NUM     (CAMERA_W * CAMERA_H)
#define CAMERA_FPS          (50)                               //50 or 75 or 112 or 150
                     //其实是100帧


void Camera_Init();
void Camera_GPIO_Init();
void Camera_DMA_Init();

void Camera_V_Isr();
void Camera_H_Isr();
void Camera_Process_Isr();

void Reserved110_IRQHandler();

void Image_Original_Get();
void img_extract_for_64x(uint8 *dst, uint8 (*src)[CAMERA_ROW_DMA_NUM]);
void img_rowcol_128X64(uint8 dst[IMAGE_HEIGHT][IMAGE_WIDTH], uint8 (*src)[CAMERA_W]);

void ImgCompress_128x64(uint8 *src, uint8 (*image)[IMAGE_WIDTH]);
void ImgExtract(uint8 *src, uint8 *dst, uint32 size);


#endif