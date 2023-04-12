#ifndef _DIRECTIONAL_H_
#define _DIRECTIONAL_H_

#include "common.h"
#include "include.h"


#define INFRA_1_PORT_ID                PTE
#define INFRA_1_PORT_PIN               GPIO_Pin0
#define INFRA_1_PORT_NUM               (0)

#define INFRA_2_PORT_ID                PTE
#define INFRA_2_PORT_PIN               GPIO_Pin1
#define INFRA_2_PORT_NUM               (1)

#define INFRA_3_PORT_ID                PTE
#define INFRA_3_PORT_PIN               GPIO_Pin2
#define INFRA_3_PORT_NUM               (2)

#define INFRA_4_PORT_ID                PTE
#define INFRA_4_PORT_PIN               GPIO_Pin3
#define INFRA_4_PORT_NUM               (3)

#define INFRA_5_PORT_ID                PTE
#define INFRA_5_PORT_PIN               GPIO_Pin4
#define INFRA_5_PORT_NUM               (4)

#define INFRA_6_PORT_ID                PTE
#define INFRA_6_PORT_PIN               GPIO_Pin5
#define INFRA_6_PORT_NUM               (5)

#define INFRA_7_PORT_ID                PTE
#define INFRA_7_PORT_PIN               GPIO_Pin6
#define INFRA_7_PORT_NUM               (6)

#define INFRA_8_PORT_ID                PTE
#define INFRA_8_PORT_PIN               GPIO_Pin7
#define INFRA_8_PORT_NUM               (7)

//#define INFRA_9_PORT_ID                PORTE
//#define INFRA_9_PORT_PIN               GPIO_Pin24
//#define INFRA_9_PORT_NUM               (24)
//
//#define INFRA_10_PORT_ID               PORTE
//#define INFRA_10_PORT_PIN              GPIO_Pin25
//#define INFRA_10_PORT_NUM              (25)
//
//#define INFRA_11_PORT_ID               PORTE
//#define INFRA_11_PORT_PIN              GPIO_Pin27
//#define INFRA_11_PORT_NUM              (27)
//
//#define INFRA_12_PORT_ID               PORTE
//#define INFRA_12_PORT_PIN              GPIO_Pin28
//#define INFRA_12_PORT_NUM              (28)
//
//#define INFRA_13_PORT_ID               PORTA
//#define INFRA_13_PORT_PIN              GPIO_Pin6
//#define INFRA_13_PORT_NUM              (6)
//
//#define INFRA_14_PORT_ID               PORTA
//#define INFRA_14_PORT_PIN              GPIO_Pin7
//#define INFRA_14_PORT_NUM              (7)
//
//#define INFRA_15_PORT_ID               PORTB
//#define INFRA_15_PORT_PIN              GPIO_Pin20
//#define INFRA_15_PORT_NUM              (20)


#define INFRA_FRONT                   (1)
#define INFRA_BACK                    (5)
#define INFRA_LEFT                    (3)
#define INFRA_RIGHT                   (7)
#define INFRA_LEFT_F                  (2)
#define INFRA_RIGHT_F                 (8)
#define INFRA_LEFT_B                  (4)
#define INFRA_RIGHT_B                 (6)

void Infrared_Init();
void Infrared_Value_Get();
void Infrared_Directional();
uint8 Infrared_Start();


#endif