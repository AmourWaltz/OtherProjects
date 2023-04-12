#ifndef _SLAVE_COMPUTER_H
#define _SLAVE_COMPUTER_H

#include "include.h"
#include "common.h"
#include "oled.h"


#define INPUT  DATA_IN
#define OUTPUT DATA_OUT


//调试页数
#define PAGE_MIN (1)
#define PAGE_MAX (9)
#define EXIT_SET (0)

//参数初始化

/**************参数定义******************/
//						0~3
#define  DESTINATION_PARAM_0  	 	g_u8DigitalCameraThreshold
#define  BUFFERED_PARAM_0	  		param_0
#define  INIT_PARAM_0   		  	(100)
#define  GAIN_PARAM_0   		  	(1)
#define  STRING_PARAM_0   	  		"camera"

#define  DESTINATION_PARAM_1  	 	gf_speed_p
#define  BUFFERED_PARAM_1	 	 	param_1
#define  INIT_PARAM_1   		  	(1.0)
#define  GAIN_PARAM_1   		  	(10)
#define  STRING_PARAM_1   	  		"speed_P"

#define  DESTINATION_PARAM_2   		gf_speed_i
#define  BUFFERED_PARAM_2	  		param_2
#define  INIT_PARAM_2   		  	(1.5)
#define  GAIN_PARAM_2   		  	(10)
#define  STRING_PARAM_2   	  		"speed_I"

#define  DESTINATION_PARAM_3  	 	gf_direction_p
#define  BUFFERED_PARAM_3	  		param_3
#define  INIT_PARAM_3   		  	(1.5)
#define  GAIN_PARAM_3   		  	(10)
#define  STRING_PARAM_3   	  		"dir_P"

//					4~7
#define  DESTINATION_PARAM_4  	 	gf_direction_d
#define  BUFFERED_PARAM_4	  		param_4
#define  INIT_PARAM_4   		  	(1.5)
#define  GAIN_PARAM_4   		  	(10)
#define  STRING_PARAM_4   	  		"dir_D"

#define  DESTINATION_PARAM_5  	 	gf_center_err_P
#define  BUFFERED_PARAM_5	 	 	param_5
#define  INIT_PARAM_5   		  	(1.5)
#define  GAIN_PARAM_5   		  	(100)
#define  STRING_PARAM_5   	  		"err_P"

#define  DESTINATION_PARAM_6  	 	guc_beep_CTL_flag
#define  BUFFERED_PARAM_6	  		param_6
#define  INIT_PARAM_6   		  	(1)
#define  GAIN_PARAM_6   		  	(1)
#define  STRING_PARAM_6   	  		"beep"

#define  DESTINATION_PARAM_7    	gl_speed_set
#define  BUFFERED_PARAM_7	  		param_7
#define  INIT_PARAM_7   		  	(3000)
#define  GAIN_PARAM_7   		  	(1)
#define  STRING_PARAM_7  	  		"speed"
//						8~11
#define  DESTINATION_PARAM_8   		gl_servo_center
#define  BUFFERED_PARAM_8	  		param_8
#define  INIT_PARAM_8   		  	(750)
#define  GAIN_PARAM_8   		  	(1)
#define  STRING_PARAM_8   	  		"servo"

#define  DESTINATION_PARAM_9   		gc_right_offset
#define  BUFFERED_PARAM_9	  		param_9
#define  INIT_PARAM_9   		  	(4)
#define  GAIN_PARAM_9   		  	(1)
#define  STRING_PARAM_9   	  		"right"


#define  DESTINATION_PARAM_10 	  	gf_angle_speed_kp
#define  BUFFERED_PARAM_10	 	 	param_10
#define  INIT_PARAM_10   		  	(800)
#define  GAIN_PARAM_10   		  	(10)
#define  STRING_PARAM_10  	  		"speed_DP"


#define  DESTINATION_PARAM_11   	gf_dece_kp
#define  BUFFERED_PARAM_11	  		param_11
#define  INIT_PARAM_11   		  	(1200)
#define  GAIN_PARAM_11   		  	(11)
#define  STRING_PARAM_11   	  		"dece_K"


//						12~15
#define  DESTINATION_PARAM_12  	        gf_acce_kp
#define  BUFFERED_PARAM_12	  		param_12
#define  INIT_PARAM_12   		  	(500)
#define  GAIN_PARAM_12   		  	(1)
#define  STRING_PARAM_12   	  		"aece_K"

#define  DESTINATION_PARAM_13   	gi_area_thredhold_1
#define  BUFFERED_PARAM_13	  		param_13
#define  INIT_PARAM_13   		  	(25)
#define  GAIN_PARAM_13   		  	(1)
#define  STRING_PARAM_13   	  		"area_D1"

#define  DESTINATION_PARAM_14   	gi_area_thredhold_2
#define  BUFFERED_PARAM_14	  		param_14
#define  INIT_PARAM_14   		  	(15)
#define  GAIN_PARAM_14   		  	(1)
#define  STRING_PARAM_14   	  		"area_D2"

#define  DESTINATION_PARAM_15   	gc_left_offset
#define  BUFFERED_PARAM_15	  		param_15
#define  INIT_PARAM_15   		  	(10)
#define  GAIN_PARAM_15   		  	(1)
#define  STRING_PARAM_15   	  		"left"

/************************以下参数使用6x8小字符*************************/
//				16~19
#define  DESTINATION_PARAM_16   	param_default
#define  BUFFERED_PARAM_16	  		param_16
#define  INIT_PARAM_16   		  	(4)
#define  GAIN_PARAM_16   		  	(1)
#define  STRING_PARAM_16   	  		"default"

#define  DESTINATION_PARAM_17   	param_default
#define  BUFFERED_PARAM_17	  		param_17
#define  INIT_PARAM_17  		  	(1)
#define  GAIN_PARAM_17   		  	(1000)
#define  STRING_PARAM_17   	  		"default"

#define  DESTINATION_PARAM_18   	param_default
#define  BUFFERED_PARAM_18	  		param_18
#define  INIT_PARAM_18   		  	(69)
#define  GAIN_PARAM_18   		  	(1)
#define  STRING_PARAM_18   	  		"default"

#define  DESTINATION_PARAM_19   	param_default
#define  BUFFERED_PARAM_19	  		param_19
#define  INIT_PARAM_19   		  	(84)
#define  GAIN_PARAM_19   		  	(1)
#define  STRING_PARAM_19   	  		"default"
//				20~23
#define  DESTINATION_PARAM_20   	param_default
#define  BUFFERED_PARAM_20	  		param_20
#define  INIT_PARAM_20   		  	(-83)
#define  GAIN_PARAM_20   		  	(1)
#define  STRING_PARAM_20   	  		"default"

#define  DESTINATION_PARAM_21   	param_default
#define  BUFFERED_PARAM_21	  		param_21
#define  INIT_PARAM_21   		  	(0)
#define  GAIN_PARAM_21   		  	(1)
#define  STRING_PARAM_21   	  		"default"

#define  DESTINATION_PARAM_22   	param_default
#define  BUFFERED_PARAM_22	  		param_22
#define  INIT_PARAM_22   		  	(0)
#define  GAIN_PARAM_22   		  	(1)
#define  STRING_PARAM_22   	  		"default"

#define  DESTINATION_PARAM_23   	param_default
#define  BUFFERED_PARAM_23	 	 	param_23
#define  INIT_PARAM_23   		  	(0)
#define  GAIN_PARAM_23   		  	(1)
#define  STRING_PARAM_23   	  		"default"

//						24~27
#define  DESTINATION_PARAM_24   	param_default
#define  BUFFERED_PARAM_24	  		param_24
#define  INIT_PARAM_24   		  	(0)
#define  GAIN_PARAM_24   		  	(1)
#define  STRING_PARAM_24   	  		"default"


#define  DESTINATION_PARAM_25   	param_default
#define  BUFFERED_PARAM_25		  	param_25
#define  INIT_PARAM_25   		  	(0)
#define  GAIN_PARAM_25   		  	(1)
#define  STRING_PARAM_25   	  		"default"

#define  DESTINATION_PARAM_26   	param_default
#define  BUFFERED_PARAM_26	  		param_26
#define  INIT_PARAM_26   		  	(0)
#define  GAIN_PARAM_26  		  	(1)
#define  STRING_PARAM_26   	  		"default"

#define  DESTINATION_PARAM_27   	param_default
#define  BUFFERED_PARAM_27	  		param_27
#define  INIT_PARAM_27   		  	(0)
#define  GAIN_PARAM_27   		  	(1)
#define  STRING_PARAM_27   	  		"default"

//						28~31
#define  DESTINATION_PARAM_28   	param_default
#define  BUFFERED_PARAM_28	  		param_28
#define  INIT_PARAM_28   		  	(0)
#define  GAIN_PARAM_28   		  	(1)
#define  STRING_PARAM_28   	  		"default"

#define  DESTINATION_PARAM_29   	param_default
#define  BUFFERED_PARAM_29	  		param_29
#define  INIT_PARAM_29   		  	(0)
#define  GAIN_PARAM_29   		  	(1)
#define  STRING_PARAM_29   	  		"default"

#define  DESTINATION_PARAM_30   	param_default
#define  BUFFERED_PARAM_30	  		param_30
#define  INIT_PARAM_30   		  	(0)
#define  GAIN_PARAM_30   		  	(1)
#define  STRING_PARAM_30   	  		"default"

#define  DESTINATION_PARAM_31   	param_default
#define  BUFFERED_PARAM_31	  		param_31
#define  INIT_PARAM_31   		  	(0)
#define  GAIN_PARAM_31   		  	(1)
#define  STRING_PARAM_31   	  		"default"

//						32~35
#define  DESTINATION_PARAM_32   	param_default
#define  BUFFERED_PARAM_32	  		param_32
#define  INIT_PARAM_32   		  	(0)
#define  GAIN_PARAM_32   		  	(1)
#define  STRING_PARAM_32   	  		"default"

#define  DESTINATION_PARAM_33   	param_default
#define  BUFFERED_PARAM_33	  		param_33
#define  INIT_PARAM_33   		  	(0)
#define  GAIN_PARAM_33   		  	(1)
#define  STRING_PARAM_33   	  		"default"

#define  DESTINATION_PARAM_34   	param_default
#define  BUFFERED_PARAM_34	  		param_34
#define  INIT_PARAM_34   		  	(0)
#define  GAIN_PARAM_34   		  	(1)
#define  STRING_PARAM_34   	  		"default"
//				36~39
#define  DESTINATION_PARAM_35   	param_default
#define  BUFFERED_PARAM_35	  		param_35
#define  INIT_PARAM_35   		  	(0)
#define  GAIN_PARAM_35   		  	(1)
#define  STRING_PARAM_35   	  		"default"

#define  DESTINATION_PARAM_36   	param_default
#define  BUFFERED_PARAM_36	  		param_36
#define  INIT_PARAM_36   		  	(0)
#define  GAIN_PARAM_36   		  	(1)
#define  STRING_PARAM_36   	  		"default"

#define  DESTINATION_PARAM_37   	param_default
#define  BUFFERED_PARAM_37	  		param_37
#define  INIT_PARAM_37   		  	(0)
#define  GAIN_PARAM_37   		  	(1)
#define  STRING_PARAM_37   	  		"default"

#define  DESTINATION_PARAM_38   	param_default
#define  BUFFERED_PARAM_38	  		param_38
#define  INIT_PARAM_38   		  	(0)
#define  GAIN_PARAM_38   		  	(1)
#define  STRING_PARAM_38   	  	    "default"

#define  DESTINATION_PARAM_39   	param_default
#define  BUFFERED_PARAM_39	  		param_39
#define  INIT_PARAM_39   		  	(0)
#define  GAIN_PARAM_39   		  	(1)
#define  STRING_PARAM_39   	  	    "default"


//参数是否应用到代码
#define PARAM_INIT_FLAG         (123)
#define PARAM_INIT_ENBALE       (PARAM_DISABLE)
#define PARAM_ENABLE            (123)
#define PARAM_DISABLE           (-123)

//参数结构体 
typedef struct
{
	int16 BUFFERED_PARAM_0; 
	int16 BUFFERED_PARAM_1; 
	int16 BUFFERED_PARAM_2; 
	int16 BUFFERED_PARAM_3; 
	
	int16 BUFFERED_PARAM_4; 
	int16 BUFFERED_PARAM_5; 
	int16 BUFFERED_PARAM_6; 
	int16 BUFFERED_PARAM_7; 
	
	int16 BUFFERED_PARAM_8; 
	int16 BUFFERED_PARAM_9; 
	int16 BUFFERED_PARAM_10; 
	int16 BUFFERED_PARAM_11; 
	
	int16 BUFFERED_PARAM_12; 
	int16 BUFFERED_PARAM_13; 
	int16 BUFFERED_PARAM_14; 
	int16 BUFFERED_PARAM_15; 
	
	int16 BUFFERED_PARAM_16; 
	int16 BUFFERED_PARAM_17; 
	int16 BUFFERED_PARAM_18; 
	int16 BUFFERED_PARAM_19; 
	
	int16 BUFFERED_PARAM_20; 
	int16 BUFFERED_PARAM_21; 
	int16 BUFFERED_PARAM_22; 
	int16 BUFFERED_PARAM_23; 
	
	int16 BUFFERED_PARAM_24; 
	int16 BUFFERED_PARAM_25; 
	int16 BUFFERED_PARAM_26; 
	int16 BUFFERED_PARAM_27; 
	
	int16 BUFFERED_PARAM_28; 
	int16 BUFFERED_PARAM_29; 
	int16 BUFFERED_PARAM_30; 
	int16 BUFFERED_PARAM_31; 
	
	int16 BUFFERED_PARAM_32; 
	int16 BUFFERED_PARAM_33; 
	int16 BUFFERED_PARAM_34; 
	int16 BUFFERED_PARAM_35; 
	
	int16 BUFFERED_PARAM_36; 
	int16 BUFFERED_PARAM_37; 
	int16 BUFFERED_PARAM_38; 
	int16 BUFFERED_PARAM_39; 

    int16 param_flag;
    int16 param_enable;
} PARAM;

enum PARA_MODE
{
    MODE_0,
    MODE_1,
    MODE_2,
    MODE_3
};
enum PARA_PAGE
{
    PAGE_0,
    PAGE_1,
    PAGE_2,
    PAGE_3,
    PAGE_4,
    PAGE_5,
    PAGE_6,
    PAGE_7,
    PAGE_8
};


struct RUNNING_PARA
{
    float dir_p;
    float dir_d;
    int speed_set;
    int max_speed_i;
    float speed_p;
    float speed_i;
};
#define MAX_MODE_NUM 4
#define MAX_PAGE_NUM 4

void init_button();
uint8 button_scan();
void Param_Set();
void read_param(PARAM *p_param);
uint8 read_param_able();
uint8 camera_debug(uint8 page_num);
void InitFlashParameter();
void SaveFlashParameter();

#endif
