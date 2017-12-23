#include "main.h"


#define B_PIN_0 GPIO_Pin_0
#define B_PIN_1 GPIO_Pin_1
#define DELAY_TIME 100

void Init_Buttons(){
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    GPIO_InitStruct.GPIO_Pin = B_PIN_0 | B_PIN_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOE, &GPIO_InitStruct);


    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);

    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_Init(&EXTI_InitStruct);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);

    EXTI_InitStruct.EXTI_Line = EXTI_Line1;
    EXTI_Init(&EXTI_InitStruct);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, DISABLE);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_Init(&NVIC_InitStruct);

}

void EXTI0_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line0) != RESET) {
        change_intensity(1);
      EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

uint32_t systimer_timestamp;
uint32_t delay;
void EXTI1_IRQHandler(void){
     if(EXTI_GetITStatus(EXTI_Line1) != RESET) {
       if(delay - systimer_timestamp > DELAY_TIME){
         change_color(1);
       }
       delay = systimer_timestamp + DELAY_TIME;
      EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

/*void SysTick_Handler(void){
     systimer_timestamp++;
}

void Init_Debounce_Timer(void){
     systimer_timestamp = 0;

     RCC_ClocksTypeDef RCC_Clocks;
     RCC_GetClocksFreq(&RCC_Clocks);
     SysTick_Config(RCC_Clocks.HCLK_Frequency / 1680);

}
*/


int main(void)
{
    delay = 0;
    //Init_Debounce_Timer();
    InitLib();
    Init_Buttons();

    while (1)
    {

    }
}
