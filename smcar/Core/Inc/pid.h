#ifndef __PID_H
#define __PID_H 

#include "stm32f1xx_hal.h"

extern int Vertical_out,Velocity_out,Turn_out,MOTO1,MOTO2;
extern int Target_Speed;
extern int Target_turn;
extern float Med_Angle;

extern float Vertical_Kp,Vertical_Kd;	
extern float Velocity_Kp,Velocity_Ki;	
extern float Turn_Kp,Turn_Kd;	

extern int Enc_left,Enc_right;

int Vertical(float Med,float Angle,float gyro_Y);
int velocity(int target,int enc_L,int enc_R);
void control(void);

#endif
