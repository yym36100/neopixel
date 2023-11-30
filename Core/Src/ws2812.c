#include "ws2812.h"
#include "main.h"
#include "tim.h"


volatile uint32_t done = 1;
uint8_t ws_brighness;

static uint16_t data[data_size];



void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  HAL_TIM_PWM_Stop_DMA(htim,TIM_CHANNEL_1);
  HAL_GPIO_WritePin(user_led_GPIO_Port,user_led_Pin,GPIO_PIN_RESET);
  done = 1;
}

void WS_Init(void){
	for(int i=0;i<data_size;i++){
			// set all off
		  if(i<(data_size-40)) data[i]=38;
		  else data[i] = 0;
	  }
}

void WS_SendOut(void){
	while(!done);
	done = 0;
	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, data, data_size);
}

static void WS_ColorToBits(uint16_t* pData,uint8_t c){
	for(int i=0;i<8;i++){
		pData[i] = (c&0x80) ? ws_bit1 : ws_bit0;
		c<<=1;
	}
}

void WS_SetLed(uint16_t led_index, ws_tstcolor c){
	WS_ColorToBits(&data[led_index*24 + 0],c.g);
	WS_ColorToBits(&data[led_index*24 + 8],c.r);
	WS_ColorToBits(&data[led_index*24 + 16],c.b);
}

