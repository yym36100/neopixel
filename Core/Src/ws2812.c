#include "ws2812.h"
#include "main.h"
#include "tim.h"


uint16_t data[data_size];


void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  HAL_TIM_PWM_Stop_DMA(&htim3,TIM_CHANNEL_1);
  HAL_GPIO_WritePin(user_led_GPIO_Port,user_led_Pin,GPIO_PIN_RESET);
}

void WS_Init(void){
	for(int i=0;i<data_size;i++){

		  if(i<(data_size-40)) data[i]=76;
		  else data[i] = 0;
	  }
}

void WS_SendOut(void){
	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, data, data_size);
}


