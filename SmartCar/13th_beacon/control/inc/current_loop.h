#ifndef _CURRENT_LOOP_H_
    #define _CURRENT_LOOP_H_
	
#define SAMPLE_R 0.001  //采样电阻	
#define REF_V 3.300       //参考电压
#define CUR_GAIN 20

#define LEFT_CURRENT_ZERO 2078
#define RIGHT_CURRENT_ZERO 2078

#define LEFT_CURRENT 0
#define RIGHT_CURRENT 1

extern float left_current;
extern float right_current;

void CurrentLoop();	
void UpdateLeftCurrent();
void UpdateRightCurrent();
void UpdateRefVoltage();
void UpdateBatteryADC();
void buff_ADC_data();







	
#endif