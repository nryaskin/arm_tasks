#include "main.h"

#define SWITCH_DELAY 1500000
#define LED_GREEN GPIO_Pin_12
#define LED2_PIN GPIO_Pin_13
#define LED_RED GPIO_Pin_14
#define LED_BLUE GPIO_Pin_15

#define L_INDICATORS (LED_RED) | (LED_GREEN) | (LED4_PIN) 

#define L_PIN_R GPIO_Pin_8
#define L_PIN_G GPIO_Pin_9
#define L_PIN_B GPIO_Pin_10


#define MAX_FREQ 168000000
#define PWM_FREQ 60
#define PRESCALER 1680
#define PERIOD (MAX_FREQ)/((PRESCALER) * (PWM_FREQ))
#define PULSE 1

//168000000/1680 = 100000
#define L_PINS (GPIO_Pin_8) | (GPIO_Pin_9) | (GPIO_Pin_10)


#define COUNT 3
#define INTESITI_LEVELS 10

int turn_off;
int j, levels;
uint16_t active_LED = GPIO_PinSource10;

uint16_t indicators[] = { LED_RED, LED_GREEN, LED_BLUE };
uint32_t *ccr_arr[] = { &(TIM1->CCR1), &(TIM1->CCR2), &(TIM1->CCR3) };

typedef void (*compare)(TIM_TypeDef*, uint32_t);

compare comp_arr[] = {TIM_SetCompare1, TIM_SetCompare2, TIM_SetCompare3};

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

void Init_Indicators(void){
    GPIO_InitTypeDef  GPIO_InitStructure;


    /* Enable peripheral clock for GPIOA port */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LED_RED | LED_GREEN | LED_BLUE; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOD, L_INDICATORS); 
    GPIO_SetBits(GPIOD, indicators[j]);
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

void change_intensity(int order){

    levels = (levels + order + INTESITI_LEVELS) % INTESITI_LEVELS;

    comp_arr[j](TIM1, PERIOD * ((float)levels * 0.1));
}

void change_color(int order){

    GPIO_ResetBits(GPIOD, indicators[j]); 
    j = (j + order + COUNT) % COUNT;
    levels = *ccr_arr[j];
    GPIO_SetBits(GPIOD, indicators[j]); 

}


void InitLib(void)
{
    j = 0;
    levels = 0;


    Init_Timer();
    TM_PWM_Init();
    //Init_Led_Red();
    Init_Leds();
    //    Init_Buttons();
    Init_Indicators();
    //GPIO_SetBits(GPIOA, L_PINS);
    //GPIO_ResetBits(GPIOA, active_LED);

    //Init_Timer_Inetr();
}
