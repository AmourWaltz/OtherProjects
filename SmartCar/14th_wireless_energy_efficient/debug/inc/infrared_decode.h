#ifndef INFRA_RED_DECODE_H
#define INFRA_RED_DECODE_H

#include "common.h"
#include "include.h"

/********************************
功能：红外解码 （NEC协议）
作者：龙乐坪
修改时间：2015 年1月11日
环境要求：须具备一个0.1ms定时器中断用于获取时间，函数名通过GET_TIME_MS宏定义 返回值为整形  时间单位是0.1ms
使用说明：将infrared_decode()置于红外边沿触发中断函数中，调用get_infrared_ready_flag()获得信号就绪标志，调用get_infrared_command()获得按键值 其他功能从略
********************************/

#define GET_TIME_MS   gul_time_100us

/*NEC协议  引导码，8位的客户码，8位客户码的补码，8位的按键值，8位按键值的补码*/
//可参考 http://www.cnblogs.com/openusb/archive/2010/01/07/1641357.html （comment by TPX)
 
/*时序宏定义*/
/*
#define INFRARED_WAIT    0 //下降沿触发 记下当前时刻 直接进入下一阶段
#define INFRARED_ACTIVE  1 //等待上升沿如果上升沿到来时计时为9ms则进入下一阶段  否则为干扰信号 返回WAIT
#define INFRARED_START   2 //等待下降沿如果下降沿到来时计时为4.5ms则正式开始解码   如果是2.25ms 则判断与最近的解码完成的时刻的时间差是否小于一定值（40ms ）若是则为第一个连续按键 记下此接收完成此连续按键的时刻 如果要舍弃第一个则转入COUTINUE_WAIT  否则转入COUTINUE  否则为干扰信号
*/
//注意由于复位时会产生一个中断，所以初始状态会是INFRARED_ACTIVE，这样当接收到第一个红外信号头的下降沿时会判断不符合条件而返回INFRARED_WAIT，经过过9ms下一个上升沿到来时，解码处在INFRARED_WAIT状态因而错过了9ms的引导码，并且由于环境噪声和信号都是偶数个边沿，所以每次信号引导码到来时程序都处在INFRARED_ACTIVE，所以永远无法进入解码（然而如果有足够的噪声激励就有可能碰巧进入）
//为了避免这种情况将INFRARED_WAIT和INFRARED_ACTIVE合并为一个状态修改后如下

#define INFRARED_WAIT    0 //双边沿触发  如果本次触发时刻与上次触发时间间隔为9ms则进入引导码识别， 否则记下当前时刻并退出
#define INFRARED_ACTIVE  1 //等待下降沿如果下降沿到来时计时为4.5ms则正式开始解码   如果是2.25ms 则判断与最近的解码完成的时刻的时间差是否小于一定值（40ms ）若是则为第一个连续按键 记下此接收完成此连续按键的时刻 如果要舍弃第一个则转入COUTINUE_WAIT  否则转入COUTINUE  否则为干扰信号

#define INFRARED_DECODE1 3 //等待上升沿计时 上升沿到来时如果计时为0.56ms则进入下一解码阶段  否则出错
#define INFRARED_DECODE2 4 //等待下降沿计时 下降沿到来时如果计时为0.56ms 则为0  如果是1.68ms则为1 否则出错  判断如果不出错则保存数据 并判断是否到达32位 是则进入下一阶段 否则返回DECODE1
#define INFRARED_CHECK     5 //等待上升沿   校验数据 如果正确则记下本次解码完成刻（上升沿到来时计时如果为0.56ms则正确  返回WAIT    否则出错警告解码过程可能出错  但NEC upd6121 编码格式中并没有对这部分的规定）

#define COUTINUE_WAIT     6  //等待下降沿  将当前时间  如果与上一次连续按键接收到的时间约为108-9-2.25-0.56=98ms则连续按键有效进入下一阶段 否则连续按键释放  返回WAIT
#define CONTINUE_ACTIVE   7  //等待上升沿如果上升沿到来时计时为9ms则进入下一阶段   否则为干扰信号 返回WAIT
#define CONTINUE_START    8  //等待下降沿如果下降沿到来时计时为2.25ms  确认有效  转入下一阶段  否则返回wait 
#define CONTINUE_CONFIRM  9  //等待上升沿 确认一个连续按键 记下当前时刻 转入下一个连续按键等待COUTINUE_WAIT（上升沿到来时计时如果为0.56ms则正确 但NEC upd6121 编码格式中并没有对这部分的规定）
#define CONTINUE_END1     10 //
#define CONTINUE_END2     11 //

#define BOOT_CODE_TIME1_MIN   85//9ms 单位0.1ms
#define BOOT_CODE_TIME1_MAX   95
#define BOOT_CODE_TIME2_MIN   40//4.5ms
#define BOOT_CODE_TIME2_MAX   50
#define DATA_SHORT_TIME_MIN   1 //0.56ms
#define DATA_SHORT_TIME_MAX   11
#define DATA_LONG_TIME_MIN    12//1.68ms
#define DATA_LONG_TIME_MAX    22 

#define FIRST_COUTINUE_TIME_MIN   510//45+9+2.25ms  实测516   
#define FIRST_COUTINUE_TIME_MAX   520//45ms   
#define COUTINUE_TIME_MIN     968//98ms   实测973左右
#define COUTINUE_TIME_MAX     978
#define COUTINUE_BOOT_TIME_MIN 20//2.25ms
#define COUTINUE_BOOT_TIME_MAX 25

#define COUTINUE_NEXT_MIN     600//76-9-2.25-0.56=64.19ms   实测632-642   但第一个对于不同按键差别较大  大致960-968之间
#define COUTINUE_NEXT_MAX     968
#define COUTINUE_TIME_NUM     9

#define COUTINUE_TIME_THRESHOLD 5//长按时间设定 (0-9)     0 不判断长按  1长按时间设定为 约约85.74+40+9+2.25+0.56=137.55ms   -  49.9++40+9+2.25+0.56=101.71ms  以后每加一增加108ms

/*常用按键值（因遥控器而异）*/
#define COMMAND_UP      140
#define COMMAND_DOWN    169
#define COMMAND_LEFT    132
#define COMMAND_RIGHT   173
#define COMMAND_OK      142
#define COMMAND_POWER   139

#define COMMAND_VOLUME_UP    192
#define COMMAND_VOLUME_DOWN  64
#define COMMAND_CHANNEL_UP   66
#define COMMAND_CHANNEL_DOWN 195
#define COMMAND_PAGE_UP      110
#define COMMAND_PAGE_UP_1      6
#define COMMAND_PAGE_UP_2    140
#define COMMAND_PAGE_UP_3    103

#define COMMAND_PAGE_DOWN    198
#define COMMAND_RETURN       74
#define COMMAND_HOME_PAGE    196
//#define COMMEND_TV_AV        209

#define COMMAND_0            67
#define COMMAND_1            201
#define COMMAND_2            73
#define COMMAND_3            230
#define COMMAND_4            199
#define COMMAND_5            71
#define COMMAND_6            228
#define COMMAND_7            197
#define COMMAND_8            69
#define COMMAND_9            226


/*内部使用 静态局部变量*/
typedef struct
{
  uint8  ready_flag;
  uint16 device_id;
  uint8  command;
  uint8  press_hold_flag;
  uint8  coutinue_flag;
}infrared_code_type;

/*接口函数*/
/*获取红外信号就绪标志*/
uint8 get_infrared_ready_flag();
/*获取红外客户码*/
uint16 get_infrared_device_id();
/*获取红外按键值*/
uint8 get_infrared_command();
/*获取按键长按标志 用于固定时长的一般操作  如开关机等*/
uint8 get_infrared_press_hold_flag();
/*获取长按结束标志  用于需要持续信号的地方 如连续加减  长按加速等  可靠的最短检测周期约220ms  如可准确控制时序则最短可达110ms*/
uint8 get_infrared_coutinue_flag();//需先调用reset_infrared_coutinue_over_flag() 经过一定时间(220ms以上)后再检测 中间不能复位
/*长按结束等待*/
void reset_infrared_coutinue_flag();
/*红外按键复位  每次接收完按键值必须复位*/
void reset_infrared();



/*解码程序*/
/*红外解码  需置于红外触发中断函数中 并设置为双边沿触发*/
void infrared_decode();

#endif