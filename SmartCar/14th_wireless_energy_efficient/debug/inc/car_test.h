#ifndef _CAR_TEST_H
#define _CAR_TEST_H

#include "common.h"
#include "include.h"

/*===============================================================
        @note      无故寻愁觅恨，有时似傻如狂，
                   纵然生得好皮囊，腹内原来草莽；
                   潦倒不通世务，愚顽怕读文章，
                   性情偏僻性乖张，那管世人诽谤。
==================================================================*/

//测试函数页数最大和最小宏定义
//注:如果增加了新的测试页面，需将 TEST_PAGE_MAX 改为相应的数值
#define TEST_PAGE_MIN (1)//1
#define TEST_PAGE_MAX (3)


void Smartcar_Test();//小车硬件测试函数

uint8 switch_test(uint8 page_num);//拨码测试

uint8 led_test(uint8 page_num);//led灯测试

uint8 infrared_test(uint8 page_num);//红外测试

uint8 encoder_test(uint8 page_num);//编码器测试函数

uint8 test_exit(uint8 page_num);//退出测试函数



#endif
