#include "PWM.h"
#include "include.h"

// 电机FTM模块信息定义
static FTM_InitTypeDef g_ftm_motor_struct;
static FTM_InitTypeDef g_ftm_servo_struct;

// 输出PWM初始化
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
    
#ifdef MOTOR_BIPOLAR    
    LPLD_FTM_PWM_Enable(PWM_MOTOR_FTM, PWM_LEFT_MOTOR_CH1, HALF_DUTY, PWM_LEFT_MOTOR_PIN1, ALIGN_LEFT);
#endif
    
	// 右轮电机PWM使能
	LPLD_FTM_PWM_Enable(PWM_MOTOR_FTM, PWM_RIGHT_MOTOR_CH, HALF_DUTY, PWM_RIGHT_MOTOR_PIN, ALIGN_LEFT);//单极性PWM要为0
        LPLD_FTM_PWM_ChangeDuty(PWM_MOTOR_FTM, PWM_RIGHT_MOTOR_CH, 0);
    
#ifdef MOTOR_BIPOLAR       
    LPLD_FTM_PWM_Enable(PWM_MOTOR_FTM, PWM_RIGHT_MOTOR_CH1, HALF_DUTY, PWM_RIGHT_MOTOR_PIN1, ALIGN_LEFT);
#endif    
    
}

void Servo_PWM_Init(void)
{
    //舵机初始化       
	g_ftm_servo_struct.FTM_Ftmx = PWM_SERVO_FTM;		
	g_ftm_servo_struct.FTM_Mode = FTM_MODE_PWM;			//PWM模式
	g_ftm_servo_struct.FTM_PwmFreq = PWM_Servo_PER_1S;		//频率20000Hz    
    
	// 初始化FTM
	LPLD_FTM_Init(g_ftm_servo_struct); 
    
    //舵机PWM使能
	LPLD_FTM_PWM_Enable(PWM_SERVO_FTM, PWM_SERVO_CH, 0, PWM_SERVO_PIN, ALIGN_LEFT);    
}


void Change_PWM_Servo(uint32 DUTY)
{
    LPLD_FTM_PWM_ChangeDuty(PWM_SERVO_FTM, PWM_SERVO_CH, DUTY);
}

// 修改左轮占空比
void Change_PWM_Motor_Left(uint32 DUTY)
{
	uint32 left_pwm_duty = 0;

	left_pwm_duty = DUTY;

	// 占空比限幅
	left_pwm_duty = (uint32)Limiter(left_pwm_duty, MIN_DUTY, MAX_DUTY);

	// 修改左轮占空比
    LPLD_FTM_PWM_ChangeDuty(PWM_MOTOR_FTM, PWM_LEFT_MOTOR_CH, left_pwm_duty);
    
#ifdef MOTOR_BIPOLAR       
    LPLD_FTM_PWM_ChangeDuty(PWM_MOTOR_FTM, PWM_LEFT_MOTOR_CH1, 10000 - left_pwm_duty);
#endif        
    
}


// 修改右轮占空比
void Change_PWM_Motor_Right(uint32 DUTY)
{
	uint32 right_pwm_duty = 0;

	right_pwm_duty = DUTY;

	// 占空比限幅
	right_pwm_duty = (uint32)Limiter(right_pwm_duty, MIN_DUTY, MAX_DUTY);

	// 修改右轮占空比
    LPLD_FTM_PWM_ChangeDuty(PWM_MOTOR_FTM, PWM_RIGHT_MOTOR_CH, right_pwm_duty);
    
#ifdef MOTOR_BIPOLAR       
    LPLD_FTM_PWM_ChangeDuty(PWM_MOTOR_FTM, PWM_RIGHT_MOTOR_CH1, 10000 - right_pwm_duty);
#endif       
    
}