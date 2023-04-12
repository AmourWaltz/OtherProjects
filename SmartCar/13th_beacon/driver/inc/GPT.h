#ifndef _GPT_H
#define _GPT_H

#include "common.h"
#include "HW_FTM.h"
#include "HW_LPTMR.h"

//正交解码

#define QD_DECODER_FTM                  FTM2
#define QD_DECODER_FTM_PHA_PIN          PTB18
#define QD_DECODER_FTM_PHB_PIN          PTB19


//计数器
#define ENCODER_LPTMR_CH                LPTMR_ALT2
#define PLACC_DECODER_DIR_PORT          PTB
#define PLACC_DECODER_DIR_PIN           22
#define PLACC_DECODER_DIR_PIN_MASK      GPIO_Pin22

//方向电平

#define FORWARD_DIR_LV                  1
#define BACKWARD_DIR_LV                 0


void GPT_Init(void);

void QD_DECODER_GPT_Init(FTM_InitTypeDef *setinfo);
int16 QD_GPT_cnv(void);
void PLACC_DECODER_GPT_Init(LPTMR_InitTypeDef *setinfo);
void PLACC_DECODER_DIR_GPIO_INIT(void);
int16 PLACC_GPT_cnv(void);

int16 LPLD_LPTMR_GetCounter();
void LPLD_LPTMR_ClearCounter();

#endif 