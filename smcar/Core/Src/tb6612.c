#include "tb6612.h"
#include "tim.h"

#define GPIO_PORT_MOTOR GPIOB
#define AIN1 GPIO_PIN_13
#define AIN2 GPIO_PIN_12
#define BIN1 GPIO_PIN_14
#define BIN2 GPIO_PIN_15
#define PWM_MAX 6000
#define PWM_MIN -6000

int abs(int a){
	if(a >= 0){
		return a;
	}else if(a < 0){
		return -a;
	}
	return 1;
}

void tb6612_init(void){
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
}

void motorcontrol(int m1,int m2){
	if(m2 < 0){
		HAL_GPIO_WritePin(GPIO_PORT_MOTOR,AIN1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIO_PORT_MOTOR,AIN2,GPIO_PIN_RESET);
	}else{
		HAL_GPIO_WritePin(GPIO_PORT_MOTOR,AIN1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIO_PORT_MOTOR,AIN2,GPIO_PIN_SET);
	}
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_4,abs(m2));
	if(m1 < 0){
		HAL_GPIO_WritePin(GPIO_PORT_MOTOR,BIN1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIO_PORT_MOTOR,BIN2,GPIO_PIN_RESET);
	}else{
		HAL_GPIO_WritePin(GPIO_PORT_MOTOR,BIN1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIO_PORT_MOTOR,BIN2,GPIO_PIN_SET);
	}
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,abs(m1));
}

void limit(int *moto1,int *moto2){
	if(*moto1 > PWM_MAX){
		*moto1 = PWM_MAX;
	}
	if(*moto1 < PWM_MIN){
		*moto1 = PWM_MIN;
	}
	if(*moto2 > PWM_MAX){
		*moto2 = PWM_MAX;
	}
	if(*moto2 < PWM_MIN){
		*moto1 = PWM_MIN;
	}
}
