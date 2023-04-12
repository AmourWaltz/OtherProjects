/*=============================================
    @file         buck.c
    @brief        充电脉冲文件
    @programmer   Beyond Hsueh
=============================================*/

#include "buck.h"

/*===============================================================
        @note      伫倚危楼风细细，望极春愁，暗暗生天际；
                   草色烟光残照里，无言谁会凭栏意。
                   拟把疏狂图一醉，对酒当歌，强乐还无味；
                   衣带渐宽终不悔，为伊消得人憔悴。
==================================================================*/

// 充电模块信息定义
static FTM_InitTypeDef g_ftm_charge_struct;

/*===============================================================
	@brief     输出PWM初始化
==================================================================*/
void PWM_Charge_Init(void)
{
    //配置充电PWM参数
    g_ftm_charge_struct.FTM_Ftmx = PWM_CHARGE_FTM;		
    g_ftm_charge_struct.FTM_Mode = FTM_MODE_PWM;//PWM模式
    g_ftm_charge_struct.FTM_PwmFreq = PWM_CHARGE_FREQ;//频率20000Hz
    //g_ftm_charge_struct.FTM_PwmDeadtimeCfg = DEAD_TIME_PWM;//通道0和1插入死区

    // 初始化FTM
    LPLD_FTM_Init(g_ftm_charge_struct);

    //充电PWM使能
    LPLD_FTM_PWM_Enable(PWM_CHARGE_FTM, PWM_CHARGE_CH, HALF_DUTY, PWM_CHARGE_PIN, ALIGN_LEFT);
    LPLD_FTM_PWM_ChangeDuty(PWM_CHARGE_FTM, PWM_CHARGE_CH, 0);
    
    GPIO_Init(CHARGE_CONTROL_PORT, CHARGE_CONTROL_PIN_MASK, DIR_OUTPUT, OFF);
  
    //CHARGE_OFF;
}

/*===============================================================
	@brief     修改Buck电路占空比
==================================================================*/
void Change_PWM_Charge(uint32 DUTY)
{
    uint32 charge_pwm_duty = 0;

    charge_pwm_duty = DUTY;

    // 占空比限幅
    charge_pwm_duty = (uint32)Limiter(charge_pwm_duty, MIN_DUTY, MAX_DUTY);

    LPLD_FTM_PWM_ChangeDuty(PWM_CHARGE_FTM, PWM_CHARGE_CH, charge_pwm_duty);     
}

