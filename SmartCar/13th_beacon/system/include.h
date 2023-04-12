/*=============================================
    @file         include.h
    @brief        小车参变量头文件设置      
    @programmer   XBY
=============================================*/
#ifndef _INCLUDE_H_
#define _INCLUDE_H_


//#define USE_INFR      //是否使用红外



#define         TIMER_PROCESS_CH        4
#define         TIMER_IMAGE_CH          7

#define         LEFT                    1
#define         RIGHT                   2

#define         FRONT                   0
#define         BACK                    1

#define         ON                      1
#define         OFF                     0

#define         TURE                    1
#define         FALSE                   0

#include "common.h"
#include "math.h"

//debug
#include "slave_computer.h"
#include "car_test.h"
#include "display_image.h"
#include "flash.h"
#include "oled.h"
#include "scope.h"
#include "tele.h"
#include "dir_kbi.h"
#include "switch.h"
#include "beep.h"
#include "infrared_decode.h"
#include "led.h"

//device
#include "camera_digital.h"
#include "motor.h"
#include "MPU6050.h"
#include "servo.h"
#include "encoder.h"
#include "FIRE_OV7725_REG.h"
#include "FIRE_OV7725_Eagle.h"


//driver
#include "adc.h"
#include "gpio.h"
#include "I2C.h"
#include "nvic.h"
#include "pit.h"
#include "pwm.h"
#include "uart.h"
#include "rtc.h"
#include "VCAN_SCCB.h"
#include "GPT.h"


//signal
#include "avoidance.h"
#include "image.h"
#include "route.h"
#include "display.h"
#include "measure.h"
#include "inductor.h"
#include "directional.h"


//control
#include "speedfast.h"
#include "direction.h"

//system
#include "interrupt.h"
#include "init.h"
#include "general.h"
#include "TimeStamp.h"

//小车状态变量参数
extern uint32 gul_time_100us;//0.1ms计时，用于红外传感器
extern uint32 gul_time_start;//起始发车时间

extern uint8 guc_beep_CTL_flag;//小车起跑标志位
extern uint8 guc_car_run_flag;//小车起跑标志位

extern int16 oled_data[16];
extern uint8 guc_switch_pin_val[2];//拨码值存储
extern float gf_VisualScopeData[10];//传送给虚拟示波器的数据

//速度
extern float gf_speed_p;//速度控制比例系数
extern float gf_speed_i;//速度控制积分系数
extern float gf_speed_d;//速度控制微分系数
extern float gf_angle_speed_kp;//弯道减速系数
extern float gf_dece_kp;//图像面积减速系数
extern float gf_acce_kp;//图像面积加速系数
extern int16 gi_area_thredhold_1;//一段减速面积阈值
extern int16 gi_area_thredhold_2;//二段减速面积阈值

extern int32 gl_car_speed;//小车速度检测值
extern int32 gl_car_running_distance;//行驶路程，编码器检测
extern int32 gl_speed_set;//速度设定值 
extern int32 gl_pwm_out;//电机PWM输出,数值为实际电机在速度分量上的PWM输出
extern int32 gl_div_pwm_out;//左右轮差速
extern int16 gi_left_speed;//左轮速度
extern int16 gi_right_speed;//右轮速度
extern int32 gl_left_encoder_count;//左轮编码器计数
extern int32 gl_right_encoder_count;//右轮编码器计数

//方向
extern int16  gi_image_err;//舵机正向与图像中心偏差值

extern float gf_direction_p;//方向控制比例系数
extern float gf_direction_d;//方向控制微分系数

extern int32  gl_direction_set;	//小车舵机打角值  
extern int32  gl_servo_angle;//舵机打角角度
extern int32  gl_servo_pwm;//舵机输出PWM
extern int32  gl_servo_center;//舵机中心值对应的PWM

//电流环
extern uint16 gui_16bat_voltage;
extern uint16 gui_16bat_ADC;

    
//图像
extern int16  gi_edge_left[IMAGE_HEIGHT];//信标灯左边缘
extern int16  gi_edge_right[IMAGE_HEIGHT];//信标灯右边缘
extern int16  gi_edge_left_value;//左侧边界值
extern int16  gi_edge_right_value;//右侧边界值
extern int16  gi_left_count;//左边界行数
extern int16  gi_right_count;//右边界行数
extern int16  gi_image_center;//信标横向中心
extern int16  gi_image_vertical;//信标纵向中心
extern int16  gi_end_line;//起始行
extern int16  gi_start_line;//截止行
extern int16  gi_image_area;//信标矫正后面积
extern uint8  guc_find_image_flag;//视野是否找到标志位
extern uint8  guc_center_flag;//切灯方位标志位
extern float  gf_center_err_P;//旁侧切灯中心偏差比例
extern float  gf_beacon_offset_P;//切灯系数
extern int8  gc_left_offset;//左切误差弥补
extern int8  gc_right_offset;//右切误差弥补

extern int16  gi_image_left_ave;//均值处理后左边界
extern int16  gi_image_right_ave;//均值处理后右边界
extern int16  gi_image_center_ave;//均值后中心线
extern int16  gi_image_area_ave;//均值后面积
extern int16  gi_image_start_ave;//均值处理后上边界
extern int16  gi_image_end_ave;//均值处理后下边界
extern int16  gi_image_vertical_ave;//均值后纵向值

extern int8   gc_not_found_count;//判断失去视野的采集次数


extern vuint8 imageshow[IMAGE_HEIGHT][IMAGE_WIDTH];//图像显示数组
extern vuint8 (*guc_cur_image)[IMAGE_WIDTH];//图像处理临时储存数组
extern uint8  guc_corrected_image[IMAGE_HEIGHT][IMAGE_WIDTH];//取行列后的图像


//摄像头
extern uint8  (*guc_image_source)[CAMERA_W];
extern uint8  guc_camera_image_Ok;//显示图像标志位
extern uint32 gul_image_period;
extern uint32 gul_image_process_time;
extern uint8  g_u8DigitalCameraThreshold;//摄像头阈值，为表示对野火底层尊重，此变量名不做修改

extern int img_furthest_line;

//超声波测距
extern int16 gi_ultrasonic_distance;//超声波测距

//超声波定向
extern int16 gi_ultrasonic_value[NUM_ULTRASONIC];//超声波AD转换值
extern int16 gi_ultrasonic_history[NUM_ULTRASONIC][NUM_ULTRASONIC_HISTORY];//超声波AD转换值历史值
//extern int16 gi_ultrasonic_offset_x;//超声波横向差比和
extern int16 gi_ultrasonic_offset_y;//超声波纵向差比和
//extern float gf_ultrasonic_angle;//超声波定向角度
extern uint8 guc_beacon_count;//超声波跳变灭灯计数
extern uint8 guc_beacon_directional_x;//超声波x轴定向
extern uint8 guc_beacon_directional_y;//超声波y轴定向

//红外避障
extern int16 gi_tele_barrier_distance;//红外测得障碍物距离
extern int8 gc_tele_barrier_count;//红外避障计数

//红外定向
extern uint8 guc_infrared_value[8];//八路红外检测电平

#endif