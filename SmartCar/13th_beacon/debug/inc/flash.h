#ifndef _FLASH_H_
#define _FLASH_H_

#include "include.h"
#include "common.h"

//定义写入的数据类型
typedef vuint16 FlashDataType;

#define FLASH_TEST_SECTOR   (255)//写Flash的最后一个扇区
#define FLASH_TEST_ADDR      (FLASH_TEST_SECTOR*(2048))
#define FLASH_BUFFER_SIZE  100//写入Flash的数据缓冲区的大小，FLASH_BUFFER_SIZE*（sizeof(FlashData)）<2048

extern FlashDataType g_flashDataBuffer[FLASH_BUFFER_SIZE];
//初始化
void FLASH_Init(void);

//读数据
void FlashReadData(void);

//写数据
void FlashWriteData(void);

#endif