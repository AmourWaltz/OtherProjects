/*=============================================
    @file         include.h
    @brief        头文件
    @programmer   Beyond Hsueh
=============================================*/

#ifndef _INCLUDE_H_
#define _INCLUDE_H_

#include "common.h"//系统参数设置
#include "math.h"
//#include "stdio.h"

/*===============================================================
        @note      秋风清，秋月明，
                   落叶聚还散，寒鸦栖复惊。
                   相思相见知何日？此时此夜难为情。
                   入我相思门，知我相思苦，
                   长相思兮长相忆，短相思兮无穷极，
                   早知如此绊人心，何如当初莫相识。
==================================================================*/ 

#define USE_INFR 
//#define COMPETITION_MODE//比赛模式 
//#define AUTOMATION_MODE//自动发车模式

//#define BEYOND_CARRIER_NO1//银河战舰一号 
//#define BEYOND_CARRIER_NO2//银河战舰一号 

#define LEFT                    2
#define RIGHT                   1
#define STREIGHT                0

#define OFF                     0
#define ON                      1
#define OK                      1

#define FLAG_OFF                0
#define FLAG_ON                 1

#define OUTPUT_L                0
#define OUTPUT_H                1

#define CAR_RUN                 1
#define CAR_STOP                0

//计时通道
#define PIT0_TIMER_NUM          0
#define PIT1_TIMER_NUM          1
#define ADC_TIMER_NUM           2
#define PROCESS_TIMER_NUM       3
#define UART_TIMER_NUM          4
#define DISPALY_TIMER_NUM       5
#define CV_TIMER_NUM            6

//system 系统文件
#include "init.h"//初始化
#include "general.h"//常用算法集合
#include "car_info.h"//小车参数设置
#include "isr.h"//中断函数

//charge 无线充电
#include "buck.h"
#include "charge.h"
#include "circuit.h"

//control 运动控制
#include "direction.h"
#include "speed.h"

//debug  参数调试
#include "car_test.h"
#include "slave_computer.h"
#include "flash.h"
#include "oled.h"
#include "tele.h"
#include "infrared_decode.h"
#include "scope.h"
#include "display.h"

//device 外设驱动
#include "beep.h"
#include "dir_kbi.h"
#include "encoder.h"
#include "led.h"
#include "motor.h"
#include "mpu6050.h"
#include "switch.h"
#include "test.h"
#include "reed.h"
//#include "ultrasound.h"

//drivers 底层扩展
#include "adc.h"
#include "gpio.h"
#include "i2c.h"
#include "gpt.h"
#include "pwm.h"
#include "nvic.h"
#include "uart.h"
#include "rtc.h"
#include "pit.h"

//signal 信号处理
#include "calibration.h"
#include "inductor.h"
#include "signal.h"
#include "process.h"
#include "obstacle.h"
#include "kalman.h"
#include "double_ranging.h"


#endif