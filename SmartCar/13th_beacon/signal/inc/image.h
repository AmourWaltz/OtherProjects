#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "common.h"
#include "math.h"
#include "include.h"

#define NOT_FOUND_IMAGE         1
#define FOUND_IMAGE             0
#define NOT_FOUND               0xfff        //没有搜索到赛道


void Image_Process();
void Image_Center();
void Image_Area();
void Image_Pre_Process(int16 *left_edge, int16 *right_edge);
void Image_Filter();
void Image_Get_Edge(vuint8 cameraimage[IMAGE_HEIGHT][IMAGE_WIDTH], int16 *left_edge, int16 *right_edge);
void Track_Liner();
void Horizon_Count();
void Image_Get_Vertical(int16 *left_edge);
void Show_Image_Process();
void Average_Filter();

#endif
