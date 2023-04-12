#include "infrared_decode.h"

static infrared_code_type infrared_code = {0,0,0,0};

uint8 get_infrared_ready_flag()
{
	return infrared_code.ready_flag;
}
uint16 get_infrared_device_id()
{
    return infrared_code.device_id;
}
uint8 get_infrared_command()
{  
    return infrared_code.command;
}
uint8 get_infrared_press_hold_flag()
{
    return infrared_code.press_hold_flag;
}

uint8 get_infrared_coutinue_flag()
{
    return infrared_code.coutinue_flag;
}

void reset_infrared_coutinue_flag()
{
    infrared_code.coutinue_flag = 0;
}

void reset_infrared()
{
    infrared_code.ready_flag = 0;
    infrared_code.device_id = 0;
    infrared_code.command = 0;
    infrared_code.press_hold_flag = 0;
    infrared_code.coutinue_flag = 0;
}

void infrared_decode()
{
    static uint8 state = INFRARED_WAIT;
    static uint8 recheck_flag = 0;//作为长按时的重复编码检测标志
    static uint8 coutinue_time;
    static uint32 time = 0,current_time = 0,d_time = 0,recent_received_time = 0; //time阶段事件时刻 current当前时间 
    static uint32 data = 0;
    static uint32 data_former;//用于长按时的重复编码核对
    static uint8 bit_num = 0;
    current_time = GET_TIME_MS;//0.1ms为单位
    d_time = current_time - time;
    switch(state)
    {
      case INFRARED_WAIT:
          if(d_time>BOOT_CODE_TIME1_MIN && d_time<BOOT_CODE_TIME1_MAX)//9ms
          {
              time = current_time;
              state = INFRARED_ACTIVE;
          }
          time = current_time;//100us为单位
          break;
      case INFRARED_ACTIVE:
          if(d_time>BOOT_CODE_TIME2_MIN && d_time<BOOT_CODE_TIME2_MAX)//4.5ms
          {
              time = current_time;
              bit_num = 0; //解码计数位清零
              infrared_code.coutinue_flag = 1;
              state = INFRARED_DECODE1;//开始解码
          }
          else if(d_time>COUTINUE_BOOT_TIME_MIN && d_time<COUTINUE_BOOT_TIME_MAX)//2.25ms
          {
              if(current_time-recent_received_time>FIRST_COUTINUE_TIME_MIN&&current_time-recent_received_time<FIRST_COUTINUE_TIME_MAX)//40ms  确认第一个连续信号
              {
                  state = CONTINUE_CONFIRM;//如果不舍弃第一个连续信号则 进入COUTINUE_CONFIRM
                  coutinue_time = 0;//连续信号开始计数
              }
              else //过时的连续信号不响应
              {
                  state = INFRARED_WAIT;
              }
          }
          else//干扰信号
          {
              state = INFRARED_WAIT;
          }
          break;
      case INFRARED_DECODE1:
          if(d_time>DATA_SHORT_TIME_MIN&&d_time<DATA_SHORT_TIME_MAX)//0.56ms  实测为0.6ms
          {
              time = current_time;
              state = INFRARED_DECODE2;
          }
          else//干扰信号
          {
              state = INFRARED_WAIT;
          }
          break;
      case INFRARED_DECODE2:
          if(d_time>DATA_SHORT_TIME_MIN&&d_time<DATA_SHORT_TIME_MAX)//0.56ms  实测为0.6ms
          {
              time = current_time;
              data &= 0x7FFFFFFF;//最高位置0
              data>>=1;
              state = INFRARED_DECODE1;
              bit_num++;
              if(bit_num==32)
              {
                  state = INFRARED_CHECK;//解码结束 开始校正
              }
          }
          else if(d_time>DATA_LONG_TIME_MIN && d_time<DATA_LONG_TIME_MAX)//1.68ms
          {
              time = current_time;
              data |= 0x80000000;//最高位置1
              data>>=1;
              state = INFRARED_DECODE1;
              bit_num++;
              if(bit_num==32)
              {
                  state = INFRARED_CHECK;//解码结束 开始校正
              }
          }
          else//干扰信号
          {
              state = INFRARED_WAIT;
          }
          break;
      case INFRARED_CHECK://可以加一个0.56ms核对
          if(recheck_flag == 0)//正常编码核对
          {
              if(((data<<8)&0xFF000000)^(data&0xFF000000)==0xFF000000)//数据正确接收  这里可以不再判断时间是否为 0.56ms
              {
                  recent_received_time = time;
                  infrared_code.ready_flag = 1;
                  infrared_code.device_id = (uint16)data&0x0000FFFF;
                  infrared_code.command = (uint16)((data&0x00FF0000)>>16);
                  infrared_code.coutinue_flag = 1;//指示长按尚未结束
              }
              else//数据出错
              {
                  //printf("device ID = %d\n",(int)data&0x0000FFFF);
                  //printf("data error\n");
              }
              data_former = data;
          }
          else//连续的重复编码核对
          {
              if(data == data_former)
              {
                  recent_received_time = time;
                  infrared_code.coutinue_flag = 1;//指示长按尚未结束
              }
              else//数据出错
              {
                  //printf("device ID = %d\n",(int)data&0x0000FFFF);
                  //printf("data error\n");
              }
              recheck_flag = 0;
          }
          state = INFRARED_WAIT;//新的引导码以及 第一个连续信号 都是在INFRARED_WAIT中检测
          break;
      case COUTINUE_WAIT:
          if(coutinue_time<COUTINUE_TIME_NUM&&d_time>COUTINUE_TIME_MIN&&d_time<COUTINUE_TIME_MAX)//98ms
          {
              time = current_time;
              state = CONTINUE_ACTIVE;
          }
          else if(coutinue_time==COUTINUE_TIME_NUM&&d_time>COUTINUE_NEXT_MIN&&d_time<COUTINUE_NEXT_MAX)//紧接连续信号的下一个重复编码 或者 噪声   也可以通过连续信号个数coutinue_time的最大值来判断
          {
              time = current_time;
              recheck_flag = 1;//准备核对编码 排除意外干扰
              state = INFRARED_WAIT;
          }
          else if(d_time<COUTINUE_NEXT_MIN)
          {
              //直接跳过中间干扰信号 考虑到干扰信号一般会有一个下降沿一个上升沿 间隔时间很短 可以设置其他去干扰算法
          }
          else//超时连续终止 也可能是下一次信号的引导码
          {
              time = current_time;//保证不错过新数据的引导码
              state = INFRARED_WAIT;
          }
          break;
      case CONTINUE_ACTIVE:
          if(d_time>BOOT_CODE_TIME1_MIN&&d_time<BOOT_CODE_TIME1_MAX)//9ms
          {
              time = current_time;
              state = CONTINUE_START;
          }
          else//干扰信号
          {
              state = INFRARED_WAIT;
          }
          break;
      case CONTINUE_START:
          if(d_time>COUTINUE_BOOT_TIME_MIN&&d_time<COUTINUE_BOOT_TIME_MAX)//2.25ms
          {
              state = CONTINUE_CONFIRM;
          }
          else//干扰信号
          {
              state = INFRARED_WAIT;
          }
          break;
      case CONTINUE_CONFIRM://可以加一个0.56ms核对
          time = current_time;
          coutinue_time ++;//连续信号个数更新
          if(coutinue_time == COUTINUE_TIME_THRESHOLD)//判断长按
          {
              infrared_code.press_hold_flag = 1;
          }
          infrared_code.coutinue_flag = 1;//指示长按尚未结束
          state = COUTINUE_WAIT;//下一个连续信号等待
          break;
    }
}