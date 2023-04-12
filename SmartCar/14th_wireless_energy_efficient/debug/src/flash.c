/*=============================================
    @file         flash.c
    @brief        flash文件
    @programmer   Beyond Hsueh
=============================================*/

#include "flash.h"

/*===============================================================
        @note      击鼓其镗，踊跃用兵。土国城漕，我独南行。 　　
                   从孙子仲，平陈与宋。不我以归，忧心有忡。 　　
                   爰居爰处？爰丧其马？于以求之？于林之下。 　　
                   死生契阔，与子成说。执子之手，与子偕老。 　　
                   于嗟阔兮，不我活兮。于嗟洵兮，不我信兮。
==================================================================*/

FlashDataType g_flashDataBuffer[FLASH_BUFFER_SIZE];

/*===============================================================
	@brief      flash初始化
==================================================================*/
void Flash_Init(void)
{
    LPLD_Flash_Init();
}

/*===============================================================
	@brief      flash读数据
==================================================================*/
void Flash_Read_Data(void)
{
    FlashDataType * pTemp = NULL;
    uint8 len = 0;

    pTemp = (FlashDataType *)FLASH_TEST_ADDR;

    for (len=0; len < FLASH_BUFFER_SIZE; len++)
    {
        g_flashDataBuffer[len] = *(pTemp + len);
    }
}

/*===============================================================
	@brief      flash写数据
==================================================================*/
void Flash_Write_Data(void)
{
    //先擦除
    LPLD_Flash_SectorErase(FLASH_TEST_ADDR);

    //再写入
    LPLD_Flash_ByteProgram((uint32)FLASH_TEST_ADDR, (uint32*)g_flashDataBuffer, FLASH_BUFFER_SIZE*sizeof(FlashDataType) );

}