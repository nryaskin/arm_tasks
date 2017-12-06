#include "main.h"

#define SWITCH_DELAY 1500000
#define LED1_PIN GPIO_Pin_12
#define LED2_PIN GPIO_Pin_13
#define LED3_PIN GPIO_Pin_14
#define LED4_PIN GPIO_Pin_15

#define L_PIN_R GPIO_Pin_8
#define L_PIN_G GPIO_Pin_9
#define L_PIN_B GPIO_Pin_10

#define B_PIN_0 GPIO_Pin_0
#define B_PIN_1 GPIO_Pin_1


#define MAX_FREQ 168000000
#define PWM_FREQ 60
#define PRESCALER 1680
#define PERIOD (MAX_FREQ)/((PRESCALER) * (PWM_FREQ))
#define PULSE (PERIOD)*(0.05)

//168000000/1680 = 100000
#define L_PINS (GPIO_Pin_8) | (GPIO_Pin_9) | (GPIO_Pin_10)


#define COUNT 3
#define INTESITI_LEVELS 10

int turn_off;
int j, levels;
uint16_t active_LED = GPIO_PinSource10;

void TM_PWM_Init(void){
    TIM_OCInitTypeDef TIM_OCStruct;
    TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;

    TIM_OCStruct.TIM_Pulse = PULSE - 1; 
    TIM_OC1Init(TIM1, &TIM_OCStruct);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    TIM_OCStruct.TIM_Pulse = PULSE - 1; 
    TIM_OC2Init(TIM1, &TIM_OCStruct);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    TIM_OCStruct.TIM_Pulse = PULSE - 1; 
    TIM_OC3Init(TIM1, &TIM_OCStruct);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);




    //TIM_SetCompare1(TIM1, 10);
    TIM_Cmd(TIM1, ENABLE);

}

void Init_Timer(void){
    TIM_TimeBaseInitTypeDef tim_struct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    tim_struct.TIM_Period = (uint16_t)(PERIOD - 1);
    tim_struct.TIM_Prescaler = (uint16_t)(PRESCALER - 1);
    tim_struct.TIM_ClockDivision = 0;
    tim_struct.TIM_RepetitionCounter = 0;
    tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &tim_struct);
  
    //TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
}

void Init_Leds(void){
  GPIO_InitTypeDef  GPIO_InitStructure;


  /* Enable peripheral clock for GPIOA port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);

  /* external leds*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

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

void change_intensity(int order){

    levels = (levels + order + INTESITI_LEVELS) % INTESITI_LEVELS;

    TIM_SetCompare1(TIM1, PERIOD * ((float)levels * 0.1));
}



void EXTI0_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line0) != RESET) {
        change_intensity(1);
      EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI1_IRQHandler(void){
     if(EXTI_GetITStatus(EXTI_Line1) != RESET) {

      EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

int main(void)
{
    j = 0;
    levels = 0;


    Init_Timer();
    TM_PWM_Init();
    //Init_Led_Red();
    Init_Leds();
    Init_Buttons();

    //GPIO_SetBits(GPIOA, L_PINS);
    //GPIO_ResetBits(GPIOA, active_LED);

    //Init_Timer_Inetr();
    while (1)
    {

    }
}
