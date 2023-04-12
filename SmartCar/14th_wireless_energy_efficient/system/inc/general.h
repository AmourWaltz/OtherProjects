#ifndef _GENERAL_H_
#define _GENERAL_H_

#include "common.h"
#include "include.h"

#define FLAG_OFF 0
#define FLAG_ON 1

#define  LEAST_SQUARES_NUMBERS    (4)             // N为要拟合的数据的个数

/*
 * 求最大值和最小值
 */
#define MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#define MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )

/*
 * 宏定义实现返回绝对值（x里不能有自加自减的语句，否则变量出错）
 */
#define ABS(x) (((x) > 0) ? (x) : (-x))

/*
 * 返回数组元素的个数
 */
#define ARR_SIZE( a ) ( sizeof( (a) ) / sizeof( ((a)[0]) ) )


float Limiter(float value, float min, float max);
void least_spuare(int *array, int lenth, float *k, float * b, float *r);		//最小二乘法拟合
void LinerFit(int16 *x, int16 *y, uint8 u8Num, float *pa, float *pb, float *pr_2, int32 *p100a);
int16 Sigma(int16 *Data, uint8 u8Num);
float Average(int16 *Data, uint8 u8Num);
int32 SigmaXY(int16 *x, int16 *y, uint8 u8Num);
int32 SigmaXX(int16 *Data, uint8 u8Num);  
void Delay_ms(uint32 u32time);
uint8 sleep_ms(uint32 u32time,uint8 channel);//定时
uint32 time_interval_100us(uint8 channel);//可以提供多个计时通道
//void getVariance(int16* data,int start,int end,int32* variance);
void getSimpilfiedVariance(int16 *data, int start,int end, int32* variance);
int16 getSimpilfiedAverage(int16 *data,int start,int end);
int32 power(int16 data, int16 index);
int16 _100aToPointAngle(int32 i32_100a);
float GP_tan(float x);
int16 least_squares_value(int16 *p_sensor_value);
float f_sqrt(float a);
uint32 i_sqrt(uint32 a);

#endif