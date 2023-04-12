#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

    
void PIT_Process_100us();
void PIT_Process_5ms();   
void PIT_Process_5s(); 
void PIT_Process_10us();
void SoftIsr_Open(IRQn_Type irqx);

#endif