/*=============================================
    @file         image.c
    @brief        图像处理
    @programmer   XBY
=============================================*/
#include "image.h"
#include "include.h"

//每行权重，视调试情况做修改
//vuint8 err_weight[IMAGE_HEIGHT]=
//{100,99,98,98,97,96,95,95,94,93,92,91,91,90,89,88,88,87,86,85,84,
//84,83,82,81,80,80,79,78,77,77,76,75,74,73,73,72,71,70,70,69,68,67,
//66,66,65,64,63,63,62,61,60,59,59,58,57,56,55,55,54,53,52,52,51
//};

////#define  BEACON_MAIN//开启主车程序
//#define  BEACON_STANDBY 
//
//#ifndef  BEACON_MAIN

/*===============================================================
	@brief     图像处理
	@function  所有图像处理函数调用函数
	@note      可外部调用
==================================================================*/

void Image_Process()
{
    Image_Filter();//二值图像去噪
    
    guc_cur_image = guc_corrected_image;//保存去噪后的图像数组
    
    Image_Get_Edge(guc_cur_image, gi_edge_left, gi_edge_right);//信标边线提取
    
    Image_Get_Vertical(gi_edge_left);//纵向边线提取

    Image_Pre_Process(gi_edge_left,gi_edge_right);//预处理矩形矫正
    
    Image_Area();//面积计算及连续采集
    
    Average_Filter();//均值滤波
    
//    Track_Liner();//切灯系数调整
    
    Horizon_Count();//无视野判断延时调整
    
    Image_Center();//旁侧切灯中心线定位
    
    Route_Planning();//路径规划
    
//    Direction_Control();

}

/*===============================================================
	@brief     图像去噪
	@function  二值图像处理椒盐噪声
	@note      相对于对原始灰度图进行中值滤波，采用这种滤波方法
                   简洁有效，处理速度快
==================================================================*/

void Image_Filter()
{
    int16 i,j;
    //连续采集三个点,三个点均为白点则认为采集到三个白点
    for(i=0; i<IMAGE_HEIGHT; i++)
    {
        for(j=1; j< IMAGE_WIDTH-1; j++)
        {
//            if(j < IMAGE_WIDTH-1)
//            {
//                if(guc_corrected_image[i][j]==1&&guc_corrected_image[i][j+1]==1&&guc_corrected_image[i][j+2]==1)
//                {
//                    guc_corrected_image[i][j]=1;
//                    guc_corrected_image[i][j+1]=1;
//                    guc_corrected_image[i][j+2]=1;
////                    guc_corrected_image[i][j+3]=1;
////                    guc_corrected_image[i][j+4]=1;
//                    j+=2;
//                }
//                else 
//                    guc_corrected_image[i][j]=0;
//            
//            }
//            if(j == IMAGE_WIDTH-1)
//            {
//                if(guc_corrected_image[i][j-2]==1&&guc_corrected_image[i][j-1]==1&&guc_corrected_image[i][j]==1)
//                {
//                    guc_corrected_image[i][j]=1;
//                }
//                else 
//                    guc_corrected_image[i][j]=0;
//            
//            }      
          
              if(guc_corrected_image[i][j]==1)
              {   
                  if((guc_corrected_image[i][j-1] == 1) || (guc_corrected_image[i][j+1] == 1))
                  {
                      guc_corrected_image[i][j]=1;
                  }
                  else 
                  guc_corrected_image[i][j]=0;
              }
        }
    }
}

/*===============================================================
	@brief      边线提取
	@function   去噪后二值化图像边线提取   
	@parameter1 图像接收数组
	@parameter2 左边线存储数组
	@parameter3 右边线存储数组
	@note       个人感觉还能继续优化，提高处理速度
==================================================================*/

void Image_Get_Edge(vuint8 cameraimage[IMAGE_HEIGHT][IMAGE_WIDTH], int16 *left_edge, int16 *right_edge)
{
    int linecount;
    int pointcount;
    uint8 last_point=0;
    int left_start_point;//寻找左边界扫描起始行
    int right_start_point;//寻找右边界扫描起始行
    int left_end_point;//寻找左边界扫描结束行
    int right_end_point;//寻找右边界扫描结束行
    
    vuint8 *image_line;

    image_line = cameraimage[0];
    guc_cur_image = cameraimage;

    for(linecount = 0;linecount < IMAGE_HEIGHT; linecount++)
    {
        image_line = cameraimage[linecount];
        if(linecount < 1)
        {
            left_start_point = 0;
            right_start_point = IMAGE_WIDTH-1;
            left_end_point = IMAGE_WIDTH-1;
            right_end_point = 0;
        }
        else if((left_edge[linecount-1] == NOT_FOUND)||(right_edge[linecount-1] == NOT_FOUND))
        {
            left_start_point = 0;
            right_start_point = IMAGE_WIDTH-1;
            left_end_point = IMAGE_WIDTH-1;
            right_end_point = 0;
        }
        else if(linecount < IMAGE_HEIGHT)
        {
            left_start_point = (int)Limiter(left_edge[linecount-1]-10,0,IMAGE_WIDTH-1);
            right_start_point = (int)Limiter(right_edge[linecount-1]+10,0,IMAGE_WIDTH-1);
            left_end_point = IMAGE_WIDTH-1;
            right_end_point = 0;
        }
        //从左向右搜寻左边界
        for(pointcount = left_start_point, last_point = 0;pointcount < left_end_point; pointcount++)
        {
            if((last_point==1)&&(image_line[pointcount]==1))
            {
                left_edge[linecount] = pointcount;
                {
                  break;
                }    
            }
            last_point = image_line[pointcount];
        }
        if(pointcount==left_end_point)
        {
            if(last_point == 0)
            {
                 left_edge[linecount] = NOT_FOUND;
            }
            else if(last_point == 1)
            {
                 left_edge[linecount] = left_end_point;
            }
        }
        //从右向左搜右边界 
        for(pointcount = right_start_point,last_point = 0;pointcount > right_end_point;pointcount--)
        {
            if((last_point==1)&&(image_line[pointcount]==1))
            {
                right_edge[linecount] = pointcount;
                {
                  break;
                }
            }
            last_point = image_line[pointcount];
        }
        if(pointcount==right_end_point)
        {
            if(last_point == 0)
            {
               right_edge[linecount] = NOT_FOUND;
            }
            else if(last_point == 1)
            {
                right_edge[linecount] = right_end_point;
            }
        }
    } 
}

/*===============================================================
	@brief     纵向边线提取
	@function  将传回来的不规则图像进行矫正使之为矩形
	@note      由于摄像头以一定角度俯视并为广角镜头，因此信标形状不规则
                   通过加权平均可以矫正为矩形便于后续处理
==================================================================*/

void Image_Get_Vertical(int16 *left_edge)
{
    int i;
    
    for(i=0; i<IMAGE_HEIGHT; i++)
    {
        if(i == 0)
        {
            if((left_edge[i] != NOT_FOUND) && (left_edge[i+1] != NOT_FOUND) && (left_edge[i+2] != NOT_FOUND))
            {
                gi_start_line = i;
                break;
            }
        }
        else if(i == 1)
        {
            if((left_edge[i-1] == NOT_FOUND) && (left_edge[i] != NOT_FOUND) && (left_edge[i+1] != NOT_FOUND) && (left_edge[i+2] != NOT_FOUND))
            {
                gi_start_line = i;
                break;
            }
        }
        else if((i >= 2) && (i <= IMAGE_HEIGHT-3))
        {
            if((left_edge[i-2] == NOT_FOUND) && (left_edge[i-1] == NOT_FOUND) && (left_edge[i] != NOT_FOUND) && (left_edge[i+1] != NOT_FOUND) && (left_edge[i+2] != NOT_FOUND))
            {
                gi_start_line = i;
                break;
            }
        }
        else if(i == IMAGE_HEIGHT-2)
        {
            if((left_edge[i-2] == NOT_FOUND) && (left_edge[i-1] == NOT_FOUND) && (left_edge[i] != NOT_FOUND) && (left_edge[i+1] != NOT_FOUND))
            {
                gi_start_line = i;
                break;
            }
        }
        else if(i == IMAGE_HEIGHT-1)
        {
            if((left_edge[i-2] == NOT_FOUND) && (left_edge[i-1] == NOT_FOUND) && (left_edge[i] != NOT_FOUND))
            {
                gi_start_line = i;
                break;
            }
            else
              gi_start_line = NOT_FOUND;
        }
    }
    
    for(i=IMAGE_HEIGHT-1; i>=0; i--)
    {
        if(i == IMAGE_HEIGHT-1)
        {
            if((left_edge[i] != NOT_FOUND) && (left_edge[i-1] != NOT_FOUND) && (left_edge[i-2] != NOT_FOUND))
            {
                gi_end_line = i;
                break;
            }
        }
        else if(i == IMAGE_HEIGHT-2)
        {
            if((left_edge[i+1] == NOT_FOUND) && (left_edge[i] != NOT_FOUND) && (left_edge[i-1] != NOT_FOUND) && (left_edge[i-2] != NOT_FOUND))
            {
                gi_end_line = i;
                break;
            }
        }
        else if((i >= 2) && (i <= IMAGE_HEIGHT-3))
        {
            if((left_edge[i+2] == NOT_FOUND) && (left_edge[i+1] == NOT_FOUND) && (left_edge[i] != NOT_FOUND) && (left_edge[i-1] != NOT_FOUND) && (left_edge[i-2] != NOT_FOUND))
            {
                gi_end_line = i;
                break;
            }
        }
        else if(i == 1)
        {
            if((left_edge[i+2] == NOT_FOUND) && (left_edge[i+1] == NOT_FOUND) && (left_edge[i] != NOT_FOUND) && (left_edge[i-1] != NOT_FOUND))
            {
                gi_end_line = i;
                break;
            }
        }
        else if(i == 0)
        {
            if((left_edge[i+2] == NOT_FOUND) && (left_edge[i+1] == NOT_FOUND) && (left_edge[i] != NOT_FOUND))
            {
                gi_end_line = i;
                break;
            }
            else
              gi_end_line = NOT_FOUND;
        }
    }
}

/*===============================================================
	@brief     矩形矫正
	@function  将传回来的不规则图像进行矫正使之为矩形
	@note      由于摄像头以一定角度俯视并为广角镜头，因此信标形状不规则
                   通过加权平均可以矫正为矩形便于后续处理
==================================================================*/

void Image_Pre_Process(int16 *left_edge, int16 *right_edge)
{
    uint32 leftweight=0,rightweight=0;//左右边界权重和
    int i;
    gi_left_count = 1;
    gi_right_count = 1;
    
    for(i=0; i<IMAGE_HEIGHT; i++)
    {
        if(left_edge[i] != NOT_FOUND)
        {
            leftweight += left_edge[i];
            //leftweight += left_edge[i] * err_weight[IMAGE_HEIGHT-1-i];
            gi_left_count++;
        }
        if(right_edge[i] != NOT_FOUND)
        {
            rightweight += right_edge[i];
            //rightweight += right_edge[i] * err_weight[IMAGE_HEIGHT-1-i];
            gi_right_count++;
        }
    }
    
    gi_edge_left_value = (int16)(leftweight / gi_left_count);
    gi_edge_right_value = (int16)(rightweight / gi_right_count);
}

/*===============================================================
	@brief     面积计算及连续采集
	@function  对识别的信标图像面积进行计算，方便估算距离，连续采集防闪烁
	@note      两种思路，一种是对检测的边界进行计算，另一种是数像素点
                   前者速度相对较快，但可能不如后者精确
                   如果连续采集三次仍未得到图像则认为失去视野
==================================================================*/

void Image_Area()
{
    int16 leftcount,rightcount;
    int16 leftedge,rightedge;
    static uint8 suc_image_count = 0;
    static int16 si_image_area_temp = 0;//储存检测到信标灯时的面积
    static int16 si_image_center_temp = 0;//储存检测到信标灯时的中心线
    static int16 si_image_vertical_temp = 0;//储存检测到信标灯时的纵坐标
    
    leftcount = gi_left_count;
    rightcount = gi_right_count;
    leftedge = gi_edge_left_value;
    rightedge = gi_edge_right_value;
    
    gi_image_area = (int16)((leftcount + rightcount)* (rightedge - leftedge)/2);
    gi_image_center = (int16)((rightedge + leftedge)/2);
    gi_image_vertical = (int16)((gi_end_line + gi_start_line)/2);
    
//    if((rightedge == leftedge) && (!rightedge) && (!leftedge))
//      gi_image_area =1;
    
    if((gi_image_area == 0)&&(gi_image_center == 0))
    {
        suc_image_count++;
        if(suc_image_count == gc_not_found_count)
        {
            guc_find_image_flag = NOT_FOUND_IMAGE;
            suc_image_count = 0;
            gc_not_found_count = 3;
        }
        if(guc_find_image_flag != NOT_FOUND_IMAGE)
        {
            gi_image_center = si_image_center_temp;
            gi_image_area = si_image_area_temp;
            gi_image_vertical = si_image_vertical_temp;
        }
    }
    else
    {
        si_image_center_temp = gi_image_center;
        si_image_area_temp = gi_image_area;
        si_image_vertical_temp = gi_image_vertical;
        guc_find_image_flag = FOUND_IMAGE;
        suc_image_count = 0;
    }
}

/*===============================================================
	@brief     中心线及面积均值滤波
	@function  对中心线及面积取n次均值后平滑输出克服图像畸变
                   测试图像处理中断最大为12ms，小车速度即使达到3m/s以上
==================================================================*/

void Average_Filter()
{
    static int16 last_img_center[3];
    static int16 last_img_vertical[3];
    static int16 last_img_area[8]; 
    static int16 last_img_right[3];
    static int16 last_img_left[3];
    static int16 last_img_start_line[3];
    static int16 last_img_end_line[3];
    
    int8 i;
    int8 count_left = 0;
    int8 count_right = 0;
    int8 count_start_line = 0;
    int8 count_end_line = 0;    
    int16 sum_center = 0;
    int16 sum_vertical = 0;
    int32 sum_area = 0;
    int16 sum_right = 0;
    int16 sum_left = 0;
    int16 sum_start_line = 0;
    int16 sum_end_line = 0;        

    last_img_area[0] = last_img_area[1];
    last_img_area[1] = last_img_area[2];
    last_img_area[2] = last_img_area[3];
    last_img_area[3] = last_img_area[4];
    last_img_area[4] = last_img_area[5];
    last_img_area[5] = last_img_area[6];
    last_img_area[6] = last_img_area[7];
    last_img_area[7] = gi_image_area;

    last_img_right[0] = last_img_right[1];
    last_img_right[1] = last_img_right[2];
    last_img_right[2] = gi_edge_right_value;
    
    last_img_left[0] = last_img_left[1];
    last_img_left[1] = last_img_left[2];
    last_img_left[2] = gi_edge_left_value;
    
    last_img_start_line[0] = last_img_start_line[1];
    last_img_start_line[1] = last_img_start_line[2];
    last_img_start_line[2] = gi_start_line;
    
    last_img_end_line[0] = last_img_end_line[1];
    last_img_end_line[1] = last_img_end_line[2];
    last_img_end_line[2] = gi_end_line;    
    
    for(i=0; i<3; i++)
    {
//      sum_center += last_img_center[i];
      //sum_area += last_img_area[i];
        if(last_img_right[i])
        {
            sum_right += last_img_right[i];
            count_right++;
        }
        if(last_img_left[i])
        {
            sum_left += last_img_left[i];
            count_left++;
        }
        if(last_img_start_line[i] != NOT_FOUND)
        {
            sum_start_line += last_img_start_line[i];
            count_start_line++;
        }
        if(last_img_end_line[i] != NOT_FOUND)
        {
            sum_end_line += last_img_end_line[i];
            count_end_line++;
        }
    }
    
    for(i=0; i<8; i++)
    {
        sum_area += last_img_area[i];
    }
    
    gi_image_area_ave = (int16)(sum_area / 8);
    
    if(!count_right)
    {
        gi_image_right_ave = 0;
    }
    else
       gi_image_right_ave = (int16)((sum_right / count_right));
    
    
    if(!count_left)
    {
      gi_image_left_ave = 0;
    }
    else
       gi_image_left_ave = (int16)((sum_left / count_left)); 
    
    if(!count_start_line)
    {
        gi_image_start_ave = 0;
    }
    else
       gi_image_start_ave = (int16)((sum_start_line / count_start_line));
    
    if(!count_end_line)
    {
      gi_image_end_ave = 0;
    }
    else
       gi_image_end_ave = (int16)((sum_end_line / count_end_line)); 
    
    last_img_vertical[0] = last_img_vertical[1];
    last_img_vertical[1] = last_img_vertical[2];
    last_img_vertical[2] = gi_image_vertical;
    
    for(i=0; i<3; i++)
    {
      sum_vertical += last_img_vertical[i]; 
    }
    gi_image_vertical_ave = (int16)((sum_vertical / 3));
    
    last_img_center[0] = last_img_center[1];
    last_img_center[1] = last_img_center[2];
    last_img_center[2] = gi_image_center;
    
    for(i=0; i<3; i++)
    {
      sum_center += last_img_center[i]; 
    }
    gi_image_center_ave = (int16)((sum_center / 3) + 10);//弥补中值去噪的误差
}

/*===============================================================
	@brief     切灯系数标定
	@function  根据与信标灯的距离不断更定切灯位置距图像中心的比例
	@note      使用matlab拟合，使轨迹尽量为一条直线，前期使用图像面积
                   后期使用超声波测得的距离
==================================================================*/

void Track_Liner()
{
    if(gi_image_vertical_ave < 10)
    {
        gf_beacon_offset_P = 0;
    }
    else if((gi_image_vertical_ave >= 10)&&(gi_image_vertical_ave < 14))
    {
        gf_beacon_offset_P = 0.5;
    }
    else if((gi_image_vertical_ave >= 14)&&(gi_image_vertical_ave < 64))
    {
        gf_beacon_offset_P = 1;
    }
}

/*===============================================================
	@brief     无视野寻灯次数判断
	@function  连续采集多少次无图像判断为失去视野
	@note      使用y轴坐标判断，低于一定值直接降为1，减少因判断延时带来的滑动
                   前期使用图像面积检测
==================================================================*/

void Horizon_Count()
{
    if(gi_image_area_ave < 1)
    {
        gc_not_found_count = 4;
    }
    else if(gi_image_area_ave >= 1)
    {
        gc_not_found_count = 3;
    }
    else if(gi_image_vertical_ave == NOT_FOUND)
    {
        gc_not_found_count = 4;
    }
    
//    if(gc_not_found_count == 1)
//      Beep_On();
//    else
//      Beep_Off();
//    else if((gi_image_vertical_ave >= 55)&&(gi_image_vertical_ave < 64))
//    {
//        gc_not_found_count = 1;
//    }

////    if(!gi_image_area_ave)
////    {
////        gc_not_found_count = 7;
////    }
//    if(gi_image_area_ave > 600)
//    {
//        gc_not_found_count = 4;
//    }
//    else if(gi_image_area_ave >= 150)
//    {
//        gc_not_found_count = 4;
//    }
//    else if(gi_image_area_ave < 150)
//    {
//        gc_not_found_count = 5;
//    }

}

/*===============================================================
	@brief     目标线定位
	@function  信标灯旁侧切灯中心线定位，右灯左切，左灯右切
==================================================================*/

void Image_Center()
{
    int16 image_center_temp;
    int16 image_semi_width;
//    static int16 si_image_width_temp = 0;
    
    gf_beacon_offset_P = 1;
    
    image_semi_width = (gi_image_right_ave - gi_image_left_ave)/2;
    
//    if(!image_semi_width)
//    {
//        image_semi_width = si_image_width_temp;
//    }


    if(gi_image_center_ave >= IMAGE_WIDTH/2)
    {
        image_center_temp = (int16)(gi_image_center_ave + gc_right_offset - (float)(image_semi_width * gf_center_err_P * gf_beacon_offset_P /10));
        guc_center_flag = RIGHT;
    }
    else if((gi_image_center_ave < IMAGE_WIDTH/2) && (gi_image_center > 0))
    {
        image_center_temp = (int16)(gi_image_center_ave + gc_left_offset + (float)(image_semi_width * gf_center_err_P * gf_beacon_offset_P /10));
        guc_center_flag = LEFT;
    }
    gi_image_err = image_center_temp - 64;
    
    if(gi_image_center_ave==10)
    {
        gi_image_err = 0;
    }
//    si_image_width_temp = image_semi_width;
}

//#endif
//
//#ifndef  BEACON_STANDBY
//
///*===============================================================
//	@brief     图像处理
//	@function  所有图像处理函数调用函数
//	@note      可外部调用
//==================================================================*/
//
//void Image_Process()
//{
//    Image_Filter();//二值图像去噪
//    
//    guc_cur_image = guc_corrected_image;//保存去噪后的图像数组
//    
//    Image_Get_Edge(guc_cur_image, gi_edge_left, gi_edge_right);//信标边线提取
//    
//    Image_Get_Vertical(gi_edge_left);//纵向边线提取
//
//    Image_Pre_Process(gi_edge_left,gi_edge_right);//预处理矩形矫正
//    
//    Image_Area();//面积计算及连续采集
//    
//    Average_Filter();//均值滤波
//    
////    Track_Liner();//切灯系数调整
//    
//    Horizon_Count();//无视野判断延时调整
//    
//    Image_Center();//旁侧切灯中心线定位
//    
//    Route_Planning();//路径规划
//    
////    Direction_Control();
//
//}
//
///*===============================================================
//	@brief     图像去噪
//	@function  二值图像处理椒盐噪声
//	@note      相对于对原始灰度图进行中值滤波，采用这种滤波方法
//                   简洁有效，处理速度快
//==================================================================*/
//
//void Image_Filter()
//{
//    int16 i,j;
//    //连续采集三个点,三个点均为白点则认为采集到三个白点
//    for(i=0; i<IMAGE_HEIGHT; i++)
//    {
//        for(j=1; j< IMAGE_WIDTH-1; j++)
//        {
////            if(j < IMAGE_WIDTH-1)
////            {
////                if(guc_corrected_image[i][j]==1&&guc_corrected_image[i][j+1]==1&&guc_corrected_image[i][j+2]==1)
////                {
////                    guc_corrected_image[i][j]=1;
////                    guc_corrected_image[i][j+1]=1;
////                    guc_corrected_image[i][j+2]=1;
//////                    guc_corrected_image[i][j+3]=1;
//////                    guc_corrected_image[i][j+4]=1;
////                    j+=2;
////                }
////                else 
////                    guc_corrected_image[i][j]=0;
////            
////            }
////            if(j == IMAGE_WIDTH-1)
////            {
////                if(guc_corrected_image[i][j-2]==1&&guc_corrected_image[i][j-1]==1&&guc_corrected_image[i][j]==1)
////                {
////                    guc_corrected_image[i][j]=1;
////                }
////                else 
////                    guc_corrected_image[i][j]=0;
////            
////            }      
//          
//              if(guc_corrected_image[i][j]==1)
//              {   
//                  if((guc_corrected_image[i][j-1] == 1) || (guc_corrected_image[i][j+1] == 1))
//                  {
//                      guc_corrected_image[i][j]=1;
//                  }
//                  else 
//                  guc_corrected_image[i][j]=0;
//              }
//        }
//    }
//}
//
///*===============================================================
//	@brief      边线提取
//	@function   去噪后二值化图像边线提取   
//	@parameter1 图像接收数组
//	@parameter2 左边线存储数组
//	@parameter3 右边线存储数组
//	@note       个人感觉还能继续优化，提高处理速度
//==================================================================*/
//
//void Image_Get_Edge(vuint8 cameraimage[IMAGE_HEIGHT][IMAGE_WIDTH], int16 *left_edge, int16 *right_edge)
//{
//    int linecount;
//    int pointcount;
//    uint8 last_point=0;
//    int left_start_point;//寻找左边界扫描起始行
//    int right_start_point;//寻找右边界扫描起始行
//    int left_end_point;//寻找左边界扫描结束行
//    int right_end_point;//寻找右边界扫描结束行
//    
//    vuint8 *image_line;
//
//    image_line = cameraimage[0];
//    guc_cur_image = cameraimage;
//
//    for(linecount = 0;linecount < IMAGE_HEIGHT; linecount++)
//    {
//        image_line = cameraimage[linecount];
//        if(linecount < 1)
//        {
//            left_start_point = 0;
//            right_start_point = IMAGE_WIDTH-1;
//            left_end_point = IMAGE_WIDTH-1;
//            right_end_point = 0;
//        }
//        else if((left_edge[linecount-1] == NOT_FOUND)||(right_edge[linecount-1] == NOT_FOUND))
//        {
//            left_start_point = 0;
//            right_start_point = IMAGE_WIDTH-1;
//            left_end_point = IMAGE_WIDTH-1;
//            right_end_point = 0;
//        }
//        else if(linecount < IMAGE_HEIGHT)
//        {
//            left_start_point = (int)Limiter(left_edge[linecount-1]-10,0,IMAGE_WIDTH-1);
//            right_start_point = (int)Limiter(right_edge[linecount-1]+10,0,IMAGE_WIDTH-1);
//            left_end_point = IMAGE_WIDTH-1;
//            right_end_point = 0;
//        }
//        //从左向右搜寻左边界
//        for(pointcount = left_start_point, last_point = 0;pointcount < left_end_point; pointcount++)
//        {
//            if((last_point==1)&&(image_line[pointcount]==1))
//            {
//                left_edge[linecount] = pointcount;
//                {
//                  break;
//                }    
//            }
//            last_point = image_line[pointcount];
//        }
//        if(pointcount==left_end_point)
//        {
//            if(last_point == 0)
//            {
//                 left_edge[linecount] = NOT_FOUND;
//            }
//            else if(last_point == 1)
//            {
//                 left_edge[linecount] = left_end_point;
//            }
//        }
//        //从右向左搜右边界 
//        for(pointcount = right_start_point,last_point = 0;pointcount > right_end_point;pointcount--)
//        {
//            if((last_point==1)&&(image_line[pointcount]==1))
//            {
//                right_edge[linecount] = pointcount;
//                {
//                  break;
//                }
//            }
//            last_point = image_line[pointcount];
//        }
//        if(pointcount==right_end_point)
//        {
//            if(last_point == 0)
//            {
//               right_edge[linecount] = NOT_FOUND;
//            }
//            else if(last_point == 1)
//            {
//                right_edge[linecount] = right_end_point;
//            }
//        }
//    } 
//}
//
///*===============================================================
//	@brief     纵向边线提取
//	@function  将传回来的不规则图像进行矫正使之为矩形
//	@note      由于摄像头以一定角度俯视并为广角镜头，因此信标形状不规则
//                   通过加权平均可以矫正为矩形便于后续处理
//==================================================================*/
//
//void Image_Get_Vertical(int16 *left_edge)
//{
//    int i;
//    
//    for(i=0; i<IMAGE_HEIGHT; i++)
//    {
//        if(i == 0)
//        {
//            if((left_edge[i] != NOT_FOUND) && (left_edge[i+1] != NOT_FOUND) && (left_edge[i+2] != NOT_FOUND))
//            {
//                gi_start_line = i;
//                break;
//            }
//        }
//        else if(i == 1)
//        {
//            if((left_edge[i-1] == NOT_FOUND) && (left_edge[i] != NOT_FOUND) && (left_edge[i+1] != NOT_FOUND) && (left_edge[i+2] != NOT_FOUND))
//            {
//                gi_start_line = i;
//                break;
//            }
//        }
//        else if((i >= 2) && (i <= IMAGE_HEIGHT-3))
//        {
//            if((left_edge[i-2] == NOT_FOUND) && (left_edge[i-1] == NOT_FOUND) && (left_edge[i] != NOT_FOUND) && (left_edge[i+1] != NOT_FOUND) && (left_edge[i+2] != NOT_FOUND))
//            {
//                gi_start_line = i;
//                break;
//            }
//        }
//        else if(i == IMAGE_HEIGHT-2)
//        {
//            if((left_edge[i-2] == NOT_FOUND) && (left_edge[i-1] == NOT_FOUND) && (left_edge[i] != NOT_FOUND) && (left_edge[i+1] != NOT_FOUND))
//            {
//                gi_start_line = i;
//                break;
//            }
//        }
//        else if(i == IMAGE_HEIGHT-1)
//        {
//            if((left_edge[i-2] == NOT_FOUND) && (left_edge[i-1] == NOT_FOUND) && (left_edge[i] != NOT_FOUND))
//            {
//                gi_start_line = i;
//                break;
//            }
//            else
//              gi_start_line = NOT_FOUND;
//        }
//    }
//    
//    for(i=IMAGE_HEIGHT-1; i>=0; i--)
//    {
//        if(i == IMAGE_HEIGHT-1)
//        {
//            if((left_edge[i] != NOT_FOUND) && (left_edge[i-1] != NOT_FOUND) && (left_edge[i-2] != NOT_FOUND))
//            {
//                gi_end_line = i;
//                break;
//            }
//        }
//        else if(i == IMAGE_HEIGHT-2)
//        {
//            if((left_edge[i+1] == NOT_FOUND) && (left_edge[i] != NOT_FOUND) && (left_edge[i-1] != NOT_FOUND) && (left_edge[i-2] != NOT_FOUND))
//            {
//                gi_end_line = i;
//                break;
//            }
//        }
//        else if((i >= 2) && (i <= IMAGE_HEIGHT-3))
//        {
//            if((left_edge[i+2] == NOT_FOUND) && (left_edge[i+1] == NOT_FOUND) && (left_edge[i] != NOT_FOUND) && (left_edge[i-1] != NOT_FOUND) && (left_edge[i-2] != NOT_FOUND))
//            {
//                gi_end_line = i;
//                break;
//            }
//        }
//        else if(i == 1)
//        {
//            if((left_edge[i+2] == NOT_FOUND) && (left_edge[i+1] == NOT_FOUND) && (left_edge[i] != NOT_FOUND) && (left_edge[i-1] != NOT_FOUND))
//            {
//                gi_end_line = i;
//                break;
//            }
//        }
//        else if(i == 0)
//        {
//            if((left_edge[i+2] == NOT_FOUND) && (left_edge[i+1] == NOT_FOUND) && (left_edge[i] != NOT_FOUND))
//            {
//                gi_end_line = i;
//                break;
//            }
//            else
//              gi_end_line = NOT_FOUND;
//        }
//    }
//}
//
///*===============================================================
//	@brief     矩形矫正
//	@function  将传回来的不规则图像进行矫正使之为矩形
//	@note      由于摄像头以一定角度俯视并为广角镜头，因此信标形状不规则
//                   通过加权平均可以矫正为矩形便于后续处理
//==================================================================*/
//
//void Image_Pre_Process(int16 *left_edge, int16 *right_edge)
//{
//    uint32 leftweight=0,rightweight=0;//左右边界权重和
//    int i;
//    gi_left_count = 1;
//    gi_right_count = 1;
//    
//    for(i=0; i<IMAGE_HEIGHT; i++)
//    {
//        if(left_edge[i] != NOT_FOUND)
//        {
//            leftweight += left_edge[i];
//            //leftweight += left_edge[i] * err_weight[IMAGE_HEIGHT-1-i];
//            gi_left_count++;
//        }
//        if(right_edge[i] != NOT_FOUND)
//        {
//            rightweight += right_edge[i];
//            //rightweight += right_edge[i] * err_weight[IMAGE_HEIGHT-1-i];
//            gi_right_count++;
//        }
//    }
//    
//    gi_edge_left_value = (int16)(leftweight / gi_left_count);
//    gi_edge_right_value = (int16)(rightweight / gi_right_count);
//}
//
///*===============================================================
//	@brief     面积计算及连续采集
//	@function  对识别的信标图像面积进行计算，方便估算距离，连续采集防闪烁
//	@note      两种思路，一种是对检测的边界进行计算，另一种是数像素点
//                   前者速度相对较快，但可能不如后者精确
//                   如果连续采集三次仍未得到图像则认为失去视野
//==================================================================*/
//
//void Image_Area()
//{
//    int16 leftcount,rightcount;
//    int16 leftedge,rightedge;
//    static uint8 suc_image_count = 0;
//    static int16 si_image_area_temp = 0;//储存检测到信标灯时的面积
//    static int16 si_image_center_temp = 0;//储存检测到信标灯时的中心线
//    static int16 si_image_vertical_temp = 0;//储存检测到信标灯时的纵坐标
//    
//    leftcount = gi_left_count;
//    rightcount = gi_right_count;
//    leftedge = gi_edge_left_value;
//    rightedge = gi_edge_right_value;
//    
//    gi_image_area = (int16)((leftcount + rightcount)* (rightedge - leftedge)/2);
//    gi_image_center = (int16)((rightedge + leftedge)/2);
//    gi_image_vertical = (int16)((gi_end_line + gi_start_line)/2);
//    
////    if((rightedge == leftedge) && (!rightedge) && (!leftedge))
////      gi_image_area =1;
//    
//    if((gi_image_area == 0)&&(gi_image_center == 0))
//    {
//        suc_image_count++;
//        if(suc_image_count == gc_not_found_count)
//        {
//            guc_find_image_flag = NOT_FOUND_IMAGE;
//            suc_image_count = 0;
//            gc_not_found_count = 3;
//        }
//        if(guc_find_image_flag != NOT_FOUND_IMAGE)
//        {
//            gi_image_center = si_image_center_temp;
//            gi_image_area = si_image_area_temp;
//            gi_image_vertical = si_image_vertical_temp;
//        }
//    }
//    else
//    {
//        si_image_center_temp = gi_image_center;
//        si_image_area_temp = gi_image_area;
//        si_image_vertical_temp = gi_image_vertical;
//        guc_find_image_flag = FOUND_IMAGE;
//        suc_image_count = 0;
//    }
//}
//
///*===============================================================
//	@brief     中心线及面积均值滤波
//	@function  对中心线及面积取n次均值后平滑输出克服图像畸变
//                   测试图像处理中断最大为12ms，小车速度即使达到3m/s以上
//==================================================================*/
//
//void Average_Filter()
//{
//    static int16 last_img_center[3];
//    static int16 last_img_vertical[3];
//    static int16 last_img_area[8]; 
//    static int16 last_img_right[3];
//    static int16 last_img_left[3];
//    static int16 last_img_start_line[3];
//    static int16 last_img_end_line[3];
//    
//    int8 i;
//    int8 count_left = 0;
//    int8 count_right = 0;
//    int8 count_start_line = 0;
//    int8 count_end_line = 0;    
//    int16 sum_center = 0;
//    int16 sum_vertical = 0;
//    int32 sum_area = 0;
//    int16 sum_right = 0;
//    int16 sum_left = 0;
//    int16 sum_start_line = 0;
//    int16 sum_end_line = 0;        
//
//    last_img_area[0] = last_img_area[1];
//    last_img_area[1] = last_img_area[2];
//    last_img_area[2] = last_img_area[3];
//    last_img_area[3] = last_img_area[4];
//    last_img_area[4] = last_img_area[5];
//    last_img_area[5] = last_img_area[6];
//    last_img_area[6] = last_img_area[7];
//    last_img_area[7] = gi_image_area;
//
//    last_img_right[0] = last_img_right[1];
//    last_img_right[1] = last_img_right[2];
//    last_img_right[2] = gi_edge_right_value;
//    
//    last_img_left[0] = last_img_left[1];
//    last_img_left[1] = last_img_left[2];
//    last_img_left[2] = gi_edge_left_value;
//    
//    last_img_start_line[0] = last_img_start_line[1];
//    last_img_start_line[1] = last_img_start_line[2];
//    last_img_start_line[2] = gi_start_line;
//    
//    last_img_end_line[0] = last_img_end_line[1];
//    last_img_end_line[1] = last_img_end_line[2];
//    last_img_end_line[2] = gi_end_line;    
//    
//    for(i=0; i<3; i++)
//    {
////      sum_center += last_img_center[i];
//      //sum_area += last_img_area[i];
//        if(last_img_right[i])
//        {
//            sum_right += last_img_right[i];
//            count_right++;
//        }
//        if(last_img_left[i])
//        {
//            sum_left += last_img_left[i];
//            count_left++;
//        }
//        if(last_img_start_line[i] != NOT_FOUND)
//        {
//            sum_start_line += last_img_start_line[i];
//            count_start_line++;
//        }
//        if(last_img_end_line[i] != NOT_FOUND)
//        {
//            sum_end_line += last_img_end_line[i];
//            count_end_line++;
//        }
//    }
//    
//    for(i=0; i<8; i++)
//    {
//        sum_area += last_img_area[i];
//    }
//    
//    gi_image_area_ave = (int16)(sum_area / 8);
//    
//    if(!count_right)
//    {
//        gi_image_right_ave = 0;
//    }
//    else
//       gi_image_right_ave = (int16)((sum_right / count_right));
//    
//    
//    if(!count_left)
//    {
//      gi_image_left_ave = 0;
//    }
//    else
//       gi_image_left_ave = (int16)((sum_left / count_left)); 
//    
//    if(!count_start_line)
//    {
//        gi_image_start_ave = 0;
//    }
//    else
//       gi_image_start_ave = (int16)((sum_start_line / count_start_line));
//    
//    if(!count_end_line)
//    {
//      gi_image_end_ave = 0;
//    }
//    else
//       gi_image_end_ave = (int16)((sum_end_line / count_end_line)); 
//    
//    last_img_vertical[0] = last_img_vertical[1];
//    last_img_vertical[1] = last_img_vertical[2];
//    last_img_vertical[2] = gi_image_vertical;
//    
//    for(i=0; i<3; i++)
//    {
//      sum_vertical += last_img_vertical[i]; 
//    }
//    gi_image_vertical_ave = (int16)((sum_vertical / 3));
//    
//    last_img_center[0] = last_img_center[1];
//    last_img_center[1] = last_img_center[2];
//    last_img_center[2] = gi_image_center;
//    
//    for(i=0; i<3; i++)
//    {
//      sum_center += last_img_center[i]; 
//    }
//    gi_image_center_ave = (int16)((sum_center / 3) + 3);//弥补中值去噪的误差
//}
//
///*===============================================================
//	@brief     切灯系数标定
//	@function  根据与信标灯的距离不断更定切灯位置距图像中心的比例
//	@note      使用matlab拟合，使轨迹尽量为一条直线，前期使用图像面积
//                   后期使用超声波测得的距离
//==================================================================*/
//
//void Track_Liner()
//{
//    if(gi_image_vertical_ave < 10)
//    {
//        gf_beacon_offset_P = 0;
//    }
//    else if((gi_image_vertical_ave >= 10)&&(gi_image_vertical_ave < 14))
//    {
//        gf_beacon_offset_P = 0.5;
//    }
//    else if((gi_image_vertical_ave >= 14)&&(gi_image_vertical_ave < 64))
//    {
//        gf_beacon_offset_P = 1;
//    }
//}
//
///*===============================================================
//	@brief     无视野寻灯次数判断
//	@function  连续采集多少次无图像判断为失去视野
//	@note      使用y轴坐标判断，低于一定值直接降为1，减少因判断延时带来的滑动
//                   前期使用图像面积检测
//==================================================================*/
//
//void Horizon_Count()
//{
//    if(gi_image_vertical_ave < 28)
//    {
//        gc_not_found_count = 4;
//    }
//    else if((gi_image_vertical_ave >= 28)&&(gi_image_vertical_ave < 64))
//    {
//        gc_not_found_count = 4;
//    }
//    else if(gi_image_vertical_ave == NOT_FOUND)
//    {
//        gc_not_found_count = 4;
//    }
//    
////    if(gc_not_found_count == 1)
////      Beep_On();
////    else
////      Beep_Off();
////    else if((gi_image_vertical_ave >= 55)&&(gi_image_vertical_ave < 64))
////    {
////        gc_not_found_count = 1;
////    }
//
//////    if(!gi_image_area_ave)
//////    {
//////        gc_not_found_count = 7;
//////    }
////    if(gi_image_area_ave > 600)
////    {
////        gc_not_found_count = 4;
////    }
////    else if(gi_image_area_ave >= 150)
////    {
////        gc_not_found_count = 4;
////    }
////    else if(gi_image_area_ave < 150)
////    {
////        gc_not_found_count = 5;
////    }
//
//}
//
///*===============================================================
//	@brief     目标线定位
//	@function  信标灯旁侧切灯中心线定位，右灯左切，左灯右切
//==================================================================*/
//
//void Image_Center()
//{
//    int16 image_center_temp;
//    int16 image_semi_width;
////    static int16 si_image_width_temp = 0;
//    
//    gf_beacon_offset_P = 1;
//    
//    image_semi_width = (gi_image_right_ave - gi_image_left_ave)/2;
//    
////    if(!image_semi_width)
////    {
////        image_semi_width = si_image_width_temp;
////    }
//
//
//    if(gi_image_center_ave >= IMAGE_WIDTH/2)
//    {
//        image_center_temp = (int16)(gi_image_center_ave + 5 - (float)(image_semi_width * gf_center_err_P * gf_beacon_offset_P /10));
//        guc_center_flag = RIGHT;
//    }
//    else if((gi_image_center_ave < IMAGE_WIDTH/2) && (gi_image_center > 0))
//    {
//        image_center_temp = (int16)(gi_image_center_ave + 9 + (float)(image_semi_width * gf_center_err_P * gf_beacon_offset_P /10));
//        guc_center_flag = LEFT;
//    }
//    gi_image_err = image_center_temp - 64;
//    
//    if(gi_image_center_ave==3)
//    {
//        gi_image_err = 0;
//    }
////    si_image_width_temp = image_semi_width;
//}


/*===============================================================
	@brief     图像显示
==================================================================*/

void Show_Image_Process()
{
    uint8 num, row;
    for(row=0; row<IMAGE_HEIGHT; row++)
    {
        for(num=0; num<IMAGE_WIDTH; num++)
        {
            if(row == img_furthest_line)
            {
                imageshow[row][num]=1;
            }
            else
            {
                imageshow[row][num]=0;		//缓存数组清0
            }

        }
        if((gi_edge_left[row]!=NOT_FOUND)&&(gi_edge_right[row]!=NOT_FOUND))
        {
            imageshow[row][gi_edge_left[row]]=1;
            imageshow[row][gi_edge_right[row]]=1;
            imageshow[row][IMAGE_WIDTH/2]=1;		//显示中线
        }
    }
    oled_print_mirror_image((uint8(*)[128])imageshow,IMAGE_HEIGHT,IMAGE_WIDTH);
}
