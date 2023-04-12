//************************串口示波器握手程序******************
#include "scope.h"
#include "uart.h"
#include "include.h"


unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++)
	{      
        CRC_Temp ^= Buf[i];
        
		for (j=0;j<8;j++)
		{
            if (CRC_Temp & 0x01)
			{
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            }
			else
            {
				CRC_Temp = CRC_Temp >> 1;
        	}
		}
    }
	
    return(CRC_Temp);
}

//还没写与摄像头有关的优化
void VisualScope_Send(uint8 u8Mode)
{
    unsigned char i, j;
    //SCOPE_MODE
	int temp[4] = {0};
	unsigned int temp1[4] = {0};
	unsigned char databuf[10] = {0};	
	unsigned short CRC16 = 0;
    //IMAGE_MODE

    
    switch (u8Mode)
    {
        case SCOPE_DATA_MODE: 
            for(i=0 ; i<4 ; i++)
            {
                temp[i]  = (int)gf_VisualScopeData[i];
                temp1[i] = (unsigned int)temp[i];
            }
           
            for(i=0 ; i<4 ; i++) 
            {
                databuf[i*2]   = (unsigned char)(temp1[i]%256);
                databuf[i*2+1] = (unsigned char)(temp1[i]/256);
            }
          
            CRC16 = CRC_CHECK(databuf,8);
            databuf[8] = CRC16%256;
            databuf[9] = CRC16/256;

            for(i=0;i<10;i++)
            {
                UART_Sendchar(databuf[i]);
            }            
            break;
            
        case SCOPE_IMAGE_MODE: 
            
            for(i = 0; i < 64; i++)
            {
                for(j = 0; j < 128; j++)
                {
                    if (guc_corrected_image[i][j] == IMAGE_WHITE)
                    {
                        UART_Sendchar(0xfe);
                    }
                    else
                    {
                        UART_Sendchar(0x00);
                    }
                }                            
            }       
            
            UART_Sendchar(0xff);
            
            
            break;
            
        default : break;
    }
}






