#ifndef _SLAVE_COMPUTER_H
#define _SLAVE_COMPUTER_H

#include "common.h"
#include "oled.h"
#include "include.h"


//按键ID定义
#define BT_UP_PRESS  (1)
#define BT_DN_PRESS  (2)
#define BT_LE_PRESS  (3)
#define BT_RI_PRESS  (4)
#define BT_OK_PRESS  (5)

//调试页数
#define PAGE_MIN (1)
#define PAGE_MAX (12)
#define EXIT_SET (0)

//参数初始化

/**************参数定义******************/
//						0~3
#define  DESTINATION_PARAM_0  	 	        gl_voltage_start_set
#define  BUFFERED_PARAM_0	  		param_0
#define  INIT_PARAM_0   		  	(1)
#define  GAIN_PARAM_0   		  	(1)
#define  STRING_PARAM_0   	  		"start_V"

#define  DESTINATION_PARAM_1  	 	        gf_direction_p
#define  BUFFERED_PARAM_1	 	 	param_1
#define  INIT_PARAM_1   		  	(1)
#define  GAIN_PARAM_1   		  	(1)
#define  STRING_PARAM_1   	  		"dir_P"

#define  DESTINATION_PARAM_2   		        gf_direction_d
#define  BUFFERED_PARAM_2	  		param_2
#define  INIT_PARAM_2   		  	(1)
#define  GAIN_PARAM_2   		  	(1)
#define  STRING_PARAM_2   	  		"dir_D"

#define  DESTINATION_PARAM_3  	 	        gf_speed_p
#define  BUFFERED_PARAM_3	  		param_3
#define  INIT_PARAM_3   		  	(1)
#define  GAIN_PARAM_3   		  	(1)
#define  STRING_PARAM_3   	  		"speed_P"

//					4~7
#define  DESTINATION_PARAM_4  	 	        gf_speed_i
#define  BUFFERED_PARAM_4	  		param_4
#define  INIT_PARAM_4   		  	(1)
#define  GAIN_PARAM_4   		  	(1)
#define  STRING_PARAM_4   	  		"speed_I"

#define  DESTINATION_PARAM_5  	 	        gui_ready_annulus_weight
#define  BUFFERED_PARAM_5	 	 	param_5
#define  INIT_PARAM_5   		  	(1)
#define  GAIN_PARAM_5   		  	(1)
#define  STRING_PARAM_5   	  		"R_weight"

#define  DESTINATION_PARAM_6  	 	        gf_angle_speed_kp
#define  BUFFERED_PARAM_6	  		param_6
#define  INIT_PARAM_6   		  	(1)
#define  GAIN_PARAM_6   		  	(1)
#define  STRING_PARAM_6   	  		"dec_P"

#define  DESTINATION_PARAM_7    	        gui_secede_annulus_weight
#define  BUFFERED_PARAM_7	  		param_7
#define  INIT_PARAM_7   		  	(1)
#define  GAIN_PARAM_7   		  	(1)
#define  STRING_PARAM_7  	  		"S_weight"

//						8~11
#define  DESTINATION_PARAM_8   		        gi_calibration_middle
#define  BUFFERED_PARAM_8	  		param_8
#define  INIT_PARAM_8   		  	(200)
#define  GAIN_PARAM_8   		  	(1)
#define  STRING_PARAM_8   	  		"cailb_M"

#define  DESTINATION_PARAM_9   		        gui_x_weight
#define  BUFFERED_PARAM_9	  		param_9
#define  INIT_PARAM_9   		  	(1)
#define  GAIN_PARAM_9   		  	(1)
#define  STRING_PARAM_9   	  		"X_weight"


#define  DESTINATION_PARAM_10 	  	        gui_entry_annulus_weight
#define  BUFFERED_PARAM_10	 	 	param_10
#define  INIT_PARAM_10   		  	(1)
#define  GAIN_PARAM_10   		  	(1)
#define  STRING_PARAM_10  	  		"E_weight"


#define  DESTINATION_PARAM_11   	        gl_speed_lost_line
#define  BUFFERED_PARAM_11	  		param_11
#define  INIT_PARAM_11   		  	(160)
#define  GAIN_PARAM_11   		  	(1)
#define  STRING_PARAM_11   	  		"speed_M"


//						12~15
#define  DESTINATION_PARAM_12  	                gl_speed_straight
#define  BUFFERED_PARAM_12	  		param_12
#define  INIT_PARAM_12   		  	(1)
#define  GAIN_PARAM_12   		  	(1)
#define  STRING_PARAM_12   	  		"speed_S"

#define  DESTINATION_PARAM_13   	        gf_angularspeed_p
#define  BUFFERED_PARAM_13	  		param_13
#define  INIT_PARAM_13   		  	(1)
#define  GAIN_PARAM_13   		  	(1)
#define  STRING_PARAM_13   	  		"yaw_P"

#define  DESTINATION_PARAM_14   	        gl_speed_obstacle
#define  BUFFERED_PARAM_14	  		param_14
#define  INIT_PARAM_14   		  	(1)
#define  GAIN_PARAM_14   		  	(1)
#define  STRING_PARAM_14   	  		"O_speed"

#define  DESTINATION_PARAM_15   	        gl_speed_annulus
#define  BUFFERED_PARAM_15	  		param_15
#define  INIT_PARAM_15   		  	(1)
#define  GAIN_PARAM_15   		  	(1)
#define  STRING_PARAM_15   	  		"A_speed"

//				16~19
#define  DESTINATION_PARAM_16   	        gf_obstacle_avoidance_p
#define  BUFFERED_PARAM_16	  		param_16
#define  INIT_PARAM_16   		  	(1)
#define  GAIN_PARAM_16   		  	(1)
#define  STRING_PARAM_16   	  		"avoid_P"

#define  DESTINATION_PARAM_17   	        gf_obstacle_avoidance_d
#define  BUFFERED_PARAM_17	  		param_17
#define  INIT_PARAM_17  		  	(1)
#define  GAIN_PARAM_17   		  	(1)
#define  STRING_PARAM_17   	  		"avoid_D"

#define  DESTINATION_PARAM_18   	        gi_aviodence_angle
#define  BUFFERED_PARAM_18	  		param_18
#define  INIT_PARAM_18   		  	(1)
#define  GAIN_PARAM_18   		  	(1)
#define  STRING_PARAM_18   	  		"angle_A"

#define  DESTINATION_PARAM_19   	        gi_regression_angle
#define  BUFFERED_PARAM_19	  		param_19
#define  INIT_PARAM_19   		  	(1)
#define  GAIN_PARAM_19   		  	(1)
#define  STRING_PARAM_19   	  		"angle_R"

//				20~23
#define  DESTINATION_PARAM_20   	        gf_obstacle_regression_p
#define  BUFFERED_PARAM_20	  		param_20
#define  INIT_PARAM_20   		  	(0)
#define  GAIN_PARAM_20   		  	(1)
#define  STRING_PARAM_20   	  		"regre_P"

#define  DESTINATION_PARAM_21   	        gf_obstacle_regression_d
#define  BUFFERED_PARAM_21	  		param_21
#define  INIT_PARAM_21   		  	(0)
#define  GAIN_PARAM_21   		  	(1)
#define  STRING_PARAM_21   	  		"regre_D"

#define  DESTINATION_PARAM_22   	        gf_obstacle_straight_p
#define  BUFFERED_PARAM_22	  		param_22
#define  INIT_PARAM_22   		  	(0)
#define  GAIN_PARAM_22   		  	(1)
#define  STRING_PARAM_22   	  		"strai_P"

#define  DESTINATION_PARAM_23   	        gf_obstacle_straight_d
#define  BUFFERED_PARAM_23	 	 	param_23
#define  INIT_PARAM_23   		  	(0)
#define  GAIN_PARAM_23   		  	(1)
#define  STRING_PARAM_23   	  		"strai_D"

//						24~27
#define  DESTINATION_PARAM_24   	        gf_power_P
#define  BUFFERED_PARAM_24	  		param_24
#define  INIT_PARAM_24   		  	(0)
#define  GAIN_PARAM_24   		  	(1)
#define  STRING_PARAM_24   	  		"power_P"


#define  DESTINATION_PARAM_25   	        gf_power_I
#define  BUFFERED_PARAM_25		  	param_25
#define  INIT_PARAM_25   		  	(0)
#define  GAIN_PARAM_25   		  	(1)
#define  STRING_PARAM_25   	  		"power_I"

#define  DESTINATION_PARAM_26   	        gl_power_set
#define  BUFFERED_PARAM_26	  		param_26
#define  INIT_PARAM_26   		  	(0)
#define  GAIN_PARAM_26  		  	(1)
#define  STRING_PARAM_26   	  		"power_S"

#define  DESTINATION_PARAM_27   	        gl_obstacle_judge_distance
#define  BUFFERED_PARAM_27	  		param_27
#define  INIT_PARAM_27   		  	(0)
#define  GAIN_PARAM_27   		  	(1)
#define  STRING_PARAM_27   	  		"dis_O"

//						28~31
#define  DESTINATION_PARAM_28   	        gl_car_destination_distance
#define  BUFFERED_PARAM_28	  		param_28
#define  INIT_PARAM_28   		  	(0)
#define  GAIN_PARAM_28   		  	(1)
#define  STRING_PARAM_28   	  		"dis_D"

#define  DESTINATION_PARAM_29   	        gl_car_stopline_distance
#define  BUFFERED_PARAM_29	  		param_29
#define  INIT_PARAM_29   		  	(0)
#define  GAIN_PARAM_29   		  	(1)
#define  STRING_PARAM_29   	  		"dis_S"

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
#define PARAM_INIT_FLAG       (123)
#define PARAM_INIT_ENBALE     (PARAM_DISABLE)
#define PARAM_ENABLE   (123)
#define PARAM_DISABLE  (0)

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

extern uint8 gl_beep_CTL_flag;

void init_button();
uint16 button_scan();
void Param_Set();
void read_param(PARAM *p_param);
uint8 read_param_able();
uint8 camera_debug(uint8 page_num);
void InitFlashParameter();
void SaveFlashParameter();
void mode_select();

uint8 flag_page(int8 *pstr_a, uint8 *param_a,
                int8 *pstr_b, uint8 *param_b,
                int8 *pstr_c, uint8 *param_c,
                int8 *pstr_d, uint8 *param_d,
                int8 *pstr_e, uint8 *param_e,
                int8 *pstr_f, uint8 *param_f,
                int8 *pstr_g, uint8 *param_g,
                int8 *pstr_h, uint8 *param_h,
                int8 *pstr_i, uint8 *param_i,
                int8 *pstr_j, uint8 *param_j,
                int8 *pstr_k, uint8 *param_k,
                int8 *pstr_l, uint8 *param_l,
                int8 *pstr_m, uint8 *param_m,
                int8 *pstr_n, uint8 *param_n,
                int8 *pstr_o, uint8 *param_o,
                int8 *pstr_p, uint8 *param_p,
                uint8 page_num);

#endif
