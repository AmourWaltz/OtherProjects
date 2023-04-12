#include "include.h"

void PIT_Process_100us()
{
    gul_time_100us++;
}

void PIT_Process_5ms()
{
    Speed_Control();
    Infrared_Directional();
    //Barrier_Distance();
    //Get_Ultrasonic_Value();
}

void PIT_Process_5s()
{
//    Car_Back();
}

void PIT_Process_10us()
{

}

void SoftIsr_Open(IRQn_Type irqx)
{
  if (irqx < 32)
  {
    NVIC->ISPR[0] |= ((uint32)1 << (irqx % 32));
  }
  else if (irqx < 64)
  {
    NVIC->ISPR[1] |= ((uint32)1 << (irqx % 32));
  }
  else if (irqx < 96)
  {
    NVIC->ISPR[2] |= ((uint32)1 << (irqx % 32));
  }
  else
  {
    NVIC->ISPR[3] |= ((uint32)1 << (irqx % 32));
  }
}
