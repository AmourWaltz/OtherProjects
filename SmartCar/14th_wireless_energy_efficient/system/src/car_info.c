/*=============================================
    @file         car_info.c
    @brief        参数设置
    @programmer   Beyond Hsueh
=============================================*/

#include "car_info.h"
      
/*===============================================================
        @note      问世间，情为何物，直教生死相许？
                   天南地北双飞客，老翅几回寒暑。
                   欢乐趣，离别苦，就中更有痴儿女。
                   君应有语：渺万里层云，千山暮雪，只影向谁去？
                   横汾路，寂寞当年箫鼓，荒烟依旧平楚。
                   招魂楚些何嗟及，山鬼暗啼风雨。
                   天也妒，未信与，莺儿燕子俱黄土。
                   千秋万古，为留待骚人，狂歌痛饮，来访雁丘处。
==================================================================*/  

//全局参数
vuint32 gul_time_100us = 0;//0.1ms计时，用于红外传感器
vuint32 gul_time_start;//起始发车时间
vuint8 guc_switch_pin_value[6];//拨码值存储
vuint32 gl_time_infrared;//红外数值
volatile float gf_visual_scope_data[4];//传送给虚拟示波器的数据
vuint8 guc_beep_start_flag;//蜂鸣器开关标志位
vuint8 guc_car_run_flag = OFF;//小车起跑标志位
uint8 guc_reed_state_value;//干簧管状态获取值
uint8 guc_automation_mode_flag;//自动发车标志
uint8 guc_start_voltage_flag;//自动发车标志

//速度
float gf_speed_p;//速度控制比例系数
float gf_speed_i;//速度控制积分系数
float gf_speed_d;//速度控制微分系数
float gf_angle_speed_kp;//弯道减速系数
float gf_right_speed;//右轮速度
float gf_left_speed;//左轮速度
float gf_difference_coefficient;//差速自适应系数

vint32 gl_pwm_output;//电机PWM输出,数值为实际电机在速度分量上的PWM输出
float gf_car_encoder_value;//编码计数值
int32 gl_car_speed;//小车速度检测值
int32 gl_car_speed_set;//小车速度设定值
int32 gl_car_running_distance;//行驶路程，编码器检测
int32 gl_car_destination_distance;//定距停车行驶路程
int32 gl_car_stopline_distance;//强制定距停车路程
vint32 gl_left_encoder_count;//左轮编码器计数
vint32 gl_right_encoder_count;//右轮编码器计数
vint32 gl_left_encoder_value;//左轮编码器计数
vint32 gl_right_encoder_value;//右轮编码器计数


int32 gl_speed_straight;//直道速度
int32 gl_speed_lost_line;//丢信号设定速度
int32 gl_speed_curve;//弯道速度
int32 gl_speed_annulus;//环岛速度
int32 gl_speed_obstacle;//路障弧形轨迹速度

//惯性测量单元
vuint8 guc_gyro_zero_flag ;//陀螺仪自动校准标志
float gf_acce_mps2;//加速度计平方和
float gf_acce_angle;//加速度计比例角度
float gf_gyro_angle;//陀螺仪积分角度
float gf_car_angle;//卡尔曼滤波车体角度

float gf_yaw_angle;//绕z轴旋转，获取偏航角
float gf_pitch_angle;//绕y轴旋转，获取俯仰角
float gf_roll_angle;//绕x轴旋转，获取翻滚角

float gf_yaw_velocity;//偏航角速度
float gf_roll_velocity;//翻滚角速度
float gf_pitch_velocity;//俯仰角速度

float gf_yaw_rotate;//运行车姿偏航角速度
float gf_roll_rotate;//运行车姿翻滚角速度
float gf_pitch_rotate;//运行车姿俯仰角速度

float gf_gyro_x_angle;//陀螺仪x轴角度
float gf_gyro_y_angle;//陀螺仪y轴角度
float gf_gyro_z_angle;//陀螺仪z轴角度

int16 gi_roll_variance;//翻滚角速度
int16 gi_pitch_variance;//俯仰角速度

//方向
float  gf_direction_p;//方向控制比例系数
float  gf_direction_d;//方向控制微分系数
float  gf_angularspeed_p;//角速度闭环控制比例系数
float  gf_angularspeed_d;//角速度闭环控制微分系数
float  gf_angularspeed_rotate;//转向角速度，即z轴角速度

int32 gl_car_error;//小车方向偏差
int32 gl_direction_set;//角速度闭环外环设定值
int32 gl_direction_output;//方向输出量

//电感信号
vint16 gi_inductor_adc[5] = {0};//电感原始值 -- 滤波前 -- 用于动态标定
vint16 gi_inductor_value[5] = {0};//电感信号 -- 滤波后 -- 用于信号处理

int16 gi_inductor_x_offset;//横电感差比和
int16 gi_inductor_t_offset;//斜电感差比和
int16 gi_inductor_t_difference;//斜电感差分输入
int16 gi_calibration_middle;//中间电感设定值
int16 gi_inductor_calibrate_value[2];//中间电感及两边电感标定值
vuint8 guc_inductor_OK_flag;//ADC转换完成标志位
vuint8 guc_calibration_flag ;//动态标定标志

uint16 gui_x_weight;//横电感偏差比重
uint16 gui_t_weight;//斜电感偏差比重
uint16 gui_t_div_weight;//斜电感差分比重
uint16 gui_secede_annulus_weight;//出环岛偏差比重权值
uint16 gui_entry_annulus_weight;//入环岛偏差比重权值
uint16 gui_ready_annulus_weight;//准备出环岛偏差比重权值
uint16 gui_annulus_x_weight1;//环岛状态机状态一横电感差分权值
uint16 gui_annulus_y_weight1;//环岛状态机状态一竖电感差分权值
uint16 gui_annulus_x_weight2;//环岛状态机状态二横电感差分权值
uint16 gui_annulus_y_weight2;//环岛状态机状态二竖电感差分权值
uint16 gui_annulus_x_weight5;//环岛状态机状态五横电感差分权值
uint16 gui_annulus_y_weight5;//环岛状态机状态五竖电感差分权值


int32 gl_x_derivative;//横电感偏差比重
int32 gl_y_derivative;//斜电感偏差比重

//状态机元素
vuint8 guc_set_position_flag ;//定距停车标志
vuint8 guc_destination_flag ;//终点线停车标志
vuint8 guc_direction_flag ;//方向检测标志
vuint8 guc_distinction_flag ;//特殊元素检测标志
vuint8 guc_lost_signal_flag;//丢信号标志
vuint8 guc_lost_start_flag;//启动丢信号处理标志位
vuint8 guc_crucifix_flag;//十字检测标志

vuint8 guc_annulus_flag;//环岛检测标志
vuint8 guc_annulus_state;//环岛状态
vuint8 guc_annulus_direction;//环岛方向
float gf_annulus_angle;//环岛积分角度值
int32 gl_annulus_distance_record;//环岛路程记录
vuint8 guc_annulus_yaw_flag;//入环后偏航角积分标志位

float gf_slope_start_angle;//坡道检测起始俯仰角
uint8 guc_slope_complete_flag;//坡道完成标志
uint8 guc_slope_pitch_flag;//坡道积分俯仰角开启标志
int32 gl_slope_distance_record;//坡道路程记录
float gf_slope_picth_angle;//坡道积分俯仰角

vuint8 guc_slope_state;//坡道检测状态标志
vuint8 guc_straight_flag = 1;//直道检测标志
vuint8 guc_curve_flag;//急弯道检测

int16 gi_x_signal_gradient;//横电感信号变化率
int16 gi_m_signal_gradient;//中心横电感信号变化率
int16 gi_y_signal_gradient;//横电感信号变化率
int16 gi_signal_gradient[5];//所有电感信号变化率

//测距避障
int16 gi_infrared_AD_distance;//夏普红外AD测距
uint8 guc_infrared_ranging_flag;//红外测距检测障碍标志
int16 gi_infrared_2nd_AD_distance;//夏普红外2号AD测距
int32 gl_obstacle_judge_distance;//发车一定距离后检测障碍

int32 gl_ultrasound_distance;//超声波测距
int32 gl_ultrasound_detection_time;//超声波测距时间
uint8 guc_ultrasound_ranging_flag;//超声波检测障碍标志

uint16 gui_camber_radius;//弧形轨迹半径
float gf_camber_angle;//弧形轨迹角度
int32 gl_obstacle_distance_record;//路障路程记录

int16 gi_camber_coefficient;//期望角速度换算系数
int16 gi_roadblock_coefficient;//期望避障角速度换算系数
int16 gi_regression_coefficient;//期望避障角速度换算系数

int16 gi_aviodence_angle;//期望避障角度
int16 gi_regression_angle;//期望回归角度
float gf_obstacle_error;//避障偏差
float gf_obstacle_avoidance_p;//避障偏差比例系数
float gf_obstacle_avoidance_d;//避障偏差微分系数
float gf_obstacle_regression_p;//回归偏差比例系数
float gf_obstacle_regression_d;//回归偏差微分系数
float gf_obstacle_straight_p;//直道偏差比例系数
float gf_obstacle_straight_d;//直道偏差微分系数


uint8 guc_avoidance_direction_flag;//避障方向
uint8 guc_first_obstacle_slope_flag;//路障坡道优先级
uint8 guc_obstacle_flag;//路障检测标志
uint8 guc_obstacle_complete_flag;//路障完成标志
uint8 guc_obstacle_state;//路障处理状态


//无线充电
int32 gl_charge_pwm_set;//Buck充电PWM设定值
uint8 guc_charge_complete_flag;//充电完成标志
uint8 guc_cease_vibrate_flag;//停震检测标志
uint8 guc_charge_output_flag;//充电控制输出控制位
uint8 guc_overload_detection_flag;//过载检测标志
uint8 guc_lightload_detection_flag;//轻载检测标志
uint8 guc_first_vibrate_flag;//首次停震触发标志
vuint32 gul_time_charge;//0.1ms计时，用于充电计时

int32 gl_voltage_input_open;//开路输入电压
int32 gl_voltage_output_set;//输出电压设定值
int32 gl_voltage_input_set;//输入电压设定值
int32 gl_voltage_charge_set;//充电电压设定值
int32 gl_voltage_start_set;//充电电压启动值
int32 gl_current_max_set;//最大检测电流值
int32 gl_power_set;//充电功率设定值
int32 gl_charge_capacitance;//充电电容容量
int32 gl_charge_resistance;//充电电容内阻
int32 gl_pwm_max;//最大输出占空比
uint32 gul_start_pwm_coefficient;//起始充电脉冲占空比系数
uint32 gul_start_pwm;//起始充电脉冲占空比

float gf_power_gradient;//功率变化率
float gf_voltage_output_P;//恒压输出充电比例系数
float gf_voltage_output_I;//恒压输出充电积分系数
float gf_voltage_input_P;//恒压输入充电比例系数
float gf_voltage_input_I;//恒压输入充电积分系数
float gf_current_output_P;//恒流充电比例系数
float gf_current_output_I;//恒流充电积分系数
float gf_power_P;//恒功率充电比例系数
float gf_power_I;//恒功率充电积分系数
float gf_power_D;//恒功率充电微分系数

int32 gl_voltage_input;//输入电压
int32 gl_voltage_output;//输出电压
int32 gl_current_input;//输出电流
int32 gl_power_output;//输出功率

