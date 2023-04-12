#ifndef _MPU6050_H
#define _MPU6050_H

#include "common.h"
#include "include.h"

/*===============================================================
        @note      弃我去者，昨日之日不可留；
                   乱我心者，今日之日多烦忧。
                   长空万里送秋雁，对此可以酣高楼；
                   蓬莱文章建安骨，中间小谢又清发；
                   俱怀逸兴壮思飞，欲上青天揽明月；
                   抽刀断水水更流，举杯消愁愁更愁；
                   人生在世不称意，明朝散发弄扁舟。
==================================================================*/

#define  GYR_RATIO                      0.015//陀螺仪系数调整

#define GYRO_BUFF_SIZE                  40

//MPU6050内部寄存器
#define	SMPLRT_DIV		        (0x19)//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			        (0x1A)//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		        (0x1B)//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	                (0x1C)//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	                (0x3B)//X轴加速度  加速度
#define	ACCEL_XOUT_L	                (0x3C)
#define	ACCEL_YOUT_H	                (0x3D)//Y轴加速度
#define	ACCEL_YOUT_L	                (0x3E)
#define	ACCEL_ZOUT_H	                (0x3F)//Z轴加速度
#define	ACCEL_ZOUT_L	                (0x40)
#define	TEMP_OUT_H		        (0x41)
#define	TEMP_OUT_L		        (0x42)
#define	GYRO_XOUT_H		        (0x43)//X轴角速度  陀螺仪
#define	GYRO_XOUT_L		        (0x44)	
#define	GYRO_YOUT_H		        (0x45)//Y轴角速度
#define	GYRO_YOUT_L		        (0x46)
#define	GYRO_ZOUT_H		        (0x47)//Z轴角速度
#define	GYRO_ZOUT_L		        (0x48)
#define	PWR_MGMT_1		        (0x6B)//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		        (0x75)//IIC地址寄存器(默认数值0x68，只读)
#define	MPU6050_I2C_ADDRESS	        (0xD0)//硬件I2C地址0x68 软件I2C地址0xD0
#define	MPU6050_I2C_DEV_ADR	        (0x68)//硬件I2C地址0x68 软件I2C地址0xD0

//定义SCL Bus Speed取值，外设总线为50Mhz时的计算结果
#define MPU6050_SCL_50KHZ               (0x33)
#define MPU6050_SCL_100KHZ              (0x2B)
#define MPU6050_SCL_150KHZ              (0x28)
#define MPU6050_SCL_200KHZ              (0x23)
#define MPU6050_SCL_250KHZ              (0x21)
#define MPU6050_SCL_300KHZ              (0x20)
#define MPU6050_SCL_400KHZ              (0x17)

extern float mpu_gyro_x,mpu_gyro_y,mpu_gyro_z;
extern float mpu_acce_x,mpu_acce_y,mpu_acce_z;

//加速度计陀螺仪三轴零漂校准
extern float gf_gyro_bias_y;
extern float gf_gyro_bias_x;
extern float gf_gyro_bias_z;
extern float gf_acce_bias_y;
extern float gf_acce_bias_x;
extern float gf_acce_bias_z;



void MPU6050_Init();//mpu6050初始化函数

void Get_Gyro_All();//获取陀螺仪三轴数值

void Get_Acc_All();//获取加速度计三轴数值

void Get_Acc_X();//获取加速度计x轴数值

void Get_Acc_Y();//获取加速度计y轴数值

void Get_Acc_Z();//获取加速度计z轴数值

void Get_Gyro_X();//获取陀螺仪x轴数值

void Get_Gyro_Y();//获取陀螺仪y轴数值

void Get_Gyro_Z();//获取陀螺仪z轴数值

void GYRO_Set_Zero();//消除零漂误差



#endif