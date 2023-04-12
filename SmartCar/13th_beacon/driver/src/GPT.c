#include "include.h"

static FTM_InitTypeDef   QD_DECODER;
static LPTMR_InitTypeDef PLACC_DECODER;

/**************C车双编码器***************/

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

int16 QD_GPT_cnv(void)
{
    int16 num = 0;
    
    num = LPLD_FTM_GetCounter(QD_DECODER_FTM);
    
    LPLD_FTM_ClearCounter(QD_DECODER_FTM);

    return (num*2);
    
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

int16 PLACC_GPT_cnv(void)
{
    int16 num = 0;
	
    num = -LPLD_LPTMR_GetCounter();
    
    LPLD_LPTMR_ClearCounter();
    
//    LPLD_LPTMR_Init(PLACC_DECODER);

    return num;
    
    //return num*2;
}

//形式函数
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
//形式函数
void LPLD_LPTMR_ClearCounter()
{
    LPLD_LPTMR_ResetCounter();
    PLACC_DECODER_GPT_Init(&PLACC_DECODER);       
}