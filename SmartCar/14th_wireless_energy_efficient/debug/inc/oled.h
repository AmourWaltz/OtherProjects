#ifndef OLED_H_
#define OLED_H_

#include "common.h"
#include "include.h"

#define OLED_CHAR_WIDTH 6 //每个字符点宽度


#define OLED_SCLK_PORT     (PTE)
#define OLED_SCLK_PIN      (GPIO_Pin7)
#define OLED_SCLK_NUM      (7)

#define OLED_SDIN_PORT     (PTE)
#define OLED_SDIN_PIN      (GPIO_Pin5)
#define OLED_SDIN_NUM      (5)

#define OLED_DC_PORT       (PTE)
#define OLED_DC_PIN        (GPIO_Pin1)
#define OLED_DC_NUM        (1)

#define OLED_RST_PORT      (PTE)
#define OLED_RST_PIN       (GPIO_Pin3)
#define OLED_RST_NUM       (3)



#define LOW  (0)
#define HIGH (1)

#define   LED_IMAGE_WHITE                    1
#define   LED_IMAGE_BLACK                    0
#define   LED_MAX_ROW_NUM                   64
#define   LED_MAX_COLUMN_NUM               128

#define   TWINK_PERIOD                      50 // per ms


#define HUST_UNIVERSITY			        (0)
#define SMARTCAR_WCES				(1)
#define SET_PARAM				(2)
#define HARDWARE_TEST				(3)
#define WIRELESS_CHARGE		                (4)
#define CAR_RUNNING				(5)
#define XUE_BOYANG                              (6)
#define PERFECT_MAKING                          (7)

enum OLED_LIGHT_MODE
{
	LOW_LIGHT_MODE = 0,
	HIGH_LIGHT_MODE = 1,
};

#define   false                              0
#define   E_SYS                               1
/**
 *  @ brief   oled_init
 *
 *  @ param   none
 *
 *	@ note    oled init
 *
 *	@ return  none
 */
void  oled_init(void);

/**
 *  @ brief   oled_set_pos
 *
 *  @ param   uc_posx   列
 *            uc_posy   页
 *
 *	@ note    设置坐标
 *
 *	@ return  none
 */
void oled_set_pos(uint8 uc_posx, uint8 uc_posy);

/**
 *  @ brief   SetInverseDisplay
 *
 *  @ param   uc_data
 *
 *	@ note    全屏写入同一个字符 清屏
 *
 *	@ return  none
 */
void oled_fill(int8 uc_data);

/**
 *  @ brief   oled_display_6x8char
 *
 *  @ param   uint8 uc_posx  显示的横坐标0~122
 * 			  uint8 uc_posy  页范围0～7
 *    		  char uc_data  显示的字符
 *
 *	@ note    display a character
 *
 *	@ return  none
 */
void oled_display_6x8char(uint8 uc_posx, uint8 uc_posy, int8 uc_data);

/**
 *  @ brief   oled_display_6x8str
 *
 *  @ param   uint8 uc_posx       显示的横坐标0~122
 *			  uint8 uc_posy       页范围0～7
 *    		  char uc_dataStr[]  显示的字符串
 *
 *	@ note    display a string
 *
 *	@ return  none
 */
void oled_display_6x8str(uint8 uc_posx, uint8 uc_posy, int8 uc_dataStr[]);

/**
 *  @ brief   oled_print_char
 *
 *  @ param   uint8 uc_posx  显示的横坐标0~122
 *    		  uint8 uc_posy  页范围0～7
 *    		  int8  c_data   需要转化显示的数值 -128~127
 *
 *	@ note    将一个Char型数转换成3位数进行显示
 *
 *	@ return  none
 */
void oled_print_char(uint8 uc_posx, uint8 uc_posy, int8 c_data);

/**
 *  @ brief   oled_print_short
 *
 *  @ param   uint8 uc_posx  显示的横坐标0~122
 *   		  uint8 uc_posy  页范围0～7
 *  		  int8  s_data   需要转化显示的数值
 *
 *	@ note    将一个int16型数转换成5位数进行显示
 *
 *	@ return  none
 */
void oled_print_short(uint8 uc_posx, uint8 uc_posy, int16 s_data);

/**
 *  @ brief   oled_print_image
 *
 *  @ param   uint8 usPicture[][168]     二维图像数组的地址
*      		  uint16 usRowNum            二维图像的行数1~64
*   		  uint16 usColumnNum         二维图像的列数1~128
 *
 *	@ note    将图像显示出来
 *
 *	@ return  none
 */
void oled_print_image(uint8 usPicture[][128], uint16 usRowNum, uint16 usColumnNum);
void oled_print_mirror_image(uint8 usPicture[][128], uint16 usRowNum, uint16 usColumnNum);

void oled_print_std_image(uint8 u8Mode);

/**
 *  @ brief   oled_gap
 *
 *  @ param   none
 *
 *	@ note    display gap between dates
 *
 *	@ return  none
 */
void  oled_gap(void);

/**
 *  @ brief   oled_twink
 *
 *  @ param   none
 *
 *	@ note    twink, to remind us the oled is working
 *
 *	@ return  none
 */
void  oled_twink(void);

/**
 *  @ brief   oled_clear
 *
 *  @ param   none
 *
 *	@ note    clear some space
 *
 *	@ return  none
 */
void  oled_clear(uint8 start_x, uint8 page, uint8 clear_size);

/**
 *  @ brief   oled_gap
 *
 *  @ param   none
 *
 *	@ note    display gap between dates
 *
 *	@ return  none
 */
void  oled_one_group(uint8 my_x, uint8 my_y, uint16 my_data);

void  oled_write_data(uint8 uc_data);


void oled_my_show(uint8 my_x, uint8 my_y, int16 mydata);


void oled_display_16x8char(uint8 uc_posx, uint8 uc_posy, int8 uc_data);
void oled_display_16x8str(uint8 uc_posx, uint8 uc_posy, int8 uc_dataStr[]);
void oled_print_16x8char(uint8 uc_posx, uint8 uc_posy, int8 c_data);
void oled_print_16x8short(uint8 uc_posx, uint8 uc_posy, int16 s_data);

void oled_display_16x8char_hl(uint8 uc_posx, uint8 uc_posy, int8 uc_data);
void oled_display_16x8str_hl(uint8 uc_posx, uint8 uc_posy, int8 uc_dataStr[]);
void oled_print_16x8short_hl(uint8 uc_posx, uint8 uc_posy, int16 s_data, int8 bits_num);

void oled_display_6x8char_hl(uint8 uc_posx, uint8 uc_posy, int8 uc_data);

void oled_display_6x8str_hl(uint8 uc_posx, uint8 uc_posy, int8 uc_dataStr[]);

void oled_print_char_hl(uint8 uc_posx, uint8 uc_posy, int8 c_data);

void oled_print_short_hl(uint8 uc_posx, uint8 uc_posy, int16 s_data, int8 bits_num);

void oled_display_chinese(uint8 uc_posx, uint8 uc_posy, uint8 Chinese_str_num, enum OLED_LIGHT_MODE mode);

void oled_display_boolean(uint8 uc_posx, uint8 uc_posy, int8 bool_data);

void oled_display_direction(uint8 uc_posx, uint8 uc_posy, int8 dir_data);

#endif /* OLED_H_ */
