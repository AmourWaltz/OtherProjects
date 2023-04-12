#ifndef _DISPLAY_IMAGE_H_
#define _DISPLAY_IMAGE_H_

#include "include.h"
#include "common.h"

#define DISPLAY_PAGE_MIN 1
#define DISPLAY_PAGE_MAX 5

#define SPECIAL_PAGE_OFFSET 30

#define ROAD_PARA_PAGE_MAX 4

#define DISPLAY_IMAGE_DELAY 100

#define _64x200_MODE 0
#define NARROW_MODE 1
#define LINER_MODE 2

uint8  DisplayPage_1();
uint8  DisplayPage_2();
uint8  DisplayPage_3();
uint8  DisplayPage_4();
uint8 display_exit(uint8 page_num);
void Display_Image();


#endif