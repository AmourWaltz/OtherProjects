/* =================================================
 *
 *@file: car_test.h
 *
 *@brief: 小车各个硬件模块测试头文件
 *
 *@verison: 0.1
 *
 *=================================================*/

//使用前会关闭1ms PIT中断 不关红外

#ifndef _CAR_TEST_H
#define _CAR_TEST_H

#include "include.h"
#include "common.h"

//测试函数页数最大和最小宏定义
//注:如果增加了新的测试页面，需将 TEST_PAGE_MAX 改为相应的数值
#define TEST_PAGE_MIN (1)//1
#define TEST_PAGE_MAX (6)

/*
 *简述：  小车硬件测试函数，测试 led 拨码 液晶 红外 电机 舵机 编码器
 *      外部函数 被 sysint() 调用
 *      调用了 servo_test
 *            led_test
 *            motor_test
 *            servo_test
 *            test_exit
 *            tele_test
 *            encoder_test几个函数
 *
 *注意： 在函数被调用前pit必须初始化，因为测速是在PIT里进行的，否则将读不到脉冲数和速度
 *     按下复位同时按下五项按键下键进入小车测试
 *
 *参数： void
 *
 *返回： void
 *
 */
void Smartcar_Test();

uint8 switch_test(uint8 page_num);
uint8 led_test(uint8 page_num);
uint8 infrared_test(uint8 page_num);

uint8 test_exit(uint8 page_num);

#endif