/*=============================================
    @file         gpt.c
    @brief        正交解码与低功耗计数器
    @programmer   Beyond Hsueh
=============================================*/

#include "gpt.h"

/*===============================================================
        @note      杀尽江南百万兵，腰间宝剑血犹腥；
                   老僧不识英雄汉，只管晓晓问姓名。
==================================================================*/

static FTM_InitTypeDef   QD_DECODER;
static LPTMR_InitTypeDef PLACC_DECODER;

/*===============================================================
	@brief     初始化正交解码与低功耗计数器
        @note      双编码器
==================================================================*/
void GPT_Init(void)
{
    QD_DECODER_GPT_Init(&QD_DECODER);
    PLACC_DECODER_GPT_Init(&PLACC_DECODER);
}

void QD_DECODER_GPT_Init(FTM_InitTypeDef *setinfo)
{
    setinfo->FTM_Ftmx = QD_DECODER_FTM;
    setinfo->FTM_Mode = FTM_MODE_QD;     //正交解码模式
    setinfo->FTM_QdMode = QD_MODE_CNTDIR;//QD_MODE_PHAB; 
    
    LPLD_FTM_Init(*setinfo);
    LPLD_FTM_QD_Enable(QD_DECODER_FTM, QD_DECODER_FTM_PHA_PIN, QD_DECODER_FTM_PHB_PIN);   //AB两相输入端口
    LPLD_FTM_ClearCounter(QD_DECODER_FTM);//清零  
}

void PLACC_DECODER_GPT_Init(LPTMR_InitTypeDef *setinfo)   
{
    setinfo->LPTMR_Mode = LPTMR_MODE_PLACC;  //脉冲累加模式   
    setinfo->LPTMR_PluseAccInput = ENCODER_LPTMR_CH; //PTC5

    LPLD_LPTMR_Init(*setinfo);
    
    PLACC_DECODER_DIR_GPIO_INIT();
}

void PLACC_DECODER_DIR_GPIO_INIT(void)
{
    GPIO_Init(PLACC_DECODER_DIR_PORT, PLACC_DECODER_DIR_PIN_MASK, DIR_INPUT, OUTPUT_L); 
}

/*===============================================================
	@brief     正交解码计数读取
==================================================================*/
int16 QD_GPT_cnv(void)
{
    int16 num = 0;
    
    num = LPLD_FTM_GetCounter(QD_DECODER_FTM);
    
    LPLD_FTM_ClearCounter(QD_DECODER_FTM);

    return (num*2);
    
}

/*===============================================================
	@brief     低功耗计数器计数读取
==================================================================*/
int16 PLACC_GPT_cnv(void)
{
    int16 num = 0;
	
    num = -LPLD_LPTMR_GetCounter();
    
    LPLD_LPTMR_ClearCounter();
    
//    LPLD_LPTMR_Init(PLACC_DECODER);

    return num;
    
    //return num*2;
}


/*===============================================================
	@brief     形式函数
==================================================================*/
int16 LPLD_LPTMR_GetCounter()
{
    int16 num = 0;
    uint8 u8DirLevel;    
    
    num = LPLD_LPTMR_GetPulseAcc();
    
    u8DirLevel = LPLD_GPIO_Input_b(PLACC_DECODER_DIR_PORT, PLACC_DECODER_DIR_PIN);
    if (u8DirLevel == BACKWARD_DIR_LV)
    {
        num = -num;
    }
    
    return num;
}

/*===============================================================
	@brief     形式函数
==================================================================*/
void LPLD_LPTMR_ClearCounter()
{
    LPLD_LPTMR_ResetCounter();
    PLACC_DECODER_GPT_Init(&PLACC_DECODER);       
}