#include "servo_pid.h"

int last_deltax = 0;
int now_delax   = 0;
uint8 straight_flag = 0;
uint8 duty_pwm = 20;

void servo_pid_test(void)
{
	last_deltax = now_delax;
	now_delax = final_mid_line - MID_W;
	if (abs(now_delax) <= 9)
	{
		straight_flag = 1;
		Speedpid.fKp = 0.6;
		Speedpid.fKd = 1;
		duty_pwm = 19;
	}
	else
	{
		straight_flag = 0;
		Speedpid.fKp = 1.4;
		Speedpid.fKd = 3;
		duty_pwm = 17;
	}
//	Control=S3010_kp*deltax+Contorl_median;
}