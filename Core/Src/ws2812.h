#pragma once

#define no_leds	(64)
#define data_size (no_leds*3*8+40)

void WS_Init(void);

void WS_SendOut(void);
