/*=============================================
    @file         kalman.c
    @brief        卡尔曼滤波
    @programmer   Beyond Hsueh
=============================================*/

#include "kalman.h"

/*===============================================================
        @note      对酒当歌，人生几何！譬如朝露，去日苦多。
                   慨当以慷，忧思难忘。何以解忧？唯有杜康。
                   青青子衿，悠悠我心。但为君故，沉吟至今。
                   呦呦鹿鸣，食野之苹。我有嘉宾，鼓瑟吹笙。
                   明明如月，何时可掇？忧从中来，不可断绝。
                   越陌度阡，枉用相存。契阔谈讌，心念旧恩。
                   月明星稀，乌鹊南飞。绕树三匝，何枝可依？
                   山不厌高，海不厌深。周公吐哺，天下归心。
==================================================================*/ 

/*===============================================================
	@brief     卡尔曼滤波
==================================================================*/
float Kalman_Fliter(float acce_angle, float gyro_angle,char ch)
{
    static float sf_last_offset[5] = {0.5,0.5,0.5,0.5,0.5};
    float predict_offset;
    float car_angle; 
    float predict_angle;
    float kg;

    predict_angle = gyro_angle;//系统预测角度为陀螺仪测量角度
    predict_offset = sf_last_offset[ch] + GYRO_ANGLE_NOISE;//计算系统方差
    kg = sqrt(predict_offset / (predict_offset + ACCE_ANGLE_NOISE));//计算卡尔曼增益
    car_angle = predict_angle + kg * (acce_angle - predict_angle);//

    sf_last_offset[ch] = (1 - kg) * predict_offset;//最终角度方差

    return car_angle;
}

/*===============================================================
	@brief     获取小车角度
==================================================================*/
void Get_Car_Angle()
{
    float gyro_dx_angle,gyro_dy_angle,gyro_dz_angle;
    static uint8 flag = 0;
    static float sinx,cosx,siny,cosy;
    static float sf_x_angle_rad, sf_y_angle_rad;
    
    gf_acce_mps2 = i_sqrt((uint32)(mpu_acce_x * mpu_acce_x + mpu_acce_y * mpu_acce_y + mpu_acce_z * mpu_acce_z));
    
    gf_acce_yz_angle = 180 / PI * atan2f(mpu_acce_y, mpu_acce_z);
    gf_acce_xz_angle = 180 / PI * atan2f(mpu_acce_z, mpu_acce_x - gl_car_speed * mpu_gyro_z * PI / 180.0);//修正 车身运动的加速度产生的影响

    sf_x_angle_rad = gf_gyro_x_angle / 180 * PI;
    sf_y_angle_rad = gf_gyro_y_angle / 180 * PI;
    gyro_dx_angle = mpu_gyro_x * (PIT_1_PERIOD_US / 1000000.0);
    gyro_dy_angle = mpu_gyro_y * (PIT_1_PERIOD_US / 1000000.0);
    gyro_dz_angle = mpu_gyro_z * (PIT_1_PERIOD_US / 1000000.0);
    
    sinx = sinf(sf_x_angle_rad);
    siny = sinf(sf_y_angle_rad);
    cosx = cosf(sf_x_angle_rad);
    cosy = cosf(sf_y_angle_rad);
    
    gf_gyro_x_angle += gyro_dx_angle * siny - gyro_dz_angle * cosy;//姿态补偿
    gf_gyro_y_angle += gyro_dy_angle * cosx - gyro_dz_angle * sinx;
    gf_gyro_z_angle += gyro_dz_angle;
    
    if(flag == 0)
    {
        gf_acce_xy_angle = 180 / PI * atan2f(gf_acce_bias_x, gf_acce_bias_y);
        gf_acce_yz_angle = 180 / PI * atan2f(gf_acce_bias_y, gf_acce_bias_z);
        gf_acce_xz_angle = 180 / PI * atan2f(gf_acce_bias_z, gf_acce_bias_x);
        
        gf_gyro_z_angle = gf_acce_xy_angle;
        gf_gyro_x_angle = gf_acce_yz_angle;
        gf_gyro_y_angle = gf_acce_xz_angle;
        
        sf_x_angle_rad = gf_gyro_x_angle / 180 * PI;
        sf_y_angle_rad = gf_gyro_y_angle / 180 * PI;
        
        sinx = sinf(sf_x_angle_rad);
        siny = sinf(sf_y_angle_rad);
        cosx = cosf(sf_x_angle_rad);
        cosy = cosf(sf_y_angle_rad);
            
        flag = 1;
    }

    if((mpu_acce_z > 700)
     &&((mpu_acce_z < 1200))
     &&((gf_acce_mps2) > 900))//坡道数据异常，影响积分准确性
    {
        gf_gyro_z_angle = Kalman_Fliter(gf_acce_xy_angle, gf_gyro_z_angle, KALMAN_CHANNEL_XY);

        gf_gyro_x_angle = Kalman_Fliter(gf_acce_yz_angle, gf_gyro_x_angle, KALMAN_CHANNEL_YZ);

        gf_gyro_y_angle = Kalman_Fliter(gf_acce_xz_angle, gf_gyro_y_angle, KALMAN_CHANNEL_ZX);
    }

    gf_roll_angle = 90 - gf_gyro_y_angle;
    gf_pitch_angle = - gf_gyro_x_angle;

    gf_acce_angle = atan2(gf_acce_xz_angle, gf_acce_yz_angle);//获取加速度角度

    gf_gyro_angle = gf_car_angle + (0.001 * PIT_1_PERIOD_MS) * gf_roll_rotate;//计算陀螺仪角度
    
    gf_car_angle = Kalman_Fliter(gf_acce_angle, gf_gyro_angle, KALMAN_CHANNEL_AG);//卡尔曼滤波
}

/*===============================================================
	@brief     获取车体姿态
        @note      计算小车在三个坐标轴方向的角速度
==================================================================*/
void Get_Car_Posture()
{
    float car_angle_rad;

    car_angle_rad = PI / 180 * gf_car_angle;

    gf_yaw_rotate = -sin(car_angle_rad) * mpu_gyro_y + cos(car_angle_rad) * mpu_gyro_z;
    gf_roll_rotate = cos(car_angle_rad) * mpu_gyro_y - sin(car_angle_rad) * mpu_gyro_z;
    
}
