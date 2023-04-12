#ifndef _CHARGE_H_
#define _CHARGE_H_

#include "common.h"
#include "include.h"

/*===============================================================
        @note      今古河山无定据，画角声中，牧马频来去。
                   满目荒凉谁可语？西风吹老丹枫树。
                   从前幽怨应无数，铁马金戈，青冢黄昏路。
                   一往情深深几许？深山夕照深秋雨。
==================================================================*/

#define VOLTAGE_START_COUNT             10//启动电压检测次数
#define MAX_PWM_INC                     70//增量式电压单次输出限幅 
#define MAX_PWM_INC_DELAY               10//增量式电压单次输出限幅 
#define MAX_CHARG_PWM                   6000//增量式电压输出限幅
#define CEASE_VIBRATE_BUFF_LENGTH       10//停震检测缓存队列长度
#define CEASE_VIBRATE_INTERVAL_SET      (500*100)//停震间隔时间设定
#define POWER_QUEUE_LENGTH              20//功率检测队列长度
#define CEASE_VIBRATE_QUEUE_LENGTH      1//停震计时队列长度

#define CHARGE_RESISTANCE               300//充电电阻 
#define CHARGE_CAPACITANCE              7//充电电容

/*===============================================================
        @note      长安古道马迟迟，高柳乱蝉嘶；
                   夕阳岛外，秋风原上，目断四天垂。
                   归云一去无踪迹，何处是前期？
                   狎兴生疏，酒徒萧索，不似少年时。
==================================================================*/


void Charge_Process();//充电过程处理

void Charge_Control();//充电控制

void Voltage_Control(int16 voltage_set);//增量式电压控制输出PWM

void Constant_Power_Control(int32 power_set);//恒功率控制输出PWM

void Battery_Control(int16 voltage_set);//电容电压控制输出PWM

uint8 Detect_Cease_Vibrate();//功率及输入电压下降检测停震

int32 Detect_Power_Increase();//检测功率上升

void Constant_Power_Charge_Test();//恒功率充电测试

uint8 Charge_Start_Test();//充电启动测试

void Set_Start_Voltage();//设置启动电压

void Set_Charge_PWM();//设定充电PWM



#endif