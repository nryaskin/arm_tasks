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

int turn_off;
int j, up;
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

  /* external leds*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

int main(void)
{

  Init_Timer();
  TM_PWM_Init();
  Init_Leds();
  //GPIO_SetBits(GPIOA, L_PINS);
   
  //GPIO_ResetBits(GPIOA, active_LED);

  
  //Init_Timer_Inetr();
  while (1)
  {

  }
}
