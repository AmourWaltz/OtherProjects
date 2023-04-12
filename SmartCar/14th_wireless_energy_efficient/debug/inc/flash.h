#ifndef _FLASH_H_
#define _FLASH_H_

#include "common.h"
#include "include.h"

/*===============================================================
        @note      风雨送春归，飞雪迎春到，
                   已是悬崖百丈冰，犹有花枝俏。
                   俏也不争春，只把春来报，
                   待到山花烂漫时，她在丛中笑。
==================================================================*/


//定义写入的数据类型
typedef vuint16 FlashDataType;

#define FLASH_TEST_SECTOR       (255)//写Flash的最后一个扇区
#define FLASH_TEST_ADDR         (FLASH_TEST_SECTOR*(2048))
#define FLASH_BUFFER_SIZE       100//写入Flash的数据缓冲区的大小，FLASH_BUFFER_SIZE*（sizeof(FlashData)）<2048

extern FlashDataType g_flashDataBuffer[FLASH_BUFFER_SIZE];


void Flash_Init(void);//初始化

void Flash_Read_Data(void);//读数据

void Flash_Write_Data(void);//写数据



#endif