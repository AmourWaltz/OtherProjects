/*=============================================
    @file         charge.c
    @brief        充电控制文件
    @programmer   Beyond Hsueh
=============================================*/

#include "charge.h"

/*===============================================================
        @note      望处雨收云断，凭栏悄悄，目送秋光；
                   晚景萧疏，堪动宋玉悲凉。
                   水风轻、蘋花渐老；月露冷、梧叶飘黄。
                   遣情伤，故人何在？烟水茫茫。
                   难忘文期酒会，几孤风月，屡变星霜；
                   海阔山遥，未知何处是潇湘？
                   念双燕、难凭音信；指暮天、空识归航。
                   黯相望，断鸿声里，立尽斜阳。
==================================================================*/

static float sf_power_set;//输出功率设定
static int32 sl_voltage_input_set;//输出电压设定
static float sf_charge_pwm_output = 0;//充电脉冲占空比
static uint32 sul_last_cease_vibrate_time = 0;//上次停震时间
static uint16 sui_cease_vibrate_count = 0;//停震计数
static uint32 sul_cease_vibrate_average_interval;//平均停震时间间隔
static uint8 suc_cease_vibrate_head = 0;//首次停震序号
static uint32 sul_cease_vibrate_time_queue[CEASE_VIBRATE_QUEUE_LENGTH];//停震时间队列

/*===============================================================
	@brief     充电过程处理
==================================================================*/
void Charge_Process()
{
    static int16 si_heavy_change_counter;//过载计数
    static int16 si_light_change_counter;//轻载计数
    static uint32 sui_const_power_time;//恒功率模式时间记录
    static uint32 sui_const_voltage_time;//恒电压模式时间记录
    static uint32 sui_const_power_start_time;//恒功率模式起始时间记录
    int32 max_pwm_temp;
    int32 min_pwm_temp;
    static uint16 sui_lost_power_count;
    static uint8 suc_lost_power_flag;
    static int32 sl_last_voltage_output;
    uint32 d_time;
    int i;

    
    if(sf_power_set == 0)
    {
        sf_power_set = gl_power_set;
        //sf_charge_pwm_output = gl_power_set * gf_power_P+500;  //输出赋初值
    }
    sl_voltage_input_set = MAX(gl_voltage_input_set, gl_voltage_input_open/2 - 1500);
    
    gf_power_gradient = Detect_Power_Increase();


    if((gl_voltage_output < gl_voltage_output_set) && (gl_voltage_input>20000))
    {
        //CHARGE_ON;
    }
    else if((gul_time_100us > 50000) && (gl_voltage_input < 20000))
    {
        //CHARGE_OFF;//关闭mos
    }
    
/********************停震处理****************************/  
    guc_cease_vibrate_flag = Detect_Cease_Vibrate();//停震检测
    
    if((guc_cease_vibrate_flag==1))
    {
        if(sleep_ms(15,3))//停震瞬间突减
        {
            d_time = time_interval_100us(CV_TIMER_NUM);
            sul_last_cease_vibrate_time = gul_time_charge;//记录最近一次停震时间

            sul_cease_vibrate_time_queue[suc_cease_vibrate_head] = d_time;//缓存停震间隔
            suc_cease_vibrate_head++;
            sui_cease_vibrate_count++;
            suc_cease_vibrate_head %= CEASE_VIBRATE_QUEUE_LENGTH;
            //sf_charge_pwm_output -= (4+gl_voltage_output/2000.0)*MAX(1,3000.0/(float)gl_voltage_output);
            
            sf_charge_pwm_output -= (int32)((5*gl_power_set*gl_voltage_output/gl_voltage_input*23500)
          /(float)((gl_voltage_output*gl_voltage_output + gl_power_set*CHARGE_RESISTANCE)*CHARGE_CAPACITANCE + 1));

        }
        else //if(d_time<8000)           //停震持续期慢减
        {   
            sf_charge_pwm_output -= (int32)((0.4*gl_power_set*gl_voltage_output/gl_voltage_input*23500)
          /(float)((gl_voltage_output*gl_voltage_output + gl_power_set*CHARGE_RESISTANCE)*CHARGE_CAPACITANCE + 1));

        }
        
        if(guc_first_vibrate_flag == 0)
        {
            guc_first_vibrate_flag = 1;
            guc_overload_detection_flag = 0;
            //sf_charge_pwm_output -= 50;
        }
//        gl_pwm_set = sf_charge_pwm_output;
        //sf_charge_pwm_output *= 0.95;
        Beep_On();
    }
//    else if(guc_first_vibrate_flag == 0)
//    {
//        //sf_charge_pwm_output += (gl_power_set*0.7 - gl_power_output)/30000.0;
//    }
    else
    {
        Beep_Off();
        sl_last_voltage_output = (int32)(Limiter(gl_voltage_output - sl_last_voltage_output,-5,5) + sl_last_voltage_output);
    }
    
/************恒功率模式，过载转恒输入*****************/
    if((guc_overload_detection_flag == 0)
       &&(si_heavy_change_counter == 0)
       &&(gl_voltage_input < sl_voltage_input_set)
       ||(gl_voltage_input*sf_charge_pwm_output/100000*0.8 > gl_voltage_output))//输出被严重拉低
    {
        guc_overload_detection_flag = 1;
        guc_lightload_detection_flag = 0;
        si_heavy_change_counter = 0;
    }
    else if((guc_overload_detection_flag)//恒输入模式，轻载转恒功率
       &&(si_heavy_change_counter == 0)
       &&(guc_first_vibrate_flag == 1))
    {
        guc_overload_detection_flag = 0;
        si_heavy_change_counter = 0;
    }
    else if(si_heavy_change_counter > 0)
    {
        si_heavy_change_counter--;
    }
    
/************判断充电模式恒功率还是恒压***************/
    if((gl_voltage_output > gl_voltage_output_set + 0) && (guc_lightload_detection_flag == 0))//轻载电压过高，切换到恒压模式
    {
        if((gul_time_100us - sui_const_power_time > 1000)
           &&(si_light_change_counter > 100*1000 / PIT_1_PERIOD_MS))
        {
            guc_lightload_detection_flag = 1;
            sui_const_voltage_time = gul_time_100us;
            //sf_charge_pwm_output = 0;
        }
        else
        {
            si_light_change_counter++;
        }
    }
    else if(((gl_power_output > gl_power_set+100)||(gl_voltage_output < gl_voltage_output_set/2))
            &&(guc_overload_detection_flag == 0)
            &&(guc_lightload_detection_flag == 1))//重载功率过大，切换恒功率模式
    {
        if((gul_time_100us - sui_const_voltage_time >1000)
           &&(si_light_change_counter > 50*1000/PIT_1_PERIOD_MS))
        {
            guc_lightload_detection_flag = 0;
            sui_const_power_time = gul_time_100us;
        }
        else
        {
            si_light_change_counter++;
        }
    }
    else
    {
        if(si_light_change_counter > 0)
        {
            si_light_change_counter--;
        }
    }
    
/*****************自适应功率策略*******************/
    if(guc_charge_output_flag == ON)//允许充电PWM输出
    {
        if(guc_first_vibrate_flag == 0)
        {
            sui_const_power_start_time = gul_time_charge;
            sul_last_cease_vibrate_time = gul_time_charge;
            sul_cease_vibrate_average_interval = CEASE_VIBRATE_INTERVAL_SET;
            sui_cease_vibrate_count = 0;
            suc_cease_vibrate_head = 0;
        }
        else if(gul_time_charge - sui_const_power_start_time < 100000)
        {
            sui_cease_vibrate_count = 0;
            suc_cease_vibrate_head = 0;
        }
        
        else if(sui_cease_vibrate_count > 0)
        {
            //获取平均停震间隔
            for(i = 0, sul_cease_vibrate_average_interval = 0;i < MIN(sui_cease_vibrate_count, CEASE_VIBRATE_QUEUE_LENGTH); i++)//多次停震，求平均时间
            {
                sul_cease_vibrate_average_interval += sul_cease_vibrate_time_queue[i];
            }
            
            if(gul_time_charge - sul_last_cease_vibrate_time > sul_cease_vibrate_average_interval/2)
            {
                sul_cease_vibrate_average_interval += gul_time_charge - sul_last_cease_vibrate_time;
                sul_cease_vibrate_average_interval /= (MIN(sui_cease_vibrate_count, CEASE_VIBRATE_QUEUE_LENGTH) + 1);
            }
            else
            {
                sul_cease_vibrate_average_interval /= MIN(sui_cease_vibrate_count, CEASE_VIBRATE_QUEUE_LENGTH);
            }
        }
        else if(gul_time_charge - sul_last_cease_vibrate_time > CEASE_VIBRATE_INTERVAL_SET/2)//首次停震前，直接计算时间
        {
            sul_cease_vibrate_average_interval = gul_time_charge - sul_last_cease_vibrate_time;
        }
        
        if(fabs(CEASE_VIBRATE_INTERVAL_SET - sul_cease_vibrate_average_interval) > CEASE_VIBRATE_INTERVAL_SET/10)
        {
            sf_power_set += -0.000010 * Limiter(CEASE_VIBRATE_INTERVAL_SET - sul_cease_vibrate_average_interval,-1*sul_cease_vibrate_average_interval,CEASE_VIBRATE_INTERVAL_SET);
        }
    }
    
/******************充电模式切换**********************/
    if(guc_cease_vibrate_flag)
    {
        ;
    }
    else if(guc_lightload_detection_flag == 1)
    {
        Voltage_Control(gl_voltage_output_set);
    }
    else if(guc_overload_detection_flag == 0)
    {
        Constant_Power_Control((int32)sf_power_set);
    }
    else
    {
        Battery_Control(sl_voltage_input_set);
    }

    max_pwm_temp = (10000*(sl_last_voltage_output + gul_start_pwm + gul_start_pwm_coefficient*gl_power_set/1000)/(gl_voltage_input));//限幅，防止低电压时pwm过大
    min_pwm_temp = (10000*(gul_start_pwm + gul_start_pwm_coefficient*gl_power_set/1000)/(gl_voltage_input));//启动电压，快速达到设定功率
   
    if(max_pwm_temp > MAX_CHARG_PWM)
    {
        max_pwm_temp = MAX_CHARG_PWM;
    }
    if((gul_time_charge < 10000) && (guc_cease_vibrate_flag == 0))//0.1s
    {
        sf_charge_pwm_output = Limiter(sf_charge_pwm_output, min_pwm_temp, max_pwm_temp);
    }
    else
    {
        sf_charge_pwm_output = Limiter(sf_charge_pwm_output, 0, max_pwm_temp);
    }
 
    gl_charge_pwm_set = (int32)Limiter(sf_charge_pwm_output, 0, MAX_CHARG_PWM); 
    
    if(gl_voltage_input < gl_voltage_output)
    {
        sui_lost_power_count++;
        if(sui_lost_power_count > 100)
        {
            suc_lost_power_flag = 1;
        }
    }
    else
    {
        sui_lost_power_count = 0;
    }
    
    if(suc_lost_power_flag)
    {
        guc_charge_output_flag = ON;
        gl_charge_pwm_set = 10000;
    }
}

/*===============================================================
	@brief     增量式电压控制输出PWM
==================================================================*/
void Voltage_Control(int16 voltage_set)
{
    int16 voltage_error;
    static int16 last_voltage_error;
    int16 voltage_error_increase;
    int16 pwm_increase;
    if(guc_charge_output_flag)
    {   
        voltage_error = voltage_set - gl_voltage_output;
        
        voltage_error_increase = voltage_error - last_voltage_error;

        pwm_increase = (int16)(voltage_error_increase * gf_voltage_output_P + voltage_error * gf_voltage_output_I);//增量pwm+前馈补偿

        pwm_increase = (int16)Limiter(pwm_increase,-MAX_PWM_INC,MAX_PWM_INC);

        sf_charge_pwm_output += pwm_increase;
            
//        //**open-loop control**//
//        gl_charge_pwm_set = 10000l*voltage_set/gl_voltage_input;

        sf_charge_pwm_output = Limiter(sf_charge_pwm_output, 0, MAX_CHARG_PWM);
        
        last_voltage_error = voltage_error;
    }
}

/*===============================================================
	@brief     恒功率控制输出PWM
==================================================================*/
void Constant_Power_Control(int32 power_set)
{
    int32 power_error = 0;
    static int32 sl_last_power_error; 
//    static int32 sl_previous_power_error;
    int32 power_error_increase = 0;
//    int32 power_error_delta = 0;
//    float voltage_correction = 1;
    float pwm_increase = 0;

    if(guc_charge_output_flag)
    {
        power_error = power_set - gl_power_output;

        power_error_increase = power_error - sl_last_power_error;
//        power_error_delta = power_error - 2 * sl_last_power_error + sl_previous_power_error;//二阶差分项，用于增量式PID

        pwm_increase = (int32)(power_error_increase * gf_power_P + power_error * gf_power_I);//增量pwm

//        //以此式作为修正系数乘在增量输出上，就可以使控制器达到稳定并较好的实现恒功率
//        voltage_correction = (float)((60000*gl_power_set*gl_voltage_output/gl_voltage_input*23500)
//                        /(float)((gl_voltage_output*gl_voltage_output + gl_power_set*CHARGE_RESISTANCE)*CHARGE_CAPACITANCE + 1));
//        
//        pwm_increase *= voltage_correction;

        //单次限幅
        if((gl_power_output > 5000) && (gul_time_charge > 800))
        {
            pwm_increase = Limiter(pwm_increase, -MAX_PWM_INC_DELAY, MAX_PWM_INC_DELAY);
        }
        else
        {
            pwm_increase = Limiter(pwm_increase, -MAX_PWM_INC, MAX_PWM_INC);
        }
        

        sf_charge_pwm_output += pwm_increase;

        //输出限幅
        sf_charge_pwm_output = Limiter(sf_charge_pwm_output, 0, MAX_CHARG_PWM);
        
//        sl_previous_power_error = sl_last_power_error;
        sl_last_power_error = power_error;
    }
}

/*===============================================================
	@brief     电容电压控制输出PWM
==================================================================*/
void Battery_Control(int16 voltage_set)
{
    int16 voltage_error;
    static int16 last_voltage_error;
    static int16 si_counter;
    int16 voltage_error_increase;
    float voltage_correction;
    float pwm_increase;

    voltage_error = (voltage_set - gl_voltage_input);
    voltage_error = (int32)Limiter(voltage_error,-5000,5000);
    voltage_error_increase = voltage_error - last_voltage_error;

    pwm_increase = (int32)(voltage_error_increase * gf_voltage_input_P + voltage_error * gf_voltage_input_I);//增量pwm+前馈补偿

    //以此式作为修正系数乘在增量输出上，就可以使控制器达到稳定并较好的实现恒功率
    voltage_correction = (float)((2*gl_power_set*gl_voltage_output/gl_voltage_input*23500)
                    /(float)((gl_voltage_output*gl_voltage_output + gl_power_set*CHARGE_RESISTANCE)*CHARGE_CAPACITANCE + 1));
    
    pwm_increase *= voltage_correction;
    
    //单次限幅
    pwm_increase = Limiter(pwm_increase, -MAX_PWM_INC, MAX_PWM_INC);
    
    sf_charge_pwm_output += pwm_increase;

    if((voltage_error > 3000) && (si_counter == 0))  //
    {
        //sl_charge_pwm_output /= (voltage_set/gl_voltage_input);
        
        //sl_charge_pwm_output *= gl_voltage_input;
        //sl_charge_pwm_output /= voltage_set;
        
        si_counter = 100;      //防止短时间重入多次
    }
    
    if(si_counter > 0)
    {
        si_counter--;
    }

    //输出限幅
    sf_charge_pwm_output = Limiter(sf_charge_pwm_output, 0, MAX_CHARG_PWM);
    
    last_voltage_error = voltage_error;
}

/*===============================================================
	@brief     功率及输入电压下降检测停震
==================================================================*/
uint8 Detect_Cease_Vibrate()
{
    static int32 sl_voltage_input_buff[CEASE_VIBRATE_BUFF_LENGTH];//输入电压缓存队列
    static int32 sl_current_input_buff[CEASE_VIBRATE_BUFF_LENGTH];//输入电压缓存队列
    int32 voltage_input_difference = 0;
    int32 current_input_difference = 0;
    int8 i;
    
    for(i = CEASE_VIBRATE_BUFF_LENGTH - 1; i>0; i--)
    {
        sl_voltage_input_buff[i] = sl_voltage_input_buff[i-1];
        sl_current_input_buff[i] = sl_current_input_buff[i-1];
    }
    sl_voltage_input_buff[0] = gl_voltage_input;
    sl_current_input_buff[0] = gl_current_input;
    
    for(i=0; i < CEASE_VIBRATE_BUFF_LENGTH/2; i++)
    {
        voltage_input_difference += sl_voltage_input_buff[i] - sl_voltage_input_buff[i + CEASE_VIBRATE_BUFF_LENGTH/2];
        current_input_difference += sl_current_input_buff[i] - sl_current_input_buff[i + CEASE_VIBRATE_BUFF_LENGTH/2];
    }
    
    //逐差法计算变化率，mv/ms，ma/ms
    voltage_input_difference *= (1/PIT_1_PERIOD_MS/(CEASE_VIBRATE_BUFF_LENGTH/2)/(CEASE_VIBRATE_BUFF_LENGTH/2));
    current_input_difference *= (1/PIT_1_PERIOD_MS/(CEASE_VIBRATE_BUFF_LENGTH/2)/(CEASE_VIBRATE_BUFF_LENGTH/2));
    
    //停震后发射线圈不再产生磁场，输入电压骤降
    if(//(sl_voltage_input_buff[0]-sl_voltage_input_buff[2]<-300)//近处电压变化率
       //(sl_voltage_input_buff[0]-sl_voltage_input_buff[9]<-5000)//平均变化率
       (guc_cease_vibrate_flag==0)
       &&(voltage_input_difference < -400)
       &&(sl_current_input_buff[9]/sl_current_input_buff[0]>2)
        //||(gl_voltage_input<28000)
       ||(gl_voltage_input < gl_voltage_input_set - 1000))      
    {
            guc_cease_vibrate_flag = 1;
            sleep_ms(10,4);//开始计时
    }    
    else if((guc_cease_vibrate_flag==1)//停震恢复后输入电流骤增
            &&(sl_current_input_buff[0]/sl_current_input_buff[2] > 2)
            &&(sl_current_input_buff[0] - sl_current_input_buff[2] > 200))
    {
        guc_cease_vibrate_flag = 0;
    }
    else if((guc_cease_vibrate_flag==1) && (sleep_ms(10,4)))//防止卡死在掉电状态
    {
        guc_cease_vibrate_flag = 0;
    }
    return guc_cease_vibrate_flag;
}

/*===============================================================
	@brief     检测功率上升
==================================================================*/
int32 Detect_Power_Increase()
{
    static int32 sl_power_queue[POWER_QUEUE_LENGTH];
    static uint16 sui_power_head;
    uint16 i;
    
    static int32 power_increase_summary = 0;
    int32 power_increase = 0;
    
    power_increase_summary = 0;
    sl_power_queue[sui_power_head] = gl_power_output;
    
    //循环队列逐差法求斜率
    for(i=0; i < POWER_QUEUE_LENGTH/2; i++)      
    {
        power_increase_summary += sl_power_queue[(sui_power_head+i) % POWER_QUEUE_LENGTH] 
                    - sl_power_queue[(sui_power_head + i + POWER_QUEUE_LENGTH/2) % POWER_QUEUE_LENGTH];
    }
    
    power_increase = (1000/PIT_1_PERIOD_MS) * power_increase_summary
        /(POWER_QUEUE_LENGTH * POWER_QUEUE_LENGTH/4);
    
    
    sui_power_head++ ;
    sui_power_head %= POWER_QUEUE_LENGTH;
    
    return power_increase;
}

/*===============================================================
	@brief     充电控制
==================================================================*/
void Charge_Control()
{
    static uint32 sui_charge_start_time;
    static uint16 sui_voltage_start_count = 0;

    int16 l_voltage_start_set = 0;
    int16 l_voltage_output = 0;
    
    if(guc_charge_output_flag == OFF)
    {
        guc_first_vibrate_flag = 0;
    }
    
    l_voltage_output = gl_voltage_output;
    l_voltage_start_set = gl_voltage_start_set;
        
    if(l_voltage_output > l_voltage_start_set)
    {
        sui_voltage_start_count++;
    }
    else 
    {
        sui_voltage_start_count = 0;
    }
    
    if((!guc_charge_complete_flag) && (sui_voltage_start_count > VOLTAGE_START_COUNT))
    {
        guc_charge_complete_flag = OK;
    }

    if((!guc_charge_complete_flag) && (guc_charge_output_flag == 1))//充电过程，实时计时
    {
        gul_time_charge = gul_time_100us - sui_charge_start_time;
    }
    else if(guc_charge_output_flag == 0)//等待充电，停止计时
    {
        sui_charge_start_time = gul_time_100us;
        guc_charge_complete_flag = 0;
    }

}

/*===============================================================
	@brief     恒功率充电测试
==================================================================*/
void Constant_Power_Charge_Test()
{
    int8 string[30];
    uint8 ui_button_value = 0;
    uint32 button_time;
    
    guc_charge_output_flag = OFF;
    gul_time_charge = 0;
    sf_power_set = gl_power_set;
    
    oled_fill(0x00);
    oled_display_6x8str(20,0,"charge test");

    while(1)
    {
	ui_button_value = button_scan();
        switch (ui_button_value)
        {
          case FIVE_DIR_UP:
          gl_power_set += 1000;
          sf_power_set = gl_power_set;
                  break;

          case FIVE_DIR_DOWN:
          if((gl_power_set > 1000))
          {
            gl_power_set -= 1000;
            sf_power_set = gl_power_set;
          }
                  break;

          case FIVE_DIR_LEFT:
                  oled_fill(0x00);
                  return ;
                  break;

          case FIVE_DIR_RIGHT:
                  oled_fill(0x00);
                  return ;
                  break;

          case FIVE_DIR_OK:
          
          if(gl_voltage_output < 2000)
          {
              guc_charge_output_flag = ON;
              if( gul_time_100us - button_time > 100000)
              {
                  button_time = gul_time_100us;
                  guc_charge_output_flag = !guc_charge_output_flag;
                  sf_power_set = gl_power_set;
                  guc_first_vibrate_flag = 0;
              }
              Beep_Off();
          }
          else
          {
              oled_display_16x8str(0,0,"HIGH OUTPUT!");
              sleep_ms(1000,4);
          }
                  break;

          default:
                  break;
          }
        //电容充电电压与充电功率设定值
        sprintf(string,"V_set:%6dmV P_set:%6duw",gl_voltage_charge_set, (int16)sf_power_set);
        oled_display_6x8str(0,1,string); 
        //输入电压与输出电压检测值
        sprintf(string,"V_input:%6dmV V_output:%6dmV", gl_voltage_input, gl_voltage_output );
        oled_display_6x8str(0,2,string);
        //输入电流与输出功率检测值
        sprintf(string,"I_input:%6dmA P_output:%6duw", gl_current_input, gl_power_output);
        oled_display_6x8str(0,3,string);
        //充电输出占空比与过载轻载检测
        sprintf(string,"PWM:%5d  %s",gl_charge_pwm_set ,guc_lightload_detection_flag?("light"):((guc_overload_detection_flag)?"heavy":"mid  "));
        oled_display_6x8str(0,4,string);
        //开路输入电压与功率变化检测
        sprintf(string,"Vopen_input:%5d P_gradient:%5d",gl_voltage_input_open, gf_power_gradient);
        oled_display_6x8str(0,5,string);
        //输入电流与输出功率检测值
        sprintf(string,"time:%5d s DT:%5d",(int16)(gul_time_charge/10000), sul_cease_vibrate_average_interval);
        oled_display_6x8str(0,7,string);     
    }
}

/*===============================================================
	@brief     设置启动电压
==================================================================*/
void Set_Start_Voltage()
{
    static uint8 ui_button_value;
    uint8 exit_flag = 0;
    int16 infrared_value = 0;
    
    oled_init();
    while(exit_flag==0)
    {
        ui_button_value = button_scan();
        
        infrared_value = get_infrared_command();
        reset_infrared();
        
        switch(infrared_value)
        {
            case COMMAND_UP:
                ui_button_value = FIVE_DIR_UP;
                break;
            case COMMAND_DOWN:
                ui_button_value = FIVE_DIR_DOWN;
                break;
            case COMMAND_LEFT:
                ui_button_value = FIVE_DIR_LEFT;
                break;
            case COMMAND_RIGHT:
                ui_button_value = FIVE_DIR_RIGHT;
                break;                
            case COMMAND_OK:
                ui_button_value = FIVE_DIR_OK;
                break;
            default:
                break;                                    
        }
        
        switch(ui_button_value)
        {
            case FIVE_DIR_UP:
              gl_voltage_start_set += 100;
                break;
            case FIVE_DIR_DOWN:
              gl_voltage_start_set -= 100;
                break;
            case FIVE_DIR_LEFT:
              exit_flag = 1;
                break;
            case FIVE_DIR_RIGHT:
              exit_flag = 1;
                break;
            case FIVE_DIR_OK:
              SaveFlashParameter();
              exit_flag = 1;
                break;
            default :
                break;
        }
                
        gl_voltage_start_set = (int32)Limiter(gl_voltage_start_set,3300,11000);
        
        oled_display_16x8str(15,0,"Start Voltage");
        
        oled_print_short(35,4,gl_voltage_start_set);
        
        oled_display_16x8str(1,6,"V_in");
        
        oled_print_short(50,6,(gl_voltage_input/10));
    }
    SaveFlashParameter();
    oled_fill(0x00);
}

/*===============================================================
	@brief     设定充电PWM
==================================================================*/
void Set_Charge_PWM()
{
    static uint8 ui_button_value;
    uint8 exit_flag = 0;
    int16 infrared_value = 0;
    
    oled_init();
    while(exit_flag==0)
    {
        ui_button_value = button_scan();
        
        infrared_value = get_infrared_command();
        reset_infrared();
        
        switch(infrared_value)
        {
            case COMMAND_UP:
                ui_button_value = FIVE_DIR_UP;
                break;
            case COMMAND_DOWN:
                ui_button_value = FIVE_DIR_DOWN;
                break;
            case COMMAND_LEFT:
                ui_button_value = FIVE_DIR_LEFT;
                break;
            case COMMAND_RIGHT:
                ui_button_value = FIVE_DIR_RIGHT;
                break;                
            case COMMAND_OK:
                ui_button_value = FIVE_DIR_OK;
                break;
            case COMMAND_POWER:
                gl_charge_pwm_set = 0;
                break;    
            default:
                break;                                    
        }
        
        switch(ui_button_value)
        {
            case FIVE_DIR_UP:
              gl_charge_pwm_set += 100;
                break;
            case FIVE_DIR_DOWN:
              gl_charge_pwm_set -= 100;
                break;
            case FIVE_DIR_LEFT:
              exit_flag = 1;
                break;
            case FIVE_DIR_RIGHT:
              exit_flag = 1;
                break;
            case FIVE_DIR_OK:
              SaveFlashParameter();
              exit_flag = 1;
                break;
            default :
                break;
        }
        
        gl_charge_pwm_set = (int32)Limiter(gl_charge_pwm_set,0,8000);
                
        oled_display_16x8str(15,0,"Charge PWM");
        
        oled_print_short(35,4,gl_charge_pwm_set);
        
        oled_display_16x8str(1,6,"V_in");
        
        oled_print_short(50,6,(gl_voltage_input/10));
    }
    SaveFlashParameter();
    oled_fill(0x00);
}

/*===============================================================
	@brief     充电启动测试
==================================================================*/
uint8 Charge_Start_Test()
{
    uint8 exit_flag = 0;
//    int16 charge_pwm_set = 1500;
    int16 infrared_value;
    uint16 ui_button_value = 0;

    uint8 power_off = 0;
    
//    static uint8 gyro_calibration_flag = 0;
    

    exit_flag = 0;
    

    if(guc_automation_mode_flag == OFF)
    {
        Set_Start_Voltage();
        Set_Charge_PWM();
    }

    
//    charge_pwm_set = gl_charge_pwm_set;
    
    guc_charge_output_flag = ON;
    
    CHARGE_ON;
    //oled_init();
    while(!exit_flag)
    { 
        ui_button_value = button_scan();
        
//        if((gyro_calibration_flag == OFF) && (guc_automation_mode_flag == OFF))
//        {
//            GYRO_Set_Zero();//陀螺仪消除零漂
//            gyro_calibration_flag = ON;
//        }
        
        
        infrared_value = get_infrared_command();
        reset_infrared();
        
        switch(infrared_value)
        {
            case COMMAND_LEFT:
                ui_button_value = FIVE_DIR_LEFT;
                break;
            case COMMAND_RIGHT:
                ui_button_value = FIVE_DIR_RIGHT;
                break;       
            case COMMAND_OK:
                ui_button_value = FIVE_DIR_OK;
                break;
            case COMMAND_POWER:
                power_off = 1;
//                charge_pwm_set = 0;
                break;   
            default:
                break;                                    
        }
        
//        Change_PWM_Charge(charge_pwm_set);
        
        gl_charge_pwm_set = (int32)sf_charge_pwm_output;
                
        if(ui_button_value == FIVE_DIR_OK)
        {
            guc_charge_complete_flag = 1;
            guc_charge_output_flag = 0;
            guc_car_run_flag = FLAG_ON;
            guc_beep_start_flag = FLAG_ON;
            guc_lost_start_flag = FLAG_ON;
            exit_flag = 1;
            oled_init();
            gl_charge_pwm_set = 0;
            Change_PWM_Charge(0);
            LPLD_GPIO_Output_b(CHARGE_CONTROL_PORT, CHARGE_CONTROL_PIN_MASK, OFF);
            return 1;
        }
        
        //左右键解决花屏
        if((ui_button_value == FIVE_DIR_LEFT) || (ui_button_value == FIVE_DIR_RIGHT))
        {
            oled_init();
        }
        
        if(power_off == 1)
        {
            exit_flag = 1;
            return 0;
        }
        

        if(guc_charge_complete_flag == OK) //计数，看持续变化
        {
            exit_flag = 1;
            guc_charge_output_flag = 0;
            guc_car_run_flag = FLAG_ON;
            guc_beep_start_flag = FLAG_ON;
            guc_lost_start_flag = FLAG_ON;
            gul_time_start = 0;
            oled_init();
            gl_charge_pwm_set = 0;
            Change_PWM_Charge(0);
            LPLD_GPIO_Output_b(CHARGE_CONTROL_PORT, CHARGE_CONTROL_PIN_MASK, OFF);
//                gf_pitch_angle = 0;//俯仰角发车后初始化为零，用于坡道判断
            return 1;
        }


        oled_display_6x8str(18,0,"Charge Start");
        oled_display_6x8str(1,2,"err");
        oled_display_6x8str(1,3,"V_out");
        oled_display_6x8str(1,4,"V_in");
        oled_display_6x8str(1,5,"I_in");
        oled_display_6x8str(1,6,"PWM");
        oled_display_6x8str(1,7,"V_start");

        oled_print_short(50,2,gl_car_error);
        oled_print_short(50,3,gl_voltage_output);
        oled_print_short(50,4,(gl_voltage_input/10));
        oled_print_short(50,5,gl_current_input);
        oled_print_short(50,6,gl_charge_pwm_set);
        oled_print_short(50,7,gl_voltage_start_set);

        gf_visual_scope_data[0] = gl_voltage_output;
        gf_visual_scope_data[1] = gl_voltage_input/10;
        gf_visual_scope_data[2] = gl_current_input*10;
        gf_visual_scope_data[3] = gl_power_output;

        VisualScope_Send(SCOPE_DATA_MODE);//待测试蓝牙      
    }
    
    oled_fill(0x00);
    return 0;
}
