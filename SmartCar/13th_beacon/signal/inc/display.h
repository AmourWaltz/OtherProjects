#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "include.h"
#include "common.h"

#define RUNNING_PAGE_MIN        1
#define RUNNING_PAGE_MAX        7


uint8  display_image(uint8 page_num);
uint8  image_parameter(uint8 page_num);
uint8  running_parameter(uint8 page_num);
uint8 infrared_parameter(uint8 page_num);
void Smartcar_Running();
void Delay();

#endif