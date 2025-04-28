#ifndef _TB6612_H
#define _TB6612_H

#include "stm32f1xx_hal.h"

void motorcontrol(int m1,int m2);
void tb6612_init(void);
void limit(int *moto1,int *moto2);

#endif
