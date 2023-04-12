//#ifndef _MPU6050_H__
//#define _MPU6050_H__
//
//#include "common.h"
//#include "I2C.h"
//
////MPU6050内部寄存器
//#define	SMPLRT_DIV		(0x19)	//陀螺仪采样率，典型值：0x07(125Hz)
//#define	CONFIG			(0x1A)	//低通滤波频率，典型值：0x06(5Hz)
//#define	GYRO_CONFIG		(0x1B)	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
//#define	ACCEL_CONFIG	        (0x1C)	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
//#define	ACCEL_XOUT_H	        (0x3B)  //X轴加速度  加速度
//#define	ACCEL_XOUT_L	        (0x3C)
//#define	ACCEL_YOUT_H	        (0x3D)  //Y轴加速度
//#define	ACCEL_YOUT_L	        (0x3E)
//#define	ACCEL_ZOUT_H	        (0x3F)  //Z轴加速度
//#define	ACCEL_ZOUT_L	        (0x40)
//#define	TEMP_OUT_H		(0x41)
//#define	TEMP_OUT_L		(0x42)
//#define	GYRO_XOUT_H		(0x43)  //X轴角速度  陀螺仪
//#define	GYRO_XOUT_L		(0x44)	
//#define	GYRO_YOUT_H		(0x45)  //Y轴角速度
//#define	GYRO_YOUT_L		(0x46)
//#define	GYRO_ZOUT_H		(0x47)  //Z轴角速度
//#define	GYRO_ZOUT_L		(0x48)
//#define	PWR_MGMT_1		(0x6B)	//电源管理，典型值：0x00(正常启用)
//#define	WHO_AM_I		(0x75)	//IIC地址寄存器(默认数值0x68，只读)
//#define	MPU6050_I2C_ADDRESS	(0xD0)	//硬件I2C地址0x68 软件I2C地址0xD0
//#define	MPU6050_I2C_DEV_ADR	(0x68)	//硬件I2C地址0x68 软件I2C地址0xD0
//
////定义SCL Bus Speed取值，外设总线为50Mhz时的计算结果
//#define MPU6050_SCL_50KHZ       (0x33)
//#define MPU6050_SCL_100KHZ      (0x2B)
//#define MPU6050_SCL_150KHZ      (0x28)
//#define MPU6050_SCL_200KHZ      (0x23)
//#define MPU6050_SCL_250KHZ      (0x21)
//#define MPU6050_SCL_300KHZ      (0x20)
//#define MPU6050_SCL_400KHZ      (0x17)
//
//#define ACCE_X_6050             (ACCEL_XOUT_H)
//#define ACCE_Y_6050             (ACCEL_YOUT_H)
//#define GYRO_X_6050             (GYRO_XOUT_H)
//#define GYRO_Y_6050             (GYRO_YOUT_H)
//
////函数声明
//int16 MPU6050_GetResult(uint8 Regs_Addr);
//void  MPU6050_Init();
//int16 I2C_GYRO_MPU6050_Get(char axis);
//int16 I2C_ACCE_MPU6050_Get(char axis);
//void I2C_ACCE_MPU6050_Get_ALL(int16 *acce_x,int16 *acce_y,int16 *acce_z);
//void I2C_GYRO_MPU6050_Get_ALL(int16 *gyro_x,int16 *gyro_y,int16 *gyro_z);
//#endif