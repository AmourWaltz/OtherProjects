#ifndef _dir_kbi_H__
#define _dir_kbi_H__

#include "include.h"
#include "common.h"

/*5Ïò°´¼ü*/
#define BT_UP_PRESS                     FIVE_DIR_UP
#define BT_DN_PRESS                     FIVE_DIR_DOWN
#define BT_LE_PRESS                     FIVE_DIR_LEFT
#define BT_RI_PRESS                     FIVE_DIR_RIGHT
#define BT_OK_PRESS                     FIVE_DIR_OK

#define FIVE_DIR_NULL                   0 
#define FIVE_DIR_UP                     1
#define FIVE_DIR_DOWN                   2
#define FIVE_DIR_LEFT                   3
#define FIVE_DIR_RIGHT                  4
#define FIVE_DIR_OK                     5


#define    FIVE_DIR_D_PORT          PTC
#define    FIVE_DIR_D_PIN           7
#define    FIVE_DIR_D_PIN_MASK      GPIO_Pin7

#define    FIVE_DIR_R_PORT          PTC
#define    FIVE_DIR_R_PIN           9
#define    FIVE_DIR_R_PIN_MASK      GPIO_Pin9

#define    FIVE_DIR_L_PORT          PTC
#define    FIVE_DIR_L_PIN           6
#define    FIVE_DIR_L_PIN_MASK      GPIO_Pin6

#define    FIVE_DIR_U_PORT          PTC
#define    FIVE_DIR_U_PIN           8
#define    FIVE_DIR_U_PIN_MASK      GPIO_Pin8

#define    FIVE_DIR_O_PORT          PTC
#define    FIVE_DIR_O_PIN           0
#define    FIVE_DIR_O_PIN_MASK      GPIO_Pin0

void Five_Dir_Init();
uint8 Get_Five_Dir_Value();
uint8 Five_Dir_Button(uint8 *pu8FiveDirValue);
void Test_Dir();

#endif 