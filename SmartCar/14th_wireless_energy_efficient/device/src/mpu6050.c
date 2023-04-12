/*=============================================
    @file         mpu6050.c
    @brief        惯性测量单元
    @programmer   Beyond Hsueh
=============================================*/

#include "mpu6050.h"

/*===============================================================
        @note      明月多情应笑我，笑我如今；
                   辜负春心，独自闲行独自吟。
                   近来怕说当时事，结编兰襟；
                   月浅灯深，梦里云归何处寻？
==================================================================*/     

float mpu_gyro_x,mpu_gyro_y,mpu_gyro_z;
float mpu_acce_x,mpu_acce_y,mpu_acce_z;

//加速度计陀螺仪三轴零漂校准
float gf_gyro_bias_y;
float gf_gyro_bias_x;
float gf_gyro_bias_z;
float gf_acce_bias_y;
float gf_acce_bias_x;
float gf_acce_bias_z;

/*===============================================================
	@brief     mpu6050初始化函数
==================================================================*/
void MPU6050_Init()
{   
    I2C_WriteReg(MPU6050_I2C_ADDRESS, PWR_MGMT_1,0x00);    //解除休眠状态
    I2C_WriteReg(MPU6050_I2C_ADDRESS, SMPLRT_DIV,0x00);    //陀螺仪采样率，典型值：0x07(125Hz)
    I2C_WriteReg(MPU6050_I2C_ADDRESS, CONFIG,0x00);        //低通滤波频率，典型值：0x06(5Hz)
    I2C_WriteReg(MPU6050_I2C_ADDRESS, GYRO_CONFIG,0x08);   //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
    I2C_WriteReg(MPU6050_I2C_ADDRESS, ACCEL_CONFIG,0x00);  //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
}

/*===============================================================
	@brief     获取陀螺仪三轴数值
==================================================================*/
void Get_Gyro_All()
{
    static uint8 gyro[6] = {0};
    
    GPIO_I2C_Read_N(MPU6050_I2C_ADDRESS, GYRO_XOUT_H, gyro, 6);
    
    mpu_gyro_x = ((int16)((int16)gyro[0]<<8 | gyro[1]));
    mpu_gyro_y = ((int16)((int16)gyro[2]<<8 | gyro[3]));
    mpu_gyro_z = ((int16)((int16)gyro[4]<<8 | gyro[5]));     //0.01526=1000/65536
}

void Get_Gyro_X()
{  
    uint8 gyro_x_data[2] = {0};
    GPIO_I2C_Read_N(MPU6050_I2C_ADDRESS, GYRO_XOUT_H, gyro_x_data, 2);
    mpu_gyro_x = ((int16)((int16)gyro_x_data[0]<<8 | gyro_x_data[1]))*GYR_RATIO - gf_gyro_bias_x;//0.0305=2000/65536

}

void Get_Gyro_Y()
{
    uint8 gyro_y_data[2] = {0};
    GPIO_I2C_Read_N(MPU6050_I2C_ADDRESS, GYRO_YOUT_H, gyro_y_data, 2);
    mpu_gyro_y = ((int16)((int16)gyro_y_data[0]<<8 | gyro_y_data[1]))*GYR_RATIO - gf_gyro_bias_y;//0.0305=2000/65536
}

void Get_Gyro_Z()
{
    uint8 gyro_z_data[2] = {0};
    GPIO_I2C_Read_N(MPU6050_I2C_ADDRESS, GYRO_ZOUT_H, gyro_z_data, 2);
    mpu_gyro_z = ((int16)((int16)gyro_z_data[0]<<8 | gyro_z_data[1]))*GYR_RATIO - gf_gyro_bias_z;//0.0305=2000/65536
}

/*===============================================================
	@brief     获取加速度计三轴数值
==================================================================*/
void Get_Acc_All()
{
    uint8 acce[6] = {0};
    GPIO_I2C_Read_N(MPU6050_I2C_ADDRESS, ACCEL_XOUT_H, acce, 6);
    mpu_acce_x = ((int16)((int16)acce[0]<<8 | acce[1]));
    mpu_acce_y = ((int16)((int16)acce[2]<<8 | acce[3]));
    mpu_acce_z = ((int16)((int16)acce[4]<<8 | acce[5]));
}

void Get_Acc_X()
{
    uint8 acce_x_data[2] = {0};
    GPIO_I2C_Read_N(MPU6050_I2C_ADDRESS, ACCEL_XOUT_H, acce_x_data, 2);
    mpu_acce_x = (float)((int16)((int16)acce_x_data[0]<<8 | acce_x_data[1]));//0.0024=2*8*9.8/65536
}

void Get_Acc_Y()
{    
    uint8 acce_y_data[2] = {0};
    GPIO_I2C_Read_N(MPU6050_I2C_ADDRESS, ACCEL_YOUT_H, acce_y_data, 2);
    mpu_acce_y = (float)((int16)((int16)acce_y_data[0]<<8 | acce_y_data[1]));
}

void Get_Acc_Z()
{
    uint8 acce_z_data[2] = {0};
    GPIO_I2C_Read_N(MPU6050_I2C_ADDRESS, ACCEL_ZOUT_H, acce_z_data, 2);
    mpu_acce_z = (float)((int16)((int16)acce_z_data[0]<<8 | acce_z_data[1]));
}

/*===============================================================
	@brief     消除零漂误差
==================================================================*/
void GYRO_Set_Zero()
{
    int16 gyro_buff[3][GYRO_BUFF_SIZE];//一次采集20个数据，分组处理
    int32 gyro_aver[3];//平均值
    int16 acce_data[3];//缓存
    int32 acce_aver[3];//平均值
    int32 gyro_S2[3];//方差
    float gyro_zero_temp[3];
    float acce_zero_temp[3];
    int16 err_temp = 0;
    int16 i=0, j=0, k = 0;
    int16 zero_counter = 0;
    uint8 button_value = 0;
    uint8 exit_flag = 0;
//    int8 s[30];
    
    oled_fill(0x00);
//    oled_display_16x8str(0,0,"GYRO SET ZERO...");
//    oled_display_6x8str(0,3,"S2");
//    oled_display_6x8str(0,5,"ZERO");
    gyro_zero_temp[0] = 0;
    gyro_zero_temp[1] = 0;
    gyro_zero_temp[2] = 0;
    acce_zero_temp[0] = 0;
    acce_zero_temp[1] = 0;
    acce_zero_temp[2] = 0;
      
    while(k<1000)
    {
        k++;

        for(j=0;j<3;j++)
        {
            gyro_aver[j] = 0;
            gyro_S2[j] = 0;
            acce_aver[j] = 0;
        }
        for(i=0;i<GYRO_BUFF_SIZE;i++)   
        {
            while(!sleep_ms(1,2));      //延时，稳定采样周期1ms，采样周期太小不利于判别姿态
            
                Get_Gyro_All();
                Get_Acc_All();

              
                gyro_buff[0][i] = (int16)mpu_gyro_x;
                gyro_buff[1][i] = (int16)mpu_gyro_y;
                gyro_buff[2][i] = (int16)mpu_gyro_z;
                acce_data[0] = (int16)mpu_acce_x;
                acce_data[1] = (int16)mpu_acce_y;
                acce_data[2] = (int16)mpu_acce_z;
                
                
                gyro_aver[0] += gyro_buff[0][i];
                gyro_aver[1] += gyro_buff[1][i];
                gyro_aver[2] += gyro_buff[2][i];
                acce_aver[0] += acce_data[0];
                acce_aver[1] += acce_data[1];
                acce_aver[2] += acce_data[2];
                
//            button_value = button_scan();
//            if(button_value == FIVE_DIR_OK)
//            {
//                exit_flag = 1;
//            }
              button_value = get_infrared_command();
              if(button_value == COMMAND_POWER)
              {
                exit_flag = 1;
              }
      
        }
        for(j=0;j<3;j++)        //计算方差
        {
            gyro_aver[j] /= GYRO_BUFF_SIZE;
            acce_aver[j] /= GYRO_BUFF_SIZE;
            for(i=0;i<GYRO_BUFF_SIZE;i++)
            {
                err_temp = gyro_buff[j][i] - gyro_aver[j];
                gyro_S2[j] += err_temp * err_temp;
            }
            gyro_S2[j] /= GYRO_BUFF_SIZE;   
        }
        if((gyro_S2[0]<400)        //三轴方差均较小，说明车姿态稳定，可以调零
           &&(gyro_S2[1]<400)
           &&(gyro_S2[2]<400))
        {
            for(j=0;j<3;j++)
            {
                gyro_zero_temp[j] += gyro_aver[j];
                acce_zero_temp[j] += acce_aver[j];
            }    
            zero_counter++;
        }
        if(zero_counter>=12)
        {
            //oled_display_16x8str(0,0,"SET ZERO SUCCESS!");
            for(j=0;j<3;j++)
            {
                gyro_zero_temp[j] /= zero_counter;
                acce_zero_temp[j] /= zero_counter;
            }
            gf_gyro_bias_x = gyro_zero_temp[0] * GYR_RATIO;
            gf_gyro_bias_y = gyro_zero_temp[1] * GYR_RATIO;
            gf_gyro_bias_z = gyro_zero_temp[2] * GYR_RATIO;
            gf_acce_bias_x = acce_zero_temp[0];
            gf_acce_bias_y = acce_zero_temp[1];
            gf_acce_bias_z = acce_zero_temp[2];
            
            guc_gyro_zero_flag = ON;
                
            break;
        }
        else if(exit_flag)
        {
            //oled_display_16x8str(0,0,"SET ZERO ABROATED!");
            break;
        }
        
        oled_display_chinese(30, 3, PERFECT_MAKING, LOW_LIGHT_MODE);//止于至善
    
////        sprintf(s,"%5d",k);
////        oled_display_6x8str(0,2,s);
////        sprintf(s,"X:%4dY:%4dZ:%4d",gyro_S2[0],gyro_S2[1],gyro_S2[2]);
////        oled_display_6x8str(0,4,s);
////        sprintf(s,"X:%.3iY:%.3iZ:%.3i",(float)(gyro_zero_temp[0]/zero_counter),(float)(gyro_zero_temp[1]/zero_counter),(float)(gyro_zero_temp[2]/zero_counter));
////        oled_display_6x8str(0,6,s);
//        oled_print_short(0, 2, k);
////        oled_display_6x8str(0,4,"x");
////        oled_display_6x8str(40,4,"y");
////        oled_display_6x8str(80,4,"z");
//        oled_print_short(4, 4, gyro_S2[0]);
//        oled_print_short(44, 4, gyro_S2[1]);
//        oled_print_short(84, 4, gyro_S2[2]);
////        oled_display_6x8str(0,6,"x");
////        oled_display_6x8str(40,6,"y");
////        oled_display_6x8str(80,6,"z");
//        oled_print_short(4, 6, (int16)(100*(gyro_zero_temp[0]/zero_counter)));
//        oled_print_short(44, 6, (int16)(100*(gyro_zero_temp[0]/zero_counter)));
//        oled_print_short(84, 6, (int16)(100*(gyro_zero_temp[0]/zero_counter)));
    }
    
    //while(!sleep_ms(500,3));
    
    oled_fill(0x00);
}