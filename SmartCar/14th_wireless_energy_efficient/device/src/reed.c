/*=============================================
    @file         reed.c
    @brief        干簧管
    @programmer   Beyond Hsueh
=============================================*/

#include "reed.h"

/*===============================================================
        @note      北方有佳人，绝世而独立。 
                   一顾倾人城，再顾倾人国。 
                   宁不知倾城与倾国？佳人难再得。
==================================================================*/ 

/*===============================================================
	@brief     干簧管初始化
==================================================================*/
void Reed_Init(void)
{
	GPIO_InitTypeDef struct_reed_switch = {0};

#ifdef REED_SWITCH_0
	struct_reed_switch.GPIO_PTx = REED_SWITCH_0_PORT;// 干簧管0端口号初始化
	struct_reed_switch.GPIO_Pins = REED_SWITCH_0_PIN;// 干簧管0引脚号初始化
	struct_reed_switch.GPIO_Dir = DIR_INPUT;// 干簧管0输入
        struct_reed_switch.GPIO_PinControl = INPUT_PULL_UP;
	struct_reed_switch.GPIO_PinControl = IRQC_DIS;// 禁用触发
	LPLD_GPIO_Init(struct_reed_switch);
#endif

#ifdef REED_SWITCH_1
	struct_reed_switch.GPIO_PTx = REED_SWITCH_1_PORT;//干簧管1端口号初始化
	struct_reed_switch.GPIO_Pins = REED_SWITCH_1_PIN;//干簧管1引脚号初始化
	struct_reed_switch.GPIO_Dir = DIR_INPUT;// 干簧管1输入
        struct_reed_switch.GPIO_PinControl = INPUT_PULL_UP;
	struct_reed_switch.GPIO_PinControl = IRQC_DIS;//禁用触发
	LPLD_GPIO_Init(struct_reed_switch);
#endif

#ifdef REED_SWITCH_2
	struct_reed_switch.GPIO_PTx = REED_SWITCH_2_PORT;//干簧管2端口号初始化
	struct_reed_switch.GPIO_Pins = REED_SWITCH_2_PIN;//干簧管2引脚号初始化
	struct_reed_switch.GPIO_Dir = DIR_INPUT;//干簧管2输入
        struct_reed_switch.GPIO_PinControl = INPUT_PULL_UP;
	struct_reed_switch.GPIO_PinControl = IRQC_DIS;//禁用触发
	LPLD_GPIO_Init(struct_reed_switch);
#endif

#ifdef REED_SWITCH_3
	struct_reed_switch.GPIO_PTx = REED_SWITCH_3_PORT;//干簧管3端口号初始化
	struct_reed_switch.GPIO_Pins = REED_SWITCH_3_PIN;//干簧管3引脚号初始化
	struct_reed_switch.GPIO_Dir = DIR_INPUT;//干簧管3输入
        struct_reed_switch.GPIO_PinControl = INPUT_PULL_UP;
	struct_reed_switch.GPIO_PinControl = IRQC_DIS;//禁用触发
	LPLD_GPIO_Init(struct_reed_switch);
#endif
}

/*===============================================================
	@brief     获取一个干簧管状态
==================================================================*/
uint8 Get_Reed_State_Once(uint8 reed_switch_num)
{
	uint8 reed_switch_state = 0;//用于接收要读取的干簧管的状态

	switch(reed_switch_num)
	{
            #ifdef REED_SWITCH_0
            case REED_SWITCH_0:
                if(LPLD_GPIO_Input_b(REED_SWITCH_0_PORT, REED_SWITCH_0_NUM) == OUTPUT_H)
                {
                    reed_switch_state = 0;
                }
                else 
                {
                    reed_switch_state = 1;
                }
                break;
            #endif

            #ifdef REED_SWITCH_1
            case REED_SWITCH_1:
                if(LPLD_GPIO_Input_b(REED_SWITCH_1_PORT, REED_SWITCH_1_NUM) == OUTPUT_H)
                {
                    reed_switch_state = 0;
                }
                else 
                {
                    reed_switch_state = 1;
                }
                break;
            #endif

            #ifdef REED_SWITCH_2
            case REED_SWITCH_2:
                if(LPLD_GPIO_Input_b(REED_SWITCH_2_PORT, REED_SWITCH_2_NUM) == OUTPUT_H)
                {
                    reed_switch_state = 0;
                }
                else 
                {
                    reed_switch_state = 1;
                }
                break;
            #endif

            #ifdef REED_SWITCH_3
            case REED_SWITCH_3:
                if(LPLD_GPIO_Input_b(REED_SWITCH_3_PORT, REED_SWITCH_3_NUM) == OUTPUT_H)
                {
                    reed_switch_state = 0;
                }
                else 
                {
                    reed_switch_state = 1;
                }
                break;
            #endif

            default:
                break;
	}

	return (reed_switch_state);
}

/*===============================================================
	@brief     获取所有干簧管状态
==================================================================*/
uint8 Get_All_Reed_State(void)
{
	uint8 switch_state = 0;//用于记录所有4个干簧管的状态
	uint8 switch_byte = 0;//用于接收一位干簧管的状态
	uint8 switch_num = 0;//用于switch_byte的左移

	for(switch_num = 0; switch_num < REED_NUM; switch_num++ )
	{
            switch_byte = 0;//初始化来准备接收一位干簧管的状态
            switch_byte = Get_Reed_State_Once(switch_num);//接收一位干簧管的状态
            switch_state += switch_byte; //拨码开关的状态累加
	}

	return (switch_state);
}