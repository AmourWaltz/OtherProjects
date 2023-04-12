#ifndef _FIRE_OV7725_EAGLE_H_
#define _FIRE_OV7725_EAGLE_H_

#include "FIRE_OV7725_REG.h"

//配置摄像头的特性
#define CAMERA_USE_HREF     1               //是否使用 行中断 (0 为 不使用，1为使用)
#define CAMERA_COLOR        0               //摄像头输出颜色 ， 0 为 黑白二值化图像 ，1 为 灰度 图像 ，2 为 RGB565 图像
#define CAMERA_POWER        0               //摄像头 电源选择， 0 为 3.3V ,1 为 5V

typedef struct
{
    uint8 addr;                 /*寄存器地址*/
    uint8 val;                   /*寄存器值*/
} reg_s;

extern  uint8   ov7725_eagle_init();

#endif  //_FIRE_OV7725_EAGLE_H_


