/*=============================================
    @file         pwm.c
    @brief        脉冲占空比
    @programmer   Beyond Hsueh
=============================================*/

#include "pwm.h"

/*===============================================================
        @note      洛阳城东桃李花，飞来飞去落谁家？
                   洛阳女儿惜颜色，坐见落花长叹息。
                   今年花落颜色改，明年花开复谁在？
                   已见松柏摧为薪，更闻桑田变成海。
                   古人无复洛城东，今人还对落花风。
                   年年岁岁花相似，岁岁年年人不同。
                   寄言全盛红颜子，应怜半死白头翁。
                   此翁白头真可怜，伊昔红颜美少年。
                   公子王孙芳树下，清歌妙舞落花前。
                   光禄池台文锦绣，将军楼阁画神仙。
                   一朝卧病无相识，三春行乐在谁边？
                   宛转蛾眉能几时？须臾鹤发乱如丝。
                   但看古来歌舞地，唯有黄昏鸟雀悲。
==================================================================*/    

// 电机FTM模块信息定义
static FTM_InitTypeDef g_ftm_motor_struct;

/*===============================================================
	@brief     输出PWM初始化
==================================================================*/ 
void Motor_PWM_Init(void)
{
    // 配置电机PWM参数
	g_ftm_motor_struct.FTM_Ftmx = PWM_MOTOR_FTM;		
	g_ftm_motor_struct.FTM_Mode = FTM_MODE_PWM;			//PWM模式
	g_ftm_motor_struct.FTM_PwmFreq = PWM_Motor_PER_1S;		//频率20000Hz
//	g_ftm_motor_struct.FTM_PwmDeadtimeCfg = PWM_MOTOR_DEADTIME;		//通道0和1插入死区

	// 初始化FTM
	LPLD_FTM_Init(g_ftm_motor_struct);

	// 左轮电机PWM使能
	LPLD_FTM_PWM_Enable(PWM_MOTOR_FTM, PWM_LEFT_MOTOR_CH, HALF_DUTY, PWM_LEFT_MOTOR_PIN, ALIGN_LEFT);//单极性PWM要为0
        LPLD_FTM_PWM_ChangeDuty(PWM_MOTOR_FTM, PWM_LEFT_MOTOR_CH, 0);
    
	// 右轮电机PWM使能
	LPLD_FTM_PWM_Enable(PWM_MOTOR_FTM, PWM_RIGHT_MOTOR_CH, HALF_DUTY, PWM_RIGHT_MOTOR_PIN, ALIGN_LEFT);//单极性PWM要为0
        LPLD_FTM_PWM_ChangeDuty(PWM_MOTOR_FTM, PWM_RIGHT_MOTOR_CH, 0);
    
}

/*===============================================================
	@brief     修改左轮占空比
==================================================================*/ 
void Change_PWM_Motor_Left(uint32 DUTY)
{
    uint32 left_pwm_duty = 0;

    left_pwm_duty = DUTY;

    // 占空比限幅
    left_pwm_duty = (uint32)Limiter(left_pwm_duty, MIN_DUTY, MAX_DUTY);

    // 修改左轮占空比
    LPLD_FTM_PWM_ChangeDuty(PWM_MOTOR_FTM, PWM_LEFT_MOTOR_CH, left_pwm_duty);
    
}

/*===============================================================
	@brief     修改右轮占空比
==================================================================*/ 
void Change_PWM_Motor_Right(uint32 DUTY)
{
    uint32 right_pwm_duty = 0;

    right_pwm_duty = DUTY;

    // 占空比限幅
    right_pwm_duty = (uint32)Limiter(right_pwm_duty, MIN_DUTY, MAX_DUTY);

    // 修改右轮占空比
    LPLD_FTM_PWM_ChangeDuty(PWM_MOTOR_FTM, PWM_RIGHT_MOTOR_CH, right_pwm_duty);     
    
}