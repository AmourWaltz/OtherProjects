#ifndef _GPT_H
#define _GPT_H

#include "common.h"
#include "include.h"
#include "HW_FTM.h"
#include "HW_LPTMR.h"

/*===============================================================
        @note      春江潮水连海平，海上明月共潮生。
                   滟滟随波千万里，何处春江无月明！
                   江流宛转绕芳甸，月照花林皆似霰；
                   空里流霜不觉飞，汀上白沙看不见。
                   江天一色无纤尘，皎皎空中孤月轮。
                   江畔何人初见月？江月何年初照人？
                   人生代代无穷已，江月年年只相似。
                   不知江月待何人，但见长江送流水。
                   白云一片去悠悠，青枫浦上不胜愁。
                   谁家今夜扁舟子？何处相思明月楼？
                   可怜楼上月徘徊，应照离人妆镜台。
                   玉户帘中卷不去，捣衣砧上拂还来。
                   此时相望不相闻，愿逐月华流照君。
                   鸿雁长飞光不度，鱼龙潜跃水成文。
                   昨夜闲潭梦落花，可怜春半不还家。
                   江水流春去欲尽，江潭落月复西斜。
                   斜月沉沉藏海雾，碣石潇湘无限路。
                   不知乘月几人归，落月摇情满江树。
==================================================================*/

//正交解码
#define QD_DECODER_FTM                  FTM2
#define QD_DECODER_FTM_PHA_PIN          PTA10
#define QD_DECODER_FTM_PHB_PIN          PTA11


//计数器
#define ENCODER_LPTMR_CH                LPTMR_ALT2
#define PLACC_DECODER_DIR_PORT          PTC
#define PLACC_DECODER_DIR_PIN           4
#define PLACC_DECODER_DIR_PIN_MASK      GPIO_Pin4

//方向电平

#define FORWARD_DIR_LV                  1
#define BACKWARD_DIR_LV                 0




void GPT_Init(void);//初始化正交解码与低功耗计数器

void QD_DECODER_GPT_Init(FTM_InitTypeDef *setinfo);//初始化正交解码与低功耗计数器

int16 QD_GPT_cnv(void);//正交解码计数读取

void PLACC_DECODER_GPT_Init(LPTMR_InitTypeDef *setinfo);//初始化正交解码与低功耗计数器

void PLACC_DECODER_DIR_GPIO_INIT(void);//初始化正交解码与低功耗计数器

int16 PLACC_GPT_cnv(void);//低功耗计数器计数读取

int16 LPLD_LPTMR_GetCounter();//形式函数

void LPLD_LPTMR_ClearCounter();//形式函数



#endif