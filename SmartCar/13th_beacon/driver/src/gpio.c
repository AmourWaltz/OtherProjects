#include "gpio.h"

void GPIO_Init(GPIO_Type *PTx, uint32 GPIO_Pinx, uint8 Dir, uint8 Level)
{
  static GPIO_InitTypeDef gpio_init_struct;
  
  switch(Dir)
  {
      case DIR_INPUT:
        gpio_init_struct.GPIO_PTx = PTx;
        gpio_init_struct.GPIO_Pins = GPIO_Pinx;                  //注意：初始化时得用GPIO_Pinx，而LPLD_GPIO_Output_b中直接用x
        gpio_init_struct.GPIO_Dir = DIR_INPUT;
        gpio_init_struct.GPIO_Output = OUTPUT_L;
        gpio_init_struct.GPIO_PinControl = IRQC_DIS;             //内部(上/下)拉|不产生中断
        LPLD_GPIO_Init(gpio_init_struct);
        break;
        
      case DIR_OUTPUT:
        gpio_init_struct.GPIO_PTx = PTx;
        gpio_init_struct.GPIO_Pins = GPIO_Pinx;                  //注意：初始化时得用GPIO_Pinx，而LPLD_GPIO_Output_b中直接用x
        gpio_init_struct.GPIO_Dir = DIR_OUTPUT;
        gpio_init_struct.GPIO_Output = Level;
        gpio_init_struct.GPIO_PinControl = IRQC_DIS;
        LPLD_GPIO_Init(gpio_init_struct);
        break;
      default :
        break;
  }
}




