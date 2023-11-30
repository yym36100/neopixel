#include <math.h>

#include "ws2812.h"
#include "main.h"
#include "tim.h"

volatile uint32_t done = 1;
uint8_t ws_brighness;

static uint16_t data[data_size];
static uint8_t gamamcurve[256];

void WS_MakeGamma(float gamma) {
	for (int i = 0; i < 256; i++) {
		gamamcurve[i] = (uint8_t) (powf(i / 255.0f, gamma) * 255.0f);
	}
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
	HAL_GPIO_WritePin(user_led_GPIO_Port, user_led_Pin, GPIO_PIN_RESET);
	done = 1;
}

void WS_Init(void) {
	for (int i = 0; i < data_size; i++) {
		// set all off
		if (i < (data_size - 40))
			data[i] = 38;
		else
			data[i] = 0;
	}
}

void WS_SendOut(void) {
	while (!done)
		;
	done = 0;
	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, data, data_size);
}

static void WS_ColorToBits(uint16_t *pData, uint8_t c) {
	for (int i = 0; i < 8; i++) {
		pData[i] = (c & 0x80) ? ws_bit1 : ws_bit0;
		c <<= 1;
	}
}

void WS_SetLed(uint16_t led_index, ws_tstcolor c3) {
	WS_ColorToBits(&data[led_index * 24 + 0], gamamcurve[c3.g]);
	WS_ColorToBits(&data[led_index * 24 + 8], gamamcurve[c3.r]);
	WS_ColorToBits(&data[led_index * 24 + 16], gamamcurve[c3.b]);
}

//@formatter:off
	static const uint8_t dither[8*8] = {
			 0, 48, 12, 60,  3, 51, 15, 63 ,
			32, 16, 44, 28, 35, 19, 47, 31 ,
			 8, 56,  4, 52, 11, 59,  7, 55 ,
			40, 24, 36, 20, 43, 27, 39, 23 ,
			 2, 50, 14, 62,  1, 49, 13, 61 ,
			34, 18, 46, 30, 33, 17, 45, 29 ,
			10, 58,  6, 54,  9, 57,  5, 53 ,
			42, 26, 38, 22, 41, 25, 37, 21 ,

	};
		//@formatter:on

void WS_Dither8(uint8_t c2) {


	ws_tstcolor c1 = { 1, 0, 0 };
	c2/=4;

	for (int y = 0; y < 64; y++) {
		if (c2 > dither[y])
			WS_SetLed(y, c1);
	}
}

void WS_Dither16(uint16_t c){
	uint8_t color = c/64;
	uint8_t patt = c%64;

	ws_tstcolor c1 = { color, color, color };
	ws_tstcolor c2 = { color+1, color+1, color+1 };
	if(color==255){c2.r=255;c2.g=255;c2.b=255;}

		for (int y = 0; y < 64; y++) {
			if (patt > dither[y])
				WS_SetLed(y, c2);
			else
				WS_SetLed(y, c1);
		}
}

