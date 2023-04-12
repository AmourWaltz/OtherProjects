#include "general.h"

//限幅函数
float Limiter(float value, float min, float max)
{
	if(value > max)
	{
		value = max;
	}
	else if(value < min)
	{
		value = min;
	}
	else
	{
		value=value;
	}
	return value;
}

void least_spuare(int *array, int lenth, float *k, float * b, float *r)		//最小二乘法拟合
{
    long sum_xy, sum_x, sum_y, sum_x2, sumx_2, sum_y2, sumy_2;
    int i, x, y;
    float temp_k;
    float devider;
    if(lenth<0)
    {
        lenth = 0;
    }
    for(i=0,sum_xy=0, sum_x=0, sum_y=0, sum_x2=0, sumx_2=0, sum_y2=0, sumy_2=0; i<lenth; i++)
    {
        y = array[i];
        x = i;
        sum_xy += x*y;
        sum_y += y;
        //sum_x += x;
        sum_y2 += y*y;
    }
    sum_x = lenth*(lenth-1)/2;
    sumx_2 = sum_x*sum_x;
    sum_x2 = lenth*(lenth-1)*(2*lenth-1)/6;
    sumy_2 = sum_y*sum_y;
    if((lenth*sum_x2-sumx_2)!=0)
    {
        devider = (lenth*sum_x2-sumx_2);
        temp_k = (lenth*sum_xy-sum_x*sum_y)/(float)(lenth*sum_x2-sumx_2);
        if(devider != 0)
        {
            temp_k = (lenth*sum_xy-sum_x*sum_y)/devider;
        }
        else
        {
            temp_k = 0;
        }
    }
    else
    {
        temp_k=0;
    }
    if(b != NULL)
    {
        if(lenth != 0)
        {
            *b = sum_y/lenth - (temp_k)*(sum_x/lenth);
        }
        else
        {
            *b = 0;
        }
    }
    if(r != NULL)
    {
        devider = (lenth*sum_y2-sumy_2);
        *r = (temp_k)*sqrt((lenth*sum_x2-sumx_2)/(float)(lenth*sum_y2-sumy_2));
        if(devider != 0)
        {
            *r = (temp_k)*sqrt((lenth*sum_x2-sumx_2)/devider);
        }
        else
        {
            *r = 0;
        }
    }
    *k = temp_k;
}

//y=ax+b
//a=(N*Σxy-ΣxΣy)/(N*Σx^2-(Σx)^2)
//b=y(平均)-a*x（平均）

//r^2=(N*Σxy-ΣxΣy)^2 / ((N*Σx^2-(Σx)^2)*(N*Σy^2-(Σy)^2))

//返回值 p100a  a 乘了100
void LinerFit(int16 *x, int16 *y, uint8 u8Num, float *pa, float *pb, float *pr_2, int32 *p100a)
{
    int32 i32Temp1, i32Temp2, i32Temp3;
    
    double f32Temp;
    
    //int32 i32youhua;
    
    i32Temp1 = u8Num*SigmaXY(x, y, u8Num) - Sigma(x, u8Num)*Sigma(y, u8Num);
    i32Temp2 = u8Num*SigmaXX(x, u8Num) - Sigma(x, u8Num)*Sigma(x, u8Num);
    
    if (pa != NULL)
    {    
        *pa = i32Temp1 / (double)i32Temp2;
    }
    
    if (p100a != NULL)
    {
        *p100a = 100 * i32Temp1 / i32Temp2;
    }
    
    if (pb != NULL)
    {
        //没要求计算a
        if (*pa == NULL)
        {
            *pa = i32Temp1 / (double)i32Temp2;
        }
        
        *pb = Average(y, u8Num) - (*pa) * Average(x, u8Num);
    }
    
    if (pr_2 != NULL)
    {
        i32Temp3 = u8Num*SigmaXX(y, u8Num) - Sigma(y, u8Num)*Sigma(y, u8Num);       
        f32Temp = i32Temp1 / (double)i32Temp2;        
            
        *pr_2 = f32Temp * i32Temp1 / i32Temp3;
    }    
 
}

int16 Sigma(int16 *Data, uint8 u8Num)
{
    uint8 u8i;
    int16 fSigma = 0;
    
    for(u8i=0; u8i<u8Num; u8i++)
    {        
        fSigma += Data[u8i];       
    }
    
    return fSigma;
}

float Average(int16 *Data, uint8 u8Num)
{
    float fAverage;
    
    fAverage = Sigma(Data, u8Num) / (float)u8Num;
    
    return fAverage;
}

int32 SigmaXY(int16 *x, int16 *y, uint8 u8Num)
{
    int16 fXY;
    uint8 u8i;    
    int32 fSigma = 0;
    
    for(u8i=0; u8i<u8Num; u8i++)
    {   
        fXY = x[u8i] * y[u8i];
        fSigma += fXY;       
    }
    
    return fSigma;   
}
    
int32 SigmaXX(int16 *Data, uint8 u8Num)  
{
    int16 fXX;
    uint8 u8i;    
    int32 fSigma = 0;
    
    for(u8i=0; u8i<u8Num; u8i++)
    {   
        fXX = Data[u8i] * Data[u8i];
        fSigma += fXX;       
    }
    
    return fSigma;    
}

void Delay_ms(uint32 u32time)
{
    uint32 u32SetTime;
    uint32 u32CurrentTime; 
    uint32 u32Cnt = 0;
    
    if (u32time == 0)
    {
        return;
    }
    
    u32time *= 10;
    u32SetTime = gul_time_100us;
    u32CurrentTime = gul_time_100us;
    
    while(((u32CurrentTime - u32SetTime) < u32time) && (u32Cnt < 0xffffff))
    {
        u32CurrentTime = gul_time_100us;    
        u32Cnt++;
    }  
}

#define MAX_TIME_CHANNEL 10
/*定时函数，   不占用系统时间
*当定时通道定时到时，返回1
*定时未到时，返回0
*只有定时到之后，才可设置有效的下一次定时
*/
uint8 sleep_ms(uint32 u32time,uint8 channel)
{
        static uint32 time_set[MAX_TIME_CHANNEL];
        static uint8 u8_is_timeup[MAX_TIME_CHANNEL];
        static uint8 flag=0;
        int8 i = 0;
        
        if(flag == 0)
        {
            for(i=0;i<MAX_TIME_CHANNEL;i++)
            {
                time_set[i] = 0;
                u8_is_timeup[i] = 1;
            }
            flag = 1;
        }
        
	if(channel<=MAX_TIME_CHANNEL)
	{
		if(u8_is_timeup[channel]==1)
                {
                      time_set[channel] = gul_time_100us + u32time*10;
                      u8_is_timeup[channel] = 0;
                }
                else if(u8_is_timeup[channel]==0)
                {
                     if(gul_time_100us>=time_set[channel])
                     {
                         u8_is_timeup[channel] = 1; 
                         time_set[channel] = gul_time_100us + u32time*10;
                     }
                }
                return u8_is_timeup[channel];
	}
        else
        {
             return 1;
        }
}

uint32 time_interval_100us(uint8 channel)	//可以提供多个计时通道
{
	static uint32 time_temp[MAX_TIME_CHANNEL];
	uint32 time_interval = 0;
	if(channel<=MAX_TIME_CHANNEL)
	{
		time_interval = gul_time_100us - time_temp[channel];
		time_temp[channel] = gul_time_100us;
	}
	
	return time_interval;
}

int32 power(int16 data, int16 index)
{
    int32 returnValue = 1;
    int16 loopi = 0;
    
    for(loopi = 0; loopi < index; loopi ++)
    {
        returnValue = returnValue * data;
    }
    
    return returnValue;
}

int16 getSimpilfiedAverage(int16 *data,int start,int end)
{
    int32 sum = 0;
    int16 loopi = 0;
    
    for(loopi = start; loopi < end; loopi ++)
    {
        sum += (10 * data[loopi]);
    }
    sum = sum / (end - start);
    
    return (int16)sum; 
}

//计算一组数据的简化方差，防止数据运算量过大
void getSimpilfiedVariance(int16 *data, int start,int end, int32* variance)
{
    int32 varianceValue = 0;
    int16 delt = 0;
    int16 loopi = 0;
    int16 averageNum = getSimpilfiedAverage(data, start,end);

    int16 i16x[64];
    int8 i8i;
    float fa, fb;
        
        for (i8i=0; i8i<64; i8i++)
        {       
            i16x[i8i] = i8i;       
        }                 
        
        LinerFit(&(i16x[start]), &(data[start]), end-start, &fa, &fb, NULL, NULL);  
        //i16ColTemp = (int16)(i8i*fa + fb);
    
    
    for(loopi = start; loopi < end; loopi ++)
    {
        delt = 10 * (data[loopi] - (int16)(loopi*fa + fb));
        varianceValue = varianceValue + (power(delt, 2));
    }
    *variance = varianceValue / (end - start) / 10;
}
/*void getVariance(int16* data,int start,int end,int32* variance)
{
  int i,sum = 0;
  int32 average = 0;
  int32 average_sum = 0;
  
  sum  = end - start;
  
  for(i=start;i<end;i++)
  {
	  average = average + data[i];
	  average_sum = average_sum + data[i] * data[i];
  }
  
  average = average/sum;
  *variance = average_sum/sum - average*average;
  
}*/

int16 i16ArctanTable_1[50] = 
{
0,
11,
23,
34,
46,
57,
68,
80,
91,
102,
113,
124,
135,
146,
156,
167,
177,
188,
198,
208,
218,
228,
237,
247,
256,
266,
275,
284,
292,
301,
310,
318,
326,
334,
342,
350,
358,
365,
372,
380,
387,
394,
400,
407,
413,
420,
426,
432,
438,
444
};


int16 i16ArctanTable_2[30] = 
{
450,
477,
502,
524,
545,
563,
580,
595,
609,
622,
634,
645,
656,
665,
674,
682,
690,
697,
703,
710,
716,
721,
726,
731,
736,
741,
745,
749,
753,
756 
};

int16 i16ArctanTable_3[10] = 
{
760,
772,
782,
791,
799,
805,
811,
816,
821,
825
};

int16 i16ArctanTable_4[11] = 
{
829,
837,
843,
848,
852,
856,
859,
862,
864,
866,
868
};

//查表法算角度
int16 _100aToPointAngle(int32 i32_100a)
{
    int16 i16Angle = 0;
    int8 i8Sign = 1;
    uint8 u8Index = 0;
    
    if (i32_100a < 0)
    {
        i8Sign = -1;
        i32_100a = -i32_100a;
    }
    
    if (i32_100a < 100)
    {
        u8Index = i32_100a / 2;
        i16Angle = i16ArctanTable_1[u8Index];
    }
    else if (i32_100a < 400)
    {
        u8Index = (i32_100a-100) / 10;
        i16Angle = i16ArctanTable_2[u8Index];               
    }
    else if (i32_100a < 800)
    {
        u8Index = (i32_100a-400) / 40;
        i16Angle = i16ArctanTable_3[u8Index];               
    }   
    else if (i32_100a < 1800)
    {
        u8Index = (i32_100a-800) / 100;
        i16Angle = i16ArctanTable_4[u8Index];          
    }
    else
    {
        i16Angle = i16ArctanTable_4[10];
    }
    
    return (i16Angle*i8Sign);
    
}

float GP_tan(float x)
{
    return (x + x*x*x/3.0 + x*x*x*x*x/5.0);
}

/**
 *  @ brief   sum_average
 *
 *  @ param   the pointer of the p_num
 *
 *	@ note    get the Array's sum_average
 *
 *	@ return  sum_average_value
 */
float sum_average(int16 *p_num)
{
	uint8 loop_i = 0;
	float sum_average_value = 0;

	for(loop_i = 0; loop_i < LEAST_SQUARES_NUMBERS; loop_i ++)
	{
		sum_average_value = sum_average_value + p_num[loop_i];
	}

	sum_average_value = (float)(sum_average_value / LEAST_SQUARES_NUMBERS);

	return (sum_average_value);
}

/**
 *  @ brief   sum_multiplied
 *
 *  @ param   the pointer of the p_num_x,
 *  		  and the pointer of the p_num_y
 *
 *	@ note    get the Array's sum_multiplied
 *
 *	@ return  multiplied_sum_value
 */
float multiplied_sum(int16 *p_num_x, int16 *p_num_y)
{
	uint8 loop_i = 0;
	float multiplied_sum_value = 0;

	for(loop_i = 0; loop_i < LEAST_SQUARES_NUMBERS; loop_i ++)
	{
		multiplied_sum_value = (float)(multiplied_sum_value + p_num_x[loop_i] * p_num_y[loop_i]);
	}

	return (multiplied_sum_value);
}

/**
 *  @ brief   squre_sum
 *
 *  @ param   the pointer of the p_num_x,
 *  		  and the pointer of the p_num_y
 *
 *	@ note    get the Array's sum_multiplied
 *
 *	@ return  multiplied_sum_value
 */
float squre_sum(int16 *p_num)
{
	uint8 loop_i = 0;
	float squre_sum_value = 0;

	for(loop_i = 0; loop_i < LEAST_SQUARES_NUMBERS; loop_i ++)
	{
		squre_sum_value = squre_sum_value + p_num[loop_i] * p_num[loop_i];
	}

	return (squre_sum_value);
}

/**
 *  @ brief   least_squares_value
 *
 *  @ param   the pointer of the p_sensor_value,
 *
 *	@ note    get the expected sensor value by least squares
 *
 *	@ return  expected_sensor_value
 */
int16 least_squares_value(int16 *p_sensor_value)
{
	static uint8  s_first_flag = 1;
	static int16  s_x_value[LEAST_SQUARES_NUMBERS]    = {0};
	static int16  s_y_value[LEAST_SQUARES_NUMBERS]    = {0};

	static float s_x_sum_average = 0;   // 数组 X[N] N个元素求和并求平均值
	static float s_x_square_sum  = 0;   // 数组 X[N] N个元素的平方和
	float y_sum_average = 0;            // 数组 Y[N] N个元素求和并求平均值
	float x_y_multiplied_sum = 0;       // 数组 X[N] Y[N] N个元素乘积并求和

	float slope_k = 0;                  // 斜率
	float intercept_r = 0;              // 截距

	int16 expected_sensor_value = 0;

	int8 loop_i = 0;

	if(s_first_flag == 1)
	{
		s_first_flag = 0;
		for(loop_i = 0; loop_i < LEAST_SQUARES_NUMBERS; loop_i ++)
		{
			s_x_value[loop_i] = loop_i;
		}

		s_x_sum_average = sum_average(s_x_value);
		s_x_square_sum  = squre_sum(s_x_value);
	}

	for(loop_i = 0; loop_i < LEAST_SQUARES_NUMBERS; loop_i ++)
	{
		s_y_value[loop_i] = p_sensor_value[loop_i];
	}

	y_sum_average = sum_average(s_y_value);
	x_y_multiplied_sum = multiplied_sum(s_x_value, s_y_value);


	slope_k = ((float)( x_y_multiplied_sum - (LEAST_SQUARES_NUMBERS * s_x_sum_average * y_sum_average))
			/ (float)(( s_x_square_sum - (float)(LEAST_SQUARES_NUMBERS * s_x_sum_average * s_x_sum_average))));

	intercept_r = ((float)y_sum_average - (float)slope_k * s_x_sum_average);

	expected_sensor_value = (uint16) ((int16)(slope_k * (0-1) + intercept_r));

	return (expected_sensor_value);
}

float f_sqrt(float a)   
{
    float x=1;
    float y=a;
    
    while(fabs(x-y)>1e-3)
    {
        x=y;
        y=(x+a/x)/2;
    }
    return y;
}
uint32 i_sqrt(uint32 a)
{
    uint32 x=1;
    uint32 y=a;
    

    while(fabs(x-y)>1)
    {
        x=y;
        y=(x+a/x+1)/2;      //+1防止出现除零错
    }
    return y;
}