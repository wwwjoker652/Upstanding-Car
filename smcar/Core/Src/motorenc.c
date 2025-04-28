#include "motorenc.h"
#include "stm32f1xx.h"
#include "tim.h"

void enc_init(void){
	HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL);
}

int read_enc(TIM_HandleTypeDef *htim){
	int res;
	res = (short)__HAL_TIM_GetCounter(htim);
	__HAL_TIM_SetCounter(htim,0);
	return res;
}
