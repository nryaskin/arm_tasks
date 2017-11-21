#include "main.h"

#define SWITCH_DELAY 1500000
#define LED1_PIN GPIO_Pin_12
#define LED2_PIN GPIO_Pin_13
#define LED3_PIN GPIO_Pin_14
#define LED4_PIN GPIO_Pin_15

#define L_PIN_1 GPIO_Pin_8
#define L_PIN_2 GPIO_Pin_9
#define L_PIN_3 GPIO_Pin_10

#define B_PIN_0 GPIO_Pin_0
#define B_PIN_1 GPIO_Pin_1

#define PERIOD 1000000
#define PRESCALER 84

#define L_PINS (GPIO_Pin_8) | (GPIO_Pin_9) | (GPIO_Pin_10)

#define COUNT 3

int turn_off;
int j, up;
uint16_t tmp[] = {L_PIN_1, L_PIN_2, L_PIN_3};

void Init_Timer(void){
    TIM_TimeBaseInitTypeDef tim_struct;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    tim_struct.TIM_Period = PERIOD - 1;
    tim_struct.TIM_Prescaler = PRESCALER - 1;
    tim_struct.TIM_ClockDivision = 0;
    tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &tim_struct);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}

void Init_Timer_Inetr(void){
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    /* priority */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
    /* sub priority */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
    /* en interrupt */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    /* Add to NVIC */
    NVIC_Init(&NVIC_InitStruct);
}

void blink_RGB(void){
        if(!turn_off){
        GPIO_SetBits(GPIOA, tmp[j]);
            step(up);
        }
}

void step(int order){
   j = (j + order + COUNT) % COUNT;
}


void EXTI0_IRQHandler(void){
    if( EXTI_GetITStatus(EXTI_Line0) != RESET) {
        if(!GPIO_ReadInputDataBit(GPIOE, B_PIN_1))
            turn_off = ~turn_off;
        else
        up = 1;
        EXTI_ClearITPendingBit(EXTI_Line0);
       /*Clear interrupt flag */
    }
}

void EXTI1_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line1) != RESET) {
        if(!GPIO_ReadInputDataBit(GPIOE, B_PIN_0))
            turn_off = ~turn_off;
        else
        up = -1;
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

void TIM2_IRQHandler(void){
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
          if(!turn_off){
	  /* Switch LED on */
	    /* empty cycle */
                      blink_RGB();
              GPIO_ResetBits(GPIOA,tmp[j]);
          }
          else{
               GPIO_SetBits(GPIOA, tmp[j]);
          }
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }

}

int main(void)
{
  j = 0;
  turn_off = 0;
  up = 1;
  GPIO_InitTypeDef  GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;


  /* Enable peripheral clock for GPIOD port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  /* Enable peripheral clock for GPIOA port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  /* Enable peripheral clock for GPIOE port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  /* Enable clock for SYSCFG*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Init leds */
  GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);


  /* set pin as input PE0 */
    GPIO_InitStructure.GPIO_Pin = B_PIN_0 | B_PIN_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* PE0 for EXTI_Line0 */
   SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);
   /* PE1 for EXTI_Line1 */


  /* PE0 is connected to EXTI_Line0 */
  EXTI_InitStruct.EXTI_Line = EXTI_Line0;
  /* Enable interrupt */
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  /* Interrupt mode */
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  /* Triggers on rising and falling edge */
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
  /* Add to EXTI */
  EXTI_Init(&EXTI_InitStruct);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);

  /* PE1 is connected to EXTI_Line1*/
  EXTI_InitStruct.EXTI_Line = EXTI_Line1;
  EXTI_Init(&EXTI_InitStruct);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, DISABLE);

  /*Add IRQ vector to NVIC */
  /* PE0 is connected to EXTI_Line0, which has EXTI0_IRQn vector */
  NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
  /* priority */
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
  /* sub priority */
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
  /* en interrupt */
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  /* Add to NVIC */
  NVIC_Init(&NVIC_InitStruct);

  /* PE1 is connected to EXTI_Line1, which has EXTI1_IRQn vector */
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x02;
  NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_Init(&NVIC_InitStruct);

  /* external leds*/
  GPIO_InitStructure.GPIO_Pin = L_PINS;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz / 1000;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  GPIO_SetBits(GPIOA, L_PINS);

  Init_Timer();
  Init_Timer_Inetr();
  while (1)
  {
	 int i;

  }
}
