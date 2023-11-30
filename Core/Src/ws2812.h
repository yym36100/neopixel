#pragma once
#include <stdint.h>


#define no_leds	(64)
#define data_size (no_leds*3*8+40)

#define ws_bit1	(76)
#define ws_bit0	(38)

void WS_Init(void);

void WS_SendOut(void);


typedef struct ws_stcolor{
	uint8_t r,g,b;
}ws_tstcolor;

void WS_SetLed(uint16_t led_index, ws_tstcolor c);

void WS_MakeGamma(float gamma);

void WS_Dither8(uint8_t c);
