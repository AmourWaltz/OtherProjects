#include "flash.h"

FlashDataType g_flashDataBuffer[FLASH_BUFFER_SIZE];

//初始化
void FLASH_Init(void)
{
	LPLD_Flash_Init();
}

//读数据
void FlashReadData(void)
{
	FlashDataType * pTemp = NULL;
	uint8 len = 0;

	pTemp = (FlashDataType *)FLASH_TEST_ADDR;

	for (len=0 ; len<FLASH_BUFFER_SIZE ; len++)
	{
		g_flashDataBuffer[len] = *(pTemp + len);
	}
}

//写数据
void FlashWriteData(void)
{
	//先擦除
	LPLD_Flash_SectorErase(FLASH_TEST_ADDR);

/*	if (result==FLASH_OK)
	{
		printf("success: ease flash success!\r\n");
	}
	else
	{
		printf("error: ease flash error! error num:%X\r\n", result);
		return;
	}
*/
	//再写入
	LPLD_Flash_ByteProgram((uint32)FLASH_TEST_ADDR, (uint32*)g_flashDataBuffer, FLASH_BUFFER_SIZE*sizeof(FlashDataType) );

/*	if (result==FLASH_OK)
	{
		printf("success: write flash success!\r\n");
	}
	else
	{
		printf("error: write flash error! error num:%X。\r\n", result);
		return;
	}
*/
}