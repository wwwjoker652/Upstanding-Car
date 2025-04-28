#include "pid.h"
#include "motorenc.h"
#include "tim.h"
#include "IIC.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "mpu6050.h"
#include "tb6612.h"
#include "oled.h"
#include <stdio.h>
#include <string.h>

char message[50];
char message7[50];

int Enc_left,Enc_right;
float pitch,roll,yaw;
short gyrox,gyroy,gyroz;
short	aacx,aacy,aacz;

int Vertical_out,Velocity_out,Turn_out,MOTO1,MOTO2;
int Target_Speed = 0;
int Target_turn = 0;
float Med_Angle=-6;//机械中值

//float Vertical_Kp=160,Vertical_Kd=2.2;			//直立环 数量级（Kp：0~1000、Kd：0~10）
//float Velocity_Kp=-0.5,Velocity_Ki=-0.0025;		//速度环 数量级（Kp：0~1）
//float Turn_Kp=-25,Turn_Kd=-1.5;					//-2						//转向环

float Vertical_Kp=500*0.6,Vertical_Kd=2.3;			//直立环
float Velocity_Kp=-0.5,Velocity_Ki=-0.0025;		//速度环 
float Turn_Kp=-25,Turn_Kd=-1.5;					//转向环


int Vertical(float Med,float Angle,float gyro_Y){
	int temp;
	temp=Vertical_Kp*(Angle-Med)+Vertical_Kd*gyro_Y;
	return temp;
}

int velocity(int target,int enc_L,int enc_R){
	static int last, enc_s;
	float a = 0.7;
	int error, error_out, temp;
	error = (enc_L + enc_R) - target;
	error_out = (1 - a) * error + a * last; //低通滤波
	last = error_out;
	enc_s += error_out;
	enc_s = enc_s > 20000 ? 20000 : (enc_s < (-20000) ? (-20000) : enc_s);
	temp = Velocity_Kp * error_out + Velocity_Ki * enc_s;
	return temp;
}

int Turn(float gyroz, int Target_turn){
	int temp;
	temp = Turn_Kp * Target_turn + Turn_Kd * gyroz;
	return temp;
}

void control(void){
		int pwm_out;
		Enc_left = -read_enc(&htim2);
		Enc_right = read_enc(&htim4);
		mpu_dmp_get_data(&pitch,&roll,&yaw);
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
		Velocity_out = velocity(Target_Speed,Enc_left,Enc_right);
		Vertical_out = Vertical(Velocity_out+Med_Angle,roll,gyrox);
		Turn_out = Turn(gyroz,Target_turn);
		pwm_out = Vertical_out;
		MOTO1 = pwm_out - Turn_out;
		MOTO2 = pwm_out + Turn_out;
		limit(&MOTO1,&MOTO2);
		motorcontrol(MOTO1,MOTO2);
}
