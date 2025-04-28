#ifndef _MOTORENC_H
#define _MOTORENC_H

#include "stm32f1xx_hal.h"

void enc_init(void);
int read_enc(TIM_HandleTypeDef *htim);

#endif
